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
    int back_limit;
    int back_straight;
    int back_sure;
}Back_Data;

int angle_count(int using_line);//计算点
int angle_breakage(int using_line);//使用角来判断断路
int normal_breakage(int using_line);//在正常的道路上判断断路
int black_straight(int using_line);//判断断路上的黑色区域
int part_straight(int using_line);//直到检测
int limit_straight(int using_line);//直到限制检测
int combine_straight(int using_line);//直道综合检测
void back_straight(int using_line);//重回直道
void break_data_init(void);
void pattern_shift(void);//模式切换
void detect_led(void);//LED灯亮检测
void breakage_move(void);
void single_control(void);
void double_control(void);
#endif