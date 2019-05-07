#include "include.h"
#include "mymath.h"

int max_s16(int temp[],u16 num)
{
  u16 i;
  int max=-32768;
  for(i=0;i<num;i++)
  {
    if(max<temp[i])
    {
      max=temp[i];
    }
  }
  return max;
}

u8 max_u8(u8 temp[],u16 num)
{
  u16 i;
  u8 max=0;
  for(i=0;i<num;i++)
  {
    if(max<temp[i])
    {
      max=temp[i];
    }
  }
  return max;
}


u8 max_u8_index(u8 temp[],u16 num)
{
  u16 i;
  u8 max=0;
  u8 index=0;
  for(i=0;i<num;i++)
  {
    if(max<temp[i])
    {
      max=temp[i];
      index=i;
    }
  }
  return index;
}

u8 min_u8_index(u8 temp[],u16 num)
{
  u16 i;
  u8 min=0xff;
  u8 index=0;
  for(i=0;i<num;i++)
  {
    if(min>temp[i])
    {
      min=temp[i];
      index = i;
    }
  }
  return index;
}

u8 min_u8_index_RIsland(u8 temp[],u16 num)
{
  u16 i;
  u8 min=0xff;
  u8 index=0;
  for(i=0;i<num;i++)
  {
    if(min>=temp[i])
    {
      min=temp[i];
      index = i;
    }
  }
  return index;
}

u8 min_int_index(int temp[],u8 num)
{
  u8 i;
  int min=32767;
  u8 index=0;
  for(i=0;i<num;i++)
  {
    if(min>temp[i])
    {
      min=temp[i];
      index = i;
    }
  }
  return index;
}

int min_s16(int temp[],u16 num)
{
  u16 i;
  int min=32767;
  for(i=0;i<num;i++)
  {
    if(min>temp[i])
    {
      min=temp[i];
    }
  }
  return min;
}
u8 min_u8(u8 temp[],u16 num)
{
  u16 i;
  u8 min=0xff;
  for(i=0;i<num;i++)
  {
    if(min>temp[i])
    {
      min=temp[i];
    }
  }
  return min;
}

int power_s16(int buttom,u8 po)
{
  u8 i;
  int result=1;//0次幂为1
  for(i=0;i<po;i++)
  {
    result*=buttom;
  }
  return result;
}

int sum_s16(int input[],u16 num)
{
  u16 i;
  int sum=0;
  for(i=0;i<num;i++)
  {
    sum+=input[i];
  }
  return sum;
}
int sum_u8(u8 input[],u16 num)
{
  u16 i;
  int sum=0;
  for(i=0;i<num;i++)
  {
    sum+=input[i];
  }
  return sum;
}
int sum_point(u8 input[],u8 num)
{
  u8 i;
  int sum=0;
  u8 temp = 0;
  for(i=0;i<num;i++)
  {
    temp = input[i];
    while(temp!=0)
    {
      temp = temp & (temp-1);
      sum++;
    }
  }
  return sum;
}

u16 abs_s16(int i)
{
    if(i>=0)return i;
  else return -i;
}


//signed char L_Diff_temp[70];
//signed char L_Second_Diff_temp[70];
//signed char R_Diff_temp[70];
//signed char R_Second_Diff_temp[70];
//u8 L_Diff_Test(int* Origin,u16 n,float* Circle_k,u8* _Turn)
//{
//  float k1,k2;
//  int sum_temp=0;
//  u8 turn_point=0;
//  u8 i,j,cnt=0,end_cnt=n;
//  for(i=0;i<n;i++)
//  {
//    L_Diff_temp[i]=Origin[i+1]-Origin[i];
//    if((Origin[i+1]>=320)||(Origin[i+1]<=0))
//    {
//      end_cnt = i;
//      break;
//    }
//  }
//  for(i=0;i<n-1;i++)
//  {
//    L_Second_Diff_temp[i]=L_Diff_temp[i+1]-L_Diff_temp[i];
//    if(abs_s16(L_Second_Diff_temp[i])<2)cnt++;
//    else if(abs_s16(L_Second_Diff_temp[i])>3)turn_point=i;
//  }
//  if(turn_point!=0)
//  {
//    *_Turn=turn_point;
//    sum_temp=0;
//    for(i=0;i<turn_point;i++)
//      sum_temp+=L_Diff_temp[i];
//    k1=sum_temp*1.0/turn_point;
//    sum_temp=0;
//    for(i=turn_point;i<end_cnt;i++)
//      sum_temp+=L_Diff_temp[i];
//    k2=sum_temp*1.0/(end_cnt-turn_point);
//    if(fabs(k1-k2)>2)
//    {
//      *Circle_k=k2;
//      return 2;
//    }
//  }
//  else if(cnt>(n*4.0/5))return 1;//线性的
//  else return 0;//非线性的
//}
//      
//u8 R_Diff_Test(int* Origin,u16 n,float* Circle_k,u8* _Turn)
//{
//  float k1,k2;
//  int sum_temp=0;
//  u8 turn_point=0;
//  u8 i,j,cnt=0,end_cnt=n;
//  for(i=0;i<n;i++)
//  {
//    R_Diff_temp[i]=Origin[i+1]-Origin[i];
//    if((Origin[i+1]>=319)||(Origin[i+1]<=0))
//    {
//      end_cnt = i;
//      break;
//    }
//  }
//  for(i=0;i<end_cnt-2;i++)
//  {
//    R_Second_Diff_temp[i]=R_Diff_temp[i+1]-R_Diff_temp[i];
//    if(abs_s16(R_Second_Diff_temp[i])<2)cnt++;
//    else if(abs_s16(R_Second_Diff_temp[i])>3)turn_point=i;
//  }
//  if(turn_point!=0)
//  {
//    *_Turn=turn_point;
//    sum_temp=0;
//    for(i=0;i<turn_point;i++)
//      sum_temp+=R_Diff_temp[i];
//    k1=sum_temp*1.0/turn_point;
//    sum_temp=0;
//    for(i=turn_point;i<end_cnt;i++)
//      sum_temp+=R_Diff_temp[i];
//    k2=sum_temp*1.0/(end_cnt-turn_point);
//    *Circle_k=k2;
//    return 2;
//  }
//  else if(cnt>(n*4.0/5))return 1;//线性的
//  else return 0;//非线性的
//}


