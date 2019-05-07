#include "LED_BEEP.h"
#include "include.h"

Beep_Str Beep_100ms = {.Delay_const=100};



void myLED_Init(void)
{
  GPIO_Init(LED_R_PORT,GPO,HIGH);
  GPIO_Init(LED_G_PORT,GPO,HIGH);
  GPIO_Init(LED_B_PORT,GPO,HIGH);
}

void myBEEP_Init(void)
{
  GPIO_Init(BUZ_PORT,GPO,LOW);
  BEEP_Open_once();
} 

void BEEP_Open_once(void)
{
  BEEP=1;
  delayms(10);
  BEEP=0;
} 

void Beep_Once(Beep_Str* Beep)
{
  BEEP = 1;
  Beep->Flag = 1;
  Beep->Delay = Beep->Delay_const;
}

void Beep_ISR(Beep_Str* Beep)
{
  if(Beep->Flag == 1)
  {
    if(Beep->Delay==0)
    {
      BEEP = 0;
      Beep->Flag = 0;
    }
    Beep->Delay -= PIT_PERIOD;
  }
}