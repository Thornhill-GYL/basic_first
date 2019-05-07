/*
* isr.c
*
*  Created on: Feb 18, 2018
*      Author: ZBT
*/

#include "include.h"
#include "isr.h"
char run;
void Error_handler(unsigned char* Log)  //������ASSERT��HARDFAULT�������˺���
{
  DisableInterrupts;
  //��������
  MOTORL_DIR = 1;
  FTM_PWM_Duty(MOTORL_FTM,65535);
  MOTORR_DIR = 0;
  FTM_PWM_Duty(MOTORR_FTM,10);
  FTM_PWM_Duty(SERVO_FTM,SERVO_MIDDLE);
  while(1)//��ֹ��������
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
  TimeCnt_start++;
  TimeCnt_20ms++;
  if(TimeCnt_20ms >= 20/PIT_PERIOD)
    TimeCnt_20ms = 0;
  if(TimeCnt_start>4)
  {
    TimeCnt_start=0;
    start_process=1;
  }
  Beep_ISR(&Beep_100ms);
  All_Delay_Task();
  ADC_get_data();
  Motor_control();
  if(TimeCnt_20ms==1)
  {
    Dir_control();
  }
  if(Test_flag == 1)
  {
    Time_delay_start(&Test_Str);
    Test_flag = 2;
  }
  if(run_flag==1)
  {
      if(((SWITCH_STATUS>>1)&1)==1)
     {
       single_control();
     }
     else if(((SWITCH_STATUS>>1)&1)==0)
     {
       double_control();
     }
     
  
  }
  else
    Speed.using_speed =0;
  if((Dir.Elec_Left<300 && Dir.Elec_Right<300))
  {
    FTM_PWM_Duty(SERVO_FTM,SERVO_MIDDLE);
    Speed.using_speed = 0;
//    Dir.PID_D.UP_Limit = 0;
//    Dir.PID_D.DOWN_Limit = 0;
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

void UART2_RX_TX_IRQHandler(void)
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
}

