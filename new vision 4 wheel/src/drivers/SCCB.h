/*
 * SCCB.h
 *
 *  Created on: Feb 18, 2018
 *      Author: ZBT
 */

#ifndef USER_DRIVER_OV7725_SCCB_H_
#define USER_DRIVER_OV7725_SCCB_H_

#define ADDR_OV7725   0x42
#define SCCB_SDA_PORT   PORTB,21
#define SCCB_SCL_PORT   PORTB,20
#define SCL_H()         PTB20_OUT = 1
#define SCL_L()         PTB20_OUT = 0
#define	SCL_DDR_OUT() 	DDRB20 = 1
#define	SCL_DDR_IN() 	DDRB20 = 0
#define SDA_H()         PTB21_OUT = 1
#define SDA_L()         PTB21_OUT = 0
#define SDA_IN()      	PTB21_IN
#define SDA_DDR_OUT()	DDRB21 = 1
#define SDA_DDR_IN()	DDRB21 = 0
#define SCCB_DELAY()	SCCB_delay(100)


void SCCB_GPIO_init(void);
int SCCB_WriteByte( u16 WriteAddress , u8 SendByte);
int SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);
void SCCB_delay(volatile u16 i);


#endif /* USER_DRIVER_OV7725_SCCB_H_ */
