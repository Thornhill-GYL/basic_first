/*
* pit.c
*
*  Created on: Feb 18, 2018
*      Author: ZBT
*/

#include "include.h"
#include "pit.h"

void pit_delay(PITn pitn, u32 cnt)
{
  SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //使能PIT时钟
  PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //使能PIT定时器时钟 ，调试模式下继续运行
  PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //禁用PIT ，以便设置加载值生效
  PIT_LDVAL(pitn)  = cnt - 1;                                     //设置溢出中断时间
  PIT_Flag_Clear(pitn);                                           //清中断标志位
  PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
  PIT_TCTRL(pitn)  = ( 0
                      | PIT_TCTRL_TEN_MASK                        //使能 PITn定时器
                        //| PIT_TCTRL_TIE_MASK                      //开PITn中断
                        );
  while( !(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK));
  PIT_Flag_Clear(pitn);                                           //清中断标志位
}

void pit_time_start(PITn pitn)
{
  SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                          //使能PIT时钟
  PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );    //使能PIT定时器时钟 ，调试模式下继续运行
  PIT_TCTRL(pitn) &= ~( PIT_TCTRL_TEN_MASK );                     //禁用PIT ，以便设置加载值生效
  PIT_LDVAL(pitn)  = ~0;                                          //设置溢出中断时间
  PIT_Flag_Clear(pitn);                                           //清中断标志位
  PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
  PIT_TCTRL(pitn)  = ( 0
                      | PIT_TCTRL_TEN_MASK                        //使能 PITn定时器
                        //| PIT_TCTRL_TIE_MASK                      //开PITn中断
                        );
}

u32 pit_time_get(PITn pitn)
{
  u32 val;
  val = (~0) - PIT_CVAL(pitn);
  if(PIT_TFLG(pitn)& PIT_TFLG_TIF_MASK)                           //判断是否时间超时
  {
    PIT_Flag_Clear(pitn);                                       //清中断标志位
    PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                    //禁止PITn定时器（用于清空计数值）
    return ~0;
  }
  if(val == (~0))
  {
    val--;              //确保 不等于 ~0
  }
  PIT_TCTRL(pitn) &= ~ PIT_TCTRL_TEN_MASK;                        //禁止PITn定时器（用于清空计数值）
  return val;
}

void pit_init(PITn pitn, u32 cnt)
{
  SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //使能PIT时钟
  PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //使能PIT定时器时钟 ，调试模式下继续运行
  PIT_LDVAL(pitn)  = cnt;                                           //设置溢出中断时间
  PIT_Flag_Clear(pitn);                                             //清中断标志位
  PIT_TCTRL(pitn) |= ( PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK );   //使能 PITn定时器,并开PITn中断
  NVIC_EnableIRQ((enum IRQn)(PIT0_IRQn + (unsigned char)pitn));     //开对应PIT中断
}
