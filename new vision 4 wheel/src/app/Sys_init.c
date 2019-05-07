#include "Sys_init.h"
#include "include.h"
#include <stdio.h>
u8 SWITCH_STATUS = 0;

InitParameter Initialization =                          //��ʼ������
{
  .MotorP = 900,
  .MotorI = 150,
  .MotorD = 10,
  .DiffP  = 300,
  .DiffI  = 0,
  .ServoP = 5,
  .ServoD = 10,
  .ServoDD = 25,
  .ElecP = 1.5,
  .ElecD =2,
  .Speedtarget = 150,
  .Cnst = 60,
  .Main_Line = 152,
  .EnableKEY = 1,
};


void System_init(void)
{
  DisableInterrupts;
  SYSMPU->CESR &= ~SYSMPU_CESR_VLD_MASK;
  Motor_init();
  Servo_init();
  LCD_init(1);
  Disp_single_colour(COLOR_WHITE);
  myBEEP_Init();
  myLED_Init();
  Switch_Init();
  UART_eDMA_init();
  parameter_Init();
  Ov7725_Init();
  pit_init_ms(PIT0,PIT_PERIOD);
  ADC0_Init();
  NVIC_SetPriority(DMA4_DMA20_IRQn,0);
  NVIC_SetPriority(VSYNC_IRQ,1);
  NVIC_SetPriority(PIT0_IRQn,2);
  EnableInterrupts;
  Camera_start();
}

void nrf_init_connect(void)
{
//  Link.NeedConnect=1;
//  Nrf_connect();
}
void parameter_Init(void)
{
//  for (int i = 80;i<200;i++)
//  {
//    ROAD.Width_stand[i] = (int)(2.0111f*i - 148.11f);
//    if(ROAD.Width_stand[i] < 60)
//      ROAD.Width_stand[i] = 60;
//    else if(ROAD.Width_stand[i] > 200)
//      ROAD.Width_stand[i] = 200;
//  }
  Main_Line = Initialization.Main_Line;
  ROADY.Three_Lie[0] = Center_Stand - 80;
  ROADY.Three_Lie[1] = Center_Stand;
  ROADY.Three_Lie[2] = Center_Stand + 80;
  All_Delay_Init();
}


void Switch_Init(void)
{
  GPIO_Init(SW1_PORT,GPI,HIGH);
  GPIO_Init(SW2_PORT,GPI,HIGH);
  GPIO_Init(SW3_PORT,GPI,HIGH);
  GPIO_Init(SW4_PORT,GPI,HIGH);
  delayms(30);
  SWITCH_STATUS = SW1|(SW2<<1)|(SW3<<2)|(SW4<<3);
  if(((SWITCH_STATUS>>0)&1)==1)
  {
    LCD_DISPLAY_FLAG = 1;
  }
  else if(((SWITCH_STATUS>>0)&1)==0)
  {
    LCD_DISPLAY_FLAG = 0;
  }
}