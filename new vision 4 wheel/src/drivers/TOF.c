#include "include.h"
#include "TOF.h"

u8   TOF_Finish_flag = 0;
int  TOF_Read_cnt = 0;;
char TOF_BUF[16];
int  TOF_Distance;


void TOF_Read(void)//串口中断里
{
  char Res = 0;
  int i = 0;
  int UART_data = 0;
  UART_Pend_Char(UART_TOF,&Res);
  
  if(TOF_Read_cnt>2
     &&TOF_BUF[TOF_Read_cnt-1]=='m'
     &&TOF_BUF[TOF_Read_cnt]=='m')
  {
    UART_data = 0;
    while(TOF_BUF[i]!='m')
    {
      if(TOF_BUF[i]<'0'||TOF_BUF[i]>'9'||i>=10)
      {
        i = 0;                              //数据错误，本次退出
        return;
      }
      UART_data = UART_data * 10 + (TOF_BUF[i++] - 0x30);
    }
    TOF_Distance = UART_data;
  }
  if(Res=='\n')
  {
    TOF_Read_cnt = 0;
  }
  else
  {
    if((Res>='0'&&Res<='9')
       ||Res=='m'
       ||Res=='\r'
       ||Res=='\n')
    {
      TOF_BUF[TOF_Read_cnt++] = Res;
      
    }
  }
  
  
  if(TOF_Read_cnt>15)
    TOF_Read_cnt = 0;
  
}
