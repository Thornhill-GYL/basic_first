#ifndef __INERTIAL_NAVIGATION_H__
#define __INERTIAL_NAVIGATION_H__

/*本套参数适用于
  新C车&&三个R60圆弧拼接
*/
#define Real2Encoder    (10800)

#define Length_S1       (0.72f*Real2Encoder)             //1m          6000
#define Length_S2       (1.44f*Real2Encoder)         //1.44m           8000
#define Length_S3       (0.72f*Real2Encoder)             //1m          6000

#define Angel_S1        (0.7854f)//弧度制
#define Angel_S2        (1.5708f)
#define Angel_S3        (0.7854f)


enum Block_STATE
{
  NoBlock,//没有路障
  Block_S0,//此状态已检测到路障，正常循迹，对正路障
  Block_S1,//第一个圆弧出赛道
  Block_S2,//第二个圆弧绕过路障
  Block_S3,//第三个圆弧回到赛道
};

typedef struct
{
  enum Block_STATE State;
  int Servo_duty_temp;
  int Speed_target_L;
  int Speed_target_R;
  int Speed_target_ave;
  int distance;
  float K;//K = V(编码器)/V(实际)
  float V;//实际线速度    V = Speed_target_ave/K
  float R;//转弯半径
  float alpha;//舵机打角
}Block_Data;

void roadblock_test(void);
void navigate_pre(void);
void navigation(void);
void roadblock_process(void);
#endif