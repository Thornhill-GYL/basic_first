#ifndef __MYBEEP_H__
#define __MYBEEP_H__

#include "common.h"

typedef struct
{
  u8 Flag;
  u16 Delay;
  u16 Delay_const;
}Beep_Str;

void myLED_Init(void);
void myBEEP_Init(void);
void BEEP_Open_once(void);
void Beep_Once(Beep_Str* Beep);
void Beep_ISR(Beep_Str* Beep);


#endif