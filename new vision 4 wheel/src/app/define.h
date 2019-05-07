/*
 * define.h
 *
 *  Created on: Feb 14, 2018
 *      Author: ZBT
 */

#ifndef DEFINE_H_
#define DEFINE_H_

//Parameter-Do not remove
#define IMG_PIXEL(y,x) (Image_fire[(x)][(y)/8]>>(7-((y)%8))&1) //摄像头数据解压语句
#define Camera_CNST 85
#define NRF_LINKADDRESS {0x74, 0x18, 0x52, 0x96, 0x30}
#define PIT_PERIOD  (5)

//SD-Do not remove
#define BOARD_SDSPI_SPI_BASE SPI0_BASE
#define BOARD_SDSPI_SPI_PCS_NUMBER 0U

//CAM-Do not remove
#define PCLK_PORT       PORTC,9
#define VSYNC_PORT      PORTB,23
#define VSYNC_IRQ       PORTB_IRQn
#define VSYNC_ISFR      PORTB_ISFR
#define CAM_SOURCEADDR  &PTC_BYTE0_IN
#define CAM_DMASOURCE   DMA_PORTC
#define CMA_DATA_PORT   PORTC
#define CMA_DATA0_PIN   0

//LCD-Do not remove
#define LCD_RST_PORT PORTE,8
#define LCD_RS_PORT PORTE,7
#define LCD_SDA_PORT PORTE,5
#define LCD_SCL_PORT PORTE,6

#define LCD_SPIn kSPI1
#define LCD_OTH_PCSn SPIn_PCS0
#define LCD_DC_PCSn SPIn_PCS1

#define rs PTE7_OUT
#define sda PTE5_OUT
#define scl PTE6_OUT
#define reset PTE8_OUT

//nRF-Do not remove
#define NRF_SPIn kSPI0
#define NRF_PCSn SPIn_PCS0
#define NRF_CE_PORT PORTB,2
#define NRF_IRQ_PORT PORTB,3

#define NRF_CE PTB2_OUT
#define NRF_IRQ PTB3_OUT

//KEY
#define KEYU_PORT       PORTA,24
#define KEYU            PTA24_IN
#define KEYU_PRES       1
#define KEYD_PORT       PORTA,28
#define KEYD            PTA28_IN
#define KEYD_PRES       2
#define KEYL_PORT       PORTA,26
#define KEYL            PTA26_IN
#define KEYL_PRES       3
#define KEYR_PORT       PORTA,27
#define KEYR            PTA27_IN 
#define KEYR_PRES       4
#define KEYC_PORT       PORTA,25
#define KEYC            PTA25_IN
#define KEYC_PRES       5


//BUZ
#define BUZ_PORT PORTE,29
#define BEEP    PTE29_OUT
   
//LED
#define LED_R_PORT PORTA,8
#define LED_G_PORT PORTA,7
#define LED_B_PORT PORTA,9
#define LED_R PTA8_OUT
#define LED_G PTA7_OUT
#define LED_B PTA9_OUT
//color 
#define LED_RED         {LED_R_ON ;LED_G_OFF;LED_B_OFF;} 
#define LED_GREEN       {LED_R_OFF;LED_G_ON ;LED_B_OFF;}
#define LED_BLUE        {LED_R_OFF;LED_G_OFF;LED_B_ON ;}
#define LED_CYAN        {LED_R_OFF;LED_G_ON ;LED_B_ON ;}
#define LED_PURPLE      {LED_R_ON ;LED_G_OFF;LED_B_ON ;}
#define LED_YELLOW      {LED_R_ON ;LED_G_ON ;LED_B_OFF;}
#define LED_WHITE       {LED_R_ON ;LED_G_ON ;LED_B_ON ;}
#define LED_NO          {LED_R_OFF;LED_G_OFF;LED_B_OFF;}

#define LED_R_ON        PTA8_OUT = 0
#define LED_G_ON        PTA7_OUT = 0
#define LED_B_ON        PTA9_OUT = 0
#define LED_R_OFF       PTA8_OUT = 1
#define LED_G_OFF       PTA7_OUT = 1
#define LED_B_OFF       PTA9_OUT = 1
   
//MOTOR
#define MOTORL_FTM FTM0,CH3
#define MOTORR_FTM FTM0,CH0
#define SERVO_FTM  FTM3,CH3
#define MOTORL_DIR_PORT PORTD,1
#define MOTORR_DIR_PORT PORTD,2
#define MOTORL_DIR PTD1_OUT
#define MOTORR_DIR PTD2_OUT

#define SERVO_MIDDLE 4550
#define SERVO_HALF   540        //35度           arm_cos_f32
   
//SW
#define SW1_PORT PORTE,25
#define SW2_PORT PORTE,26
#define SW3_PORT PORTE,24
#define SW4_PORT PORTE,13

#define SW1 PTE25_IN
#define SW2 PTE26_IN
#define SW3 PTE24_IN
#define SW4 PTE13_IN
//uart
#define UART_DEBUG UART2
#define UART_DMA_CHN 16
#define UART_BAUDRATE 115200
#define UART_DMA_SOURCE (3)

#endif /* DEFINE_H_ */
