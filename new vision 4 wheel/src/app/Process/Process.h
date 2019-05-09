#ifndef _PROCESS_H
#define _PROCESS_H

#define NORMAL_WIDTH(x) ((int)(570-3.79*(x)))
#define Cross_Center_Period_Const  (10)
//#define IMG_PIXEL(x,y) (((Image_fire[(y)][(x)/8])&(1<<((x)%8)))) //摄像头数据解压语句
#define CCD_START       (30)
#define CCD_END         (290)
#define Y_START         (200)
#define Y_END           (90)
#define Center_Stand    (153)
//typedef struct
//{
//  int black_white[270];
//  int continuous[20];
//  int status;
//  int color_flag;
//  int detect_flag;
//  int confirm_flag;
//  int detect_line;
//  int confirm_line;
//  int toelc;
//  int stop_flag;
//}Start;
enum BOARD_FIND
{
  NONE = 0,
  LEFT = 1,
  RIGHT = 2,
  BOTH = 3
};                      //赛道单行边界搜索结果

typedef struct
{
  int Left[240];
  int Right[240];
//  int Middle[240];
//  int Width[240];
//  int Width_stand[240];
  enum BOARD_FIND Find[240];
}ROAD_LINE;             //赛道单行信息

typedef struct
{
  int Y[320];//320列的边界
  int far;//0为边界位置，1,2,3为边界的列数(左中右)
  int farL;
  int farC;
  int farR;
  int Left;
  int Right;
  int Center;
  float Middle_Correct;
  int Three_Lie[3];
  int Three_lie_end[3];
}ROAD_Y_STR;

void image_process(void);
void Get_column_end(int col);
void Road_XY_Rebuild(void);
int get_Center(int hang);
void get_black_line(int hang);
u8 Find_All_White(int hang);
int All_White_Line(int hang,int *Left,int *Right);
int Find_Quanbai_Xie(int Left,int Right);
#endif