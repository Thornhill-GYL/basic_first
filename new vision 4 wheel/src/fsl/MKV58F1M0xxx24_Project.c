/*
* Copyright (c) 2017, NXP Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of NXP Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
* @file    MKV58F1M0xxx24_Project.c
* @brief   Application entry point.
*/
#include <stdio.h>
#include "MKV58F24.h"
/* TODO: insert other include files here. */
#include "include.h"
/* TODO: insert other definitions and declarations here. */

/*
* @brief   Application entry point.
*/

void Sysclk_init(void)
{
  int PRDIV = 0,VDIV = 0;
  uint32 temp_reg;

  PRDIV = 2;
  VDIV = 14;

  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);     //选择外部时钟
  MCG_C5 = MCG_C5_PRDIV(PRDIV); //晶振为48M，分频结果范围要在8M~16M 此时为 48/(prdiv+1)
  temp_reg = FMC_PFAPR;

  //禁止预取
  FMC_PFAPR |= FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
    | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
  //MCG=PLL, system (CPU) clock= MCG,
  SIM_CLKDIV1 =  SIM_CLKDIV1_OUTDIV1(0)    //system (CPU) clock		//240
               | SIM_CLKDIV1_OUTDIV2(1)    //Fast Peripheral clock	//120
               | SIM_CLKDIV1_OUTDIV3(3)    //Flexbus clock		//60
               | SIM_CLKDIV1_OUTDIV4(9);   //Bus /Flash clock		//24
  //存FMC_PFAPR原始值
  FMC_PFAPR = temp_reg;
  MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(VDIV);
                                                //PLL =  48M* (VIDV+16)/(PRDIV+1) /2
  while (!(MCG_S & MCG_S_PLLST_MASK)){};        //等待PLL输出被选中
  while (!(MCG_S & MCG_S_LOCK0_MASK)){};        //等待PLL锁定

  MCG_C1=0x00;
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};   //等待MCGOUTCLK切换到PLL输出

  core_clk_khz = OSC_FREQ_MHZ*1000*(VDIV+16)/(PRDIV+1)/2;
  fastperipheral_clk_khz = core_clk_khz/2;
  flexbus_clk_khz = core_clk_khz/4;
  bus_clk_khz = core_clk_khz/10;
  
  SIM_SCGC5 |=
    (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK
     | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);    //开全部端口时钟
}

int main(void) {
  /* Init board hardware. */
  Sysclk_init();
  System_init();
  Setup();
  while(1) {
    Loop();
  }
}