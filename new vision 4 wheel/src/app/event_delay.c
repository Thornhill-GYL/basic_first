#include "event_delay.h"
#include "include.h"



//*******************************创建任务结构体***************************
/*  调用Time_delay_start(&STR)，只调用一次  */


//*******************************初始化任务结构体*************************
void All_Delay_Init(void)
{
  Island.Out2doubleFAR.task = &Out2doubleFAR_task;
  Island.Next_Island.task   = &Next_Island_task;
  Island.ADdouble.task      = &ADdouble_task;
  Island.Stay2Out.task      = &Stay2Out_task;
}

//*******************************调用延时程序*****************************
void All_Delay_Task(void)
{
  Time_delay(&Island.Out2doubleFAR);
  Time_delay(&Island.Next_Island);
  Time_delay(&Island.ADdouble);
  Time_delay(&Island.Stay2Out);
}

//*******************************延时后任务 (参数和返回值都为void)*******************************


void Out2doubleFAR_task(void)
{
  Island.doubleFAR_Allow_flag = 1;
}

void Next_Island_task(void)
{
  Island.State = NoIsland;//清标志
  Island.In_Center = 0;
  Island.Stay_Center = 0;
  Island.Out_Center_ = 0;
  Island.Out_Center  = 0;
  Island.Stay_Island_center_cnt = 0;
  Island.Out_Allow_flag = 0;
  Island.doubleFAR_Allow_flag = 0;
  Island.Stay2Out_cnt = 0;
  Island.In2Stay_cnt = 0;
  LED_NO;
}

void ADdouble_task(void)
{
  
}

void Stay2Out_task(void)
{
  Island.Out_Allow_flag = 1;
}