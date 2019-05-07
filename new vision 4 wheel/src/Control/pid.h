#ifndef __PID_H__
#define __PID_H__

//PID结构体
typedef struct                    //结构体，存放PID相关变量
{
  float P;                        //参数P
  float I;                        //参数I
  float D;                        //参数D
  float error[3];                 //误差存储数组
  float delat;                    //每次的调节结果
  float derr;                     //一阶误差
  float dderr;                    //二阶误差
  float result;                      //PID的输出，由于是增量式的所以初值请设为导轨平衡时的输出值，重要！不能是0要不就炸了
  
  float target;                   //PID调节的目标值     
  float feedback;
  float UP_Limit;
  float DOWN_Limit;
}PID_Struct;

void PID_process(PID_Struct* pid);
void PID_Init(PID_Struct* pid,float P_,float I_,float D_,signed long Result,float Target,float up_limit,float low_limit);
void Diff_PID_Process(PID_Struct* pid);
#endif