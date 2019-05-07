#include "event_delay.h"
#include "include.h"
int Test_delay = 0;


u8 Test_flag = 0;
//*******************************创建任务结构体***************************
/*  调用Time_delay_start(&STR)，只调用一次  */
Time_Delay_STR Test_Str;

//*******************************初始化任务结构体*************************
void All_Delay_Init(void)
{
  Test_Str.delay_const = 2000;
  Test_Str.task = &Test_task;
}

//*******************************调用延时程序*****************************
void All_Delay_Task(void)
{
  Time_delay(&Test_Str);
}

//*******************************延时后任务 (参数和返回值都为void)*******************************
void Test_task(void)
{
  Test_delay = 1000;
}

