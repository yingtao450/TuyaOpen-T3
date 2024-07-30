// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include <driver/aon_rtc.h>
#include <driver/ckmn.h>
#include <sys_driver.h>
#include <os/mem.h>
#include "rosc_calib.h"


#define CLK_26M     (26 * 1000 * 1000)
#define CLK_32K     (32 * 1000)
#define ROSC_CALIB_THREAD_PRIORITY       (8)
#define ROSC_CALIB_THREAD_NAME           "rosc_calib"
#define ROSC_CALIB_THREAD_STACK_SIZE     (0x4<<10)
#define ROSC_CALIB_MANUAL_MODE           (1)
#define ROSC_CALIB_CIN0_DEFAULT          (0x40)
#define ROSC_CALIB_CIN0_MAX              (0x80)
#define ROSC_CALIB_CIN1_DEFAULT          (0x100)
#define ROSC_CALIB_CIN1_MAX              (0x200)
#define ROSC_CALIB_32K_COUNT_2MS         (2*32)

#if ROSC_CALIB_DEBUG
#define ROSC_CAL_s_records_cnt_MAX 50

static rosc_calib_record_t* s_records_array = NULL;
static uint32_t s_records_cnt = 0;
#endif


static float s_freq_32k = 0.0;
static beken_semaphore_t s_rosc_calib_sema = NULL;
static beken_thread_t s_rosc_calib_thread = NULL;

static void rosc_calib_delay_for_ckest_ready(void)
{
	// wait at least one 32k tick for ckest to reset ok
	uint64_t tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	while(bk_aon_rtc_get_current_tick(AON_RTC_ID_1) < tick + 5);
}


static void rosc_calib_ckmn_isr(void)
{
	int rc26m_count = 0;
	int rc32k_count = 0;

	bk_ckmn_ckest_disable();
	rc26m_count = bk_ckmn_get_rc26m_count();
	rc32k_count = bk_ckmn_get_rc32k_count();

	if (rc26m_count <= 0 || rc32k_count <= 0) {
		ROSC_CALIB_LOGI("ckmn get rc26m cycle failed!\n");
		return;
	}

	s_freq_32k = CLK_26M * (rc32k_count / ((float)rc26m_count));
	rtos_set_semaphore(&s_rosc_calib_sema);
}

static void rosc_calib_thread(void *args)
{
	bk_err_t ret;
	uint8_t progress_count = 0;
	uint32_t cin0 = ROSC_CALIB_CIN0_DEFAULT;
	uint32_t cin1 = ROSC_CALIB_CIN1_DEFAULT;
	float loss, _loss = 0.0;

	// init
	ROSC_CALIB_LOGI("ckmn calib rc32k thread start\r\n");
	sys_drv_rosc_calibration(ROSC_CALIB_MANUAL_MODE, cin0 + (cin1 << 16)); // manual mode
	// calibration progress
	while (1)
	{
		// get 32k_freq
		bk_ckmn_set_rc32k_count(ROSC_CALIB_32K_COUNT_2MS); // 2ms
		bk_ckmn_register_isr(CKMN_INT_CKEST, rosc_calib_ckmn_isr);
		rosc_calib_delay_for_ckest_ready();
		bk_ckmn_ckest_enable();
		// wait for ckmn isr
		ret = rtos_get_semaphore(&s_rosc_calib_sema, 100);
		if(kNoErr == ret) {
			// loss check
			loss = s_freq_32k - CLK_32K;
#if ROSC_CALIB_DEBUG
			rosc_calib_record_t record = {progress_count, cin0, cin1, loss};
			rosc_calib_records_insert(&record);
#endif
			if (loss == 0) {
				break;
			}
			if (loss * _loss < 0) {
				if ((loss < 0) ? (-loss < _loss) : (loss < -_loss)) {
					if (progress_count++ == 3)
						break;
				}
			}
			_loss = loss;
			// next prog
			switch (progress_count)
			{
			case 0:
				if (loss > 0) cin0--;
				else cin0++;
				break;
			case 1:
				if (loss > 0) cin1-=32;
				else cin1+=20;
				break;
			case 2:
				if (loss > 0) cin1-=4;
				else cin1+=4;
				break;
			case 3:
				if (loss > 0) cin1--;
				else cin1++;
				break;
			default:
				progress_count = 0;
				break;
			}

			if (cin0 < ROSC_CALIB_CIN0_MAX && cin1 < ROSC_CALIB_CIN1_MAX)
			{
				sys_drv_rosc_calibration(ROSC_CALIB_MANUAL_MODE, cin0 + (cin1 << 16)); // manual mode
			} else {
				sys_drv_rosc_calibration(3, 0); // fix it todo
				ret = BK_FAIL;
				break;
			}
		}
	}

	// complete
	if (ret) {
		ROSC_CALIB_LOGE("rosc calib failed reason code: %d\r\n", ret);
	} else {
		ROSC_CALIB_LOGI("rosc calib complete %d %d\r\n", cin0, cin1);
	}

#if ROSC_CALIB_DEBUG
	rosc_calib_records_dump();

	s_records_cnt = 0;

	os_free(s_records_array);
	s_records_array = NULL;
#endif

	rtos_deinit_semaphore(&s_rosc_calib_sema);
	s_rosc_calib_sema = NULL;

	rtos_delete_thread(NULL);
	s_rosc_calib_thread = NULL;
}

