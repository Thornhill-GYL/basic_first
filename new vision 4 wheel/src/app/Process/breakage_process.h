#ifndef _BREAKAGE_PROCESS_H
#define _BREAKAGE_PROCESS_H
typedef struct
{
    int breakage_line;
    int angle_line;
    int part_flag;
    int limit_flag;
    int combine_flag;
    int black_flag;
    int angle_flag;
    int breakage_flag;
    int middle_stable;
    int middle_with_black;
    int middle_with_white;
    int sum_black;
    int sum_white;
    int sure_flag;
    int meeting_flag;
    int back_straight;
    int back_sure;
}Break_Data;
typedef struct
{
    int dot_color[300];
    int left_line[5][2];
    int right_line[5][2];
    int sum_black;
    int sum_white;
    int status_time;
    int line_count[3];
    int left_delat_line[4];
    int right_delat_line[4];
    int left_standard;
    int right_standard;
    int decrease;
    int maybe;
    int pre_left_may;
    int pre_right_may;

}Angle_Data;

typedef struct
{
    int back_part;
    int back_black;
    int back_limit;
    int back_straight;
    int back_sure;
}Back_Data;
typedef struct
{
    int start_out_flag;
    int go_back_flag;
    int normal_flag;
    int diff_flag;
    int quit_safe;
    long int Dist_L;
    long int Dist_R;
    long int get_Dist_L;
    long int get_Dist_R;
    long int aver_D;
}Move_Data;
typedef struct
{
  int x;
  int y;
}coordinate;
typedef struct
{
  int far_x;
  int far_y;
  int count_lie;
  int delat_ly[5];
  int delat_ry[5];
  int delat_ty[300];
  int three_hang;
  coordinate up_line[300];
  int scan_flag;
}Scanbreak_Data;
int angle_count(int using_line);//计算点
int angle_breakage(int using_line);//使用角来判断断路
int normal_breakage(int using_line);//在正常的道路上判断断路
int black_straight(int using_line);//判断断路上的黑色区域
int part_straight(int using_line);//直到检测
int limit_straight(int using_line);//直到限制检测
int combine_straight(int using_line);//直道综合检测
int back_limit(int using_line);
void back_straight(int using_line);//重回直道
void break_data_init(void);
void pattern_shift(void);//模式切换
void detect_led(void);//LED灯亮检测
void breakage_move(void);//双车断路会车时运动
void single_control(void);//断路单车控制
void double_control(void);//断路栓车控制
void ad_breakage(void);//检测黑胶带
extern int AD_in;

void scan_breakage(void);//从下往上扫得到点的坐标
void scan_sure(void);
#endif