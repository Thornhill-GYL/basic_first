/*
* tools.c
*
*  Created on: Feb 17, 2018
*      Author: ZBT
*/

#include "include.h"
#include "tools.h"
uart_edma_handle_t uart_eDMA_Handler;
uart_transfer_t uart_tx_data;
edma_handle_t g_uartTxEdmaHandle;
struct
{
  unsigned char Header[2];
  unsigned char data[3][8];
  unsigned char checksum;
  unsigned char xorsum;
}Uart_data;

void Button_Handler(int n)
{
  if(!Initialization.EnableKEY)
  {
    PORT_PCR_REG(PORTB, n) &= (~EITHER);
  }
  else
  {
    switch(n)
    {
    case 6:
      Initialization.KeyPressed |= KEYU_PRES;
      while(!KEYU);
      break;
    case 18:
      Initialization.KeyPressed |= KEYD_PRES;
      while(!KEYD);
      break;
    case 8:
      Initialization.KeyPressed |= KEYL_PRES;
      while(!KEYL);
      break;
    case 17:
      Initialization.KeyPressed |= KEYR_PRES;
      while(!KEYR);
      break;
    case 16:
      Initialization.KeyPressed |= KEYC_PRES;
      while(!KEYC);
      break;
    }
    BEEP_Open_once();
  }
}
void UART_eDMA_init(void)
{
  uart_config_t uartConfig;
  edma_config_t config;
  UART_GetDefaultConfig(&uartConfig);
  
  ////////////////
  SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
  PORT_PCR_REG(PORTE,16)=PORT_PCR_MUX(3);
  PORT_PCR_REG(PORTE,17)=PORT_PCR_MUX(3);
  UART_C2_REG(UART2)|=UART_C2_RIE_MASK;
  NVIC_EnableIRQ(UART2_RX_TX_IRQn);
  ///////////////
  
  uartConfig.baudRate_Bps = UART_BAUDRATE;
  uartConfig.enableTx = true;
//  uartConfig.enableRx = false;
  uartConfig.enableRx = true;
  UART_Init(UART_DEBUG, &uartConfig, CLOCK_GetFreq(kCLOCK_FastPeriphClk));
  DMAMUX_Init(DMAMUX);
  DMAMUX_SetSource(DMAMUX, UART_DMA_CHN, UART_DMA_SOURCE);
  DMAMUX_EnableChannel(DMAMUX, UART_DMA_CHN);
  EDMA_GetDefaultConfig(&config);
  EDMA_Init(DMA0, &config);
  EDMA_CreateHandle(&g_uartTxEdmaHandle, DMA0, UART_DMA_CHN);
  UART_TransferCreateHandleEDMA(UART_DEBUG, &uart_eDMA_Handler, NULL, NULL,
                                &g_uartTxEdmaHandle, NULL);
  Uart_data.Header[0] = 'S';
  Uart_data.Header[1] = 'T';
  //Uart_data.Header[0] = Link.Rx.buf[17];
//  Uart_data.Header[0] = 0x5A;
//  Uart_data.Header[1] = 0x5A;
//  Uart_data.Header[2] = 0x5A;
//  Uart_data.Header[3] = 0x5A;
}

void Uart_make_data()
{
  	static unsigned short int send_data[3][4] = { { 0 }, { 0 }, { 0 } };
	short int checksum=0;
	unsigned char xorsum=0,high,low;

	send_data[0][0] = (unsigned short int)(Speed.Rate * 1000);
//        send_data[0][0] = (unsigned short int)(Gyro_ADC.x);
//	send_data[0][1] = (unsigned short int)(Carspeed.Goal_speed);
//	send_data[0][2] = (unsigned short int)(30);
//	send_data[0][3] = (unsigned short int)(40); 
//
	send_data[1][0] = (unsigned short int)(Dir.PID_D.target);//Direction.DD * 100    Carspeed.Speed_I*10000
	send_data[1][1] = (unsigned short int)(Dir.PID_D.feedback);
//      send_data[1][0] = (unsigned short int)(Direction.Ratio * Direction.Gyro);
//	send_data[1][1] = (unsigned short int)(.fly_R);
//	send_data[1][2] = (unsigned short int)(Angle.G_angle);
//	send_data[1][3] = (unsigned short int)(mpu9250_Gyro_y);
//
//	send_data[2][0] = (unsigned short int)(0);
//        send_data[2][0] = (unsigned short int)(Direction.PWM/10);//Carspeed.I_Limit_PWM_max  Slowdown_Integral
//        send_data[2][1] = (unsigned short int)(Carspeed.expect_angle);
//	send_data[2][1] = (unsigned short int)(Elec_PID.result/10);
//      send_data[2][0] = (unsigned short int)(Dir_error);
//	send_data[2][1] = (unsigned short int)(Gyro_Pwm_PID.D*Gyro_Pwm_PID.derr);
//	send_data[2][2] = (unsigned short int)(0);
//	send_data[2][3] = (unsigned short int)(speed_out);

for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++)
    {
      low=(unsigned char)(send_data[i][j] & 0x00ff);
      high=(unsigned char)(send_data[i][j] >> 8u);
      Uart_data.data[i][2*j] = low;
      Uart_data.data[i][2*j+1] = high;
      checksum+=low; checksum+=high;
      xorsum^=low; xorsum^= high;
    }
      Uart_data.checksum = (unsigned char)(checksum & 0x00ff);
      Uart_data.xorsum = xorsum;
}

//void Uart_make_data()
//{
////  Uart_data.data[0] = (int32_t)(Tmp_Speed_P);
////  Uart_data.data[1] = (int32_t)(PWM_Integral);
////  Uart_data.data[2] = (int32_t)(Gyro_Pwm_PID.D*Gyro_Pwm_PID.derr);Variable_integral
//  Uart_data.data[0] = (int32_t)(Variable_integral*1000);
////  Uart_data.data[1] = (int32_t)(20);
////  Uart_data.data[2] = (int32_t)(30);
//  Uart_data.data[3] = (int32_t)(0);
//}

void UART_eDMA_datasend(void)
{
  if(EDMA_GetChannelStatusFlags(DMA0,UART_DMA_CHN)==0)
  {
    Uart_make_data();
    uart_tx_data.data = (void *)&Uart_data;
    uart_tx_data.dataSize = sizeof(Uart_data);
    UART_SendEDMA(UART_DEBUG, &uart_eDMA_Handler, &uart_tx_data);
  }
}


void Time_delay_start(Time_Delay_STR * delay)
{
  delay->flag = 1;
  delay->cnt = delay->delay_const;
}

void Time_delay(Time_Delay_STR * delay)
{
  if(delay->flag)
  {
    delay->cnt -= PIT_PERIOD;
    if(delay->cnt == 0)
    {
      delay->flag = 0;
      (*(delay->task))();
    }
  }
}

