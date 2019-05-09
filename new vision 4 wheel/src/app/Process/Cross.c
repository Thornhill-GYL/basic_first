#include "Cross.h"
#include "Process.h"
#include "include.h"

Cross_Data     Cross={
  .Test_hang = Y_START
};

u8 Cross_process(void)
{
  Cross.State = NoCross;//清空状态
  if(Island.State!=NoIsland)
    return 1;
  In_Cross_test();//斜入十字检测
  In_Cross();//斜入十字
  Str_Cross();
  return 0;
}

u8 In_Cross_test()//斜入十字检测
{
  Cross_pre_test();
  Cross_curve_test();
  return 0;
}

u8 Cross_pre_test(void)
{
  u8 i;
  int L_Far_Lie[10],R_Far_Lie[10];
  int L_Diff_Far_Lie[9],R_Diff_Far_Lie[9];//一阶差分
  int L_DDiff_Far_Lie[8],R_DDiff_Far_Lie[8];//二阶差分
  int L_Liner_cnt = 0,R_Liner_cnt = 0;
  
  int L_Near_Hang[10],R_Near_Hang[10];
  int L_Diff_Near_Hang[10],R_Diff_Near_Hang[10];
  int L_DDiff_Near_Hang[10],R_DDiff_Near_Hang[10];
  int L_H_Liner_cnt = 0,R_H_Liner_cnt = 0;
  
  for(i=0;i<10;i++)
  {
    L_Far_Lie[i] = ROADY.Y[CCD_START  + i*4];
    R_Far_Lie[i] = ROADY.Y[CCD_END    - i*4];
    L_Near_Hang[i] = ROAD.Left[Y_START - 4 - i*3];
    R_Near_Hang[i] = ROAD.Right[Y_START - 4 - i*3];
  }
  for(i=0;i<9;i++)
  {
    L_Diff_Far_Lie[i] = L_Far_Lie[i+1] - L_Far_Lie[i];
    R_Diff_Far_Lie[i] = R_Far_Lie[i+1] - R_Far_Lie[i];
    L_Diff_Near_Hang[i] = L_Near_Hang[i+1] - L_Near_Hang[i];
    R_Diff_Near_Hang[i] = R_Near_Hang[i+1] - R_Near_Hang[i];
  }
  for(i=0;i<8;i++)
  {
    L_DDiff_Far_Lie[i] = L_Diff_Far_Lie[i+1] - L_Diff_Far_Lie[i];
    R_DDiff_Far_Lie[i] = R_Diff_Far_Lie[i+1] - R_Diff_Far_Lie[i];
    L_DDiff_Near_Hang[i] = L_Diff_Near_Hang[i+1] - L_Diff_Near_Hang[i];
    R_DDiff_Near_Hang[i] = R_Diff_Near_Hang[i+1] - R_Diff_Near_Hang[i];
    if(Abs_(L_DDiff_Far_Lie[i])<5
       &&L_Diff_Far_Lie[i]<0
         )//线性标志
      L_Liner_cnt++;
    if(Abs_(R_DDiff_Far_Lie[i])<5
       &&R_Diff_Far_Lie[i]<0
         )//线性标志
      R_Liner_cnt++;
    
    if(Abs_(L_DDiff_Near_Hang[i])<5
       &&L_Diff_Near_Hang[i]>0)
      L_H_Liner_cnt++;
    if(Abs_(R_DDiff_Near_Hang[i])<5
       &&R_Diff_Near_Hang[i]<0)
      R_H_Liner_cnt++;
  }
  if((L_Liner_cnt>4&&R_Liner_cnt>4)
     ||(L_H_Liner_cnt>4&&R_H_Liner_cnt>4))
  {
    return 1;
  }
  else if(L_Liner_cnt>4||L_H_Liner_cnt>4)
    Cross.State = R2Cross_Pre;
  else if(R_Liner_cnt>4||R_H_Liner_cnt>4)
    Cross.State = L2Cross_Pre;
  return 0;
}

