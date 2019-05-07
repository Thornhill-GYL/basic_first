/*
 * pit.h
 *
 *  Created on: Feb 18, 2018
 *      Author: ZBT
 */

#ifndef USER_DRIVER_PIT_PIT_H_
#define USER_DRIVER_PIT_PIT_H_

typedef enum PITn
{
    PIT0,
    PIT1,
    PIT2,
    PIT3
} PITn;
void            pit_init(PITn, u32 cnt);
#define         pit_init_ms(PITn,ms)          pit_init(PITn,ms * bus_clk_khz);
#define         PIT_Flag_Clear(PITn)          PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK

void            pit_delay(PITn pitn, u32 cnt);                            //PIT延时
void            pit_time_start(PITn pitn);                                  //PIT计时
u32             pit_time_get(PITn pitn);

#define         pit_delay_ms(PITn,ms)          pit_delay(PITn,ms * bus_clk_khz);        //PIT延时 ms
#define         pit_delay_us(PITn,us)          pit_delay(PITn,us * bus_clk_khz/1000);   //PIT延时 us
#define         pit_time_get_ms(pitn)          (pit_time_get(pitn)/bus_clk_khz)
#define         pit_time_get_us(pitn)          (pit_time_get(pitn)/(bus_clk_khz/1000))


#endif /* USER_DRIVER_PIT_PIT_H_ */
