#include "include.h"
#include "Control.h"

Speed_Status Speed;
u16 AD_data[6];
Dir_STR Dir = {
                .Dir_mode = Camera_Mode,// .Dir_mode = Camera_Mode
                .Width  = 0.155f,
                .Length = 0.2f,
                ._2Real_V = 0.15703f,
                .pwm2rad = 0.0013f,
              };


void Speed_control(void)
{
  Speed.PID.target = Speed.using_speed;
  if(Block.State == NoBlock)
   Diff_Speed();
  else if(Block.State == Block_S0)
  {
//    Motor_L.PID.target = Block.Speed_target_ave;
//    Motor_R.PID.target = Block.Speed_target_ave;
  }
  //Motor_L.PID.target=Initialization.Speedtarget;
  // Motor_R.PID.target=Initialization.Speedtarget;
  else
  {
//    Motor_L.PID.target = Block.Speed_target_L;
//    Motor_R.PID.target = Block.Speed_target_R;
  }
  PID_process(&Speed.PID);
}


void Motor_control(void)
{
  Speed.Speed_L = -FTM_QD_Get(FTM1);
  Speed.Speed_R = FTM_QD_Get(FTM2);
  Speed.PID.feedback = Speed.Speed_All = (Speed.Speed_L + Speed.Speed_R)/2;
  Speed.Rate = Speed.Speed_All *1.0f / Speed.PID.target;
  if(Block.State > Block_S0)//1、2、3状态
  {
    Block.distance += Speed.Speed_All;
    if(Block.State == Block_S1&&(Block.distance)> Length_S1)
    {
      Beep_Once(&Beep_100ms);
      Block.State = Block_S2;
    }
    else if(Block.State == Block_S2&&(Block.distance) > (Length_S1+Length_S2))
    {
      Beep_Once(&Beep_100ms);
      Block.State = Block_S3;
    }
    else if(Block.State == Block_S3&&(Block.distance) > (Length_S1+Length_S2+Length_S3))
    {
      Beep_Once(&Beep_100ms);
      Block.State = NoBlock;
      Block.distance = 0;//清空
    }
  }
  
  Speed_control();

  Speed.Duty_L = (int)(Speed.PID.result + Dir.PID_D.result);
  Speed.Duty_R = (int)(Speed.PID.result - Dir.PID_D.result);
  if(Speed.Duty_L > 65500)
    Speed.Duty_L = 65500;
  else if(Speed.Duty_L < -65500)
    Speed.Duty_L = -65500;
  if(Speed.Duty_R > 65500)
    Speed.Duty_R = 65500;
  else if(Speed.Duty_R < -65500)
    Speed.Duty_R = -65500;
  if(Speed.Duty_L>=0)
  {
    MOTORL_DIR = 1;
    FTM_PWM_Duty(MOTORL_FTM,(u32)(65535-Speed.Duty_L));
  }
  else
  {
    MOTORL_DIR = 0;
    FTM_PWM_Duty(MOTORL_FTM,(u32)(-Speed.Duty_L));
  }
  if(Speed.Duty_R>=0)
  {
    MOTORR_DIR = 0;
    FTM_PWM_Duty(MOTORR_FTM,(u32)(Speed.Duty_R));
  }
  else
  {
    MOTORR_DIR = 1;
    FTM_PWM_Duty(MOTORR_FTM,(u32)(65535+Speed.Duty_R));
  }
}

void Dir_control(void)
{
  if(Block.State > Block_S0)
  {
    Block.alpha = atanf(Dir.Length / Block.R);
    Block.Servo_duty_temp = (u32)(Block.alpha/Dir.pwm2rad+SERVO_MIDDLE);
    if(Block.Servo_duty_temp > SERVO_MIDDLE + SERVO_HALF)
      Block.Servo_duty_temp = SERVO_MIDDLE + SERVO_HALF;
    else if(Block.Servo_duty_temp < SERVO_MIDDLE - SERVO_HALF)
      Block.Servo_duty_temp = SERVO_MIDDLE - SERVO_HALF;
    FTM_PWM_Duty(SERVO_FTM,Block.Servo_duty_temp);
    return;
  }
  
  
  if(Dir.Dir_mode == Camera_Mode)
  {
   Dir.PID_C.feedback = Dir.Dir_C_error;
   Diff_PID_Process(&Dir.PID_C);
//   DIR.PID.result += Initialization.ServoDD * mpu9250_Gyro_x * mpu9250_Gyro_x * mpu9250_Gyro_x/28000000000.0f;
   FTM_PWM_Duty(SERVO_FTM,(u32)(Dir.PID_C.result+SERVO_MIDDLE));
   }
  else if(Dir.Dir_mode == Elec_Mode)
  {
    Dir.PID_E.feedback = Dir.Dir_E_error;
    Diff_PID_Process(&Dir.PID_E);
//    Elec_PID.PID.result += Initialization.ServoDD * mpu9250_Gyro_x * mpu9250_Gyro_x * mpu9250_Gyro_x/28000000000.0f;
    FTM_PWM_Duty(SERVO_FTM,(u32)(Dir.PID_E.result+SERVO_MIDDLE));
  }
}

