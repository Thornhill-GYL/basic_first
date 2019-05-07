/*
 * include.h
 *
 *  Created on: Feb 14, 2018
 *      Author: ZBT
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_

//Device
#include "MKV58F24.h"
#include "core_cm7.h"
#include "arm_math.h"

//FSL
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_uart.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_uart_edma.h"
#include "fsl_hsadc.h"


//Driver
#include "common.h"
#include "Port_cfg.h"
#include "GPIO.h"
#include "FTM.h"
#include "SPI.h"
#include "LCD.h"
#include "pit.h"
#include "SCCB.h"
#include "DMA.h"
#include "OV7725.h"
#include "gfx.h"
#include "gfxconf.h"
#include "board_ILI9341.h"
#include "ADC.h"
#include "NRF24L01.h"
#include "UART.h"
#include "Flash.h"
#include "i2c.h"
#include "ff.h"
#include "LPTMR.h"
#include "LED_BEEP.h"
#include "mykey.h"
#include "mpu9250.h"
#include "TOF.h"

//User
#include "Sys_init.h"
#include "define.h"
#include "isr.h"
#include "menu.h"
#include "mymath.h"
#include "Control.h"
#include "inertial_navigation.h"
#include "Link.h"
#include "Meeting.h"
#include "pid.h"
#include "Process.h"
#include "tools.h"
#include "event_delay.h"



extern u8 Memory_use_Flag;
extern u8 Image_fire_Memory1[CAMERA_H+1][CAMERA_DMA_NUM];
extern u8 Image_fire_Memory2[CAMERA_H+1][CAMERA_DMA_NUM];
extern u8 (*Image_fire)[CAMERA_DMA_NUM];//÷∏’Î
extern u8 Image_Flag;
extern InitParameter Initialization;
extern ROAD_LINE ROAD;
extern ROAD_Y_STR ROADY;
extern int Main_Line;
extern Dir_STR Dir;
extern Block_Data Block;
extern Beep_Str Beep_100ms;
extern u8 Test_flag;
extern Time_Delay_STR Test_Str;
extern u8 LCD_DISPLAY_FLAG;
extern Speed_Status Speed;

#endif /* INCLUDE_H_ */
