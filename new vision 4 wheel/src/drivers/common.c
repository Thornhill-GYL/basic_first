/*
 * common.c
 *
 *  Created on: Feb 17, 2018
 *      Author: ZBT
 */

#include "include.h"
#include "common.h"

int  core_clk_khz = 240000;
int  fastperipheral_clk_khz = 120000;
int  flexbus_clk_khz = 60000;
int  bus_clk_khz = 24000;


void ASSERT(unsigned char x,unsigned char* Log)
{
  if(x)
    return;
  else
    Error_handler(Log);
}

void  delayms(uint32  ms)
{

    uint32  i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = core_clk_khz; j > 0; j--)
        {
            asm("nop");
        }
    }
}

void  delayus(uint32  us)
{

    uint32 j;
    for(j = (int)(bus_clk_khz*us/1000.0); j > 0; j--)
    {
      asm("nop");
    }
}

__ramfunc int abs(int d)
{
  if(d>=0)
    return d;
  else
    return -d;
}

__ramfunc int sign(int i)
{
  if(i>0)
    return 1;
  else if(i<0)
    return -1;
  else
    return 0;
}