u8 Cross_curve_test()
{
  int L_black[60],R_black[60];//左右边界
  int Diff_L[59],Diff_R[59];//一阶差分
  int DDiff_L[58],DDiff_R[58];//二阶差分
  int   Liner_L_cnt  = 0,Liner_R_cnt  = 0;
  u8    Liner_L_flag = 0,Liner_R_flag = 0;
  u8    Turn_L_Flag = 0,Turn_R_Flag = 0;//出现转折点
  u8    Turn_L_index= 0,Turn_R_index= 0;//出现转折点的位置
  u8  Impulse_L_Flag = 0,Impulse_R_Flag = 0;//一阶差分中出现阶跃
  int i,All_hang;
  All_hang = (Cross.Test_hang - Y_END - 10)/2;
  for(i=0;i<All_hang;i++)//10行
  {
    if(Cross.State==L2Cross_Pre)//左转找右边界
    {
      R_black[i] = ROAD.Right[Cross.Test_hang-i*2];
    }
    else if(Cross.State==R2Cross_Pre)//左转找右边界
    {
      L_black[i] = ROAD.Left[Cross.Test_hang-i*2];
    }
  }
  if(Cross.State==L2Cross_Pre)
  {
    for(i=0;i<All_hang-1;i++)
    {
      Diff_R[i] = R_black[i+1] - R_black[i];
      if(Diff_R[i]<-100
         &&(Cross.Test_hang-i*2)>=ROADY.far)
      {
        Impulse_R_Flag = 1;
      }
    }
    for(i=0;i<All_hang-5;i++)
    {
      if(Turn_R_Flag==0
         &&Diff_R[i]>0
           &&Diff_R[i+1]>0
             &&Diff_R[i+2]>0)//出现转折点
      {
        Turn_R_Flag = 1;
        Turn_R_index = i;//找到转折点的位置
        break;
      }
    }
    
  }
  else if(Cross.State==R2Cross_Pre)
  {
    for(i=0;i<All_hang-1;i++)
    {
      Diff_L[i] = L_black[i+1] - L_black[i];
      if(Diff_L[i]>100
         &&(Cross.Test_hang-i*2)>=ROADY.far)
      {
        Impulse_L_Flag = 1;
      }
    }
    for(i=0;i<All_hang-5;i++)
    {
      if(Turn_L_Flag==0
         &&Diff_L[i]<0
           &&Diff_L[i+1]<0
             &&Diff_L[i+2]<0)//出现转折点
      {
        Turn_L_Flag = 1;
        Turn_L_index = i;//找到转折点的位置
        break;
      }
    }
  }
  if(Cross.State==L2Cross_Pre)
  {
    for(i=0;i<All_hang-2;i++)
    {
      DDiff_R[i] = Diff_R[i+1] - Diff_R[i];
      if(Abs_(DDiff_R[i])<7)Liner_R_cnt++;
    }
    if(Liner_R_cnt>All_hang/2)Liner_R_flag = 1;
  }
  else if(Cross.State==R2Cross_Pre)
  {
    for(i=0;i<All_hang-2;i++)
    {
      DDiff_L[i] = Diff_L[i+1] - Diff_L[i];
      if(Abs_(DDiff_L[i])<7)Liner_L_cnt++;
    }
    if(Liner_L_cnt>All_hang/2)Liner_L_flag = 1;
  }
  if(Cross.State==L2Cross_Pre)
  {
    if(Liner_R_flag
       &&Turn_R_Flag
         &&Turn_R_index>5
           &&Turn_R_index<All_hang*4/5
             &&Impulse_R_Flag
               )
    {
      Cross.State = L2Cross_True;
      return 1;
    }
  }
  else if(Cross.State==R2Cross_Pre)
  {
    if(
       Liner_L_flag
         &&Turn_L_Flag
           &&Turn_L_index>5
             &&Turn_L_index<All_hang*4/5
               &&Impulse_L_Flag
                 )
    {
      Cross.State = R2Cross_True;
      return 1;
    }
  }
  return 0;
}

