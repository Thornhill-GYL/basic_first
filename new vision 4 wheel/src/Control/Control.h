#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "pid.h"
#include "common.h"

#define Camera_Mode     (0)
#define Elec_Mode       (1)

typedef struct{
  s16 Speed_L;
  s16 Speed_R;
  s16 Speed_All;
  int using_speed;
  float Rate;
  PID_Struct PID;//pid结构体
  int Duty_L;
  int Duty_R;
}Speed_Status;          //电机状态结构体

typedef struct{
  u8 Dir_mode;//循迹模式
  u8 diff_done_flag;
  int Dir_C_error;
  int Elec_Left;
  int Elec_Right;
  int Dir_E_error;
  float Width;                  //15.5cm
  float Length;                 //20cm
  float _2Real_V;               //0.15703
  float pwm2rad;                //PWM-DEGREE  0.00156
  float Diff_rate;
  PID_Struct PID_C;
  PID_Struct PID_E;
  PID_Struct PID_D;
}Dir_STR;                       //D=6.4cm


void Speed_control(void);
void Motor_control(void);
void Dir_control(void);
void Motor_init(void);
void Servo_init(void);
u8   CenterlineToDiff(int center);
void Diff_Speed(void);
void ADC_get_data(void);
void Elec_get_center(void);
void control_for_breakage(void);
void meeting_pre(void);
void god_with_us(void);
void Start_Slow_Run(void);
void Start_Slow_Run2(void);
void keep_safe(void);
#endif
