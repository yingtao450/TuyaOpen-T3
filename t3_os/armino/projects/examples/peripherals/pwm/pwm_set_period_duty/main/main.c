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
	pwm_init_config_t init_config = {0};
	pwm_period_duty_config_t config = {0};
	const pwm_chan_t chan = 4;

	BK_LOG_ON_ERR(bk_pwm_driver_init());

	/* freq:200Hz, duty ratio:20% */
	init_config.period_cycle = 5000;
	init_config.duty_cycle = 1000;
	init_config.duty2_cycle = 0;
	init_config.duty3_cycle = 0;

	/* freq: 200Hz, duty ratio:20% */
	BK_LOG_ON_ERR(bk_pwm_init(chan, &init_config));
	os_printf("pwm init, chan=%d period=%d duty=%d\n", chan, init_config.period_cycle, init_config.duty_cycle);
	BK_LOG_ON_ERR(bk_pwm_start(chan));

	rtos_delay_milliseconds(1000);

	/* freq:200Hz, duty ratio:20% -> 40% */
	config.period_cycle = 5000;
	config.duty_cycle = 2000;
	BK_LOG_ON_ERR(bk_pwm_set_period_duty(chan, &config));

	rtos_delay_milliseconds(1000);

	/* freq:200Hz -> 500Hz, duty ratio:40% */
	config.period_cycle = 2000;
	config.duty_cycle = 800;
	BK_LOG_ON_ERR(bk_pwm_set_period_duty(chan, &config));

	return 0;
}