float calculate_fangcha(s16 *a,u16 cnt)
{
  float sum = 0;
  float ave;
  u16 i;
  for (i = 0;i<cnt;i++)
  {
    sum += a[i];
  }
  ave = sum  / cnt;
  sum = 0;
  for (i = 0;i<cnt;i++)
  {
    sum += (a[i] - ave)*(a[i] - ave);
  }
  
  return sum/cnt;
}


float myInvSqrt(float x)	/*快速开平方求倒*/
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}


int ave_s16(int temp[],const u16 num)
{
  u16 i;
  int sum = 0;
  if(num==0)return -1;
  
  for(i=0;i<num;i++)
  {
    sum += temp[i];
  }
  
  return sum/num;
}

u8 ave_u8(u8 temp[],const u16 num)
{
  u16 i;
  u16 sum = 0;
  if(num==0)return -1;
  
  for(i=0;i<num;i++)
  {
    sum += temp[i];
  }
  
  return sum/num;
}


float fave_s16(int temp[],const u16 num)
{
  u16 i;
  int sum = 0;
  if(num==0)return -1;
  
  for(i=0;i<num;i++)
  {
    sum += temp[i];
  }
  
  return sum*1.0/num;
}

//int min_Y(Y_TEST temp[],u16 num)
//{
//  u16 i;
//  int min=10000;
//  for(i=0;i<num;i++)
//  {
//    if(min>temp[i].Y)
//    {
//      min=temp[i].Y;
//    }
//  }
//  return min;
//}

float filter_1st(const float ResrcData, Filter_1st_Str *Fil)
{
   Fil->ResrcData_mem[1] = Fil->ResrcData_mem[0];
   Fil->ResrcData_mem[0] = ResrcData;	 
   Fil->output_mem[1] = Fil->output_mem[0];	
   Fil->output_mem[0] = Fil->m_filter*Fil->ResrcData_mem[0]+(1-Fil->m_filter)*Fil->output_mem[1];	
  return Fil->output_mem[0];
}

void QuickSort(int *pArray, int iBegin, int iEnd)
{
    if (iBegin < iEnd)
    {
        int iLeft = iBegin;
        int iRight = iEnd;
        int iPivot = pArray[iBegin];

        while (iLeft < iRight)
        {
            while (iLeft < iRight && pArray[iRight] >= iPivot)
            {
                iRight--;
            }
            if(iLeft < iRight) pArray[iLeft++] = pArray[iRight];

            while (iLeft < iRight && pArray[iLeft] <= iPivot)
            {
                iLeft++;
            }
            if(iLeft < iRight) pArray[iRight--] = pArray[iLeft];
        }
        pArray[iLeft] = iPivot;
        QuickSort(pArray, iBegin, iLeft - 1);
        QuickSort(pArray, iRight + 1, iEnd);
    }
}

void my_sort(int *a,int n)
{
  int i,j;
  int t;
  for(i=0;i<n-1;++i)//因为每次需要和a[i]后面的数进行比较,所以到a[n-2](倒数第2个元素)就行
  {
      for(j=i+1;j<n;++j)//j从i后一个开始,a[i]与a[j]进行比较
      {
          if(a[i]>a[j])//a[i]为当前值,若是比后面的a[j]大,进行交换
          {
              t=a[i];
              a[i]=a[j];
              a[j]=t;
          }
      }//每排序一次,就会将a[i](包括a[i])之后的最小值放在a[i]的位置
  }
}