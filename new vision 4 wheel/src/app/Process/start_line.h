#ifndef _START_LINE_H
#define _START_LINE_H
typedef struct
{
    int black_white[270];
    int continuous[40];
    int status;
    int color_flag;
    int detect_flag;
    int confirm_flag;
    int detect_line;
    int confirm_line;
    int stop_count;
    int twice_stop;
    int final_stop;
    int dismove;
}Start_Data;

int detect_startline(int using_line);
void start_operate(void);
void start_init(void);
void double_start_move(void);
void single_start_move(void);
void start_led(void);
#endif