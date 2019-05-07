/*
 * FTM.c
 *
 *  Created on: Feb 17, 2018
 *      Author: ZBT
 */

#include "include.h"
#include "FTM.h"

void FTM_PWM_init(FTM_Type* ftmn,unsigned char ch, u32 freq, u32 duty)
{
  u32 clk_hz = (fastperipheral_clk_khz * 1000) >> 1;       //bus频率/2
  u16 mod;
  u8 sc_ps;
  u16 cv;
  ASSERT((duty>=0)&&(duty<=FTM_PRECISON),"FTM PWMduty error");
  ASSERT((freq>0)&&(freq<clk_hz),"FTM PWMfreq error");
  /*       计算频率设置        */
  mod = (clk_hz >> 16 ) / freq ;
  for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
  if(freq < 1000)sc_ps++;
  mod = (clk_hz >> sc_ps) / freq;
  cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;
  switch((u32)ftmn)
  {
  case FTM0_BASE:
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;       //使能FTM0时钟
    switch(ch)
    {
    case CH0:
      if(FTM0_CH0 == PTC1)
        PORT_PCR_REG(PORTC,1)=PORT_PCR_MUX(4);
      else if(FTM0_CH0 == PTA3)
        PORT_PCR_REG(PORTA,3)=PORT_PCR_MUX(3);
      else if(FTM0_CH0 == PTE24)
        PORT_PCR_REG(PORTE,24)=PORT_PCR_MUX(3);
      else if(FTM0_CH0 == PTD0)
        PORT_PCR_REG(PORTD,0)=PORT_PCR_MUX(5);
      break;
    case CH1:
      if(FTM0_CH1 == PTC2)
        PORT_PCR_REG(PORTC,2)=PORT_PCR_MUX(4);
      else if(FTM0_CH1 == PTA4)
        PORT_PCR_REG(PORTA,4)=PORT_PCR_MUX(3);
      else if(FTM0_CH1 == PTE25)
        PORT_PCR_REG(PORTE,25)=PORT_PCR_MUX(3);
      else if(FTM0_CH1 == PTD1)
        PORT_PCR_REG(PORTD,1)=PORT_PCR_MUX(5);
      break;
    case CH2:
      if(FTM0_CH2 == PTC3)
        PORT_PCR_REG(PORTC,3)=PORT_PCR_MUX(4);
      else if(FTM0_CH2 == PTA5)
        PORT_PCR_REG(PORTA,5)=PORT_PCR_MUX(5);
      else if(FTM0_CH2 == PTE29)
        PORT_PCR_REG(PORTE,29)=PORT_PCR_MUX(3);
      else if(FTM0_CH2 == PTC5)
        PORT_PCR_REG(PORTC,5)=PORT_PCR_MUX(4);
      else if(FTM0_CH2 == PTD2)
        PORT_PCR_REG(PORTD,2)=PORT_PCR_MUX(4);
      break;
    case CH3:
      if(FTM0_CH3 == PTC4)
        PORT_PCR_REG(PORTC,4)=PORT_PCR_MUX(4);
      else if(FTM0_CH3 == PTA6)
        PORT_PCR_REG(PORTA,6)=PORT_PCR_MUX(3);
      else if(FTM0_CH3 == PTE30)
        PORT_PCR_REG(PORTE,30)=PORT_PCR_MUX(3);
      else if(FTM0_CH3 == PTD3)
        PORT_PCR_REG(PORTD,3)=PORT_PCR_MUX(5);
      break;
    case CH4:
      if(FTM0_CH4 == PTD4)
        PORT_PCR_REG(PORTD,4)=PORT_PCR_MUX(4);
      else if(FTM0_CH4 == PTE26)
        PORT_PCR_REG(PORTE,26)=PORT_PCR_MUX(3);
      else if(FTM0_CH4 == PTA7)
        PORT_PCR_REG(PORTA,7)=PORT_PCR_MUX(3);
      break;
    case CH5:
      if(FTM0_CH5 == PTD5)
        PORT_PCR_REG(PORTD,5)=PORT_PCR_MUX(4);
      else if(FTM0_CH5 == PTA0)
        PORT_PCR_REG(PORTA,0)=PORT_PCR_MUX(3);
      break;
    case CH6:
      if(FTM0_CH6 == PTD6)
        PORT_PCR_REG(PORTD,6)=PORT_PCR_MUX(4);
      else if(FTM0_CH6 == PTA1)
        PORT_PCR_REG(PORTA,1)=PORT_PCR_MUX(3);
      break;
    case CH7:
      if(FTM0_CH7 == PTD7)
        PORT_PCR_REG(PORTD,7)=PORT_PCR_MUX(4);
      else if(FTM0_CH7 == PTA2)
        PORT_PCR_REG(PORTA,2)=PORT_PCR_MUX(3);
      break;
    default:
      return;
    }
    break;
  case FTM1_BASE:
    SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
    switch(ch)
    {
    case CH0:
      if(FTM1_CH0 == PTA8)
        PORT_PCR_REG(PORTA,8)=PORT_PCR_MUX(3);
      else if(FTM1_CH0 == PTA12)
        PORT_PCR_REG(PORTA,12)=PORT_PCR_MUX(3);
      else if(FTM1_CH0 == PTB0)
        PORT_PCR_REG(PORTB,0)=PORT_PCR_MUX(3);
      else if(FTM1_CH0 == PTE20)
        PORT_PCR_REG(PORTE,20)=PORT_PCR_MUX(3);
      else if(FTM1_CH0 == PTA2)
        PORT_PCR_REG(PORTA,2)=PORT_PCR_MUX(6);
      else if(FTM1_CH0 == PTD6)
        PORT_PCR_REG(PORTD,6)=PORT_PCR_MUX(5);
      break;
    case CH1:
      if(FTM1_CH1 == PTA9)
        PORT_PCR_REG(PORTA,9)=PORT_PCR_MUX(3);
      else if(FTM1_CH1 == PTA13)
        PORT_PCR_REG(PORTA,13)=PORT_PCR_MUX(3);
      else if(FTM1_CH1 == PTB1)
        PORT_PCR_REG(PORTB,1)=PORT_PCR_MUX(3);
      else if(FTM1_CH1 == PTE21)
        PORT_PCR_REG(PORTE,21)=PORT_PCR_MUX(3);
      else if(FTM1_CH1 == PTA1)
        PORT_PCR_REG(PORTA,1)=PORT_PCR_MUX(6);
      else if(FTM1_CH1 == PTD7)
        PORT_PCR_REG(PORTD,7)=PORT_PCR_MUX(5);
      break;
    default:
      return;
    }
    break;
  case FTM2_BASE:
    SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;
    switch(ch)
    {
    case CH0:
      if(FTM2_CH0 == PTA10)
        PORT_PCR_REG(PORTA,10)=PORT_PCR_MUX(3);
      else if(FTM2_CH0 == PTB18)
        PORT_PCR_REG(PORTB,18)=PORT_PCR_MUX(3);
      else if(FTM2_CH0 == PTE22)
        PORT_PCR_REG(PORTE,22)=PORT_PCR_MUX(3);
      break;
    case CH1:
      if(FTM2_CH1 == PTA11)
        PORT_PCR_REG(PORTA,11)=PORT_PCR_MUX(3);
      else if(FTM2_CH1 == PTB19)
        PORT_PCR_REG(PORTB,19)=PORT_PCR_MUX(3);
      else if(FTM2_CH1 == PTE23)
        PORT_PCR_REG(PORTE,23)=PORT_PCR_MUX(3);
      break;
    default:
      return;
    }
    break;
  case FTM3_BASE:
    SIM_SCGC6 |= SIM_SCGC6_FTM3_MASK;
    switch(ch)
    {
    case CH0:
      if(FTM3_CH0 == PTE5)
        PORT_PCR_REG(PORTE,5)=PORT_PCR_MUX(6);
      else if(FTM3_CH0 == PTD0)
        PORT_PCR_REG(PORTD,0)=PORT_PCR_MUX(4);
      break;
    case CH1:
      if(FTM3_CH1 == PTE6)
        PORT_PCR_REG(PORTE,6)=PORT_PCR_MUX(6);
      else if(FTM3_CH1 == PTD1)
        PORT_PCR_REG(PORTD,1)=PORT_PCR_MUX(4);
      break;
    case CH2:
      if(FTM3_CH2 == PTE7)
        PORT_PCR_REG(PORTE,7)=PORT_PCR_MUX(6);
      else if(FTM3_CH2 == PTD2)
        PORT_PCR_REG(PORTD,2)=PORT_PCR_MUX(4);
      else if(FTM3_CH2 == PTA18)
        PORT_PCR_REG(PORTA,18)=PORT_PCR_MUX(6);
      else if(FTM3_CH2 == PTB18)
        PORT_PCR_REG(PORTB,18)=PORT_PCR_MUX(4);
      break;
    case CH3:
      if(FTM3_CH3 == PTE8)
        PORT_PCR_REG(PORTE,8)=PORT_PCR_MUX(6);
      else if(FTM3_CH3 == PTD3)
        PORT_PCR_REG(PORTD,3)=PORT_PCR_MUX(4);
      else if(FTM3_CH3 == PTB19)
        PORT_PCR_REG(PORTB,19)=PORT_PCR_MUX(4);
      break;
    case CH4:
      if(FTM3_CH4 == PTE9)
        PORT_PCR_REG(PORTE,9)=PORT_PCR_MUX(6);
      else if(FTM3_CH4 == PTC8)
        PORT_PCR_REG(PORTC,8)=PORT_PCR_MUX(3);
      break;
    case CH5:
      if(FTM3_CH5 == PTE10)
        PORT_PCR_REG(PORTE,10)=PORT_PCR_MUX(6);
      else if(FTM3_CH5 == PTC9)
        PORT_PCR_REG(PORTC,9)=PORT_PCR_MUX(3);
      break;
    case CH6:
      if(FTM3_CH6 == PTE11)
        PORT_PCR_REG(PORTE,11)=PORT_PCR_MUX(6);
      else if(FTM3_CH6 == PTC10)
        PORT_PCR_REG(PORTC,10)=PORT_PCR_MUX(3);
      break;
    case CH7:
      if(FTM3_CH7 == PTE12)
        PORT_PCR_REG(PORTE,12)=PORT_PCR_MUX(6);
      else if(FTM3_CH7 == PTC11)
        PORT_PCR_REG(PORTC,11)=PORT_PCR_MUX(3);
      break;
    default:  return;
    }
  default:  break;
  }

  /********************************************************/
  FTM_CnSC_REG(ftmn, ch) &= ~FTM_CnSC_ELSA_MASK;
  FTM_CnSC_REG(ftmn, ch) = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
  /******************** 配置时钟和分频 ********************/
  FTM_SC_REG(ftmn)    = FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断
  FTM_CNTIN_REG(ftmn) = 0;                                                      // Channel (n) Value  。设置脉冲宽度：(CnV - CNTIN).
  FTM_MOD_REG(ftmn)   = mod;                                                    //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
  FTM_CNTIN_REG(ftmn) = 0;                                                      //Counter Initial Value 计数器初始化值
  FTM_CnV_REG(ftmn, ch) = cv;
  FTM_CNT_REG(ftmn)   = 0;                                                      //计数器。只有低16位可用

}

