#ifndef __MYMATH_H__
#define __MYMATH_H__
#include "common.h"

#define Max_2_num(x1,x2)       (((x1)>(x2))? (x1):(x2))
#define Min_2_num(x1,x2)       (((x1)<(x2))? (x1):(x2))
#define Abs_(x)                (((x) >= 0)? (x) :(-(x)))

typedef struct
{
  float m_filter;
  float ResrcData_mem[2];
  float output_mem[2];
}Filter_1st_Str; 

int max_s16(int temp[],u16 num);
u8 max_u8(u8 temp[],u16 num);
int min_s16(int temp[],u16 num);
u8 min_u8(u8 temp[],u16 num);
int power_s16(int buttom,u8 po);
int sum_s16(int input[],u16 num);
int sum_u8(u8 input[],u16 num);
u16 abs_s16(int i);
int sum_point(u8 input[],u8 num);
//u8 L_Diff_Test(int* Origin,u16 n,float* Circle_k,u8* _Turn);
//u8 R_Diff_Test(int* Origin,u16 n,float* Circle_k,u8* _Turn);
float calculate_fangcha(s16 *a,u16 cnt);
float myInvSqrt(float x);	/*���ٿ�ƽ����*/
u8 max_u8_index(u8 temp[],u16 num);
u8 min_u8_index(u8 temp[],u16 num);
u8 min_u8_index_RIsland(u8 temp[],u16 num);
u8 min_int_index(int temp[],u8 num);
int ave_s16(int temp[],const u16 num);
u8 ave_u8(u8 temp[],const u16 num);
float fave_s16(int temp[],const u16 num);
//int min_Y(Y_TEST temp[],u16 num);
float filter_1st(const float ResrcData, Filter_1st_Str *Fil);
void QuickSort(int *pArray, int iBegin, int iEnd);
void my_sort(int *a,int n);
#endif