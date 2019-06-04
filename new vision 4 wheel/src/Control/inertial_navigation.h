#ifndef __INERTIAL_NAVIGATION_H__
#define __INERTIAL_NAVIGATION_H__

/*���ײ���������
  ��C��&&����R60Բ��ƴ��
*/
#define Real2Encoder    (10800)

#define Length_S1       (0.72f*Real2Encoder)             //1m          6000
#define Length_S2       (1.44f*Real2Encoder)         //1.44m           8000
#define Length_S3       (0.72f*Real2Encoder)             //1m          6000

#define Angel_S1        (0.7854f)//������
#define Angel_S2        (1.5708f)
#define Angel_S3        (0.7854f)


enum Block_STATE
{
  NoBlock,//û��·��
  Block_S0,//��״̬�Ѽ�⵽·�ϣ�����ѭ��������·��
  Block_S1,//��һ��Բ��������
  Block_S2,//�ڶ���Բ���ƹ�·��
  Block_S3,//������Բ���ص�����
};

typedef struct
{
  enum Block_STATE State;
  int Servo_duty_temp;
  int Speed_target_L;
  int Speed_target_R;
  int Speed_target_ave;
  int distance;
  float K;//K = V(������)/V(ʵ��)
  float V;//ʵ�����ٶ�    V = Speed_target_ave/K
  float R;//ת��뾶
  float alpha;//������
}Block_Data;

void roadblock_test(void);
void navigate_pre(void);
void navigation(void);
void roadblock_process(void);
#endif