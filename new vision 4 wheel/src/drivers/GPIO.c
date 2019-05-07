/*
 * GPIO.c
 *
 *  Created on: Feb 17, 2018
 *      Author: ZBT
 */

#include "include.h"
#include "GPIO.h"

/*****************************************************
*GPIO_Init GPIO初始化
*       GPIO_Type端口 n引脚号 cfg引脚配置 data初始值
*       EXTI_Init(GPIOA, 19, falling_up)
*****************************************************/
void GPIO_Init (PORT_Type* ptn, u8 n, PORT_CFG cfg,u8 data)
{
  GPIO_Type* port;
  ASSERT((n>=0)&&(n<32),"Pin number error");
  switch((u32)ptn)
  {
  case PORTA_BASE:
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
    port = GPIOA;
    break;
  case PORTB_BASE:
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
    port = GPIOB;
    break;
  case PORTC_BASE:
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
    port = GPIOC;
    break;
  case PORTD_BASE:
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
    port = GPIOD;
    break;
  case PORTE_BASE:
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
    port = GPIOE;
    break;
  default:
    break;
  }
  PORT_PCR_REG(ptn, n) = (PORT_PCR_MUX(1) | (cfg&0x0F));        //只配置上拉下拉推挽开漏和滤波器
  if(cfg & GPO)        //output
  {
    GPIO_PDDR_REG(port) |= (1<<n);
    if(data == HIGH)    //output HIGH
      GPIO_PDOR_REG(port) |= (1<<n);
    else
      GPIO_PDOR_REG(port) &= ~(1<<n);
  }
  else
    GPIO_PDDR_REG(port) &= ~(1<<n);
}

/*****************************************************
*port_init 端口初始化
*       ptn端口 n引脚号 cfg引脚配置
*****************************************************/

void port_init(PORT_Type* ptn, u8 n, PORT_CFG cfg)
{
  GPIO_Type * port;
  IRQn_Type IRQn;
  ASSERT((n>=0)&&(n<32),"Pin number error");
  switch((u32)ptn)
  {
  case PORTA_BASE:
    port = GPIOA;
    IRQn=PORTA_IRQn;
    break;
  case PORTB_BASE:
    port = GPIOB;
    IRQn=PORTB_IRQn;
    break;
  case PORTC_BASE:
    port = GPIOC;
    IRQn=PORTC_IRQn;
    break;
  case PORTD_BASE:
    port = GPIOD;
    IRQn=PORTD_IRQn;
    break;
  case PORTE_BASE:
    port = GPIOE;
    IRQn=PORTE_IRQn;
    break;
  default:
    break;
  }
  GPIO_PDDR_REG(port) &= ~(1<<n);
  PORT_PCR_REG(ptn, n) = cfg & 0x1FF8FFF;     //去除无效位上的值
  if(cfg >= LOW_ISR)
    NVIC_EnableIRQ(IRQn);
}

void exti_init(PORT_Type* ptn, u8 n, PORT_CFG cfg)
{
  GPIO_Type * port;
  IRQn_Type IRQn;
  ASSERT((n>=0)&&(n<32),"Pin number error");
  switch((u32)ptn)
  {
  case PORTA_BASE:
    port = GPIOA;
    IRQn=PORTA_IRQn;
    break;
  case PORTB_BASE:
    port = GPIOB;
    IRQn=PORTB_IRQn;
    break;
  case PORTC_BASE:
    port = GPIOC;
    IRQn=PORTC_IRQn;
    break;
  case PORTD_BASE:
    port = GPIOD;
    IRQn=PORTD_IRQn;
    break;
  case PORTE_BASE:
    port = GPIOE;
    IRQn=PORTE_IRQn;
    break;
  default:
    break;
  }
  GPIO_PDDR_REG(port) &= ~(1<<n);
  PORT_PCR_REG(ptn, n) = cfg & 0x1FF8FFF | (PORT_PCR_MUX(1)) ;     //去除无效位上的值
  if(cfg >= LOW_ISR)
    NVIC_EnableIRQ(IRQn);
}

void port_NOMUX(PORT_Type* ptn, u8 n)
{
  ASSERT((n>=0)&&(n<32),"Pin number error");
  PORT_PCR_REG(ptn, n) = PORT_PCR_MUX(1) ;     //去除无效位上的值
}