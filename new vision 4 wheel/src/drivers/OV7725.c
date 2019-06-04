/*
* OV7725.c
*
*  Created on: Feb 18, 2018
*      Author: ZBT
*/

#include "include.h"
#include "define.h"
#include "SCCB.h"
#include "OV7725.h"
#include "OV7725_REG.h"

u8 Memory_use_Flag=1;
u8 Image_fire_Memory1[CAMERA_H+1][CAMERA_DMA_NUM]={{0}};
u8 Image_fire_Memory2[CAMERA_H+1][CAMERA_DMA_NUM]={{0}};
u8 (*Image_fire)[CAMERA_DMA_NUM];//指针
volatile u8 img_flag = IMG_FINISH;		//图像状态
edma_tcd_t OV7725_tcd;
edma_handle_t g_EDMA_Handle;
edma_config_t userConfig;
u16 OV7725_cnt = 0;
u16 OV7725_fps = 0;

edma_transfer_config_t DMA_Transfer_config;

void Ov7725_exti_Init()
{
  //FIFO端口初始化
//  GPIO_Init(PORTC, 0, GPI, HIGH);
//  GPIO_Init(PORTC, 1, GPI, HIGH);
//  GPIO_Init(PORTC, 2, GPI, HIGH);
//  GPIO_Init(PORTC, 3, GPI, HIGH);
//  GPIO_Init(PORTC, 4, GPI, HIGH);
//  GPIO_Init(PORTC, 5, GPI, HIGH);
//  GPIO_Init(PORTC, 6, GPI, HIGH);
//  GPIO_Init(PORTC, 7, GPI, HIGH);
  for(int i = 0;i<8;i++)
    GPIO_Init(CMA_DATA_PORT, CMA_DATA0_PIN+i, GPI, HIGH);
  
  port_init(PCLK_PORT, ALT1 | DMA_FALLING | PULL_UP);    //PCLK
  exti_init(VSYNC_PORT, RISING | PULL_DOWN | PF);    //场中断，下拉，下降沿触发中断，带滤波
  //PORT_PCR_REG(PORTB,23) = PORT_PCR_MUX(1);//由于有端口冲突，所以这里直接对端口号操作。
  NVIC_DisableIRQ(VSYNC_IRQ);
  //关闭PTB的中断
  Image_fire=&Image_fire_Memory1[0];
}

void ov7725_get_img()
{
  img_flag = IMG_START;			//开始采集图像
  NVIC_DisableIRQ(VSYNC_IRQ);
   //PORT_PCR_REG(PORTB,23) = PORT_PCR_MUX(1) ;//
  if(Memory_use_Flag==2)
  {
    Memory_use_Flag=1;
  }
  else if(Memory_use_Flag==1)  
  {
    Memory_use_Flag=2;
  }
  DMA_IRQ_EN(CAMERA_DMA_CH);
  DMA_EN(CAMERA_DMA_CH);
}

void Camera_start(void)
{
  if(Memory_use_Flag==2)
  {
    DMA_PORTx2BUFF_Init(CAMERA_DMA_CH,CAM_SOURCEADDR,Image_fire_Memory2,DMA_BYTE1,CAMERA_SIZE,CAM_DMASOURCE,DMA_PERIPHERAL_TO_MEMORY,DMA_REVERT);
  }
  else if(Memory_use_Flag==1)
  {
    DMA_PORTx2BUFF_Init(CAMERA_DMA_CH,CAM_SOURCEADDR,Image_fire_Memory1,DMA_BYTE1,CAMERA_SIZE,CAM_DMASOURCE,DMA_PERIPHERAL_TO_MEMORY,DMA_REVERT);
  }
  DMA_DIS(CAMERA_DMA_CH);
  VSYNC_ISFR = ~0;
  NVIC_EnableIRQ(VSYNC_IRQ);
  //PORT_PCR_REG(PORTB,23) = (PORT_PCR_MUX(1) | RISING | PULL_DOWN | PF);//由于有端口冲突，所以这里直接对端口号操作。
}