u8 In_Cross(void)//斜入十字
{
  int Start_End, End_End;
  int center_impulse;//存储突变点列数
  int center_use;
  
  //检测斜入十字标志
  if(Cross.State!=R2Cross_True&&Cross.State!=L2Cross_True)
    return 1;
  
  center_impulse = Cross_center_test(&Start_End,&End_End);
  if(Cross.State==R2Cross_True)//补线
  {
    center_use = (int)(((center_impulse - (float)((center_impulse - CCD_START)*(End_End - Main_Line))/(End_End - Start_End)) + CCD_END)/2 );
    Cross.Center = center_use;
  }
  else if(Cross.State==L2Cross_True)
  {
    center_use = (int)(((center_impulse - (float)((center_impulse - CCD_END)*(End_End - Main_Line))/(End_End - Start_End)) + CCD_START)/2 );
    Cross.Center = center_use;
  }
  CenterlineToDiff(Cross.Center);
  return 0;
}

u8 Cross_center_test(int* start_end, int* end_end)//和出环岛时找中心点的代码一样
{
  u8 Far_Lie[25];
  int Diff_Far_Lie[24];//一阶差分
  int DDiff_Far_Lie[23];//二阶差分
  int Liner_cnt  = 0;
  int out_center = 160;
  
  u8 i = 0;
  if(Cross.State==R2Cross_True)
  {
    for(i=0;i<25;i++)
    {
      Far_Lie[i] = ROADY.Y[CCD_START + i*4];
    }
    for(i=0;i<24;i++)
    {
      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
    }
    for(i=0;i<23;i++)
    {
      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
      if(Abs_(DDiff_Far_Lie[i])<5)//线性标志
        Liner_cnt++;
      if(DDiff_Far_Lie[i]<-15&&Liner_cnt>i/2&&Liner_cnt>0)//之前线性，出现冲激
      {
        break;
      }
    }
    out_center = CCD_START + 4*i;//记录突变点
  }
  else if(Cross.State==L2Cross_True)
  {
    for(i=0;i<25;i++)
    {
      Far_Lie[i] = ROADY.Y[CCD_END - i*4];
    }
    for(i=0;i<24;i++)
    {
      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
    }
    for(i=0;i<23;i++)
    {
      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
      if(Abs_(DDiff_Far_Lie[i])<5)//线性标志
        Liner_cnt++;
      if(DDiff_Far_Lie[i]<-15&&Liner_cnt>i/2&&Liner_cnt>0)//之前线性，出现冲激
      {
        break;
      }
    }
    out_center = CCD_END-4*i;//记录突变点
  }
  *start_end = Far_Lie[0];
  *end_end   = Far_Lie[i];
  if(out_center>290)
    out_center = 290;
  else if(out_center<30)
    out_center = 30;
  return out_center;
}

u8 Str_Cross(void)
{
  if(Cross.State==L2Cross_True||Cross.State==R2Cross_True)
    return 1;
  if(Str_Cross_Test()==1)//直入十字
  {
    Cross.State = Str2Cross;
    Cross.Str_Cross_test_hang = ROADY.far + 15;
    Cross.Center = get_Center(Cross.Str_Cross_test_hang);
    CenterlineToDiff(Cross.Center);
  }
  
  return 0;
}

u8 Str_Cross_Test(void)
{
  u8 i;
  u8 Start_line = 0;
  int Diff_temp;
  int Decrease_Cnt = 0;
  int Increase_Cnt = 0;
  u8 Quanbai_flag = 0;
  Start_line = max_s16(&ROADY.Y[120],80);
  for(i=0;i<20;i++)
  {
    Diff_temp = ROADY.Y[65+(i+1)*10] - ROADY.Y[65+i*10];
    if(Diff_temp>=0)
      Increase_Cnt++;
    if(Diff_temp<=0)
      Decrease_Cnt++;
  }
  if(!((Increase_Cnt>16)
         ||(Decrease_Cnt>16))
     &&(Start_line+2) < Y_START)
  {
    Quanbai_flag = Find_All_White(Start_line);
  }
  if(Quanbai_flag
     &&(ROADY.Three_lie_end[0]<140||ROADY.Three_lie_end[2]<140)
       )//近处没有尖角
  {
    return 1;
  }
  else 
    return 0;
}