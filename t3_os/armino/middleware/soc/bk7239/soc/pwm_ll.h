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

/***********************************************************************************************************************************
* This file is generated from PWM_register_map.xlsx automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7239,GENARATE TIME:2023-03-23 15:46:12                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#include <soc/soc.h>          
#include "pwm_hw.h"        
#include "pwm_ll_macro_def.h"         

#ifdef __cplusplus          
extern "C" {              
#endif                      

#if 0

/* REG_0x00 */

static inline uint32_t pwm_ll_get_deviceid_value(pwm_hw_t *hw)
{
    return hw->deviceid.v;
}

/* REG_0x00:deviceid->DeviceID:0x0[31:0],ASCII Code of "PWM",0x00808777,RO*/
static inline uint32_t pwm_ll_get_deviceid_deviceid(pwm_hw_t *hw)
{
    return hw->deviceid.v;
}

/* REG_0x01 */

static inline uint32_t pwm_ll_get_versionid_value(pwm_hw_t *hw)
{
    return hw->versionid.v;
}

/* REG_0x01:versionid->VersionID:0x1[31:0],版本号V1.1:w(区别不同版本),0x00010001,RO*/
static inline uint32_t pwm_ll_get_versionid_versionid(pwm_hw_t *hw)
{
    return hw->versionid.v;
}

/* REG_0x02 */

static inline uint32_t pwm_ll_get_cg_reset_value(pwm_hw_t *hw)
{
    return hw->cg_reset.v;
}

static inline void pwm_ll_set_cg_reset_value(pwm_hw_t *hw, uint32_t value)
{
    hw->cg_reset.v = value;
}

/* REG_0x02:cg_reset->Soft_Reset:0x2[0],写1：PWM控制器软复位,0x0,R/W*/
static inline uint32_t pwm_ll_get_cg_reset_soft_reset(pwm_hw_t *hw)
{
    return hw->cg_reset.soft_reset;
}

static inline void pwm_ll_set_cg_reset_soft_reset(pwm_hw_t *hw, uint32_t value)
{
    hw->cg_reset.soft_reset = value;
}

/* REG_0x02:cg_reset->Bps_Clkgate:0x2[1],配置时钟门控禁止位,0x0,R/W*/
static inline uint32_t pwm_ll_get_cg_reset_bps_clkgate(pwm_hw_t *hw)
{
    return hw->cg_reset.bps_clkgate;
}

static inline void pwm_ll_set_cg_reset_bps_clkgate(pwm_hw_t *hw, uint32_t value)
{
    hw->cg_reset.bps_clkgate = value;
}

/* REG_0x02:cg_reset->Soft_Reset:0x2[31:2],写1：PWM控制器软复位,0x0,R/W*/
static inline uint32_t pwm_ll_get_cg_reset_soft_reset(pwm_hw_t *hw)
{
    return hw->cg_reset.soft_reset;
}

static inline void pwm_ll_set_cg_reset_soft_reset(pwm_hw_t *hw, uint32_t value)
{
    hw->cg_reset.soft_reset = value;
}

/* REG_0x03 */

static inline uint32_t pwm_ll_get_status_value(pwm_hw_t *hw)
{
    return hw->status.v;
}

/* REG_0x03:status->Global_Status:0x3[31:0],反应外设的工作状态,0x0,RO*/
static inline uint32_t pwm_ll_get_status_global_status(pwm_hw_t *hw)
{
    return hw->status.v;
}

/* REG_0x04 */

static inline uint32_t pwm_ll_get_pwm_cr1_value(pwm_hw_t *hw)
{
    return hw->pwm_cr1.v;
}

static inline void pwm_ll_set_pwm_cr1_value(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.v = value;
}

/* REG_0x04:pwm_cr1->CEN3:0x4[0],计数器TIM3使能,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_cen3(pwm_hw_t *hw)
{
    return hw->pwm_cr1.cen3;
}

static inline void pwm_ll_set_pwm_cr1_cen3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.cen3 = value;
}

/* REG_0x04:pwm_cr1->CEN2:0x4[1],计数器TIM2使能,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_cen2(pwm_hw_t *hw)
{
    return hw->pwm_cr1.cen2;
}

static inline void pwm_ll_set_pwm_cr1_cen2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.cen2 = value;
}

/* REG_0x04:pwm_cr1->CEN1:0x4[2],计数器TIM1使能,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_cen1(pwm_hw_t *hw)
{
    return hw->pwm_cr1.cen1;
}

static inline void pwm_ll_set_pwm_cr1_cen1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.cen1 = value;
}

/* REG_0x04:pwm_cr1->ARPE3:0x4[3],计数器TIM3终点寄存器preload使能,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_arpe3(pwm_hw_t *hw)
{
    return hw->pwm_cr1.arpe3;
}

static inline void pwm_ll_set_pwm_cr1_arpe3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.arpe3 = value;
}

/* REG_0x04:pwm_cr1->ARPE2:0x4[4],计数器TIM2终点寄存器preload使能,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_arpe2(pwm_hw_t *hw)
{
    return hw->pwm_cr1.arpe2;
}

static inline void pwm_ll_set_pwm_cr1_arpe2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.arpe2 = value;
}

/* REG_0x04:pwm_cr1->ARPE1:0x4[5],计数器TIM1终点寄存器preload使能  0为preload寄存器中中的值立即同步更新到阴影寄存器，1为在更新事件时更新（Auto-Reload Preload Enable),0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_arpe1(pwm_hw_t *hw)
{
    return hw->pwm_cr1.arpe1;
}

static inline void pwm_ll_set_pwm_cr1_arpe1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.arpe1 = value;
}

/* REG_0x04:pwm_cr1->OC3PE:0x4[6],TIM3的比较寄存器preload使能见OC1PE,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_oc3pe(pwm_hw_t *hw)
{
    return hw->pwm_cr1.oc3pe;
}

static inline void pwm_ll_set_pwm_cr1_oc3pe(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.oc3pe = value;
}

/* REG_0x04:pwm_cr1->OC2PE:0x4[7],TIM2的比较寄存器preload使能见OC1PE,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_oc2pe(pwm_hw_t *hw)
{
    return hw->pwm_cr1.oc2pe;
}

static inline void pwm_ll_set_pwm_cr1_oc2pe(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.oc2pe = value;
}

/* REG_0x04:pwm_cr1->OC1PE:0x4[8],TIM1的比较寄存器CCR1~CCR3 preload使能  0为preload寄存器中中的值立即同步更新到阴影寄存器，1为在更新事件时更新,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_oc1pe(pwm_hw_t *hw)
{
    return hw->pwm_cr1.oc1pe;
}

static inline void pwm_ll_set_pwm_cr1_oc1pe(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.oc1pe = value;
}

/* REG_0x04:pwm_cr1->CMS_TIM3:0x4[19:18],计数器TIM1计数模式：; 00单向计数模式; 01双向计数模式 且只有向上计数比较成功时才置位比较成功中断flag; 10双向计数模式 且只有向下计数比较成功时才置位比较成功中断flag; 11双向计数模式 且向上和向下计数比较成功时都置位比较成功中断flag,2,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_cms_tim3(pwm_hw_t *hw)
{
    return hw->pwm_cr1.cms_tim3;
}

static inline void pwm_ll_set_pwm_cr1_cms_tim3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.cms_tim3 = value;
}

/* REG_0x04:pwm_cr1->CMS_TIM2:0x4[21:20],计数器TIM2计数模式：同CMS_TIM1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_cms_tim2(pwm_hw_t *hw)
{
    return hw->pwm_cr1.cms_tim2;
}

static inline void pwm_ll_set_pwm_cr1_cms_tim2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.cms_tim2 = value;
}

/* REG_0x04:pwm_cr1->CMS_TIM1:0x4[23:22],计数器TIM3计数模式：同CMS_TIM1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_cms_tim1(pwm_hw_t *hw)
{
    return hw->pwm_cr1.cms_tim1;
}

static inline void pwm_ll_set_pwm_cr1_cms_tim1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.cms_tim1 = value;
}

/* REG_0x04:pwm_cr1->URS3:0x4[26],见URS1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_urs3(pwm_hw_t *hw)
{
    return hw->pwm_cr1.urs3;
}

static inline void pwm_ll_set_pwm_cr1_urs3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.urs3 = value;
}

/* REG_0x04:pwm_cr1->URS2:0x4[27],见URS1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_urs2(pwm_hw_t *hw)
{
    return hw->pwm_cr1.urs2;
}

static inline void pwm_ll_set_pwm_cr1_urs2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.urs2 = value;
}

/* REG_0x04:pwm_cr1->URS1:0x4[28],更新事件源选择  0：UG与向上向下溢出可产生更新事件;                 1：仅向上向下溢出可产生更新事件（捕获模式必须设置为1，否则会同时产生捕获成功中断和更新事件中断）,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_urs1(pwm_hw_t *hw)
{
    return hw->pwm_cr1.urs1;
}

static inline void pwm_ll_set_pwm_cr1_urs1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.urs1 = value;
}

/* REG_0x04:pwm_cr1->DIR_TIM3:0x4[29],计数器TIM3的方向：同DIR_TIM1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_dir_tim3(pwm_hw_t *hw)
{
    return hw->pwm_cr1.dir_tim3;
}

static inline void pwm_ll_set_pwm_cr1_dir_tim3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.dir_tim3 = value;
}

/* REG_0x04:pwm_cr1->DIR_TIM2:0x4[30],计数器TIM2的方向：同DIR_TIM1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_dir_tim2(pwm_hw_t *hw)
{
    return hw->pwm_cr1.dir_tim2;
}

static inline void pwm_ll_set_pwm_cr1_dir_tim2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.dir_tim2 = value;
}

/* REG_0x04:pwm_cr1->DIR_TIM1:0x4[31],计数器TIM1的方向  0为向上计数  1为向下计数 （当工作在编码器模式或者中心对称模式时，该位为只读）,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr1_dir_tim1(pwm_hw_t *hw)
{
    return hw->pwm_cr1.dir_tim1;
}

static inline void pwm_ll_set_pwm_cr1_dir_tim1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr1.dir_tim1 = value;
}

/* REG_0x05 */

