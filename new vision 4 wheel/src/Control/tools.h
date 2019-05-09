/*
 * tools.h
 *
 *  Created on: Feb 17, 2018
 *      Author: ZBT
 */

#ifndef CONTROL_TOOLS_H_
#define CONTROL_TOOLS_H_

typedef struct
{
  u8  flag;
  u8  dec;
  int cnt;
  int delay_const;
  void (*task)(void);
}Time_Delay_STR;


void Button_Handler(int n);
void UART_eDMA_init(void);
void Uart_make_data();
void UART_eDMA_datasend(void);
void Time_delay_start(Time_Delay_STR * delay);
void Time_delay(Time_Delay_STR * delay);

#endif /* CONTROL_TOOLS_H_ */
