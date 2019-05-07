/*
 * FTM.h
 *
 *  Created on: Feb 17, 2018
 *      Author: ZBT
 */

#ifndef USER_DRIVER_FTM_FTM_H_
#define USER_DRIVER_FTM_FTM_H_

#define FTM_PRECISON 65535u

typedef enum
{
    CH0,
    CH1,
    CH2,
    CH3,
    CH4,
    CH5,
    CH6,
    CH7,
} FTM_CHn;

void FTM_PWM_init(FTM_Type* ftmn,unsigned char ch, u32 freq, u32 duty);
void FTM_PWM_Duty(FTM_Type * ftmn,unsigned char ch, u32 duty);
void FTM_QD_Init(FTM_Type * ftmn);
int16 FTM_QD_Get(FTM_Type * ftmn);

#endif /* USER_DRIVER_FTM_FTM_H_ */