static inline uint32_t pwm_ll_get_pwm_cr2_value(pwm_hw_t *hw)
{
    return hw->pwm_cr2.v;
}

static inline void pwm_ll_set_pwm_cr2_value(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.v = value;
}

/* REG_0x05:pwm_cr2->TI1S:0x5[8],TIM1的触发输入源选择; 0 cap_i[0]作为TI1的输入; 1 cap_i[2:0]的XOR作为TI1的输入，霍尔传感器接口模式用,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_ti1s(pwm_hw_t *hw)
{
    return hw->pwm_cr2.ti1s;
}

static inline void pwm_ll_set_pwm_cr2_ti1s(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.ti1s = value;
}

/* REG_0x05:pwm_cr2->CCPC:0x5[9],CCE，CCNE bit preload   ; 该bi为1时：CCE，CCEN只有在COM事件（霍尔模式时TRGI上升沿）发生时才更新 ,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_ccpc(pwm_hw_t *hw)
{
    return hw->pwm_cr2.ccpc;
}

static inline void pwm_ll_set_pwm_cr2_ccpc(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.ccpc = value;
}

/* REG_0x05:pwm_cr2->TRIOS3:0x5[12:10],TIM3的触发输出选择 同TRIOS1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_trios3(pwm_hw_t *hw)
{
    return hw->pwm_cr2.trios3;
}

static inline void pwm_ll_set_pwm_cr2_trios3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.trios3 = value;
}

/* REG_0x05:pwm_cr2->ADCS3:0x5[15:13],TIM3的ADC触发选择  同ADCS1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_adcs3(pwm_hw_t *hw)
{
    return hw->pwm_cr2.adcs3;
}

static inline void pwm_ll_set_pwm_cr2_adcs3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.adcs3 = value;
}

/* REG_0x05:pwm_cr2->ccus:0x5[17], ,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_ccus(pwm_hw_t *hw)
{
    return hw->pwm_cr2.ccus;
}

static inline void pwm_ll_set_pwm_cr2_ccus(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.ccus = value;
}

/* REG_0x05:pwm_cr2->TRIOS2:0x5[20:18],TIM2的触发输出选择 同TRIOS1,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_trios2(pwm_hw_t *hw)
{
    return hw->pwm_cr2.trios2;
}

static inline void pwm_ll_set_pwm_cr2_trios2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.trios2 = value;
}

/* REG_0x05:pwm_cr2->ADCS2:0x5[23:21],TIM2的ADC触发选择  同ADCS1 ,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_adcs2(pwm_hw_t *hw)
{
    return hw->pwm_cr2.adcs2;
}

static inline void pwm_ll_set_pwm_cr2_adcs2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.adcs2 = value;
}

/* REG_0x05:pwm_cr2->TRIOS1:0x5[28:26],TIM1的触发输出选择 ; 0 触发输出0; 1 CEN作为TRIO触发输出; 2 UG作为TRIO触发输出; 3 update事件触发输出; 4 OC1REF作为TRIO触发输出; 5 CCR1比较成功信号cc1g作为TRIO触发输出; 6 CCR2比较成功信号cc2g作为TRIO触发输出; 7 CCR3比较成功信号cc3g作为TRIO触发输出; 8 OC1REF作为TRIO触发输出; 9 CCIF信号即（比较或者捕获成功）作为TRIO触发输出,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_trios1(pwm_hw_t *hw)
{
    return hw->pwm_cr2.trios1;
}

static inline void pwm_ll_set_pwm_cr2_trios1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.trios1 = value;
}

/* REG_0x05:pwm_cr2->ADCS1:0x5[31:29],TIM1的触发ADC选择 ; 0 触发ADC0; 1 CEN作为TRIO触发ADC; 2 UG作为TRIO触发ADC; 3 update事件触发ADC; 4 OC1REF作为TRIO触发ADC; 5 CCR1比较成功信号cc1g作为TRIO触发ADC; 6 CCR2比较成功信号cc2g作为TRIO触发ADC; 7 CCR3比较成功信号cc3g作为TRIO触发ADC; 8 OC1REF作为TRIO触发ADC; 9 CCIF信号即（比较或者捕获成功）作为TRIO触发ADC,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_cr2_adcs1(pwm_hw_t *hw)
{
    return hw->pwm_cr2.adcs1;
}

static inline void pwm_ll_set_pwm_cr2_adcs1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_cr2.adcs1 = value;
}

/* REG_0x06 */

static inline uint32_t pwm_ll_get_smcr_value(pwm_hw_t *hw)
{
    return hw->smcr.v;
}

