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

#pragma once

#include <common/bk_include.h>
#include <common/bk_err.h>
#include <driver/hal/hal_int_types.h>

typedef enum {
	PRI_GROUP_BITS_7_1 = 0,
	PRI_GOURP_BITS_7_2,
	PRI_GOURP_BITS_7_3,
	PRI_GOURP_BITS_7_4,
	PRI_GOURP_BITS_7_5,
	PRI_GOURP_BITS_7_6,
	PRI_GOURP_BITS_7_7,
	PRI_GOURP_BITS_NONE,
} pri_group_t;

#define IRQ_DEFAULT_PRIORITY    (6)

void arch_interrupt_unregister_int(uint32_t int_number);
void arch_interrupt_register_int(uint32_t int_number, int_group_isr_t isr_callback);
void arch_interrupt_set_priority(uint32_t int_number, uint32_t int_priority);
void arch_int_enable_irq(uint32_t irq);
void arch_int_disable_irq(uint32_t irq);
uint32_t arch_int_get_enable_irq(uint32_t irq);
void arch_int_set_target_state(uint32_t irq);
void arch_int_clear_target_state(uint32_t irq);
uint32_t arch_int_get_target_state(uint32_t irq);
void arch_int_set_target_state_all(void);

unsigned int arch_is_enter_exception(void);
void arch_set_enter_exception(void); 