/*OV7725初始化配置表*/
Register_Info ov7727_reg[] = {
  //寄存器，寄存器值次
  { COM4, 0xC1 }, 
  { CLKRC, 0x00 }, 
  { COM2, 0x03 }, 
  { COM3, 0xD0 }, 
  { COM7,  0x40 }, 
  { HSTART, 0x3F }, 
  { HSIZE, 0x50 }, 
  { VSTRT, 0x03 }, 
  { VSIZE, 0x78 }, 
  { HREF, 0x00 }, 
  { SCAL0, 0x0A }, 
  { AWB_Ctrl0, 0xE0 }, 
  { DSPAuto, 0xff }, 
  { DSP_Ctrl2, 0x0C }, 
  { DSP_Ctrl3, 0x00 }, 
  { DSP_Ctrl4, 0x00 },
        
#if (CAMERA_W == 80)
        { HOutSize ,0x14},
#elif (CAMERA_W == 160)
        { HOutSize ,0x28},
#elif (CAMERA_W == 240)
        { HOutSize ,0x3c},
#elif (CAMERA_W == 320)
        { HOutSize, 0x50 },
#else
        
#endif
        
#if (CAMERA_H == 60 )
        { VOutSize ,0x1E},
#elif (CAMERA_H == 120 )
        { VOutSize ,0x3c},
#elif (CAMERA_H == 180 )
        { VOutSize ,0x5a},
#elif (CAMERA_H == 240 )
        { VOutSize, 0x78 },
#else
        
#endif
        
        { EXHCH, 0x00 }, 
        { GAM1, 0x0c }, 
        { GAM2, 0x16 }, 
        { GAM3, 0x2a }, 
        { GAM4, 0x4e }, 
        { GAM5, 0x61 }, 
        { GAM6, 0x6f }, 
        { GAM7, 0x7b }, 
        { GAM8, 0x86 }, 
        { GAM9, 0x8e }, 
        { GAM10, 0x97 }, 
        { GAM11, 0xa4 }, 
        { GAM12, 0xaf }, 
        { GAM13, 0xc5 },
        { GAM14, 0xd7 }, 
        { GAM15, 0xe8 }, 
        { SLOP, 0x20 }, 
        { LC_RADI, 0x00 }, 
        { LC_COEF, 0x13 }, 
        { LC_XC, 0x08 }, 
        { LC_COEFB, 0x14 }, 
        { LC_COEFR, 0x17 }, 
        { LC_CTR, 0x05 },
        { BDBase, 0x99 }, 
        { BDMStep, 0x03 }, 
        { SDE, 0x04 }, 
        { BRIGHT, 0x00 },
	{ CNST, 100 }, 	//100//150:楼上楼下都能跑（第一个车稳）70
	{ SIGN, 0x06 }, { UVADJ0, 0x11 }, { UVADJ1, 0x02 }, };

u8 cfgnum = sizeof(ov7727_reg) / sizeof(ov7727_reg[0]); /*结构体数组成员数目*/

/************************************************
* 函数名：Ov7725_Init
* 描述  ：Sensor初始化
* 输入  ：无
* 输出  ：返回1成功，返回0失败
* 注意  ：无
************************************************/
u8 Ov7725_Init()
{
  while (Ov7725_reg_Init() == 0)
    ;
  Ov7725_exti_Init();
  return 0;
}

/************************************************
* 函数名：Ov7725_reg_Init
* 描述  ：Sensor 寄存器 初始化
* 输入  ：无
* 输出  ：返回1成功，返回0失败
* 注意  ：无
************************************************/
u8 Ov7725_reg_Init(void)
{
  u16 i = 0;
  u8 Sensor_IDCode = 0;
  SCCB_GPIO_init();
  OV7725_Delay_ms(50);
  while (0 == SCCB_WriteByte(0x12, 0x80)) /*复位sensor */
  {
    i++;
    if (i == 20)
    {
      return 0;
    }
  }
  OV7725_Delay_ms(50);
  if (0 == SCCB_ReadByte(&Sensor_IDCode, 1, 0x0b)) /* 读取sensor ID号*/
  {
    return 0;
  }
  if (Sensor_IDCode == OV7725_ID)
  {
    for (i = 0; i < cfgnum; i++)
    {
      if(ov7727_reg[i].Address == CNST)
      {
        if (0 == SCCB_WriteByte(ov7727_reg[i].Address, Initialization.Cnst))
        {
          return 0;
        }
      }
      else
      {
        if (0 == SCCB_WriteByte(ov7727_reg[i].Address, ov7727_reg[i].Value))
        {
          return 0;
        }
      }
      
    }
  }
  else
  {
    return 0;
  }
  return 1;
}