static inline void pwm_ll_set_smcr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->smcr.v = value;
}

/* REG_0x06:smcr->SMS1:0x6[3:0],TIM1从模式选择：; 0000 从模式关闭  定时器对内部时钟沿计数; 0001 LED驱动模式; 0010 编码器接口模式1  只对TI1的边沿向上下计数; 0011 编码器接口模式2  只对TI2的边沿向上下计数; 0100 编码器接口模式3  对TI1和TI2的边沿都向上下计数; 101 重置模式1   TRGI2的上升沿UG1置1，定时器重置（捕获模式时可使用）; 110 门控模式   TRGI2为高电平CEN1置1，为低CEN1清零（模块外部定时器控制TIM1启停）; 111 触发模式   TRGI2的上升沿CEN1置1（对外部脉冲计数）; 1000 霍尔传感器接口模式,0x0,R/W*/
static inline uint32_t pwm_ll_get_smcr_sms1(pwm_hw_t *hw)
{
    return hw->smcr.sms1;
}

static inline void pwm_ll_set_smcr_sms1(pwm_hw_t *hw, uint32_t value)
{
    hw->smcr.sms1 = value;
}

/* REG_0x06:smcr->SMS2:0x6[7:4],TIM2从模式选择：; 000 从模式关闭  定时器对内部时钟沿计数; 001 LED驱动模式; 010 编码器接口模式1  只对TI3的边沿向上下计数; 011 编码器接口模式2  只对TI4的边沿向上下计数; 100 编码器接口模式3  对TI3和TI4的边沿都向上下计数; 101 重置模式1   TRGI2的上升沿UG2置1，定时器重置（捕获模式时可使用）; 110 门控模式   TRGI2为高电平CEN2置1，为低CEN2清零（模块外部定时器控制TIM3启停）; 111 触发模式   TRGI2的上升沿CEN2置1（对外部脉冲计数）,0x0,R/W*/
static inline uint32_t pwm_ll_get_smcr_sms2(pwm_hw_t *hw)
{
    return hw->smcr.sms2;
}

static inline void pwm_ll_set_smcr_sms2(pwm_hw_t *hw, uint32_t value)
{
    hw->smcr.sms2 = value;
}

/* REG_0x06:smcr->SMS3:0x6[11:8],TIM3从模式选择：; 000 从模式关闭  定时器对内部时钟沿计数; 001 LED驱动模式; 010 编码器接口模式1  只对TI5的边沿向上下计数; 011 编码器接口模式2  只对TI6的边沿向上下计数; 100 编码器接口模式3  对TI5和TI6的边沿都向上下计数; 101 重置模式1   TRGI3的上升沿UG3置1，定时器重置（捕获模式时可使用）; 110 门控模式   TRGI3为高电平CEN3置1，为低CEN3清零（模块外部定时器控制TIM3启停）; 111 触发模式   TRGI3的上升沿CEN3置1（对外部脉冲计数）,0x0,R/W*/
static inline uint32_t pwm_ll_get_smcr_sms3(pwm_hw_t *hw)
{
    return hw->smcr.sms3;
}

static inline void pwm_ll_set_smcr_sms3(pwm_hw_t *hw, uint32_t value)
{
    hw->smcr.sms3 = value;
}

/* REG_0x06:smcr->TS1:0x6[14:12],TIM1触发输入源选择：; 000 TI1P TI1的单边沿检测脉冲; 001 TI2P TI2的单边沿检测脉冲; 010 TI1_ED  TI1的双边沿检测脉冲; 010 TI2_ED  TI2的双边沿检测脉冲; 101 tri_i[2] ; 101 tri_i[1] ; 110 tri_i[0] ,0x0,R/W*/
static inline uint32_t pwm_ll_get_smcr_ts1(pwm_hw_t *hw)
{
    return hw->smcr.ts1;
}

static inline void pwm_ll_set_smcr_ts1(pwm_hw_t *hw, uint32_t value)
{
    hw->smcr.ts1 = value;
}

/* REG_0x06:smcr->TS2:0x6[18:16],TIM2触发输入源选择：; 000 TI3P TI3的单边沿检测脉冲; 001 TI4P TI4的单边沿检测脉冲; 010 TI3_ED  TI3的双边沿检测脉冲; 011 TI4_ED  TI4的双边沿检测脉冲; 100 tri_i[2] ; 101 tri_i[1] ; 110 tri_i[0] ,0x0,R/W*/
static inline uint32_t pwm_ll_get_smcr_ts2(pwm_hw_t *hw)
{
    return hw->smcr.ts2;
}

static inline void pwm_ll_set_smcr_ts2(pwm_hw_t *hw, uint32_t value)
{
    hw->smcr.ts2 = value;
}

/* REG_0x06:smcr->TS3:0x6[21:19],TIM3触发输入源选择：; 000 TI5P TI5的单边沿检测脉冲; 001 TI6P TI6的单边沿检测脉冲; 010 TI5_ED  TI3的双边沿检测脉冲; 011 TI6_ED  TI4的双边沿检测脉冲; 100 tri_i[2] ; 101 tri_i[1] ; 110 tri_i[0] ,0x0,R/W*/
static inline uint32_t pwm_ll_get_smcr_ts3(pwm_hw_t *hw)
{
    return hw->smcr.ts3;
}

static inline void pwm_ll_set_smcr_ts3(pwm_hw_t *hw, uint32_t value)
{
    hw->smcr.ts3 = value;
}

/* REG_0x07 */

static inline uint32_t pwm_ll_get_pwm_dier_value(pwm_hw_t *hw)
{
    return hw->pwm_dier.v;
}

static inline void pwm_ll_set_pwm_dier_value(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.v = value;
}

