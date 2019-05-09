#ifndef __CROSS_H__
#define __CROSS_H__

#include "common.h"
enum Cross_STATE//ʮ��״̬
{
  NoCross,//û��ʮ��
  R2Cross_Pre, //����������б��ʮ��
  L2Cross_Pre, //����������б��ʮ��
  R2Cross_True,//���������б��ʮ��
  L2Cross_True,//���������б��ʮ��
  Str2Cross//ֱ��ʮ��
};

typedef struct//ʮ�������ṹ��
{
  enum Cross_STATE State;
  int In_center;
  int Center;//����Image_processĩβ
  u8  Test_hang;
  int Str_Cross_test_hang;
}Cross_Data;

u8 Cross_process(void);
u8 In_Cross_test();
u8 Cross_pre_test(void);
u8 Cross_curve_test();
u8 In_Cross(void);//б��ʮ��
u8 Cross_center_test(int* start_end, int* end_end);
u8 Str_Cross(void);
u8 Str_Cross_Test(void);

#endif