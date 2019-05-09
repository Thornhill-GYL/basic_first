#ifndef __CROSS_H__
#define __CROSS_H__

#include "common.h"
enum Cross_STATE//十字状态
{
  NoCross,//没有十字
  R2Cross_Pre, //可能在向右斜入十字
  L2Cross_Pre, //可能在向左斜入十字
  R2Cross_True,//真的在向右斜入十字
  L2Cross_True,//真的在向左斜入十字
  Str2Cross//直入十字
};

typedef struct//十字描述结构体
{
  enum Cross_STATE State;
  int In_center;
  int Center;//传入Image_process末尾
  u8  Test_hang;
  int Str_Cross_test_hang;
}Cross_Data;

u8 Cross_process(void);
u8 In_Cross_test();
u8 Cross_pre_test(void);
u8 Cross_curve_test();
u8 In_Cross(void);//斜入十字
u8 Cross_center_test(int* start_end, int* end_end);
u8 Str_Cross(void);
u8 Str_Cross_Test(void);

#endif