/* REG_0x07:pwm_dier->CC1IE:0x7[0],CCR1比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc1ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc1ie;
}

static inline void pwm_ll_set_pwm_dier_cc1ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc1ie = value;
}

/* REG_0x07:pwm_dier->CC2IE:0x7[1],CCR2比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc2ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc2ie;
}

static inline void pwm_ll_set_pwm_dier_cc2ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc2ie = value;
}

/* REG_0x07:pwm_dier->CC3IE:0x7[2],CCR3比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc3ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc3ie;
}

static inline void pwm_ll_set_pwm_dier_cc3ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc3ie = value;
}

/* REG_0x07:pwm_dier->CC4IE:0x7[3],CCR4比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc4ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc4ie;
}

static inline void pwm_ll_set_pwm_dier_cc4ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc4ie = value;
}

/* REG_0x07:pwm_dier->CC5IE:0x7[4],CCR5比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc5ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc5ie;
}

static inline void pwm_ll_set_pwm_dier_cc5ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc5ie = value;
}

/* REG_0x07:pwm_dier->CC6IE:0x7[5],CCR6比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc6ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc6ie;
}

static inline void pwm_ll_set_pwm_dier_cc6ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc6ie = value;
}

/* REG_0x07:pwm_dier->CC7IE:0x7[6],CCR7比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc7ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc7ie;
}

static inline void pwm_ll_set_pwm_dier_cc7ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc7ie = value;
}

/* REG_0x07:pwm_dier->CC8IE:0x7[7],CCR8比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc8ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc8ie;
}

static inline void pwm_ll_set_pwm_dier_cc8ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc8ie = value;
}

/* REG_0x07:pwm_dier->CC9IE:0x7[8],CCR9比较成功中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_cc9ie(pwm_hw_t *hw)
{
    return hw->pwm_dier.cc9ie;
}

static inline void pwm_ll_set_pwm_dier_cc9ie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.cc9ie = value;
}

/* REG_0x07:pwm_dier->UIE1:0x7[9],TIM1更新事件中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_uie1(pwm_hw_t *hw)
{
    return hw->pwm_dier.uie1;
}

static inline void pwm_ll_set_pwm_dier_uie1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.uie1 = value;
}

/* REG_0x07:pwm_dier->UIE2:0x7[10],TIM2更新事件中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_uie2(pwm_hw_t *hw)
{
    return hw->pwm_dier.uie2;
}

static inline void pwm_ll_set_pwm_dier_uie2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.uie2 = value;
}

/* REG_0x07:pwm_dier->UIE3:0x7[11],TIM3更新事件中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_uie3(pwm_hw_t *hw)
{
    return hw->pwm_dier.uie3;
}

static inline void pwm_ll_set_pwm_dier_uie3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.uie3 = value;
}

/* REG_0x07:pwm_dier->TIE1:0x7[12],触发输入中断使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_tie1(pwm_hw_t *hw)
{
    return hw->pwm_dier.tie1;
}

static inline void pwm_ll_set_pwm_dier_tie1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.tie1 = value;
}

/* REG_0x07:pwm_dier->TIE2:0x7[13],触发输入DMA请求使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_tie2(pwm_hw_t *hw)
{
    return hw->pwm_dier.tie2;
}

static inline void pwm_ll_set_pwm_dier_tie2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.tie2 = value;
}

/* REG_0x07:pwm_dier->TIE3:0x7[14],CCR1比较成功DMA请求使能  0 disable  1 enable,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_tie3(pwm_hw_t *hw)
{
    return hw->pwm_dier.tie3;
}

static inline void pwm_ll_set_pwm_dier_tie3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.tie3 = value;
}

/* REG_0x07:pwm_dier->COMIE:0x7[15],COM事件中断使能  0 disable  1 enble （霍尔传感器模式用）,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_comie(pwm_hw_t *hw)
{
    return hw->pwm_dier.comie;
}

static inline void pwm_ll_set_pwm_dier_comie(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.comie = value;
}

/* REG_0x07:pwm_dier->df_tim1:0x7[23:20],数字滤波选择,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_df_tim1(pwm_hw_t *hw)
{
    return hw->pwm_dier.df_tim1;
}

static inline void pwm_ll_set_pwm_dier_df_tim1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.df_tim1 = value;
}

/* REG_0x07:pwm_dier->df_tim2:0x7[27:24], ,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_df_tim2(pwm_hw_t *hw)
{
    return hw->pwm_dier.df_tim2;
}

static inline void pwm_ll_set_pwm_dier_df_tim2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.df_tim2 = value;
}

/* REG_0x07:pwm_dier->df_tim3:0x7[31:28], ,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_dier_df_tim3(pwm_hw_t *hw)
{
    return hw->pwm_dier.df_tim3;
}

static inline void pwm_ll_set_pwm_dier_df_tim3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_dier.df_tim3 = value;
}

/* REG_0x08 */

static inline void pwm_ll_set_pwm_sr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.v = value;
}

/* REG_0x08:pwm_sr->CC1IF:0x8[0],CCR1比较成功中断标志位  ; 硬件置位：输出模式时比较成功时置位。对于双向计数模式有三种选项在CMS_TIM1中设定。 ; 输入模式时当TIM1的值被CCR1捕获时置位。; 软件清零,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc1if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc1if = value;
}

/* REG_0x08:pwm_sr->CC2IF:0x8[1],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc2if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc2if = value;
}

/* REG_0x08:pwm_sr->CC3IF:0x8[2],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc3if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc3if = value;
}

/* REG_0x08:pwm_sr->CC4IF:0x8[3],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc4if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc4if = value;
}

/* REG_0x08:pwm_sr->CC5IF:0x8[4],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc5if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc5if = value;
}

/* REG_0x08:pwm_sr->CC6IF:0x8[5],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc6if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc6if = value;
}

/* REG_0x08:pwm_sr->CC7IF:0x8[6],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc7if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc7if = value;
}

/* REG_0x08:pwm_sr->CC8IF:0x8[7],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc8if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc8if = value;
}

/* REG_0x08:pwm_sr->CC9IF:0x8[8],见CC1IF,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_cc9if(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.cc9if = value;
}

/* REG_0x08:pwm_sr->UIF1:0x8[9],TIM1更新事件中断标志位; 硬件置位：1.当计数器向上溢出或者向下溢出时置位;          2.当UG1位被软件置1或者被触发信号置1时置位  ; 软件清零,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_uif1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.uif1 = value;
}

/* REG_0x08:pwm_sr->UIF2:0x8[10],见UIF1,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_uif2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.uif2 = value;
}

/* REG_0x08:pwm_sr->UIF3:0x8[11],见UIF1,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_uif3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.uif3 = value;
}

/* REG_0x08:pwm_sr->TIF1:0x8[12],触发中断标志位 ; 硬件置位：当工作在触发输入模式时，TRGI1的上升沿置位。; 软件清零,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_tif1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.tif1 = value;
}

/* REG_0x08:pwm_sr->TIF2:0x8[13],触发中断标志位 ; 硬件置位：当工作在触发输入模式时，TRGI2的上升沿置位。; 软件清零,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_tif2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.tif2 = value;
}

/* REG_0x08:pwm_sr->TIF3:0x8[14],触发中断标志位 ; 硬件置位：当工作在触发输入模式时，TRGI3的上升沿置位。; 软件清零,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_tif3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.tif3 = value;
}

/* REG_0x08:pwm_sr->COMIF:0x8[15],COM事件中断标志位（霍尔传感器模式用）; 硬件置位：; 软件清零,0x0,W1C*/
static inline void pwm_ll_set_pwm_sr_comif(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_sr.comif = value;
}

/* REG_0x09 */

static inline void pwm_ll_set_pwm_edtr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.v = value;
}

/* REG_0x09:pwm_edtr->UG1:0x9[9],软件产生更新事件 TIM1,0x0,W*/
static inline void pwm_ll_set_pwm_edtr_ug1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.ug1 = value;
}

/* REG_0x09:pwm_edtr->UG2:0x9[10],软件产生更新事件 TIM2,0x0,W*/
static inline void pwm_ll_set_pwm_edtr_ug2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.ug2 = value;
}

/* REG_0x09:pwm_edtr->UG3:0x9[11],软件产生更新事件 TIM3,0x0,W*/
static inline void pwm_ll_set_pwm_edtr_ug3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.ug3 = value;
}

/* REG_0x09:pwm_edtr->TG1:0x9[12],触发事件产生  软件/硬件置位  硬件自动清零,0x0,W*/
static inline void pwm_ll_set_pwm_edtr_tg1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.tg1 = value;
}

/* REG_0x09:pwm_edtr->TG2:0x9[13],触发事件产生  软件/硬件置位  硬件自动清零,0x0,W*/
static inline void pwm_ll_set_pwm_edtr_tg2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.tg2 = value;
}

/* REG_0x09:pwm_edtr->TG3:0x9[14],触发事件产生  软件/硬件置位  硬件自动清零,0x0,W*/
static inline void pwm_ll_set_pwm_edtr_tg3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.tg3 = value;
}