bk_err_t bk_rosc_calib()
{
	bk_err_t ret = BK_OK;

	if ((s_rosc_calib_sema != NULL) || (s_rosc_calib_thread != NULL))
	{
		ROSC_CALIB_LOGE("rosc calib is ongoing\r\n");
		return BK_FAIL;
	}

	ret = rtos_init_semaphore(&s_rosc_calib_sema, 1);
	if (kNoErr != ret)
	{
		ROSC_CALIB_LOGE("init sema ret=%d\r\n", ret);
		goto err_exit;
	}

	ret = rtos_create_thread(
								&s_rosc_calib_thread,
								ROSC_CALIB_THREAD_PRIORITY,
								ROSC_CALIB_THREAD_NAME,
								rosc_calib_thread,
								ROSC_CALIB_THREAD_STACK_SIZE,
								NULL
							);
	if (kNoErr != ret)
	{
		ROSC_CALIB_LOGE("init thread ret=%d\r\n", ret);
		goto err_exit;
	}

#if ROSC_CALIB_DEBUG
	s_records_array = (rosc_calib_record_t*)os_malloc(sizeof(rosc_calib_record_t) * ROSC_CAL_s_records_cnt_MAX);
	if (NULL == s_records_array)
	{
		ROSC_CALIB_LOGE("debug records malloc fail\r\n");
	}
#endif

	return ret;

err_exit:

	if (s_rosc_calib_sema)
	{
		rtos_deinit_semaphore(&s_rosc_calib_sema);
		s_rosc_calib_sema = NULL;
	}

	if (s_rosc_calib_thread)
	{
		rtos_delete_thread(&s_rosc_calib_thread);
		s_rosc_calib_thread = NULL;
	}

	return ret;
}

#if ROSC_CALIB_DEBUG
static bk_err_t rosc_calib_records_insert(rosc_calib_record_t* record)
{
	bk_err_t ret = BK_OK;
	rosc_calib_record_t* records_node;

	if (NULL == s_records_array)
	{
		return BK_FAIL;
	}

	if (s_records_cnt >= ROSC_CAL_s_records_cnt_MAX)
	{
		// ROSC_CALIB_LOGI("records array is full\r\n");
		return BK_FAIL;
	}

	records_node = &s_records_array[s_records_cnt++];
	records_node->progress_count = record->progress_count;
	records_node->cin0 = record->cin0;
	records_node->cin1 = record->cin1;
	records_node->loss = record->loss;

	return ret;
}

static void rosc_calib_records_dump(void)
{
	uint32_t i;
	rosc_calib_record_t* records_node;

	ROSC_CALIB_LOGI("rosc calib records dump start\r\n");

	for (i = 0; i < s_records_cnt; i++)
	{
		records_node = &s_records_array[i];
		ROSC_CALIB_LOGI("%d %d %d %f\r\n", 	records_node->progress_count,
											records_node->cin0,
											records_node->cin1,
											records_node->loss);
	}

	ROSC_CALIB_LOGI("rosc calib records dump end\r\n");
}
#endif