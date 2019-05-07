#ifndef _UART_H_
#define _UART_H_

typedef enum
{
    UART_0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_MAX,
} UARTn_e;

extern UART_MemMapPtr UARTN[UART_MAX];

/*********************** UART功能函数 **************************/
//初始化
extern void Uart_Init      (UARTn_e, uint32 baud);             //初始化uartx模块

//接收相关代码
extern  char UART_Get_Char (UARTn_e uratn);                //等待接受1个字节
extern  char UART_Pend_Char (UARTn_e uratn, char *ch);
extern  char UART_Query_Char (UARTn_e, char *ch);                //查询接收1个字符
extern  char UART_Query_Str  (UARTn_e, char *str, uint32 max_len); //查询接收字符串
extern  char UART_Query    (UARTn_e);                          //查询是否接收到一个字节
//发送相关代码
extern void UART_Put_Char   (UARTn_e, char ch);                 //发送1个字节
extern void UART_Put_Buff   (UARTn_e , s8 *buff, uint32 len);//发送len个字节buff
extern void UART_Put_Str    (UARTn_e , const uint8 *str);       //发送字符串

void UART_IRQ_EN(UARTn_e uratn);
void UART_IRQ_DIS(UARTn_e uratn);
/********************************************************************/
#endif 