/* REG_0x09:pwm_edtr->COMG:0x9[15],COM事件产生   硬件置位  硬件自动清零,0x0,W*/
static inline void pwm_ll_set_pwm_edtr_comg(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.comg = value;
}

/* REG_0x09:pwm_edtr->DTM1:0x9[17:16],CH1和CH2的死区时间模式：; 00 OCP在ocref上升沿延迟，OCN在ocref下降沿延迟; 01 OCN在ocref上升沿延迟，OCP在ocref下降沿延迟; 10 OCP在ocref上升沿延迟，OCP在ocref下降沿延迟; 11 OCN在ocref上升沿延迟，OCN在ocref下降沿延迟,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_edtr_dtm1(pwm_hw_t *hw)
{
    return hw->pwm_edtr.dtm1;
}

static inline void pwm_ll_set_pwm_edtr_dtm1(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.dtm1 = value;
}

/* REG_0x09:pwm_edtr->DTM2:0x9[19:18],CH3和CH4的死区时间模式：,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_edtr_dtm2(pwm_hw_t *hw)
{
    return hw->pwm_edtr.dtm2;
}

static inline void pwm_ll_set_pwm_edtr_dtm2(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.dtm2 = value;
}

/* REG_0x09:pwm_edtr->DTM3:0x9[21:20],CH5和CH6的死区时间模式：,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_edtr_dtm3(pwm_hw_t *hw)
{
    return hw->pwm_edtr.dtm3;
}

static inline void pwm_ll_set_pwm_edtr_dtm3(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_edtr.dtm3 = value;
}

/* REG_0x0a */

static inline uint32_t pwm_ll_get_pwm_ccmr_value(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.v;
}

static inline void pwm_ll_set_pwm_ccmr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.v = value;
}

/* REG_0x0a:pwm_ccmr->CH1P:0xa[1:0],TI1的输入极性: ; 00 上升沿敏感(捕获/触发) TI1P不反向(encoder mode); 01 下降沿敏感（捕获/触发）  同时TI1P反向（encoder mode）; 10 上升下降沿都敏感（捕获/触发）,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch1p(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch1p;
}

static inline void pwm_ll_set_pwm_ccmr_ch1p(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch1p = value;
}

/* REG_0x0a:pwm_ccmr->CH2P:0xa[3:2],见CH1P,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch2p(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch2p;
}

static inline void pwm_ll_set_pwm_ccmr_ch2p(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch2p = value;
}

/* REG_0x0a:pwm_ccmr->CH3P:0xa[5:4],见CH1P,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch3p(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch3p;
}

static inline void pwm_ll_set_pwm_ccmr_ch3p(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch3p = value;
}

/* REG_0x0a:pwm_ccmr->CH4P:0xa[7:6],见CH1P,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch4p(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch4p;
}

static inline void pwm_ll_set_pwm_ccmr_ch4p(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch4p = value;
}

/* REG_0x0a:pwm_ccmr->CH5P:0xa[9:8],见CH1P,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch5p(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch5p;
}

static inline void pwm_ll_set_pwm_ccmr_ch5p(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch5p = value;
}

/* REG_0x0a:pwm_ccmr->CH6P:0xa[11:10],见CH1P,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch6p(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch6p;
}

static inline void pwm_ll_set_pwm_ccmr_ch6p(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch6p = value;
}

/* REG_0x0a:pwm_ccmr->CH1E:0xa[12],CH1E通道输入/输出使能,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch1e(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch1e;
}

static inline void pwm_ll_set_pwm_ccmr_ch1e(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch1e = value;
}

/* REG_0x0a:pwm_ccmr->CH2E:0xa[13],见CH1E,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch2e(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch2e;
}

static inline void pwm_ll_set_pwm_ccmr_ch2e(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch2e = value;
}

/* REG_0x0a:pwm_ccmr->CH3E:0xa[14],见CH1E,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch3e(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch3e;
}

static inline void pwm_ll_set_pwm_ccmr_ch3e(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch3e = value;
}

/* REG_0x0a:pwm_ccmr->CH4E:0xa[15],见CH1E,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch4e(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch4e;
}

static inline void pwm_ll_set_pwm_ccmr_ch4e(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch4e = value;
}

/* REG_0x0a:pwm_ccmr->CH5E:0xa[16],见CH1E,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch5e(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch5e;
}

static inline void pwm_ll_set_pwm_ccmr_ch5e(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch5e = value;
}

/* REG_0x0a:pwm_ccmr->CH6E:0xa[17],见CH1E,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_ch6e(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.ch6e;
}

static inline void pwm_ll_set_pwm_ccmr_ch6e(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.ch6e = value;
}

/* REG_0x0a:pwm_ccmr->TIM1CCM:0xa[18],TIM1的比较捕获模式: 0输出比较模式 1输入捕获模式,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_tim1ccm(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.tim1ccm;
}

static inline void pwm_ll_set_pwm_ccmr_tim1ccm(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.tim1ccm = value;
}

/* REG_0x0a:pwm_ccmr->TIM2CCM:0xa[19],见TIM1CCM,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_tim2ccm(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.tim2ccm;
}

static inline void pwm_ll_set_pwm_ccmr_tim2ccm(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.tim2ccm = value;
}

/* REG_0x0a:pwm_ccmr->TIM3CCM:0xa[20],见TIM1CCM,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_tim3ccm(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.tim3ccm;
}

static inline void pwm_ll_set_pwm_ccmr_tim3ccm(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.tim3ccm = value;
}

/* REG_0x0a:pwm_ccmr->OC1M:0xa[23:21],TIM1的比较输出模式选择：; 000：OC1REF不受计数值的影响保持不变; 001：OC1REF碰到AAR不翻转，碰到CCRx翻转; 010：OC1REF set 1; 011：OC1REF碰到CCRx时翻转，碰到AAR也翻转->ARR; 100：OC1REF set 0,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_oc1m(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.oc1m;
}

static inline void pwm_ll_set_pwm_ccmr_oc1m(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.oc1m = value;
}

/* REG_0x0a:pwm_ccmr->OC2M:0xa[26:24],见OC1M,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_oc2m(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.oc2m;
}

static inline void pwm_ll_set_pwm_ccmr_oc2m(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.oc2m = value;
}

/* REG_0x0a:pwm_ccmr->OC3M:0xa[29:27],见OC1M,0x0,R/W*/
static inline uint32_t pwm_ll_get_pwm_ccmr_oc3m(pwm_hw_t *hw)
{
    return hw->pwm_ccmr.oc3m;
}

static inline void pwm_ll_set_pwm_ccmr_oc3m(pwm_hw_t *hw, uint32_t value)
{
    hw->pwm_ccmr.oc3m = value;
}

/* REG_0x0b */

static inline uint32_t pwm_ll_get_tim1_cnt_value(pwm_hw_t *hw)
{
    return hw->tim1_cnt.v;
}

/* REG_0x0b:tim1_cnt->TIM1_CNT:0xb[31:0],TIM1的计数器回读值,0x0,RO*/
static inline uint32_t pwm_ll_get_tim1_cnt_tim1_cnt(pwm_hw_t *hw)
{
    return hw->tim1_cnt.v;
}

/* REG_0x0c */

