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
#include "aon_pmu_hal.h"
#include "aon_pmu_ll.h"
#include "system_hw.h"
#include "sys_types.h"
#include "modules/pm.h"

bk_err_t aon_pmu_hal_init(void)
{
	return BK_OK;
}

int aon_pmu_hal_set_sleep_parameters(uint32_t value)
{
	aon_pmu_ll_set_r40(value);
	return 0;
}

void aon_pmu_hal_set_wakeup_source_reg(uint32_t value)
{
	//TODO fix it
	aon_pmu_ll_set_r41(value);
}

uint32_t aon_pmu_hal_get_wakeup_source_reg(void)
{
	return aon_pmu_ll_get_r41();
}

uint32_t aon_pmu_hal_get_chipid(void)
{
	return aon_pmu_ll_get_r7c_id();
}

__IRAM_SEC void aon_pmu_hal_clear_wakeup_source(wakeup_source_t value)
{
	uint32_t wakeup_source = 0;
	wakeup_source = aon_pmu_ll_get_r41_wakeup_ena();
	wakeup_source &= ~(0x1 << value);
	aon_pmu_ll_set_r41_wakeup_ena(wakeup_source);
}

void aon_pmu_hal_set_wakeup_source(wakeup_source_t value)
{
	uint32_t wakeup_source = 0;
	wakeup_source = aon_pmu_ll_get_r41_wakeup_ena();
	wakeup_source |= (0x1 << value);
	aon_pmu_ll_set_r41_wakeup_ena(wakeup_source);
}

void aon_pmu_hal_usbplug_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_r1_usbplug_int_en();
	int_state |= value;
	aon_pmu_ll_set_r1_usbplug_int_en(int_state);
}

void aon_pmu_hal_touch_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_r1_touch_int_en();
	int_state |= value;
	aon_pmu_ll_set_r1_touch_int_en(int_state);
}

uint32_t aon_pmu_hal_get_touch_int_status(void)
{
	return aon_pmu_ll_get_reg73_mul_touch_int();
}

uint32_t aon_pmu_hal_get_cap_cal(void)
{
	return aon_pmu_ll_get_reg73_cap_cal();
}

uint32_t aon_pmu_hal_get_touch_state(void)
{
	return aon_pmu_ll_get_reg73_mul_touch_int();
}

uint32_t aon_pmu_hal_get_adc_cal()
{
	return aon_pmu_ll_get_r7d_adc_cal();
}

__IRAM_SEC void aon_pmu_hal_reg_set(pmu_reg_e reg, uint32_t value)
{
    pmu_address_map_t pmu_addr_map[] = PMU_ADDRESS_MAP;
    pmu_address_map_t *pmu_addr = &pmu_addr_map[reg];

    uint32_t pmu_reg_addr = pmu_addr->reg_address;

	REG_WRITE(pmu_reg_addr, value);
}
__IRAM_SEC uint32_t aon_pmu_hal_reg_get(pmu_reg_e reg)
{
    pmu_address_map_t pmu_addr_map[] = PMU_ADDRESS_MAP;
    pmu_address_map_t *pmu_addr = &pmu_addr_map[reg];

    uint32_t pmu_reg_addr = pmu_addr->reg_address;

	return REG_READ(pmu_reg_addr);
}


void aon_pmu_hal_wdt_rst_dev_enable()
{
	uint32_t aon_pmu_r2 = 0;
	aon_pmu_r2 = aon_pmu_ll_get_r2();
	aon_pmu_r2 |= 0x1ff;

	aon_pmu_ll_set_r2(aon_pmu_r2);
}

void aon_pmu_hal_lpo_src_extern32k_enable(void)
{
	//Empty function
}

void aon_pmu_hal_lpo_src_set(uint32_t lpo_src)
{
	volatile uint32_t count = PM_POWER_ON_ROSC_STABILITY_TIME;
	if(lpo_src == PM_LPO_SRC_ROSC)
	{
		if(sys_ll_get_ana_reg5_pwd_rosc_spi() != 0x0)
		{
			sys_ll_set_ana_reg5_pwd_rosc_spi(0x0);//power on rosc
			while(count--)//delay time for stability when power on rosc
			{
			}
		}
	}

	if(aon_pmu_ll_get_r41_lpo_config() != lpo_src)
	{
		aon_pmu_ll_set_r41_lpo_config(lpo_src);
	}

	if(lpo_src == PM_LPO_SRC_ROSC)
	{
		if(sys_ll_get_ana_reg5_en_xtall() == 0x1)
		{
			sys_ll_set_ana_reg5_en_xtall(0x0);
		}

		if(sys_ll_get_ana_reg5_itune_xtall() != 0xF)
		{
			sys_ll_set_ana_reg5_itune_xtall(0xF);
		}
	}
}

__IRAM_SEC uint32_t aon_pmu_hal_lpo_src_get()
{
	return aon_pmu_ll_get_r41_lpo_config();
}

uint32_t aon_pmu_hal_bias_cal_get()
{
	return aon_pmu_ll_get_r7e_cbcal();
}

uint32_t aon_pmu_hal_gpio_retention_bitmap_get()
{
	return (aon_pmu_ll_get_r0() >> 8) & 0xFF;
}

void aon_pmu_hal_gpio_retention_bitmap_set(uint32_t bitmap)
{
	uint32_t aon_pmu_r0 = aon_pmu_ll_get_r0();

	aon_pmu_r0 &= ~(0xFF << 8);
	aon_pmu_r0 |= (bitmap << 8);

	aon_pmu_ll_set_r0(aon_pmu_r0);
	aon_pmu_ll_set_r25(0x424B55AA);
	aon_pmu_ll_set_r25(0xBDB4AA55);
}