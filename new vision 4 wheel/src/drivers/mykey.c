#include "mykey.h"
#include "include.h"


void myKEY_Init(void)
{
  GPIO_Init(KEYU_PORT,GPI,HIGH);
  port_init(KEYU_PORT,PULL_UP);
  GPIO_Init(KEYD_PORT,GPI,HIGH);
  port_init(KEYD_PORT,PULL_UP);
  GPIO_Init(KEYR_PORT,GPI,HIGH);
  port_init(KEYR_PORT,PULL_UP);
  GPIO_Init(KEYL_PORT,GPI,HIGH);
  port_init(KEYL_PORT,PULL_UP);
  GPIO_Init(KEYC_PORT,GPI,HIGH);
  port_init(KEYC_PORT,PULL_UP);
}

//void myKEY_Exti_Init(void)
//{
//   exti_init(PORTB,1,falling_up);
//   exti_init(PORTA,17,falling_up);
//   exti_init(PORTA,15,falling_up);
//}

void myKEY_Exti_Init(void)
{
  port_init(KEYU_PORT, GPI | FALLING | PULL_UP);
  port_init(KEYD_PORT, GPI | FALLING | PULL_UP);
  port_init(KEYR_PORT, GPI | FALLING | PULL_UP);
  port_init(KEYL_PORT, GPI | FALLING | PULL_UP);
  port_init(KEYC_PORT, GPI | FALLING | PULL_UP);
}

u8 KEY_Scan(void)
{
  static u8 key_up=1;//按键按松开标志	  
  if(key_up&&(KEYU==0||KEYD==0||KEYL==0||KEYR==0||KEYC==0))
  {
    delayms(10);//去抖动 
    key_up=0;
    if(KEYU==0)                  return KEYU_PRES;
    else if(KEYD==0)            return KEYD_PRES;
    else if(KEYL==0)            return KEYL_PRES;
    else if(KEYR==0)            return KEYR_PRES;
    else if(KEYC==0)            return KEYC_PRES;
  }
  else if(KEYU==1&&KEYD==1&&KEYL==1&&KEYR==1&&KEYC==1)
  {
    key_up=1; 	    
  }
  return 0;// 无按键按下
}


//  static u8 Key;
//  Key = KEY_Scan();
//  if(Key == KEYU_PRES)
//  {
//  }
//  else if(Key == KEYD_PRES)
//  {
//  }
//  else if(Key == KEYL_PRES)
//  {
//  }
//  else if(Key == KEYR_PRES)
//  {
//  }
//  else if(Key == KEYC_PRES)
//  {
//  }
