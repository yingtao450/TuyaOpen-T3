// Copyright 2024-2025 Beken
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

#include <stdio.h>
#include <os/os.h>
#include <driver/pwm.h>
#include "pwm_hal.h"

int main(void)
{
	pwm_group_t group = 0;
	pwm_group_init_config_t init_config = {0};
	pwm_group_config_t config = {0};

	BK_LOG_ON_ERR(bk_pwm_driver_init());

	/* freq:1KHz, duty ratio:20%, 40% */
	init_config.chan1 = 0;
	init_config.chan2 = 4;
	init_config.period_cycle = 1000;
	init_config.chan1_duty_cycle = 200;
	init_config.chan2_duty_cycle = 400;

	BK_LOG_ON_ERR(bk_pwm_group_init(&init_config, &group));
	BK_LOG_ON_ERR(bk_pwm_group_start(group));

	rtos_delay_milliseconds(1000);

	/* update chan1 and chan2 duty cycle */
	config.period_cycle = 1000;
	config.chan1_duty_cycle = 300;
	config.chan2_duty_cycle = 300;
	BK_LOG_ON_ERR(bk_pwm_group_set_config(group, &config));

	rtos_delay_milliseconds(1000);

	BK_LOG_ON_ERR(bk_pwm_group_stop(group));

	return 0;
}
