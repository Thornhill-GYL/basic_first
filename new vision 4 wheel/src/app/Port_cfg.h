/*
 * Port_cfg.h
 *
 *  Created on: Feb 17, 2018
 *      Author: ZBT
 */

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

/**********************************  FTM  *******************************************/

#define FTM0_CH0    PTD0        //PTE24、PTA3 、PTC1 、PTD0
#define FTM0_CH1    PTE25        //PTE25、PTA4 、PTC2 、PTD1
#define FTM0_CH2    PTE29        //PTE29、PTA5 、PTC3 、PTC5 、PTD2
#define FTM0_CH3    PTE30        //PTE30、PTA6 、PTC4 、PTD3
#define FTM0_CH4    PTE26        //PTE26、PTA7 、PTD4
#define FTM0_CH5    PTD5        //PTA0 、PTD5
#define FTM0_CH6    PTD6        //PTA1 、PTD6
#define FTM0_CH7    PTD7        //PTA2 、PTD7

#define FTM1_CH0    PTA8        //PTE20、PTA8 、PTB0 、PTD6 、PTA2 、PTA12
#define FTM1_CH1    PTA9        //PTE21、PTA9 、PTB1 、PTD7 、PTA1 、PTA13

#define FTM2_CH0    PTB18       //PTE22、PTA10、PTB18
#define FTM2_CH1    PTB19       //PTE23、PTA11、PTB19

#define FTM3_CH0    PTE5        //PTE5 、PTD0
#define FTM3_CH1    PTE6        //PTE6 、PTD1
#define FTM3_CH2    PTB18        //PTE7 、PTA18、PTB18、PTD2
#define FTM3_CH3    PTE8        //PTE8 、PTB19、PTD3
#define FTM3_CH4    PTE9        //PTE9 、PTC8 、
#define FTM3_CH5    PTE10       //PTE10、PTC9 、
#define FTM3_CH6    PTE11       //PTE11、PTC10
#define FTM3_CH7    PTE12       //PTE12、PTC11

//正交解码
#define FTM1_QDPHA  PTE20       //PTE20、PTA12、PTB0 、PTA1
#define FTM1_QDPHB  PTE21       //PTE21、PTA13、PTB1 、PTA2

#define FTM2_QDPHA  PTE22       //PTE22、PTB18、PTA10
#define FTM2_QDPHB  PTE23       //PTE23、PTB19、PTA11

/**********************************  SPI  *******************************************/

#define SPI0_SCK    PTD1        //PTE17、PTA15、PTC5 、PTD1
#define SPI0_SOUT   PTD2        //PTE18、PTA16、PTC6 、PTD2
#define SPI0_SIN    PTD3        //PTE19、PTA17、PTC7 、PTD3

#define SPI0_PCS0   PTD0        //PTE16、PTA14、PTC0 、PTC4 、PTD0
#define SPI0_PCS1   NULL        //PTC3 、PTD4
#define SPI0_PCS2   NULL        //PTC2 、PTD5
#define SPI0_PCS3   NULL        //PTC1 、PTD6
#define SPI0_PCS4   NULL        //PTC0
#define SPI0_PCS5   NULL        //PTB23


#define SPI1_SCK    PTE2        //PTE2 、PTB11、PTD5
#define SPI1_SOUT   PTE1        //PTE1 、PTB16、PTD6
#define SPI1_SIN    PTE3        //PTE3 、PTB17、PTD7

#define SPI1_PCS0   PTE4        //PTE4 、PTB10、PTD4
#define SPI1_PCS1   NULL        //PTE0 、PTB9
#define SPI1_PCS2   NULL        //PTE5
#define SPI1_PCS3   NULL        //PTE6


#define SPI2_SCK    PTD12       //PTB21、PTD12
#define SPI2_SOUT   PTD13       //PTB22、PTD13
#define SPI2_SIN    PTD14       //PTB23、PTD14
#define SPI2_PCS0   PTD11       //PTB20、PTD11
#define SPI2_PCS1   NULL        //PTD15、PTC12

/**********************************  I2C   ***************************************/

#define I2C0_SCL    PTB0        //PTE19、PTE24、PTA12、PTB0 、PTB2 、PTC6 、PTC14、PTD2
#define I2C0_SDA    PTB1        //PTE18、PTE25、PTA11、PTB1 、PTB3 、PTC7 、PTC15、PTD3

#define I2C1_SCL    PTC10       //PTE1 、PTA14、PTC10、PTC14、PTD8
#define I2C1_SDA    PTC11       //PTE0 、PTA13、PTC11、PTC15、PTD9

/**********************************  UART   **************************************/

#define UART0_RX    PTB0        //PTE21、PTA1 、PTA15、PTB0、PTB16、PTC6 、PTD6
#define UART0_TX    PTB1        //PTE20、PTA2 、PTA14、PTB1、PTB17、PTC7 、PTD7

#define UART1_RX    PTC3        //PTE1 、PTC3
#define UART1_TX    PTC4        //PTE0 、PTC4

#define UART2_RX    PTE17       //PTE17、PTD2
#define UART2_TX    PTE16       //PTE16、PTD3

#define UART3_RX    PTB10       //PTE5 、PTB10、PTC16
#define UART3_TX    PTB11       //PTE4 、PTB11、PTC17

#define UART4_RX    PTE25       //PTE25、PTC14
#define UART4_TX    PTE24       //PTE24、PTC15

#define UART5_RX    PTE9        //PTE9 、PTD8
#define UART5_TX    PTE8        //PTE8 、PTD9

#endif /* PORT_CFG_H_ */
