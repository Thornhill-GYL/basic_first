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
//
//enum Cross_STATE//十字状态
//{
//  NoCross,//没有十字
//  R2Cross_Pre, //可能在向右斜入十字
//  L2Cross_Pre, //可能在向左斜入十字
//  R2Cross_True,//真的在向右斜入十字
//  L2Cross_True,//真的在向左斜入十字
//  Str2Cross//直入十字
//};
//
//typedef struct
//{
//  float num;
//  int hang;
//}Bondary;//断路结构体
//
//typedef struct//十字描述结构体
//{
//  enum Cross_STATE State;
//  int In_center;
//  int Center;//传入Image_process末尾
//  u8  Test_hang;
//  int Str_Cross_test_hang;
//  int  Cross_delay_cnt;
//  u16  Cross_delay_cnt_const;
//  u8   Cross_delay_flag;
//}Cross_Data;
//
//enum ISLAND_STATE
//{
//  NoIsland,
//  Left_Island_pre,
//  Right_Island_pre,
//  Left_Island_in,
//  Right_Island_in,
//  Left_Island_out,
//  Right_Island_out,
//  Left_Wait_Next,
//  Right_Wait_Next
//};
//
//#define Island_Center_Period_Const  (25)
//typedef struct
//{
//  enum ISLAND_STATE State;
//  
////  float L_P;
////  float L_D;
////  float R_P;
////  float R_D;
////  u16   Speed;
//  
//  u16 black_L[40];
//  u16 black_R[40];
//  
//  u8 Image_Start_hang;
//  int Image_Start_hang_half;
//  u8 Half_longer_cnt;
//  u8  Correct_hang;     //用来补线的行
//  int In_Center;
//  
//  int Stay_Center;
//  
//  int Out_Center;//用作出环岛的标准
//  
//  u8  Stay_hang_use;
//  u8  Stay2Out_flag;
//  u16 Stay2Out_flag_delay;
//  u16 Stay2Out_flag_delay_const;
//  u8  Out_Allow_flag;
//  
//  u8  Out2doubleFAR_flag;
//  u16 Out2doubleFAR_flag_delay;
//  u16 Out2doubleFAR_flag_delay_const;
//  u8  doubleFAR_Allow_flag;
//  
//  u8  In2Stay_cnt;
//  u8  Stay2Out_cnt;
//  
//  int Out_Center_[100];
//  s16 Out_error;
//  u16 Stay_Island_center_cnt;
//  u16 Stay_Island_center_cnt_all;
//  
//  u8  Next_Island_flag;         //和下一个环岛之间的时间间隔
//  u16 Next_Island_flag_delay;
//  u16 Next_Island_flag_delay_const;
//  
//  u8  _2Next_Island_cnt;
//}Island_Data;
//
void image_process(void);
void Get_column_end(int col);
void Road_XY_Rebuild(void);
int get_Center(int hang);
void get_black_line(int hang);
//u8 Cross_process(void);
//u8 In_Cross_test();//斜入十字检测;
//u8 Cross_pre_test(void);
//u8 Cross_curve_test();
//u8 In_Cross(void);//斜入十字
//u8 Cross_center_test(int* start_end, int* end_end);//和出环岛时找中心点的代码一样
//u8 Str_Cross(void);
//u8 Str_Cross_Test(void);
////下面的函数是用于判断断路的
//
//int breakage_judge(void);
//
//void change_pattern(void);
//int judge_straight_new(int using_line);
//int limit_judge(int using_line);
//int straight_all_judge(int using_line);
//
//void show_the_line(void);
//int breakage_new(int using_line);
//void block_detect(void);
//u8 Island_process(void);
//u8 In_double_AD(void);
//u8 Out_double_AD(void);
//u8 Elec_Island(void);
//u8 In_Island(void);
//int In_Island_center(u8* hang);
//u8 In2Stay_Island(int center,int hang);
//u8 Stay_Island(void);
//u8 Out_Island(void);
//u8 Out_Island_find_Quanbai(void);
//u8 Image_Island_Test(void);
//int Test_Far_Lie();
//int Test_jubu_Far_Lie(int lie,int cnt,int *far_end);
//int Test_jubu_Near_lie(int lie,int cnt,int *near_end);
//int Out_Island_Test(int* start_end, int* end_end);
//int Stay2Out_test();
//int Wait_Next_Island();
//int Wait_Next_center(u8* hang);
//int detect_startline(int line);
//void start_operate(void);
//int All_White_Line(int hang,int *Left,int *Right);
//int Find_Quanbai_Xie(int Left,int Right);
#endif