/*
 * Sys_init.h
 *
 *  Created on: Feb 14, 2018
 *      Author: ZBT
 */

#ifndef SYS_INIT_H_
#define SYS_INIT_H_

typedef struct
{
  int MotorP;
  int MotorI;
  int MotorD;
  float ServoP;
  float ServoD;
  float ServoDD;
  float ElecP;
  float ElecD;
  int DiffP;
  int DiffI;
  int Speedtarget;
  int Cnst;
  int Main_Line;
  unsigned char EnableKEY;
  unsigned char KeyPressed;
}InitParameter;
   
   
void System_init(void);
void nrf_init_connect(void);
void parameter_Init(void);
void Switch_Init(void);

#endif /* SYS_INIT_H_ */
