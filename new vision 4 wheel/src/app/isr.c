/*
* isr.c
*
*  Created on: Feb 18, 2018
*      Author: ZBT
*/

#include "include.h"
#include "isr.h"
char run;
void Error_handler(unsigned char* Log)  //错误处理，ASSERT和HARDFAULT都会掉入此函数
{
  DisableInterrupts;
  //紧急处理
  MOTORL_DIR = 1;
  FTM_PWM_Duty(MOTORL_FTM,65535);
  MOTORR_DIR = 0;
  FTM_PWM_Duty(MOTORR_FTM,10);
  FTM_PWM_Duty(SERVO_FTM,SERVO_MIDDLE);
  while(1)//终止程序运行
  {
    LED_R = !LED_R;
    delayms(500);
  }
}

void HardFault_Handler(void)
{
  Error_handler("HardFault");
}

void PORTB_IRQHandler(void)
{
  u32 flag = PORTB_ISFR;
  PORTB_ISFR = ~0;
  if(flag & (1<<23))    //VSYNC
  {
    ov7725_get_img();
  }
}

void PIT0_IRQHandler(void)
{
  PIT_Flag_Clear(PIT0);
  static unsigned char TimeCnt_20ms = 0;
  static unsigned char TimeCnt_start = 0;
  static unsigned int  TimeCnt_1s = 0;
  TimeCnt_start++;
  TimeCnt_20ms++;
  TimeCnt_1s++;
  if(TimeCnt_20ms >= 20/PIT_PERIOD)
    TimeCnt_20ms = 0;
  if(TimeCnt_start>4)
  {
    TimeCnt_start=0;
    start_process=1;
  }
  if(TimeCnt_1s >= 1000/PIT_PERIOD)
    TimeCnt_1s = 0;
  Beep_ISR(&Beep_100ms);
  
  ADC_get_data();
  Motor_control();
  All_Delay_Task();
  //测试对管，之后删除即可。
  ad_breakage();
  
  //坡道
  READ_DATE();
  Filter_2nd_LPF2ndFilter();
  Measure_Acc();
  Angle_control();
  ramp_filter();
  navigation();
  if(TimeCnt_20ms==1)
  {
   
       if(AD_in==1&&move_info.normal_flag==0&&((SWITCH_STATUS>>1)&1)==0)
      {
          if(move_info.diff_flag==0)
          {
             move_info.start_out_flag=1;
          }
          else
             move_info.start_out_flag=0;
          if(((SWITCH_STATUS>>3)&1)==0)
              FTM_PWM_Duty(SERVO_FTM,SERVO_MIDDLE+540);
          else if(((SWITCH_STATUS>>3)&1)==1)
              FTM_PWM_Duty(SERVO_FTM,SERVO_MIDDLE-540);
       }
    
    else
      Dir_control();
  }
  
  if(run_flag==1)
  {
      if(((SWITCH_STATUS>>1)&1)==1)
     {
       if(start_info.final_stop==0)
          single_control();
       else
         single_start_move();
         
     }
     else if(((SWITCH_STATUS>>1)&1)==0)
     {
       if(start_info.final_stop==0)
         double_control();
       else
         double_start_move();
     }
     
  
  }
  else
    Speed.using_speed =0;
  
//  if(move_info.quit_safe!=1&&Block.State==NoBlock)//在断路过程中取消安全保护
//  {
//    keep_safe();
//  }
  if(Block.State==NoBlock)
  {
    keep_safe();
  }
  if(TimeCnt_1s==1)
  {
    OV7725_fps = OV7725_cnt;
    OV7725_cnt = 0;
  }
 
  UART_eDMA_datasend();
}


void DMA4_DMA20_IRQHandler(void)
{
  DMA_IRQ_CLEAN(CAMERA_DMA_CH);
  DMA_DIS(CAMERA_DMA_CH);
  Camera_start();
  Image_Flag = 1;
}

void DMA0_DMA16_IRQHandler(void)
{
  extern uart_edma_handle_t uart_eDMA_Handler;
  extern edma_handle_t g_uartTxEdmaHandle;
  DMA0->CINT = 16;
  DMA0->CDNE = 16;
  UART_EnableTxDMA(UART_DEBUG, false);
  EDMA_AbortTransfer(uart_eDMA_Handler.txEdmaHandle);
  uart_eDMA_Handler.txState = 0;
}

void UART0_RX_TX_IRQHandler(void)
{
  
  UART_Pend_Char(UART_2,&run);
//  if(run==0)
//    return;
  if(run=='a')
  {
    run_flag=1;
  }
  else if(run=='b')
  {
    run_flag=0;
  }
  if(run=='c')
  {
    move_info.go_back_flag=1;
  }
  if(run=='d')
  {
    start_info.twice_stop=1;
  }
  if(run=='e')
  {
    Block.State = Block_S1;
  }
}
void UART3_RX_TX_IRQHandler(void)
{
  TOF_Read();
}