void Diff_Speed(void)//using_speed
{
  static float TAN_theta = 0;
  static float Theta = 0;
  float Diff_K = 0;
  if(Dir.Dir_mode == Camera_Mode)
  {
    Theta = Dir.PID_C.result*Dir.pwm2rad;
  }
  else if(Dir.Dir_mode == Elec_Mode)
  { 
    Theta = Dir.PID_E.result*Dir.pwm2rad;
  }
  TAN_theta = tanf(Theta);
//  TAN_theta *= 0.9;//抑制超调对差速的影响
  Diff_K = Dir.Width*TAN_theta/(2*Dir.Length);
  Dir.PID_D.target = Speed.using_speed * Diff_K;
  Dir.PID_D.feedback = (Speed.Speed_R - Speed.Speed_L)/2.0f;
//  Dir.Diff_rate = Dir.PID_D.feedback/Dir.PID_D.target;
  Diff_PID_Process(&Dir.PID_D);
  
}

u8 CenterlineToDiff(int center)
{
  if(Dir.diff_done_flag == 1)return 0;
  
//*********************对中心点均值滤波*************************************  
  
  Dir.diff_done_flag = 1;
  Dir.Dir_C_error = Center_Stand - center;
  if(Dir.Dir_C_error > 105)
    Dir.Dir_C_error = 105;
  else if(Dir.Dir_C_error < -105)
    Dir.Dir_C_error = -105;
  
  
  return 0;
}

void Motor_init(void)
{
  FTM_PWM_init(MOTORL_FTM, 13000, 65535);
  GPIO_Init(MOTORL_DIR_PORT, GPO, HIGH);
  FTM_PWM_init(MOTORR_FTM, 13000, 0);
  GPIO_Init(MOTORR_DIR_PORT, GPO, LOW);
  FTM_QD_Init(FTM1);
  FTM_QD_Init(FTM2);
  PID_Init(&Speed.PID,
           Initialization.MotorP,
           Initialization.MotorI,
           Initialization.MotorD,
           50000,
           Initialization.Speedtarget,
           65500,
           -65500);
  PID_Init(&Dir.PID_D,
         Initialization.DiffP,
         Initialization.DiffI,
         0,
         50000,
         Initialization.Speedtarget,
         40000,
         -40000);
  Speed.using_speed=Initialization.Speedtarget;
}

void Servo_init(void)
{
  FTM_PWM_init(SERVO_FTM,50,SERVO_MIDDLE);
  PID_Init(&Dir.PID_C,
           Initialization.ServoP,
           0,
           Initialization.ServoD,
           0,
           0,
           SERVO_HALF,
           -SERVO_HALF);
  PID_Init(&Dir.PID_E,
           Initialization.ElecP,
           0,
           Initialization.ElecD,
           0,
           0,
           SERVO_HALF,
           -SERVO_HALF);
  
}
void ADC_get_data(void)
{
  //long timeout = 0;

  AD_data[0] = ADC0_Once(ADC0_SE7b,ADC_12bit);
  AD_data[1] = ADC0_Once(ADC0_SE11,ADC_12bit);
  for(int i=0;i<2;i++)
  {
    if(AD_data[i]<=5)
      AD_data[i]=5;
    else if(AD_data[i]>=4050)
    {
      AD_data[i]=4050;
    }
  }
  

  Elec_get_center();
  
}

//差比和 

//差比和的目的在于把由于两电感的差比和的值与车的位置用线性相关的方式表现出来，然而实际上
//这个值在车靠近中心的地方能够很好的线性表出，但赛道边缘地方（车偏向赛道极左或极右），由于电感中至少一个值的减小，
//差比和分母急剧减小，导致趋势此时并不是线性减小，甚至有时反而会逆势上升，所以需要特殊限幅处理
void Elec_get_center()
{
  s16 now_err = 0;
  static int delta=500;
  Dir.Elec_Left=AD_data[0];
  Dir.Elec_Right=AD_data[1];
 
    if(Min_2_num(Dir.Elec_Left,Dir.Elec_Right)<300)//防止其中至少一个电感值因为太小而失效的处理 400数值的调整应该是卡在你差比和数据图像异常变化之前（我的电感归一后满值是3100）
    {
      if(Abs_(Dir.Elec_Left-Dir.Elec_Right) > Abs_(delta))//存在至少一个电感值太小时，只有当差值大于历史值才算有效
      {
        if((Dir.Elec_Left-Dir.Elec_Right)*delta > 0)    //并且此时因为不可能车突然从正偏差变到负偏差（即不可能上一次左电感>右，下次右>左），该情况也要过滤
        {
          delta=Dir.Elec_Left-Dir.Elec_Right;
        }
        else ;
       }  
       else ;
     }
     else
     {
      delta=Dir.Elec_Left-Dir.Elec_Right;
     }	
  
    now_err=(int)(400*(delta)/(Dir.Elec_Left+Dir.Elec_Right+1));//差比和
    if(now_err>500)			//330
    {
      now_err=500;
    }
    else;
    
    if(now_err<-500)
    {
      now_err=-500;
    }
    else;
    
    Dir.Dir_E_error = now_err;
}




