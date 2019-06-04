/*
 * isr.h
 *
 *  Created on: Feb 18, 2018
 *      Author: ZBT
 */

#ifndef ISR_H_
#define ISR_H_
void Error_handler(unsigned char* Log);
void HardFault_Handler(void);
void PORTB_IRQHandler(void);
void PIT0_IRQHandler(void);
void DMA4_DMA20_IRQHandler(void);
void DMA0_DMA16_IRQHandler(void);
void UART3_RX_TX_IRQHandler(void);
#endif /* ISR_H_ */
