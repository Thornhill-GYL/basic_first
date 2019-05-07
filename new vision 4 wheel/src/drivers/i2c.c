
#include "include.h"
#include "i2c.h"
volatile I2C_Type* I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; 


void I2C_Init(I2Cn i2cn)
{    
  if(i2cn == I2C_0)
  {
    /* 开启时钟 */
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //开启 I2C0时钟
    
    /* 配置 I2C0功能的 GPIO 接口 */
    if(I2C0_SCL == PTB0)
    {  
      PORTB_PCR0 = PORT_PCR_MUX(2);
      PORTB_PCR0 = PORTB_PCR0|(1<<5);
    }
    else if(I2C0_SCL == PTB2)
    {
      PORTB_PCR2 = PORT_PCR_MUX(2);
      PORTB_PCR2 = PORTB_PCR2|(1<<5);
    }
    else if(I2C0_SCL == PTD8)
    {
      PORTD_PCR8 = PORT_PCR_MUX(2);
      PORTD_PCR8 = PORTD_PCR8|(1<<5);
    }
    else if(I2C0_SCL == PTE19)
    {
      PORTE_PCR19 = PORT_PCR_MUX(4);
      PORTE_PCR19 = PORTE_PCR19|(1<<5);
    }
    else if(I2C0_SCL == PTE24)
    {
      PORTE_PCR24 = PORT_PCR_MUX(5);
      PORTE_PCR24 = PORTE_PCR24|(1<<5);
    }
    else if(I2C0_SCL == PTA12)
    {
      PORTA_PCR12 = PORT_PCR_MUX(8);
      PORTA_PCR12 = PORTA_PCR12|(1<<5);
    }
    else if(I2C0_SCL == PTC6)
    {
      PORTC_PCR6 = PORT_PCR_MUX(7);
      PORTC_PCR6 = PORTC_PCR6|(1<<5);
    }
    else if(I2C0_SCL == PTC14)
    {
      PORTC_PCR14 = PORT_PCR_MUX(3);
      PORTC_PCR14 = PORTC_PCR14|(1<<5);
    }
    else if(I2C0_SCL == PTD2)
    {
      PORTD_PCR2 = PORT_PCR_MUX(7);
      PORTD_PCR2 = PORTD_PCR2|(1<<5);
    }
    
    
    if(I2C0_SDA == PTB1)
    {
      PORTB_PCR1 = PORT_PCR_MUX(2);
      PORTB_PCR1 = PORTB_PCR1|(1<<5);
    }
    else if(I2C0_SDA == PTB3)
    {  
      PORTB_PCR3 = PORT_PCR_MUX(2);
      PORTE_PCR3 = PORTE_PCR3|(1<<5);
    }
    else if(I2C0_SDA == PTD9)
    {
      PORTD_PCR9 = PORT_PCR_MUX(2);
      PORTD_PCR9 = PORTD_PCR9|(1<<5);
    }  
    else if(I2C0_SDA == PTE18)
    {
      PORTE_PCR18 = PORT_PCR_MUX(4);
      PORTE_PCR18 = PORTE_PCR18|(1<<5);
    } 
    else if(I2C0_SDA == PTE25)
    {
      PORTE_PCR25 = PORT_PCR_MUX(5);
      PORTE_PCR25 = PORTE_PCR25|(1<<5);
    } 
    else if(I2C0_SDA == PTA11)
    {
      PORTA_PCR11 = PORT_PCR_MUX(8);
      PORTA_PCR11 = PORTA_PCR11|(1<<5);
    }
    else if(I2C0_SDA == PTC7)
    {
      PORTC_PCR7 = PORT_PCR_MUX(8);
      PORTC_PCR7 = PORTC_PCR7|(1<<5);
    }
    else if (I2C0_SDA == PTC15)
    {
      PORTC_PCR15 = PORT_PCR_MUX(3);
      PORTC_PCR15 = PORTC_PCR15 | (1<<5);
    }
    else if (I2C0_SDA == PTD3)
    {
      PORTD_PCR3 = PORT_PCR_MUX(7);
      PORTD_PCR3 = PORTD_PCR3 | (1<<5);
    }
  }
  else
  {
    /* 开启时钟 */
    SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //开启 I2C1时钟
    
    /* 配置 I2C1功能的 GPIO 接口 */
    if(I2C1_SCL == PTE1)
    { 
      PORTE_PCR1 = PORT_PCR_MUX(6);
      PORTE_PCR1 = PORTE_PCR1|(1<<5);
    }
    else if(I2C1_SCL == PTC10)
    {
      PORTC_PCR10 = PORT_PCR_MUX(2);
      PORTC_PCR10 = PORTC_PCR10 |(1<<5);
    }
    else if(I2C1_SCL == PTC14)
    {
      PORTC_PCR14 = PORT_PCR_MUX(2);
      PORTC_PCR14 = PORTC_PCR14|(1<<5);
    }
    else if(I2C1_SCL == PTA14)
    {
      PORTA_PCR14 = PORT_PCR_MUX(8);
      PORTA_PCR14 = PORTA_PCR14|(1<<5);
    }
    else if(I2C1_SCL == PTD8)
    {
      PORTD_PCR8 = PORT_PCR_MUX(2);
      PORTD_PCR8 = PORTD_PCR8|(1<<5);
    }
    
    if(I2C1_SDA == PTE0)
    { 
      PORTE_PCR0 = PORT_PCR_MUX(6);
      PORTE_PCR0 = PORTE_PCR0 | (1<<5);//设置成开漏模式   只设置SDA即可，两个都设置也没事
    }
    else if (I2C1_SDA == PTC11)
    {
      PORTC_PCR11 = PORT_PCR_MUX(2);
      PORTC_PCR11 = PORTC_PCR11 | 0x22;/////////////////////////////////////
    }
    else if (I2C1_SDA == PTC15)
    {
      PORTC_PCR15 = PORT_PCR_MUX(2);
      PORTC_PCR15 = PORTC_PCR15 | (1<<5);
    }
    else if (I2C1_SDA == PTA13)
    {
      PORTA_PCR13 = PORT_PCR_MUX(8);
      PORTA_PCR13 = PORTC_PCR13 | (1<<5);
    } 
    else if(I2C1_SDA == PTD9)
    {
      PORTD_PCR9 = PORT_PCR_MUX(2);
      PORTD_PCR9 = PORTD_PCR9|(1<<5);
    }
  }
  
  /* 设置频率 */
  I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(1) | I2C_F_ICR(0x0B) ; 
  /* 使能 IIC1 */
  I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

//-------------------------------------------------------------------------*
//函数名: I2C_StartTransmission                                                        
//功  能: 启动IIC传输                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Mode    :传输模式
//返  回: 无                                                              
//简  例: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_StartTransmission (I2Cn i2cn, u8 SlaveID, MSmode Mode)
{
  SlaveID = ( SlaveID << 1 ) | Mode ;            //确定写地址和读地址
  
  /* send start signal */
  I2C_Start(i2cn);
  
  /* send ID with W/R bit */
  I2C_write_byte(i2cn, SlaveID);
}

//-------------------------------------------------------------------------*
//函数名: Pause                                                        
//功  能: 延时                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: Pause;              
//-------------------------------------------------------------------------*
void Pause(void)
{
  u16 n;
  for(n = 1; n < 1000; n++)      //不可太小
  {
    asm("nop");
  }
}


//-------------------------------------------------------------------------*
//函数名: I2C_ReadAddr                                                        
//功  能: 读取IIC设备指定地址寄存器的数据                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Addr    :从机的寄存器地址
//返  回: result                                                              
//简  例: p[0]  = I2C_ReadAddr(i2cn,SlaveID,OUT_X_MSB_REG);              
//-------------------------------------------------------------------------*
u8 I2C_ReadAddr(I2Cn i2cn, u8 SlaveID, u8 Addr)
{
  u8 result;
  
  /* Send Slave Address */
  I2C_StartTransmission (i2cn, SlaveID, write);
  I2C_Wait(i2cn);
  
  /* Write Register Address */
  I2C_write_byte(i2cn, Addr);
  I2C_Wait(i2cn);
  
  /* Do a repeated start */
  I2C_RepeatedStart(i2cn);
  
  /* Send Slave Address */
  I2C_write_byte(i2cn, ( SlaveID << 1) | read );
  I2C_Wait(i2cn);
  
  /* Put in Rx Mode */
  I2C_PutinRxMode(i2cn);
  
  /* Turn off ACK since this is second to last byte being read*/
  I2C_DisableAck(i2cn); //不应答
  
  /* Dummy read 虚假读取*/
  result = I2C_D_REG(I2Cx[i2cn]);
  I2C_Wait(i2cn);
  
  /* Send stop since about to read last byte */
  I2C_Stop(i2cn);
  
  /* Read byte */
  result = I2C_D_REG(I2Cx[i2cn]);
  Pause();
  return result;
}

//-------------------------------------------------------------------------*
//函数名: I2C_ReadAddr16                                                        
//功  能: 读取IIC设备指定地址16bit数据                                                        
//参  数: i2cn    :端口名 I2C_0,I2C_1 
//        SlaveID :从机地址 
//        Addr    :从机的寄存器地址
//返  回: result  
//-------------------------------------------------------------------------*
u16 I2C_ReadAddr16(I2Cn i2cn, u8 SlaveID, u8 Addr)
{
  u16 result;
  /* Send Slave Address */
  I2C_StartTransmission (i2cn, SlaveID, write);
  I2C_Wait(i2cn);
  /* Write Register Address */
  I2C_write_byte(i2cn, Addr);
  I2C_Wait(i2cn);
  /* Do a repeated start */
  I2C_RepeatedStart(i2cn);
  /* Send Slave Address */
  I2C_write_byte(i2cn, ( SlaveID << 1) | read );
  I2C_Wait(i2cn);
  /* Put in Rx Mode */
  I2C_EnterRxMode(i2cn);
  /* Turn off ACK since this is second to last byte being read*/
  /* Dummy read 虚假读取*/
  result = I2C_D_REG(I2Cx[i2cn]);
  I2C_Wait(i2cn);
  result = I2C_D_REG(I2Cx[i2cn])<<8;
  I2C_Wait(i2cn);
  /* Send stop since about to read last byte */
  I2C_Stop(i2cn);
  /* Read byte */
  result |= I2C_D_REG(I2Cx[i2cn]);
  Pause();
  return result;
}

//-------------------------------------------------------------------------*
//函数名: I2C_ReadAddr                                                        
//功  能: 读取IIC设备指定地址寄存器的数据                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Addr    :从机的寄存器地址
//        Data    :数据
//返  回: 无                                                              
//简  例: I2C_WriteAddr(I2C1, SlaveAddress2100, CTRL_REG1_2100, 0x02);             
//-------------------------------------------------------------------------*
void I2C_WriteAddr(I2Cn i2cn, u8 SlaveID, u8 Addr, u8 Data)
{
  /* send data to slave */
  I2C_StartTransmission(i2cn, SlaveID, write);    //启动传输
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Addr);                    //写地址
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Data);                    //写数据
  I2C_Wait(i2cn);
  
  I2C_Stop(i2cn);
  
  Pause();                                        //延时太短的话，可能写出错
}
void I2C_WriteAddr16(I2Cn i2cn, u8 SlaveID, u8 Addr, u16 Data)
{
  /* send data to slave */
  I2C_StartTransmission(i2cn, SlaveID, write);    //启动传输
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Addr);                    //写地址
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, (u8)(Data>>8));                    //写数据
  I2C_Wait(i2cn);
  I2C_write_byte(i2cn, (u8)Data);                    //写数据
  I2C_Wait(i2cn);
  
  I2C_Stop(i2cn);
  
  Pause();                                        //延时太短的话，可能写出错
}
