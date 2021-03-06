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
#include "stdio.h"

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
#include "Cross.h"
#include "Island.h"
#include "event_delay.h"
#include "breakage_process.h"
#include "start_line.h"



extern u8 Memory_use_Flag;
extern u8 Image_fire_Memory1[CAMERA_H+1][CAMERA_DMA_NUM];
extern u8 Image_fire_Memory2[CAMERA_H+1][CAMERA_DMA_NUM];
extern u8 (*Image_fire)[CAMERA_DMA_NUM];//指针
extern u8 Image_Flag;
extern InitParameter Initialization;
extern ROAD_LINE ROAD;
extern ROAD_Y_STR ROADY;
extern int Main_Line;
extern Dir_STR Dir;
extern Block_Data Block;
extern Beep_Str Beep_100ms;
extern u8 LCD_DISPLAY_FLAG;
extern Speed_Status Speed;
extern int run_flag;
extern u8 SWITCH_STATUS;

//特殊元素处理参数
extern int start_process;
//断路参数
extern Break_Data break_info;
extern Angle_Data angle_info;
extern Back_Data  back_info; 
extern Move_Data  move_info;
extern Scanbreak_Data scan_info;
//十字
extern Cross_Data Cross;
extern u8 Liner_threshold;
extern Island_Data Island;
extern u16 OV7725_cnt;
extern u16 OV7725_fps;
//斑马线
extern Start_Data start_info;
//nrf
extern int recevie_flag;
//坡道
extern CarThreeNum  Acc_ADC;
extern CarThreeNum  Gyro_ADC;
extern int TOF_Distance;
extern int ramp_flag;
extern int ave_dc;
extern int ramp_ave[10];
extern int test_delat;
extern int test_delat_l;
extern int ramp_sum;


//新硬件测试 之后删除
extern int ad_test;
#endif /* INCLUDE_H_ */
