#include "event_delay.h"
#include "include.h"
int Test_delay = 0;


u8 Test_flag = 0;
//*******************************��������ṹ��***************************
/*  ����Time_delay_start(&STR)��ֻ����һ��  */
Time_Delay_STR Test_Str;

//*******************************��ʼ������ṹ��*************************
void All_Delay_Init(void)
{
  Test_Str.delay_const = 2000;
  Test_Str.task = &Test_task;
}

//*******************************������ʱ����*****************************
void All_Delay_Task(void)
{
  Time_delay(&Test_Str);
}

//*******************************��ʱ������ (�����ͷ���ֵ��Ϊvoid)*******************************
void Test_task(void)
{
  Test_delay = 1000;
}

