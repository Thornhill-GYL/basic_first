#ifndef __EVENT_DELAY_H__ 
#define __EVENT_DELAY_H__

#define PIT_DEC         (0)
#define DIS_DEC         (1)


void All_Delay_Init(void);
void All_Delay_Task(void);
void Out2doubleFAR_task(void);
void Next_Island_task(void);
void ADdouble_task(void);
void Stay2Out_task(void);

#endif