static inline uint32_t pwm_ll_get_tim2_cnt_value(pwm_hw_t *hw)
{
    return hw->tim2_cnt.v;
}

/* REG_0x0c:tim2_cnt->TIM2_CNT:0xc[31:0],TIM2的计数器回读值,0x0,RO*/
static inline uint32_t pwm_ll_get_tim2_cnt_tim2_cnt(pwm_hw_t *hw)
{
    return hw->tim2_cnt.v;
}

/* REG_0x0d */

static inline uint32_t pwm_ll_get_tim3_cnt_value(pwm_hw_t *hw)
{
    return hw->tim3_cnt.v;
}

/* REG_0x0d:tim3_cnt->TIM3_CNT:0xd[31:0],TIM3的计数器回读值,0x0,RO*/
static inline uint32_t pwm_ll_get_tim3_cnt_tim3_cnt(pwm_hw_t *hw)
{
    return hw->tim3_cnt.v;
}

/* REG_0x0e */

static inline uint32_t pwm_ll_get_prescaler_value(pwm_hw_t *hw)
{
    return hw->prescaler.v;
}

static inline void pwm_ll_set_prescaler_value(pwm_hw_t *hw, uint32_t value)
{
    hw->prescaler.v = value;
}

/* REG_0x0e:prescaler->PSC1:0xe[7:0],TIM1的时钟预分频值 从1到256,0x0,R/W*/
static inline uint32_t pwm_ll_get_prescaler_psc1(pwm_hw_t *hw)
{
    return hw->prescaler.psc1;
}

static inline void pwm_ll_set_prescaler_psc1(pwm_hw_t *hw, uint32_t value)
{
    hw->prescaler.psc1 = value;
}

/* REG_0x0e:prescaler->PSC2:0xe[15:8],TIM2的时钟预分频值 从1到256,0x0,R/W*/
static inline uint32_t pwm_ll_get_prescaler_psc2(pwm_hw_t *hw)
{
    return hw->prescaler.psc2;
}

static inline void pwm_ll_set_prescaler_psc2(pwm_hw_t *hw, uint32_t value)
{
    hw->prescaler.psc2 = value;
}

/* REG_0x0e:prescaler->PSC3:0xe[23:16],TIM3的时钟预分频值 从1到256,0x0,R/W*/
static inline uint32_t pwm_ll_get_prescaler_psc3(pwm_hw_t *hw)
{
    return hw->prescaler.psc3;
}

static inline void pwm_ll_set_prescaler_psc3(pwm_hw_t *hw, uint32_t value)
{
    hw->prescaler.psc3 = value;
}

/* REG_0x0f */

static inline uint32_t pwm_ll_get_tim1_arr_value(pwm_hw_t *hw)
{
    return hw->tim1_arr.v;
}

static inline void pwm_ll_set_tim1_arr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->tim1_arr.v = value;
}

/* REG_0x0f:tim1_arr->TIM1_ARR:0xf[31:0],TIM1的定时器计数终点值preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_tim1_arr_tim1_arr(pwm_hw_t *hw)
{
    return hw->tim1_arr.v;
}

static inline void pwm_ll_set_tim1_arr_tim1_arr(pwm_hw_t *hw, uint32_t value)
{
    hw->tim1_arr.v = value;
}

/* REG_0x10 */

static inline uint32_t pwm_ll_get_tim2_arr_value(pwm_hw_t *hw)
{
    return hw->tim2_arr.v;
}

static inline void pwm_ll_set_tim2_arr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->tim2_arr.v = value;
}

/* REG_0x10:tim2_arr->TIM2_ARR:0x10[31:0],TIM2的定时器计数终点值preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_tim2_arr_tim2_arr(pwm_hw_t *hw)
{
    return hw->tim2_arr.v;
}

static inline void pwm_ll_set_tim2_arr_tim2_arr(pwm_hw_t *hw, uint32_t value)
{
    hw->tim2_arr.v = value;
}

/* REG_0x11 */

static inline uint32_t pwm_ll_get_tim3_arr_value(pwm_hw_t *hw)
{
    return hw->tim3_arr.v;
}

static inline void pwm_ll_set_tim3_arr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->tim3_arr.v = value;
}

/* REG_0x11:tim3_arr->TIM3_ARR:0x11[31:0],TIM3的定时器计数终点值preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_tim3_arr_tim3_arr(pwm_hw_t *hw)
{
    return hw->tim3_arr.v;
}

static inline void pwm_ll_set_tim3_arr_tim3_arr(pwm_hw_t *hw, uint32_t value)
{
    hw->tim3_arr.v = value;
}

/* REG_0x12 */

static inline uint32_t pwm_ll_get_tim1_rcr_value(pwm_hw_t *hw)
{
    return hw->tim1_rcr.v;
}

static inline void pwm_ll_set_tim1_rcr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->tim1_rcr.v = value;
}

/* REG_0x12:tim1_rcr->TIM1_RCR:0x12[31:0],TIM1的重复计数器设定值,在RCR+1次溢出后产生更新事件,0x0,R/W*/
static inline uint32_t pwm_ll_get_tim1_rcr_tim1_rcr(pwm_hw_t *hw)
{
    return hw->tim1_rcr.v;
}

static inline void pwm_ll_set_tim1_rcr_tim1_rcr(pwm_hw_t *hw, uint32_t value)
{
    hw->tim1_rcr.v = value;
}

/* REG_0x13 */

static inline uint32_t pwm_ll_get_tim2_rcr_value(pwm_hw_t *hw)
{
    return hw->tim2_rcr.v;
}

static inline void pwm_ll_set_tim2_rcr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->tim2_rcr.v = value;
}

/* REG_0x13:tim2_rcr->TIM2_RCR:0x13[31:0],TIM2的重复计数器设定值,0x0,R/W*/
static inline uint32_t pwm_ll_get_tim2_rcr_tim2_rcr(pwm_hw_t *hw)
{
    return hw->tim2_rcr.v;
}

static inline void pwm_ll_set_tim2_rcr_tim2_rcr(pwm_hw_t *hw, uint32_t value)
{
    hw->tim2_rcr.v = value;
}

/* REG_0x14 */

static inline uint32_t pwm_ll_get_tim3_rcr_value(pwm_hw_t *hw)
{
    return hw->tim3_rcr.v;
}

static inline void pwm_ll_set_tim3_rcr_value(pwm_hw_t *hw, uint32_t value)
{
    hw->tim3_rcr.v = value;
}

/* REG_0x14:tim3_rcr->TIM3_RCR:0x14[31:0],TIM3的重复计数器设定值,0x0,R/W*/
static inline uint32_t pwm_ll_get_tim3_rcr_tim3_rcr(pwm_hw_t *hw)
{
    return hw->tim3_rcr.v;
}

static inline void pwm_ll_set_tim3_rcr_tim3_rcr(pwm_hw_t *hw, uint32_t value)
{
    hw->tim3_rcr.v = value;
}

/* REG_0x15 */

static inline uint32_t pwm_ll_get_ccr1_value(pwm_hw_t *hw)
{
    return hw->ccr1.v;
}

static inline void pwm_ll_set_ccr1_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr1.v = value;
}

/* REG_0x15:ccr1->CCR1:0x15[31:0],比较捕获寄存器CCR1的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr1_ccr1(pwm_hw_t *hw)
{
    return hw->ccr1.v;
}

static inline void pwm_ll_set_ccr1_ccr1(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr1.v = value;
}

/* REG_0x16 */

