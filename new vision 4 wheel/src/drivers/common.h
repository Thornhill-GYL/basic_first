/*
 * common.h
 *
 *  Created on: Feb 17, 2018
 *      Author: ZBT
 */

#ifndef USER_DRIVER_COMMON_H_
#define USER_DRIVER_COMMON_H_

#define OSC_FREQ_MHZ (48)
#define HIGH (1u)
#define LOW (0u)
#define EnableInterrupts  asm(" CPSIE i");//开总中断
#define DisableInterrupts asm(" CPSID i");//关总中断

extern int  core_clk_khz;
extern int  fastperipheral_clk_khz;
extern int  flexbus_clk_khz;
extern int  bus_clk_khz;

typedef unsigned char        uint8;
typedef unsigned short 	uint16;
typedef unsigned int 	uint32;
typedef unsigned long 	uint64;

typedef signed char	        int8;
typedef signed short        int16;
typedef signed int		int32;
typedef signed long		int64;


typedef volatile int8	vint8;
typedef volatile int16	vint16;
typedef volatile int32	vint32;

typedef volatile uint8  vuint8;
typedef volatile uint16	vuint16;
typedef volatile uint32	vuint32;


typedef uint8   u8;
typedef uint16  u16;
typedef uint32  u32;
typedef uint64  u64;

typedef int8    s8;
typedef int16   s16;
typedef int32   s32;
typedef int64   s64;

typedef enum
{
    PTA0,  PTA1,  PTA2,  PTA3,  PTA4,  PTA5,  PTA6,  PTA7,  PTA8,  PTA9,  PTA10, PTA11, PTA12, PTA13, PTA14, PTA15,
    PTA16, PTA17, PTA18, PTA19, PTA20, PTA21, PTA22, PTA23, PTA24, PTA25, PTA26, PTA27, PTA28, PTA29, PTA30, PTA31,
    PTB0,  PTB1,  PTB2,  PTB3,  PTB4,  PTB5,  PTB6,  PTB7,  PTB8,  PTB9,  PTB10, PTB11, PTB12, PTB13, PTB14, PTB15,
    PTB16, PTB17, PTB18, PTB19, PTB20, PTB21, PTB22, PTB23, PTB24, PTB25, PTB26, PTB27, PTB28, PTB29, PTB30, PTB31,
    PTC0,  PTC1,  PTC2,  PTC3,  PTC4,  PTC5,  PTC6,  PTC7,  PTC8,  PTC9,  PTC10, PTC11, PTC12, PTC13, PTC14, PTC15,
    PTC16, PTC17, PTC18, PTC19, PTC20, PTC21, PTC22, PTC23, PTC24, PTC25, PTC26, PTC27, PTC28, PTC29, PTC30, PTC31,
    PTD0,  PTD1,  PTD2,  PTD3,  PTD4,  PTD5,  PTD6,  PTD7,  PTD8,  PTD9,  PTD10, PTD11, PTD12, PTD13, PTD14, PTD15,
    PTD16, PTD17, PTD18, PTD19, PTD20, PTD21, PTD22, PTD23, PTD24, PTD25, PTD26, PTD27, PTD28, PTD29, PTD30, PTD31,
    PTE0,  PTE1,  PTE2,  PTE3,  PTE4,  PTE5,  PTE6,  PTE7,  PTE8,  PTE9,  PTE10, PTE11, PTE12, PTE13, PTE14, PTE15,
    PTE16, PTE17, PTE18, PTE19, PTE20, PTE21, PTE22, PTE23, PTE24, PTE25, PTE26, PTE27, PTE28, PTE29, PTE30, PTE31,
} PTXn;

void ASSERT(unsigned char x,unsigned char* Log);
void System_init(void);
void Loop(void);
void Setup(void);
void  delayms(uint32  ms);
void  delayus(uint32  us);
__ramfunc int abs(int d);
__ramfunc int sign(int i);
#endif /* USER_DRIVER_COMMON_H_ */