void FTM_PWM_Duty(FTM_Type * ftmn,unsigned char ch, u32 duty)
{
  u32 cv;
  u32 mod;
  ASSERT((duty>=0)&&(duty<=FTM_PRECISON),"FTM PWMduty error");
  mod = FTM_MOD_REG(ftmn);        //读取 MOD 的值
  cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;
  FTM_CnV_REG(ftmn, ch) = cv;
}

void FTM_QD_Init(FTM_Type * ftmn)
{
  ASSERT((ftmn==FTM1)||(ftmn==FTM2),"FTM module error");
  /******************* 开启时钟 和 复用IO口*******************/
  switch((u32)ftmn)
  {

  case FTM1_BASE:
    SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
    if(FTM1_QDPHA == PTA8)                  //管脚复用
      PORTA_PCR8= PORT_PCR_MUX(6);
    else if(FTM1_QDPHA == PTA12)

      PORTA_PCR12= PORT_PCR_MUX(7);
    else if(FTM1_QDPHA == PTB0)
      PORTB_PCR0= PORT_PCR_MUX(6);
    else if(FTM1_QDPHA == PTE20)
      PORTE_PCR20= PORT_PCR_MUX(5);
    if(FTM1_QDPHB == PTA9)
      PORTA_PCR9= PORT_PCR_MUX(6);
    else if(FTM1_QDPHB == PTA13)
      PORTA_PCR13= PORT_PCR_MUX(7);
    else if(FTM1_QDPHB == PTB1)
      PORTB_PCR1= PORT_PCR_MUX(6);
    else if(FTM1_QDPHB == PTE21)
      PORTE_PCR21= PORT_PCR_MUX(5);
    break;
  case FTM2_BASE:
    SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;
    if(FTM2_QDPHA == PTA10)
      PORTA_PCR10= PORT_PCR_MUX(6);
    else if(FTM2_QDPHA == PTB18)
      PORTB_PCR18= PORT_PCR_MUX(6);
    else if(FTM2_QDPHA == PTE22)
      PORTE_PCR22= PORT_PCR_MUX(5);
    else if(FTM2_QDPHA == PTA1)
      PORTA_PCR1= PORT_PCR_MUX(5);
    if(FTM2_QDPHB == PTA11)                  //管脚复用
      PORTA_PCR11= PORT_PCR_MUX(6);
    else if(FTM2_QDPHB == PTB19)
      PORTB_PCR19= PORT_PCR_MUX(6);
    else if(FTM2_QDPHB == PTE23)
      PORTE_PCR23= PORT_PCR_MUX(5);
    else if(FTM2_QDPHB == PTA2)
      PORTA_PCR2= PORT_PCR_MUX(5);
    break;
  }
  FTM_MODE_REG(ftmn)  |= FTM_MODE_WPDIS_MASK          //写保护禁止
    | FTM_MODE_FTMEN_MASK;      //使能 FTM
  FTM_QDCTRL_REG(ftmn) |=  FTM_QDCTRL_QUADMODE_MASK;
  FTM_CNTIN_REG(ftmn)   = 0;
  FTM_MOD_REG(ftmn)     = FTM_MOD_MOD_MASK;
  FTM_QDCTRL_REG(ftmn) |=  FTM_QDCTRL_QUADEN_MASK;    //FTM_QDCTRL_QUADMODE_MASK QUADMODE 0:相位模式 1:方向模式
  FTM_MODE_REG(ftmn)  |= FTM_MODE_FTMEN_MASK;
  FTM_CNT_REG(ftmn)     = 0;                    //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

int16 FTM_QD_Get(FTM_Type * ftmn)
{
  ASSERT((ftmn==FTM1)||(ftmn==FTM2),"FTM module error");
  int16 val;
  val = FTM_CNT_REG(ftmn);
  FTM_CNT_REG(ftmn) = 0;
  return val;
}