static inline uint32_t pwm_ll_get_ccr2_value(pwm_hw_t *hw)
{
    return hw->ccr2.v;
}

static inline void pwm_ll_set_ccr2_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr2.v = value;
}

/* REG_0x16:ccr2->CCR2:0x16[31:0],比较捕获寄存器CCR2的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr2_ccr2(pwm_hw_t *hw)
{
    return hw->ccr2.v;
}

static inline void pwm_ll_set_ccr2_ccr2(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr2.v = value;
}

/* REG_0x17 */

static inline uint32_t pwm_ll_get_ccr3_value(pwm_hw_t *hw)
{
    return hw->ccr3.v;
}

static inline void pwm_ll_set_ccr3_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr3.v = value;
}

/* REG_0x17:ccr3->CCR3:0x17[31:0],比较捕获寄存器CCR3的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr3_ccr3(pwm_hw_t *hw)
{
    return hw->ccr3.v;
}

static inline void pwm_ll_set_ccr3_ccr3(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr3.v = value;
}

/* REG_0x18 */

static inline uint32_t pwm_ll_get_ccr4_value(pwm_hw_t *hw)
{
    return hw->ccr4.v;
}

static inline void pwm_ll_set_ccr4_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr4.v = value;
}

/* REG_0x18:ccr4->CCR4:0x18[31:0],比较捕获寄存器CCR4的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr4_ccr4(pwm_hw_t *hw)
{
    return hw->ccr4.v;
}

static inline void pwm_ll_set_ccr4_ccr4(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr4.v = value;
}

/* REG_0x19 */

static inline uint32_t pwm_ll_get_ccr5_value(pwm_hw_t *hw)
{
    return hw->ccr5.v;
}

static inline void pwm_ll_set_ccr5_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr5.v = value;
}

/* REG_0x19:ccr5->CCR5:0x19[31:0],比较捕获寄存器CCR5的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr5_ccr5(pwm_hw_t *hw)
{
    return hw->ccr5.v;
}

static inline void pwm_ll_set_ccr5_ccr5(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr5.v = value;
}

/* REG_0x1a */

static inline uint32_t pwm_ll_get_ccr6_value(pwm_hw_t *hw)
{
    return hw->ccr6.v;
}

static inline void pwm_ll_set_ccr6_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr6.v = value;
}

/* REG_0x1a:ccr6->CCR6:0x1a[31:0],比较捕获寄存器CCR6的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr6_ccr6(pwm_hw_t *hw)
{
    return hw->ccr6.v;
}

static inline void pwm_ll_set_ccr6_ccr6(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr6.v = value;
}

/* REG_0x1b */

static inline uint32_t pwm_ll_get_ccr7_value(pwm_hw_t *hw)
{
    return hw->ccr7.v;
}

static inline void pwm_ll_set_ccr7_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr7.v = value;
}

/* REG_0x1b:ccr7->CCR7:0x1b[31:0],比较捕获寄存器CCR7的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr7_ccr7(pwm_hw_t *hw)
{
    return hw->ccr7.v;
}

static inline void pwm_ll_set_ccr7_ccr7(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr7.v = value;
}

/* REG_0x1c */

static inline uint32_t pwm_ll_get_ccr8_value(pwm_hw_t *hw)
{
    return hw->ccr8.v;
}

static inline void pwm_ll_set_ccr8_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr8.v = value;
}

/* REG_0x1c:ccr8->CCR8:0x1c[31:0],比较捕获寄存器CCR8的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr8_ccr8(pwm_hw_t *hw)
{
    return hw->ccr8.v;
}

static inline void pwm_ll_set_ccr8_ccr8(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr8.v = value;
}

/* REG_0x1d */

static inline uint32_t pwm_ll_get_ccr9_value(pwm_hw_t *hw)
{
    return hw->ccr9.v;
}

static inline void pwm_ll_set_ccr9_value(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr9.v = value;
}

/* REG_0x1d:ccr9->CCR9:0x1d[31:0],比较捕获寄存器CCR9的preload寄存器,0x0,R/W*/
static inline uint32_t pwm_ll_get_ccr9_ccr9(pwm_hw_t *hw)
{
    return hw->ccr9.v;
}

static inline void pwm_ll_set_ccr9_ccr9(pwm_hw_t *hw, uint32_t value)
{
    hw->ccr9.v = value;
}

/* REG_0x1e */

static inline uint32_t pwm_ll_get_dt_value(pwm_hw_t *hw)
{
    return hw->dt.v;
}

static inline void pwm_ll_set_dt_value(pwm_hw_t *hw, uint32_t value)
{
    hw->dt.v = value;
}

/* REG_0x1e:dt->DT1:0x1e[9:0],out1与out0死区时间设置：0到0x400个周期,0x0,R/W*/
static inline uint32_t pwm_ll_get_dt_dt1(pwm_hw_t *hw)
{
    return hw->dt.dt1;
}

static inline void pwm_ll_set_dt_dt1(pwm_hw_t *hw, uint32_t value)
{
    hw->dt.dt1 = value;
}

/* REG_0x1e:dt->DT2:0x1e[19:10],out3与out2死区时间设置：0到0x400个周期,0x0,R/W*/
static inline uint32_t pwm_ll_get_dt_dt2(pwm_hw_t *hw)
{
    return hw->dt.dt2;
}

static inline void pwm_ll_set_dt_dt2(pwm_hw_t *hw, uint32_t value)
{
    hw->dt.dt2 = value;
}

/* REG_0x1e:dt->DT3:0x1e[29:20],out5与out4死区时间设置：0到0x400个周期,0x0,R/W*/
static inline uint32_t pwm_ll_get_dt_dt3(pwm_hw_t *hw)
{
    return hw->dt.dt3;
}

static inline void pwm_ll_set_dt_dt3(pwm_hw_t *hw, uint32_t value)
{
    hw->dt.dt3 = value;
}

/* REG_0x1f */

static inline uint32_t pwm_ll_get_tim1_arr_shad_value(pwm_hw_t *hw)
{
    return hw->tim1_arr_shad.v;
}

/* REG_0x1f:tim1_arr_shad->TIM1_ARR_shad:0x1f[31:0],TIM1的定时器计数终点值shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_tim1_arr_shad_tim1_arr_shad(pwm_hw_t *hw)
{
    return hw->tim1_arr_shad.v;
}

/* REG_0x20 */

static inline uint32_t pwm_ll_get_tim2_arr_shad_value(pwm_hw_t *hw)
{
    return hw->tim2_arr_shad.v;
}

/* REG_0x20:tim2_arr_shad->TIM2_ARR_shad:0x20[31:0],TIM2的定时器计数终点值shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_tim2_arr_shad_tim2_arr_shad(pwm_hw_t *hw)
{
    return hw->tim2_arr_shad.v;
}

/* REG_0x21 */

static inline uint32_t pwm_ll_get_tim3_arr_shad_value(pwm_hw_t *hw)
{
    return hw->tim3_arr_shad.v;
}

/* REG_0x21:tim3_arr_shad->TIM3_ARR_shad:0x21[31:0],TIM3的定时器计数终点值shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_tim3_arr_shad_tim3_arr_shad(pwm_hw_t *hw)
{
    return hw->tim3_arr_shad.v;
}

/* REG_0x22 */

