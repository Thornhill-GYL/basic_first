#ifndef __ISLAND_H__
#define __ISLAND_H__

#include "common.h"
#include "tools.h"
enum ISLAND_STATE
{
  NoIsland,
  Left_Island_pre,
  Right_Island_pre,
  Left_Island_in,
  Right_Island_in,
  Left_Island_out,
  Right_Island_out,
  Left_Wait_Next,
  Right_Wait_Next
};

typedef struct
{
  enum ISLAND_STATE State;
  
  u8 Image_Start_hang;
  u8  Correct_hang;     //用来补线的行
  int In_Center;
  int Stay_Center;
  Time_Delay_STR ADdouble;
  int Out_Center;//用作出环岛的标准
  
  u8  Stay_hang_use;
  Time_Delay_STR  Stay2Out;
  u8  Out_Allow_flag;
  
  Time_Delay_STR Out2doubleFAR;
  u8  doubleFAR_Allow_flag;
  
  u8  In2Stay_cnt;
  u8  Stay2Out_cnt;
  
  int Out_Center_;
  u16 Stay_Island_center_cnt;
    
  Time_Delay_STR Next_Island;
}Island_Data;

u8 Island_process(void);
u8 In_double_AD(void);
u8 Elec_Island(void);
u8 In_Island(void);
int In_Island_center(u8* hang);
u8 In2Stay_Island(int center,int hang);
u8 Stay_Island(void);
u8 Out_Island(void);
u8 Image_Island_Test(void);
int Stay2Out_test();
int Wait_Next_Island();
u8 Out_Island_find_Quanbai(void);

#endif