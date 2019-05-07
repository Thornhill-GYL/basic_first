#ifndef __PID_H__
#define __PID_H__

//PID�ṹ��
typedef struct                    //�ṹ�壬���PID��ر���
{
  float P;                        //����P
  float I;                        //����I
  float D;                        //����D
  float error[3];                 //���洢����
  float delat;                    //ÿ�εĵ��ڽ��
  float derr;                     //һ�����
  float dderr;                    //�������
  float result;                      //PID�����������������ʽ�����Գ�ֵ����Ϊ����ƽ��ʱ�����ֵ����Ҫ��������0Ҫ����ը��
  
  float target;                   //PID���ڵ�Ŀ��ֵ     
  float feedback;
  float UP_Limit;
  float DOWN_Limit;
}PID_Struct;

void PID_process(PID_Struct* pid);
void PID_Init(PID_Struct* pid,float P_,float I_,float D_,signed long Result,float Target,float up_limit,float low_limit);
void Diff_PID_Process(PID_Struct* pid);
#endif