static inline uint32_t pwm_ll_get_tim1_rcr_shad_value(pwm_hw_t *hw)
{
    return hw->tim1_rcr_shad.v;
}

/* REG_0x22:tim1_rcr_shad->TIM1_RCR_shad:0x22[31:0],TIM1的重复计数器设定值,在RCR+1次溢出后产生更新事件,0x0,RO*/
static inline uint32_t pwm_ll_get_tim1_rcr_shad_tim1_rcr_shad(pwm_hw_t *hw)
{
    return hw->tim1_rcr_shad.v;
}

/* REG_0x23 */

static inline uint32_t pwm_ll_get_tim2_rcr_shad_value(pwm_hw_t *hw)
{
    return hw->tim2_rcr_shad.v;
}

/* REG_0x23:tim2_rcr_shad->TIM2_RCR_shad:0x23[31:0],TIM2的重复计数器设定值,0x0,RO*/
static inline uint32_t pwm_ll_get_tim2_rcr_shad_tim2_rcr_shad(pwm_hw_t *hw)
{
    return hw->tim2_rcr_shad.v;
}

/* REG_0x24 */

static inline uint32_t pwm_ll_get_tim3_rcr_shad_value(pwm_hw_t *hw)
{
    return hw->tim3_rcr_shad.v;
}

/* REG_0x24:tim3_rcr_shad->TIM3_RCR_shad:0x24[31:0],TIM3的重复计数器设定值,0x0,RO*/
static inline uint32_t pwm_ll_get_tim3_rcr_shad_tim3_rcr_shad(pwm_hw_t *hw)
{
    return hw->tim3_rcr_shad.v;
}

/* REG_0x25 */

static inline uint32_t pwm_ll_get_ccr1_shad_value(pwm_hw_t *hw)
{
    return hw->ccr1_shad.v;
}

/* REG_0x25:ccr1_shad->CCR1_shad:0x25[31:0],比较捕获寄存器CCR1的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr1_shad_ccr1_shad(pwm_hw_t *hw)
{
    return hw->ccr1_shad.v;
}

/* REG_0x26 */

static inline uint32_t pwm_ll_get_ccr2_shad_value(pwm_hw_t *hw)
{
    return hw->ccr2_shad.v;
}

/* REG_0x26:ccr2_shad->CCR2_shad:0x26[31:0],比较捕获寄存器CCR2的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr2_shad_ccr2_shad(pwm_hw_t *hw)
{
    return hw->ccr2_shad.v;
}

/* REG_0x27 */

static inline uint32_t pwm_ll_get_ccr3_shad_value(pwm_hw_t *hw)
{
    return hw->ccr3_shad.v;
}

/* REG_0x27:ccr3_shad->CCR3_shad:0x27[31:0],比较捕获寄存器CCR3的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr3_shad_ccr3_shad(pwm_hw_t *hw)
{
    return hw->ccr3_shad.v;
}

/* REG_0x28 */

static inline uint32_t pwm_ll_get_ccr4_shad_value(pwm_hw_t *hw)
{
    return hw->ccr4_shad.v;
}

/* REG_0x28:ccr4_shad->CCR4_shad:0x28[31:0],比较捕获寄存器CCR4的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr4_shad_ccr4_shad(pwm_hw_t *hw)
{
    return hw->ccr4_shad.v;
}

/* REG_0x29 */

static inline uint32_t pwm_ll_get_ccr5_shad_value(pwm_hw_t *hw)
{
    return hw->ccr5_shad.v;
}

/* REG_0x29:ccr5_shad->CCR5_shad:0x29[31:0],比较捕获寄存器CCR5的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr5_shad_ccr5_shad(pwm_hw_t *hw)
{
    return hw->ccr5_shad.v;
}

/* REG_0x2a */

static inline uint32_t pwm_ll_get_ccr6_shad_value(pwm_hw_t *hw)
{
    return hw->ccr6_shad.v;
}

/* REG_0x2a:ccr6_shad->CCR6_shad:0x2a[31:0],比较捕获寄存器CCR6的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr6_shad_ccr6_shad(pwm_hw_t *hw)
{
    return hw->ccr6_shad.v;
}

/* REG_0x2b */

static inline uint32_t pwm_ll_get_ccr7_shad_value(pwm_hw_t *hw)
{
    return hw->ccr7_shad.v;
}

/* REG_0x2b:ccr7_shad->CCR7_shad:0x2b[31:0],比较捕获寄存器CCR7的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr7_shad_ccr7_shad(pwm_hw_t *hw)
{
    return hw->ccr7_shad.v;
}

/* REG_0x2c */

static inline uint32_t pwm_ll_get_ccr8_shad_value(pwm_hw_t *hw)
{
    return hw->ccr8_shad.v;
}

/* REG_0x2c:ccr8_shad->CCR8_shad:0x2c[31:0],比较捕获寄存器CCR8的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr8_shad_ccr8_shad(pwm_hw_t *hw)
{
    return hw->ccr8_shad.v;
}

/* REG_0x2d */

static inline uint32_t pwm_ll_get_ccr9_shad_value(pwm_hw_t *hw)
{
    return hw->ccr9_shad.v;
}

/* REG_0x2d:ccr9_shad->CCR9_shad:0x2d[31:0],比较捕获寄存器CCR9的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_ccr9_shad_ccr9_shad(pwm_hw_t *hw)
{
    return hw->ccr9_shad.v;
}

/* REG_0x2e */

static inline uint32_t pwm_ll_get_chxe_shad_value(pwm_hw_t *hw)
{
    return hw->chxe_shad.v;
}

/* REG_0x2e:chxe_shad->CH6e_shad:0x2e[0],channel使能寄存器ch6e的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_ch6e_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.ch6e_shad;
}

/* REG_0x2e:chxe_shad->CH5e_shad:0x2e[1],channel使能寄存器ch5e的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_ch5e_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.ch5e_shad;
}

/* REG_0x2e:chxe_shad->CH4e_shad:0x2e[2],channel使能寄存器ch4e的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_ch4e_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.ch4e_shad;
}

/* REG_0x2e:chxe_shad->CH3e_shad:0x2e[3],channel使能寄存器ch3e的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_ch3e_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.ch3e_shad;
}

/* REG_0x2e:chxe_shad->CH2e_shad:0x2e[4],channel使能寄存器ch2e的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_ch2e_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.ch2e_shad;
}

/* REG_0x2e:chxe_shad->CH1e_shad:0x2e[5],channel使能寄存器ch1e的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_ch1e_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.ch1e_shad;
}

/* REG_0x2e:chxe_shad->oc1m_shad:0x2e[8:6],TIM1输出比较模式寄存器oc1m的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_oc1m_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.oc1m_shad;
}

/* REG_0x2e:chxe_shad->oc2m_shad:0x2e[11:9],TIM2输出比较模式寄存器oc2m的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_oc2m_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.oc2m_shad;
}

/* REG_0x2e:chxe_shad->oc3m_shad:0x2e[14:12],TIM3输出比较模式寄存器oc3m的shadow寄存器,0x0,RO*/
static inline uint32_t pwm_ll_get_chxe_shad_oc3m_shad(pwm_hw_t *hw)
{
    return hw->chxe_shad.oc3m_shad;
}

#endif 

#ifdef __cplusplus 
}                  
#endif             
