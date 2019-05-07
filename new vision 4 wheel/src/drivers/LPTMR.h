#ifndef __LPTMR_H__
#define __LPTMR_H__

void LPTMR_delay_us(uint16 us);
void LPTMR_time_close();
void LPTMR_time_start(void);
uint32 LPTMR_time_get_us(void);
uint32 LPTMR_time_get_ms(void);

#define LPTMR_delay_ms(ms) LPTMR_delay_us((ms)*1000)

#endif