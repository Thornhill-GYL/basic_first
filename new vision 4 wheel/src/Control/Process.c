#include "include.h"
#include "process.h"
#include "mymath.h"

ROAD_LINE ROAD;
ROAD_Y_STR ROADY = {.Middle_Correct = 0.0037,
                    };
int Main_Line;

void image_process(void)
{
  Dir.diff_done_flag = 0;
  
  for(int i = CCD_START;i<CCD_END;i++)
  {
    Get_column_end(i);//纵向边界
  }
  ROADY.Three_lie_end[0] = ROADY.Y[ROADY.Three_Lie[0]];
  ROADY.Three_lie_end[1] = ROADY.Y[ROADY.Three_Lie[1]];
  ROADY.Three_lie_end[2] = ROADY.Y[ROADY.Three_Lie[2]];
  Road_XY_Rebuild();
  if(break_info.meeting_flag==0)
    pattern_shift();
  get_Center(Main_Line);
  CenterlineToDiff(ROADY.Center);
 
}
//
void Get_column_end(int col)
{
  int line = Y_START;
  while (!((IMG_PIXEL(col, line))
           && (IMG_PIXEL(col, line - 1))
             && (IMG_PIXEL(col, line - 2)))
         && (line >= Y_END))
    line--;                                                           //探测顶端
  ROADY.Y[col] = line;
}

void Road_XY_Rebuild(void)
{
  ROADY.far = Y_START;
  int ccd_start = CCD_START,ccd_end = CCD_END;
//  if(Island.State==Left_Island_in)
//  {
//    ccd_end   = ROADY.Three_Lie[1] - 40;
//  }
//  else if(Island.State==Right_Island_in)
//  {
//    ccd_start = ROADY.Three_Lie[1] + 40;
//  }
  for(int i =ccd_start;i<ccd_end;i++)
  {
    if(ROADY.Y[i]<ROADY.far)
    {
      ROADY.far = ROADY.Y[i];
      ROADY.farL = i;
    }
    if(ROADY.Y[i]<=ROADY.far)
    {
      ROADY.far = ROADY.Y[i];
      ROADY.farR = i;
    }
  }
  ROADY.farC = (ROADY.farL+ROADY.farR)/2;
  for(int i = ROADY.far + 5;i <= Y_START-2;i++)//行行扫描
  {
    get_black_line(i);
  }

}
int get_Center(int hang)
{
  int count;
  int Middle;
  
  Middle = ROADY.farC;//中心赋值
  
  count = Middle;//把黑线中间值赋给计数起点
  if(count>CCD_END)
    count = CCD_END;
  else if(count < CCD_START)
    count = CCD_START;
  while(!(ROADY.Y[count+3]>hang
          &&ROADY.Y[count+2]>hang
            &&ROADY.Y[count+1]>hang)
        && count < CCD_END)
    count++;
  ROADY.Right = count;
  
  count = Middle;//把黑线中间值赋给计数起点
  if(count>CCD_END)
    count = CCD_END;
  else if(count < CCD_START)
    count = CCD_START;
  while(!(ROADY.Y[count-3]>hang
          &&ROADY.Y[count-2]>hang
            &&ROADY.Y[count-1]>hang)
        && count > CCD_START)
    count--;
  ROADY.Left = count;
  
  ROADY.Center = (ROADY.Right + ROADY.Left)/2;
  
//    if(Cross.State==NoCross)
//    {
  if(ROADY.Center>Center_Stand)//右侧补偿
  {
    if(ROADY.far<hang)
      ROADY.Center -= (int)((ROADY.far-hang)*(ROADY.far-hang)*ROADY.Middle_Correct);
    else 
      ROADY.Center += (int)((ROADY.far-hang)*(ROADY.far-hang)*ROADY.Middle_Correct);
    if(ROADY.Center<Center_Stand)
      ROADY.Center = Center_Stand;
  }
  else if(ROADY.Center<Center_Stand)//左侧补偿
  {
    if(ROADY.far<hang)
      ROADY.Center += (int)((ROADY.far-hang)*(ROADY.far-hang)*ROADY.Middle_Correct);
    else 
      ROADY.Center -= (int)((ROADY.far-hang)*(ROADY.far-hang)*ROADY.Middle_Correct);
    if(ROADY.Center>Center_Stand)
      ROADY.Center = Center_Stand;
  }
//    }
  return ROADY.Center;
  
}

void get_black_line(int hang)
{
  int Middle=160;  //黑线中间默认为CENNTER
  int count;
  int _black_R,_black_L;//黑线左右端
  
  Middle = ROADY.farC;//中心赋值
  count = Middle;//把黑线中间值赋给计数起点
  while(!((IMG_PIXEL(count+3,hang)) 
          && (IMG_PIXEL(count+2,hang))
            && (IMG_PIXEL(count+1,hang)))
        && count < CCD_END)//如果在有效区内没有找到连续三个黑点
    count++;//从中间位置开始，往右数，发现往右三点都是黑点停
  ROAD.Right[hang] = _black_R = count;
  
  if(_black_R<CCD_END)//如果在有效范围内
  {
    ROAD.Find[hang] |= RIGHT;
  }
  else
  {
    ROAD.Find[hang] &= ~RIGHT;
  }
  
  count = Middle;//把黑线中间值赋给计数起点
  while(!((IMG_PIXEL(count-3,hang)) 
          && (IMG_PIXEL(count-2,hang))
            && (IMG_PIXEL(count-1,hang)))
        && count > CCD_START)//如果在有效区内没有找到连续三个黑点
    count--;//从中间位置开始，往左数，发现往左三点都是黑点停
  ROAD.Left[hang] = _black_L = count;
  
  if(_black_L>CCD_START)//如果在有效范围内
  {
    ROAD.Find[hang] |= LEFT;
  }
  else
  {
    ROAD.Find[hang] &= ~LEFT;
  }
}
//
//u8 Cross_process(void)
//{
//  Cross.State = NoCross;//清空状态
//  if(Island.State!=NoIsland)
//    return 1;
//  In_Cross_test();//斜入十字检测
//  In_Cross();//斜入十字
//  Str_Cross();
//  if(Island.State == Str2Cross)
//  {
//    Beep_Once(&normal_beep);
//  }
//  
//  if(Cross.State != NoCross)
//  {
////    for (int i = 80;i<200;i++)
////    {
////      ROAD.Width[i] = ROAD.Width_stand[i];
////    }
//    arm_copy_q31((q31_t *)(&ROAD.Width_stand[80]),(q31_t *)(&ROAD.Width[80]),120);
//  }
//  return 0;
//}
//
//u8 In_Cross_test()//斜入十字检测
//{ 
//  if(Cross.State==R2Cross_True
//     ||Cross.State==L2Cross_True
//       ||Cross.State==Str2Cross)
//    return 1;
//  
//  Cross_pre_test();
//  Cross_curve_test();
//  return 0;
//}
//
//u8 Cross_pre_test(void)
//{
//  u8 i;
//  int L_Far_Lie[10],R_Far_Lie[10];
//  int L_Diff_Far_Lie[9],R_Diff_Far_Lie[9];//一阶差分
//  int L_DDiff_Far_Lie[8],R_DDiff_Far_Lie[8];//二阶差分
//  int L_Liner_cnt = 0,R_Liner_cnt = 0;
//  
//  int L_Near_Hang[10],R_Near_Hang[10];
//  int L_Diff_Near_Hang[10],R_Diff_Near_Hang[10];
//  int L_DDiff_Near_Hang[10],R_DDiff_Near_Hang[10];
//  int L_H_Liner_cnt = 0,R_H_Liner_cnt = 0;
//  //  if(LCD_DISPLAY_FLAG)
//  //    Draw_single_line('H',Y_START - 32 -IMAGE_CORRECT_FAR,COLOR_CYAN);
//  
//  for(i=0;i<10;i++)
//  {
//    L_Far_Lie[i] = ROADY.Y[CCD_START  + i*4];
//    R_Far_Lie[i] = ROADY.Y[CCD_END    - i*4];
//    L_Near_Hang[i] = ROAD.Left[Y_START - 4 - i*3];
//    R_Near_Hang[i] = ROAD.Right[Y_START - 4 - i*3];
//  }
//  for(i=0;i<9;i++)
//  {
//    L_Diff_Far_Lie[i] = L_Far_Lie[i+1] - L_Far_Lie[i];
//    R_Diff_Far_Lie[i] = R_Far_Lie[i+1] - R_Far_Lie[i];
//    L_Diff_Near_Hang[i] = L_Near_Hang[i+1] - L_Near_Hang[i];
//    R_Diff_Near_Hang[i] = R_Near_Hang[i+1] - R_Near_Hang[i];
//  }
//  for(i=0;i<8;i++)
//  {
//    L_DDiff_Far_Lie[i] = L_Diff_Far_Lie[i+1] - L_Diff_Far_Lie[i];
//    R_DDiff_Far_Lie[i] = R_Diff_Far_Lie[i+1] - R_Diff_Far_Lie[i];
//    L_DDiff_Near_Hang[i] = L_Diff_Near_Hang[i+1] - L_Diff_Near_Hang[i];
//    R_DDiff_Near_Hang[i] = R_Diff_Near_Hang[i+1] - R_Diff_Near_Hang[i];
//    if(Abs_(L_DDiff_Far_Lie[i])<5
//       &&L_Diff_Far_Lie[i]<0
//         //       &&L_Far_Lie[i]>=130
//         )//线性标志
//      L_Liner_cnt++;
//    if(Abs_(R_DDiff_Far_Lie[i])<5
//       &&R_Diff_Far_Lie[i]<0
//         //       &&R_Far_Lie[i]>=130
//         )//线性标志
//      R_Liner_cnt++;
//    
//    if(Abs_(L_DDiff_Near_Hang[i])<5
//       &&L_Diff_Near_Hang[i]>0)
//      L_H_Liner_cnt++;
//    if(Abs_(R_DDiff_Near_Hang[i])<5
//       &&R_Diff_Near_Hang[i]<0)
//      R_H_Liner_cnt++;
//  }
//  if((L_Liner_cnt>4&&R_Liner_cnt>4)
//     ||(L_H_Liner_cnt>4&&R_H_Liner_cnt>4))
//  {
//    return 1;
//  }
//  else if(L_Liner_cnt>4||L_H_Liner_cnt>4)
//    Cross.State = R2Cross_Pre;
//  else if(R_Liner_cnt>4||R_H_Liner_cnt>4)
//    Cross.State = L2Cross_Pre;
//  return 0;
//}
//
//u8 Cross_curve_test()
//{
//  int L_black[70],R_black[70];//左右边界
//  int Diff_L[69],Diff_R[69];//一阶差分
//  int DDiff_L[68],DDiff_R[68];//二阶差分
//  int   Liner_L_cnt  = 0,Liner_R_cnt  = 0;
//  u8    Liner_L_flag = 0,Liner_R_flag = 0;
//  u8    Turn_L_Flag = 0,Turn_R_Flag = 0;//出现转折点
//  u8    Turn_L_index= 0,Turn_R_index= 0;//出现转折点的位置
//  //  int   Turn_L_early_cnt = 0,Turn_R_early_cnt = 0;//出现转折点之前边沿捕获
//  //  int   Turn_L_late_cnt = 0 ,Turn_R_late_cnt = 0 ;//出现转折点之后边沿捕获
//  u8  Impulse_L_Flag = 0,Impulse_R_Flag = 0;//一阶差分中出现阶跃
//  int i,All_hang;
//  All_hang = (Cross.Test_hang - Y_END - 10)/2;
//  for(i=0;i<All_hang;i++)//10行
//  {
//    if(Cross.State==L2Cross_Pre)//左转找右边界
//    {
//      R_black[i] = ROAD.Right[Cross.Test_hang-i*2];
//    }
//    else if(Cross.State==R2Cross_Pre)//左转找右边界
//    {
//      L_black[i] = ROAD.Left[Cross.Test_hang-i*2];
//    }
//  }
//  if(Cross.State==L2Cross_Pre)
//  {
//    for(i=0;i<All_hang-1;i++)
//    {
//      Diff_R[i] = R_black[i+1] - R_black[i];
//      if(Diff_R[i]<-100
//         &&(Cross.Test_hang-i*2)>=ROADY.far)
//      {
//        Impulse_R_Flag = 1;
//      }
//    }
//    for(i=0;i<All_hang-5;i++)
//    {
//      //      if(Turn_R_Flag==0&&Diff_R[i]<=0)//未出现转折点
//      //        Turn_R_early_cnt++;
//      //      else 
//      if(Turn_R_Flag==0
//         //              &&i<50            //防止内存溢出
//         &&Diff_R[i]>0
//           &&Diff_R[i+1]>0
//             &&Diff_R[i+2]>0)//出现转折点
//      {
//        Turn_R_Flag = 1;
//        //        Turn_R_late_cnt++;
//        Turn_R_index = i;//找到转折点的位置
//        break;
//      }
//      //      else if(Turn_R_Flag==1&&Diff_R[i]>0)
//      //      {
//      //        Turn_R_late_cnt++;
//      //      }
//    }
//    
//  }
//  else if(Cross.State==R2Cross_Pre)
//  {
//    for(i=0;i<All_hang-1;i++)
//    {
//      Diff_L[i] = L_black[i+1] - L_black[i];
//      if(Diff_L[i]>100
//         &&(Cross.Test_hang-i*2)>=ROADY.far)
//      {
//        Impulse_L_Flag = 1;
//      }
//    }
//    for(i=0;i<All_hang-5;i++)
//    {
//      //      if(Turn_L_Flag==0&&Diff_L[i]>=0)//未出现转折点
//      //        Turn_L_early_cnt++;
//      //      else 
//      if(Turn_L_Flag==0
//         //              &&i<50            //防止内存溢出
//         &&Diff_L[i]<0
//           &&Diff_L[i+1]<0
//             &&Diff_L[i+2]<0)//出现转折点
//      {
//        Turn_L_Flag = 1;
//        //        Turn_L_late_cnt++;
//        Turn_L_index = i;//找到转折点的位置
//        break;
//      }
//      //      else if(Turn_L_Flag==1&&Diff_L[i]<0)
//      //      {
//      //        Turn_L_late_cnt++;
//      //      }
//    }
//  }
//  if(Cross.State==L2Cross_Pre)
//  {
//    for(i=0;i<All_hang-2;i++)
//    {
//      DDiff_R[i] = Diff_R[i+1] - Diff_R[i];
//      if(Abs_(DDiff_R[i])<7)Liner_R_cnt++;
//    }
//    if(Liner_R_cnt>All_hang/2)Liner_R_flag = 1;
//  }
//  else if(Cross.State==R2Cross_Pre)
//  {
//    for(i=0;i<All_hang-2;i++)
//    {
//      DDiff_L[i] = Diff_L[i+1] - Diff_L[i];
//      if(Abs_(DDiff_L[i])<7)Liner_L_cnt++;
//    }
//    if(Liner_L_cnt>All_hang/2)Liner_L_flag = 1;
//  }
//  if(Cross.State==L2Cross_Pre)
//  {
//    if(Liner_R_flag
//       &&Turn_R_Flag
//         &&Turn_R_index>5
//           //       &&Turn_R_early_cnt>Turn_R_index/2
//           &&Turn_R_index<All_hang*4/5
//             &&Impulse_R_Flag
//               //       &&Turn_R_late_cnt>(Impulse_R_index-Turn_R_index)/2
//               )
//    {
//      //      Beep_Once(&Test_100ms);
//      //      if(Beep_Debug_Flag)
//      //        Buz_flag = 10;
//      Cross.State = L2Cross_True;
//      return 1;
//    }
//  }
//  else if(Cross.State==R2Cross_Pre)
//  {
//    if(
//       Liner_L_flag
//         &&Turn_L_Flag
//           &&Turn_L_index>5
//             //       &&Turn_L_early_cnt>Turn_L_index/2
//             &&Turn_L_index<All_hang*4/5
//               &&Impulse_L_Flag
//                 //       &&Turn_L_late_cnt>(Impulse_L_index-Turn_L_index)/2
//                 )
//    {
//      //      Beep_Once(&Test_100ms);
//      //      if(Beep_Debug_Flag)
//      //        Buz_flag = 10;
//      Cross.State = R2Cross_True;
//      return 1;
//    }
//  }
//  return 0;
//}
//
//u8 In_Cross(void)//斜入十字
//{
//  int Start_End, End_End;
//  int center_impulse;//存储突变点列数
//  int center_use;
//  static u8  center_Period = 0;
//  static u16 Center_[Cross_Center_Period_Const];
//  
//  //检测斜入十字标志
//  if(Cross.State!=R2Cross_True&&Cross.State!=L2Cross_True)
//    return 1;
//  
//  center_impulse = Cross_center_test(&Start_End,&End_End);
//  if(Cross.State==R2Cross_True)//补线
//  {
//    Cross.Cross_delay_flag = 1;
//    Cross.Cross_delay_cnt = Cross.Cross_delay_cnt_const;
//    center_use = (int)(((center_impulse - (float)((center_impulse - CCD_START)*(End_End - Main_Line))/(End_End - Start_End)) + CCD_END)/2 );
//    if(center_use>ROADY.Three_Lie[1]+10)//有效性检验
//    {
//      if(center_use > CCD_END)
//        center_use = CCD_END;
//      Cross.Center = center_use;
//      if(center_Period > (Cross_Center_Period_Const - 1))
//        center_Period = 0;
//      
//      Cross.In_center  -= Center_[center_Period];
//      Center_[center_Period] = center_use;
//      Cross.In_center  += Center_[center_Period];
//      center_Period++;  
//    }
//    else//补线失败，使用之前保存的中心点
//    {
//      Cross.Center = Cross.In_center/Cross_Center_Period_Const;
//    }
//  }
//  else if(Cross.State==L2Cross_True)
//  {
//    Cross.Cross_delay_flag = 1;
//    Cross.Cross_delay_cnt = Cross.Cross_delay_cnt_const;
//    center_use = (int)(((center_impulse - (float)((center_impulse - CCD_END)*(End_End - Main_Line))/(End_End - Start_End)) + CCD_START)/2 );
//    if(center_use<ROADY.Three_Lie[1]-10)//有效性检验
//    {
//      if(center_use < CCD_START)
//        center_use = CCD_START;
//      Cross.Center = center_use;
//      if(center_Period > (Cross_Center_Period_Const - 1))
//        center_Period = 0;
//      
//      Cross.In_center  -= Center_[center_Period];
//      Center_[center_Period] = center_use;
//      Cross.In_center  += Center_[center_Period];
//      center_Period++;  
//    }
//    else//补线失败，使用之前保存的中心点
//    {
//      Cross.Center = Cross.In_center/Cross_Center_Period_Const;
//    }
//  }
//  //  if(LCD_DISPLAY_FLAG)
//  //  {
//  //    LCD_DrawBigPoint(Cross.Center,Main_Line - IMAGE_CORRECT_FAR,COLOR_GREEN);
//  //  }
//  
//  CenterlineToDiff(Cross.Center);
//  return 0;
//}
//
//u8 Cross_center_test(int* start_end, int* end_end)//和出环岛时找中心点的代码一样
//{
//  u8 Far_Lie[25];
//  int Diff_Far_Lie[24];//一阶差分
//  int DDiff_Far_Lie[23];//二阶差分
//  int Liner_cnt  = 0;
//  int out_center = 160;
//  
//  u8 i = 0;
//  if(Cross.State==R2Cross_True)
//  {
//    for(i=0;i<25;i++)
//    {
//      Far_Lie[i] = ROADY.Y[CCD_START + i*4];
//    }
//    for(i=0;i<24;i++)
//    {
//      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
//    }
//    for(i=0;i<23;i++)
//    {
//      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
//      if(Abs_(DDiff_Far_Lie[i])<5)//线性标志
//        Liner_cnt++;
//      if(DDiff_Far_Lie[i]<-15&&Liner_cnt>i/2&&Liner_cnt>0)//之前线性，出现冲激
//      {
//        break;
//      }
//    }
//    out_center = CCD_START + 4*i;//记录突变点
//  }
//  else if(Cross.State==L2Cross_True)
//  {
//    for(i=0;i<25;i++)
//    {
//      Far_Lie[i] = ROADY.Y[CCD_END - i*4];
//    }
//    for(i=0;i<24;i++)
//    {
//      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
//    }
//    for(i=0;i<23;i++)
//    {
//      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
//      if(Abs_(DDiff_Far_Lie[i])<5)//线性标志
//        Liner_cnt++;
//      if(DDiff_Far_Lie[i]<-15&&Liner_cnt>i/2&&Liner_cnt>0)//之前线性，出现冲激
//      {
//        break;
//      }
//    }
//    out_center = CCD_END-4*i;//记录突变点
//  }
//  *start_end = Far_Lie[0];
//  *end_end   = Far_Lie[i];
//  if(out_center>290)
//    out_center = 290;
//  else if(out_center<30)
//    out_center = 30;
//  return out_center;
//}
//
//u8 Str_Cross(void)
//{
//  
//  if(Cross.State==L2Cross_True||Cross.State==R2Cross_True)
//    return 1;
//  if(Str_Cross_Test()==1)//直入十字
//  {
//    //    if(Beep_Debug_Flag)
//    //      Buz_flag = 10;
//    Cross.State = Str2Cross;
//    Cross.Str_Cross_test_hang = ROADY.far + 15;
////    get_Center(Cross.Str_Cross_test_hang);
//    Cross.Center = get_Center(Cross.Str_Cross_test_hang);
//    CenterlineToDiff(Cross.Center);
//  }
//  
//  return 0;
//}
//
//u8 Str_Cross_Test(void)
//{
//  u8 i,cnt=0;
//  u8 Start_line = 0;
//  int Diff_temp;
//  int Decrease_Cnt = 0;
//  int Increase_Cnt = 0;
//  int Quanbai_Left,Quanbai_Right;
//  u8 Quanbai_flag = 0;
//  Start_line = max_s16(&ROADY.Y[120],80);
//  //  Start_line = PIX_LINE(min_Y(&RoadY[100],120));
//  for(i=0;i<20;i++)
//  {
//    Diff_temp = ROADY.Y[65+(i+1)*10] - ROADY.Y[65+i*10];
//    if(Diff_temp>=0)
//      Increase_Cnt++;
//    if(Diff_temp<=0)
//      Decrease_Cnt++;
//  }
////  for(i=0;i<20;i++)
////  {
////    if(!((Increase_Cnt>16)
////         ||(Decrease_Cnt>16))
////       &&Start_line+(i+1)*2 < Y_START
//////         &&sum_point(Image_fire[Start_line+(i+1)*2],40)<=5
////           )//全白
////      
////  }
//  if(!((Increase_Cnt>16)
//         ||(Decrease_Cnt>16))
//     &&Start_line+(i+1)*2 < Y_START)
//  {
//    if(All_White_Line(Start_line,&Quanbai_Left,&Quanbai_Right)==1)
//    {
//      if(LCD_DISPLAY_FLAG)
//      {
//        LCD_DrawBigPoint(CCD_START+3,Quanbai_Left  - IMAGE_CORRECT,COLOR_BLUE);
//        LCD_DrawBigPoint(CCD_END-  3,Quanbai_Right - IMAGE_CORRECT,COLOR_BLUE);
//      }
//      if(Find_Quanbai_Xie(Quanbai_Left,Quanbai_Right)==1)
//      {
//        Quanbai_flag = 1;
//      }
//      else
//        Quanbai_flag = 0;
//    }
//    else
//    {
//      Quanbai_flag = 0;
//    }
//  }
//  if(Quanbai_flag
//     &&(ROADY.Three_lie_end[0]<140||ROADY.Three_lie_end[2]<140)
//       )//近处没有尖角
//  {
//    return 1;
//  }
//  else 
//    return 0;
//}
//
//
//int sum_new=0;
//int basic_flag=0;//此标志设为全局变量
//int judge_straight_new(int using_line)
//{
//  int middle_stable=160;
//  int middle_test_with=15;
// // int hang_move=0;
//  
//  //hang_move=breakage_line+5;
//  //右边小范围判断
//  sum_new=0;
//  for(int i=middle_stable;i<middle_test_with+middle_stable;i++)
//  {
//    if(IMG_PIXEL(i,using_line)>=1)
//      break;
//    else if(IMG_PIXEL(i,using_line)==0)
//    {
//      sum_new+=1;
//    }
//  }
//  //左边小范围判断
//  for(int i=middle_stable;i>middle_stable-middle_test_with;i--)
//  {
//    if(IMG_PIXEL(i,using_line)>=1)
//      break;
//    else if(IMG_PIXEL(i,using_line)==0)
//    {
//      sum_new++;
//    }
//  }
//  if(sum_new>600)
//  {
//    sum_new=600;
//  }
//  if(sum_new==30)
//  {
//    basic_flag=1;
//  }
//  return basic_flag;
//}
//int limit_flag=0;
//int right_gets=0;
//int left_gets=0;
//int sum_left=0;
//int sum_right=0;
//
//int Save[10] = {0};
//
//int limit_judge(int using_line)
//{
// // int hang_move=0;
//  int left_side=0,left_flag=0;
//  int right_side=0,right_flag=0;
//  int down_line=20;
//  // int sum_left=0,sum_right=0;
//  
//  int left_straight;
//  int right_straight;
//  //右边直线检测
//  //hang_move=breakage_line+10;
//  right_side=160;
//  
//  Save[0] = using_line;
//  
//  while(!((IMG_PIXEL(right_side+3,using_line)) 
//          && (IMG_PIXEL(right_side+2,using_line))
//            && (IMG_PIXEL(right_side+1,using_line)))
//        && right_side < CCD_END)
//  {
//    right_side++;
//    Save[1] = right_side;
//  }
//  if(right_side<CCD_END)
//  {
//    
//    right_gets=right_side;
//    right_flag=1;
//  }
//  else
//    right_flag=0;
//  
//  sum_right=0;
//  if(right_flag==1)
//  {
//    for(int i=using_line;i<using_line+down_line;i++)
//    {
//      if(IMG_PIXEL(right_gets,i)==0)
//      {
//        sum_right++;
//      }
//      else
//        break;
//    }
//  }
//  else 
//    sum_right=0;
//  Save[2] = sum_right;
//  if(sum_right>600)
//  {
//    sum_right=600;
//  }
//  if(sum_right==20)
//    right_straight=1;
//  else 
//    right_straight=0;
//  //左边直道检测
//  
//  
//  left_side=160;
//  while(!((IMG_PIXEL(left_side-3,using_line)) 
//          && (IMG_PIXEL(left_side-2,using_line))
//            && (IMG_PIXEL(left_side-1,using_line)))
//        && left_side > CCD_START)
//  {
//    left_side--;
//    Save[3] = left_side;
//  }
//  if(left_side>CCD_START)
//  {
//    
//    left_gets=left_side;
//    left_flag=1;
//  }
//  else
//    left_flag=0;
//  sum_left=0;
//  if(left_flag==1)
//  {
//    for(int i=using_line;i<using_line+down_line;i++)
//    {
//      if(IMG_PIXEL(left_gets,i)==0)
//      {
//        sum_left++;
//      }
//      else
//        break;
//    }
//  }
//  else 
//    sum_left=0;
//  Save[4] = sum_left;
//  if(sum_left>500)
//  {
//    sum_left=500;
//  }
//  if(sum_left==20)
//    left_straight=1;
//  else 
//    left_straight=0;
//  
//  if(right_straight==1&&left_straight==1)
//    limit_flag=1;//全局变量
//  else
//    limit_flag=0;
//  
//  return limit_flag;
//}
//int straight_all_flag=0;
//
//int straight_all_judge(int using_line)
//{
//  int com_str_flag=0;
//  int limit_str_flag=0;
//  com_str_flag=judge_straight_new(using_line);
//  limit_str_flag=limit_judge(using_line);
//  if(com_str_flag==1&&limit_str_flag==1)
//    straight_all_flag=1;
//  else
//    straight_all_flag=0;
//  return straight_all_flag;
//}
//int black_or_white_right[150];
//int black_or_white_left[150];
//void show_the_line(void)
//{
//  int j=0;
//  j=0;
//  for(int i=160;i<CCD_END;i++)
//  {
//    if(j<149)
//    {
//      j++;
//      if(IMG_PIXEL(i,Main_Line)==0)
//      {
//        black_or_white_right[j]=0;
//        
//      }
//      else
//        black_or_white_right[j]=1;
//    }
//  }
//  
//}
//
//
//int breakage_new(int using_line)
//{
//  int middle_stable=160;
//  int middle_test_with=20;
//  int sum_black=0;
//  //breakage_line=Main_Line-10;
//  int breakage_temp_flag=0;
//  //右边小范围判断
//  sum_black=0;
//  for(int i=middle_stable;i<middle_test_with+middle_stable;i++)
//  {
//    if(IMG_PIXEL(i,using_line)==0)
//      break;
//    else if(IMG_PIXEL(i,using_line)>=1)
//    {
//      sum_black+=1;
//    }
//  }
//  //左边小范围判断
//  for(int i=middle_stable;i>middle_stable-middle_test_with;i--)
//  {
//    if(IMG_PIXEL(i,using_line)==0)
//      break;
//    else if(IMG_PIXEL(i,using_line)>=1)
//    {
//      sum_black++;
//    }
//  }
//  if(sum_black>500)
//  {
//    sum_black=500;
//  }
//  if(sum_black==40)
//  {
//    breakage_temp_flag=1;
//  }
//  return breakage_temp_flag;
//}
//int breakage_flag=0;
//int breakage_done_flag=0;
//int change_done_flag=0;
//int straight_com_flag=0;
//int back_flag=0;
//void change_pattern(void)
//{
////  Save_SP();
////  return;
//  //show_the_line();
//  breakage_flag=breakage_new(breakage_line);
//  straight_com_flag=straight_all_judge(hang_move);
//  back_flag=straight_all_judge(breakage_line-2);
//  if((straight_com_flag==1)&&(breakage_flag==1)&&(check_pattern==1))
//  {
//    breakage_done_flag=1;
//    
//  }
//  
//  if(breakage_done_flag==1)
//  {
//    Dir_mode=Elec_Mode;
//   
//  }
//  else
//  {
//    Dir_mode = Camera_Mode;
//  }
//  change_done_flag=0;
//  
//}
//int black_block=0;
//int white_block=0;
//int sure_block=0;
//void block_detect(void)
//{
//  black_block=breakage_new(block_line);
//  white_block=straight_all_judge(block_white);
//  if(white_block==1&&black_block==1&&check_pattern==1)
//  {
//    sure_block=1;
//  }
// else
//    sure_block=0;
//  
//}
//
//u8 Island_process(void)
//{ 
//  Save_SP();
//  Elec_Island();//电磁检测，只在入环和出环的时候检测
//  In_Island();//入环岛
//  Stay_Island();//在环岛里
//  Out_Island();//出环岛
//  Wait_Next_Island();//防止再次进入环岛
//  
//  return 0;
//}
//int elec_detcet=0;
//u8 In_double_AD(void)
//{
//  if(((Dir.Elec_Left>3400)&&(Dir.Elec_Right>1600))
//     ||((Dir.Elec_Left>1600)&&(Dir.Elec_Right>3400))
//     ||((Dir.Elec_Left>3900)&&(Dir.Elec_Right>1200))
//     ||((Dir.Elec_Left>1200)&&(Dir.Elec_Right>3900))
//       )
//  {
//    return 1;
//  }
//  else 
//  {
//    return 0;
//  }
//}
//
//u8 Out_double_AD(void)
//{
//  //  if((Dir.Elec_Left>3900)||(Dir.Elec_Right>3900))
//  if(((Dir.Elec_Left>2000)&&(Dir.Elec_Right>1500))
//     ||((Dir.Elec_Left>1500)&&(Dir.Elec_Right>2000))
//       //     ||(Dir.Elec_Left+Dir.Elec_Right>6300)
//       )
//  {
//    return 1;
//  }
//  else 
//  {
//    return 0;
//  }
//}
//int go_in_island=0;
//u8 Elec_Island(void)
//{
//  u8 doublt_island = 0;
//  
//  if(Island.State == NoIsland)//无环岛时检测环岛
//  {
//    doublt_island = Image_Island_Test();
//    if(doublt_island!=0)
//    {
////      Beep_Once(&normal_beep);
//    }
//    
//    
//    if(doublt_island!=0&&In_double_AD()==1)
//    {
//      LED_CYAN;
//      Island.State = doublt_island;
//      if(Island.State==Left_Island_pre)
//      {
//        Island.In_Center = ROADY.Three_Lie[1] - 25;
//      }
//      else if(Island.State==Right_Island_pre)
//      {
//        Island.In_Center = ROADY.Three_Lie[1] + 25;
//      }
//    }
//  }
//  
//  if(Island.State == Left_Island_out)
//  {
//    
//    if(Island.doubleFAR_Allow_flag
//       &&ROADY.farC<ROADY.Three_Lie[1]-50)
//    {
//      
//      //      Beep_Once(&Test_100ms);
//      Island.State = Left_Wait_Next;//等待下一个环岛的时间间隔
//      
//    }
//  }
//  else if(Island.State == Right_Island_out)
//  {
//    
//    if(Island.doubleFAR_Allow_flag
//       &&ROADY.farC>ROADY.Three_Lie[1]+50)
//    {
//      
//      //      Beep_Once(&Test_100ms);
//      Island.State = Right_Wait_Next;//等待下一个环岛的时间间隔
//    }
//  }
//  return 0;
//}
//
//u8 In_Island(void)
//{
//  int center;
//  static int center_use;
//  u8  Impulse_hang = 0;
//  if(Island.State!=Left_Island_pre&&Island.State!=Right_Island_pre)//不在此状态下
//    return 1;//直接返回
//  else 
//  {
//    center = In_Island_center(&Impulse_hang);//寻找中心点
//    In2Stay_Island(center,Impulse_hang);
//    if(center == -1)//寻找失败或者已完全进入环岛
//    {
//      CenterlineToDiff(Island.In_Center);//使用上一次的旧值
//    }
//    else
//    {
//      if(Island.State==Left_Island_pre)
//      {
//        center_use = (int)(((center - (center - CCD_END)*(Impulse_hang - Island.Stay_hang_use)*1.0/(Impulse_hang - Island.Correct_hang)) + CCD_START)/2);//补线（三角形相似）
//        if(center_use<CCD_START)
//          center_use = CCD_START;
//        Island.In_Center = center_use;//保存上一次的中心点
//        CenterlineToDiff(center_use);
//      }
//      else if(Island.State==Right_Island_pre)
//      {
//        center_use = (int)(((center - (center - CCD_START)*(Impulse_hang - Island.Stay_hang_use)*1.0/(Impulse_hang - Island.Correct_hang)) + CCD_END)/2);//补线（三角形相似）
//        if(center_use>CCD_END)
//          center_use = CCD_END;
//        
//        Island.In_Center = center_use;//保存上一次的中心点
//        CenterlineToDiff(center_use);
//      }
//      //调试用，显示中心点
//      if(LCD_DISPLAY_FLAG==1)
//      {
//        //        LCD_DrawBigPoint(center_use,Main_Line,Blue);//行列颠倒
//      }
//    }
//  }
//  return 0;
//}
//
//int In_Island_center(u8* hang)//入环岛时寻找突变点+补线
//{
//  int Middle;
//  int center = 160;
//  int ccd_start=10,ccd_end=310;  //ccd扫描起点10，终点310   
//  int Left_Count=0,Right_Count=0;//左右计数为0
//  static int Diff_L[19],Diff_R[19];//一阶差分
//  static u16 In_black_L[20];
//  static u16 In_black_R[20];
//  u8  Impulse_L_Flag = 0,Impulse_R_Flag = 0;//一阶差分中出现阶跃
//  u8 i = 0;
//  
//  Middle = ROADY.farC;
//  if(Island.State == Right_Island_pre)
//  {
//    if(Middle>ROADY.Three_Lie[1]+40)return -1;//前方直道已经看不到了
//  }
//  else if(Island.State == Left_Island_pre)
//  {
//    if(Middle<ROADY.Three_Lie[1]-40)return -1;//前方直道已经看不到了
//  }
//  
//  for(i=0;i<20;i++)//20行
//  {
//    if(Island.State == Right_Island_pre)
//    {
//      Right_Count = ROAD.Right[i*3+Island.Image_Start_hang];
//      if(Right_Count<ccd_end)//如果在有效范围内
//      {
//        In_black_R[i] = Right_Count;
//      }
//      else if(Right_Count<ROADY.Three_Lie[1]+10)
//      {
//        In_black_R[i] = ccd_end;
//      }
//      else
//      {
//        In_black_R[i] = ccd_end;
//      }
//    }
//    else if(Island.State == Left_Island_pre)
//    {
//      //      Left_Count = Middle;
//      //      while(!(ImageData[Left_Count-3]==1 
//      //              && ImageData[Left_Count-2]==1
//      //                && ImageData[Left_Count-1]==1)
//      //            && Left_Count > ccd_start)	  
//      //        Left_Count--;
//      Left_Count = ROAD.Left[i*3+Island.Image_Start_hang];
//      if(Left_Count > ccd_start)
//      {
//        In_black_L[i] = Left_Count; 
//      }
//      else if(Left_Count>ROADY.Three_Lie[1]-10)
//      {
//        In_black_L[i] = ccd_start;
//      }
//      else
//      {
//        In_black_L[i] = ccd_start;
//      }
//    }
//  }
//  
//  if(Island.State == Right_Island_pre)
//  {
//    for(i=0;i<19;i++)
//    {
//      Diff_R[i] = In_black_R[i+1] - In_black_R[i];
//      if(Diff_R[i]>30)
//      {
//        Impulse_R_Flag = 1;
//        center = In_black_R[i];//出现跳转的行
//        *hang   = i*3+Island.Image_Start_hang;
//        break;
//      }
//    }
//  }
//  else if(Island.State == Left_Island_pre)
//  {
//    for(i=0;i<19;i++)
//    {
//      Diff_L[i] = In_black_L[i+1] - In_black_L[i];
//      if(Diff_L[i]<-30)
//      {
//        Impulse_L_Flag = 1;//出现冲激
//        center = In_black_L[i];//出现跳转的行
//        *hang   = i*3+Island.Image_Start_hang;
//        break;
//      }
//    }
//  }
//  if(Impulse_R_Flag==0&&Impulse_L_Flag==0)
//  {
//    return -1;//没有出现
//  }
//  else 
//  {
//    return center;
//  }
//}
//
//u8 In2Stay_Island(int center,int hang)
//{
//  u8 i;
//  u8  Curve_cnt = 0;
//  if(Island.State!=Left_Island_pre&&Island.State!=Right_Island_pre)
//    return 1;
//  
//  if(Island.State==Left_Island_pre)
//  {
//    for(i=0;i<29;i++)
//    {
//      //      if(Image_lie.Far_30[i+1]-Image_lie.Far_30[i]>0)
//      if(ROADY.Y[40+(i+1)*8]-ROADY.Y[40+i*8]>0)
//        Curve_cnt ++;
//    }
//  }
//  else if(Island.State==Right_Island_pre)
//  {
//    for(i=0;i<29;i++)
//    {
//      if(ROADY.Y[40+(i+1)*8]-ROADY.Y[40+i*8]<0)
//        Curve_cnt ++;
//    }
//  }
//  
//  if(Island.State==Left_Island_pre)
//  {
//    if(
//       ROADY.farC<ROADY.Three_Lie[1]-60
//         &&((hang>(Island.Stay_hang_use+5)&&center>ROADY.Three_Lie[1]+60)
//            ||Curve_cnt>22))
//    {
//      Island.In2Stay_cnt++;
//    }
//  }
//  else if(Island.State==Right_Island_pre)
//  {
//    if(
//       ROADY.farC>ROADY.Three_Lie[1]+60
//         &&((hang>(Island.Stay_hang_use+5)&&center>ROADY.Three_Lie[1]-60)
//            ||Curve_cnt>22))
//    {
//      Island.In2Stay_cnt++;
//    }
//  }
//  if(Island.In2Stay_cnt>1)
//  {
////    Beep_Once(&Test_100ms);
//    if(Island.State==Left_Island_pre)
//    {
//      Island.State = Left_Island_in;
//      Island.Stay_Center = 50;
//      Island.Stay2Out_flag = 1;
//      Island.Stay2Out_flag_delay = Island.Stay2Out_flag_delay_const;
//    }
//    else if(Island.State==Right_Island_pre)
//    {
//      Island.State = Right_Island_in;
//      Island.Stay_Center = 269;
//      Island.Stay2Out_flag = 1;
//      Island.Stay2Out_flag_delay = Island.Stay2Out_flag_delay_const;
//    }
//  }
//  return 0;
//}
//
//u8 Stay_Island(void)
//{
//  if(Island.State!=Left_Island_in&&Island.State!=Right_Island_in)
//    return 1;
//  
//  //当作普通弯道寻找中线
//  Island.Stay_Center = get_Center(Island.Stay_hang_use);//ROAD.Middle[Island.Stay_hang_use];
//  CenterlineToDiff(Island.Stay_Center);
//  
////  Island.Out_Center_ += Island.Stay_Center;
////  Island.Stay_Island_center_cnt++;
//  Island.Out_Center_[Island.Stay_Island_center_cnt] = Island.Stay_Center;
//  Island.Stay_Island_center_cnt++;
//  Island.Stay_Island_center_cnt_all++;
//  if(Island.Stay_Island_center_cnt >= 100)
//  {
//    Island.Stay_Island_center_cnt = 0;
//  }
//  
//  if(Island.Out_Allow_flag==1)//允许改变状态
//  {
//    if(Stay2Out_test()==1)
//    {
//      Island.Stay2Out_cnt ++;
//    }
//    else
//    {
//      Island.Stay2Out_cnt = 0;
//    }
//    if(Island.Stay2Out_cnt > 1)//连续两次测到突变点状态改变
//    {
////      Beep_Once(&normal_beep);
//      if(Island.Stay_Island_center_cnt_all>=100)
//      {
//        my_sort(Island.Out_Center_,100);
//        Island.Out_Center = (Island.Out_Center_[49]+Island.Out_Center_[50])/2;
//      }
//      else
//      {
//        my_sort(Island.Out_Center_,Island.Stay_Island_center_cnt_all);
//        if(Island.Stay_Island_center_cnt_all%2)//奇数
//        {
//          Island.Out_Center = Island.Out_Center_[Island.Stay_Island_center_cnt_all/2];
//        }
//        else
//        {
//          Island.Out_Center = (Island.Out_Center_[Island.Stay_Island_center_cnt_all/2-1]+Island.Out_Center_[Island.Stay_Island_center_cnt_all/2])/2;
//        }
//      }
////      Island.Out_Center *= 0.6f;
//      
//      if(Island.State==Left_Island_in)
//      {
//        Island.Out_Center += (Center_Stand - Island.Out_Center)*0.2;
////        Island.Out_Center = (Island.Out_Center_ / Island.Stay_Island_center_cnt) * 0.9f;
//        Island.State = Left_Island_out;
//        Island.Out2doubleFAR_flag = 1;
//        Island.Out2doubleFAR_flag_delay = Island.Out2doubleFAR_flag_delay_const;
//        
//        Island.Next_Island_flag = 1;
//        Island.Next_Island_flag_delay = Island.Next_Island_flag_delay_const;
//        
//      }
//      else if(Island.State==Right_Island_in)
//      {
//        Island.Out_Center += (Center_Stand - Island.Out_Center)*0.2;
////        Island.Out_Center = (Island.Out_Center_ / Island.Stay_Island_center_cnt) * 0.9f;
//        Island.State = Right_Island_out;
//        Island.Out2doubleFAR_flag = 1;
//        Island.Out2doubleFAR_flag_delay = Island.Out2doubleFAR_flag_delay_const;
//        
//        Island.Next_Island_flag = 1;
//        Island.Next_Island_flag_delay = Island.Next_Island_flag_delay_const;
//      }
//    }
//  }
//  
//  return 0;
//}
//
//u8 Out_Island(void)
//{
//  int center_use;
//  
//  //检测出环岛标志
//  if(Island.State!=Right_Island_out&&Island.State!=Left_Island_out)
//    return 1;
//  
//  if(Island.State==Right_Island_out)//补线
//  {
//    if(Out_Island_find_Quanbai()==1)
//    {
//      center_use = Island.Out_Center ;
//    }
//    else//补线失败，使用之前保存的中心点
//    {
//      center_use = Island.Out_Center;
//    }
//    CenterlineToDiff(center_use);
//  }
//  else if(Island.State==Left_Island_out)
//  {
//    if(Out_Island_find_Quanbai()==1)
//    {
//      //      Beep_Once(&Image_Island_Test_Beep);
//      center_use = Island.Out_Center ;
//    }
//    else//补线失败，使用之前保存的中心点
//    {
//      center_use = Island.Out_Center;
//    }
//    CenterlineToDiff(center_use);
//  }
//  return 0;
//}
//
//u8 Out_Island_find_Quanbai(void)
//{
//  u8 i,cnt=0;
//  u8 Start_line = 0;
//  Start_line = max_s16(&ROADY.Y[120],80);
//  for(i=0;i<20;i++)
//  {
//    if(Start_line+(i+1)*2 < Y_START
//       &&sum_point(Image_fire[Start_line+(i+1)*2],40)<=5)
//      cnt++;
//  }
//  if(cnt>5&&(ROADY.Three_lie_end[0]<140||ROADY.Three_lie_end[2]<140))//近处没有尖角
//  {
//    return 1;
//  }
//  else 
//    return 0;
//}
//
//u8 Image_Island_Test(void)//捕捉黑线  
//{
//  int ccd_start=CCD_START,ccd_end=CCD_END;  //ccd扫描起点10，终点310   
//  int Left_Count=0,Right_Count=0;//左右计数为0
//  static int Diff_L[39],Diff_R[39];//一阶差分
//  static int DDiff_L[38],DDiff_R[38];//二阶差分
//  int   Liner_L_cnt  = 0,Liner_R_cnt  = 0;
//  u8    Liner_L_flag = 0,Liner_R_flag = 0;
//  u8    Impulse_L_Flag = 0,Impulse_R_Flag = 0;//一阶差分中出现阶跃
//  u8 i = 0;
//  int Quanbai_flag = 0;
//  u8 Start_line = 0;
//  int Quanbai_Left = 0,Quanbai_Right = 0;
//  
//  Start_line = max_s16(&ROADY.Y[120],80);///////////////////////////////////////////////////
////  for(i=0;i<20;i++)
////  {
////    if(Start_line+(i+1)*2 < Y_START
////       &&sum_point(Image_fire[Start_line+(i+1)*2],40)<=5)
////      Quanbai_cnt++;
////  }
//  if(All_White_Line(Start_line,&Quanbai_Left,&Quanbai_Right)==1)
//  {
//    if(LCD_DISPLAY_FLAG)
//    {
//      LCD_DrawBigPoint(CCD_START+3,Quanbai_Left  - IMAGE_CORRECT,COLOR_BLUE);
//      LCD_DrawBigPoint(CCD_END-  3,Quanbai_Right - IMAGE_CORRECT,COLOR_BLUE);
//    }
//    if(Find_Quanbai_Xie(Quanbai_Left,Quanbai_Right)==1)
//    {
//      Quanbai_flag = 1;
//    }
//    else 
//      Quanbai_flag = 0;
//  }
//  else
//  {
//    Quanbai_flag = 0;
//  }
//  
//  for(i=0;i<40;i++)//10行
//  {
//    Right_Count = ROAD.Right[i+Island.Image_Start_hang];
//    if(Right_Count<ccd_end)//如果在有效范围内
//    {
//      Island.black_R[i] = Right_Count;
//    }
//    else
//    {
//      Island.black_R[i] = ccd_end;
//    }
//    Left_Count = ROAD.Left[i+Island.Image_Start_hang];
//    if(Left_Count > ccd_start)
//    {
//      Island.black_L[i] = Left_Count; 
//    }
//    else
//    {
//      Island.black_L[i] = ccd_start;
//    }
//  }
//  for(i=0;i<39;i++)
//  {
//    Diff_L[i] = Island.black_L[i+1] - Island.black_L[i];
//    Diff_R[i] = Island.black_R[i+1] - Island.black_R[i];
//  }
//  for(i=0;i<38;i++)
//  {
//    DDiff_L[i] = Diff_L[i+1] - Diff_L[i];
//    DDiff_R[i] = Diff_R[i+1] - Diff_R[i];
//    if(Abs_(DDiff_L[i])<Liner_threshold
//       &&(Abs_(Diff_L[i])>0
//          ||Abs_(Diff_L[i+1])>0))
//      Liner_L_cnt++;
//    if(Abs_(DDiff_R[i])<Liner_threshold
//       &&(Abs_(Diff_R[i])>0
//          ||Abs_(Diff_R[i+1])>0))
//      Liner_R_cnt++;
//    if(DDiff_L[i]<-40
////       &&Island.black_L[i]>50
//         &&Liner_L_cnt>i/2
//           &&Liner_L_cnt>0
//             )
//    {
//      Impulse_L_Flag=1;//出现冲激
//    }
//    if(DDiff_R[i]>40
////       &&Island.black_R[i]<269
//         &&Liner_R_cnt>i/2
//           &&Liner_R_cnt>0
//             )
//    {
//      Impulse_R_Flag=1;
//    }
//  }
//  if(Liner_L_cnt>29)Liner_L_flag = 1;
//  if(Liner_R_cnt>29)Liner_R_flag = 1;
//  
//  if((Impulse_R_Flag&&Impulse_L_Flag)
//  ||Quanbai_flag == 1)
//    return 0;
//  else if(Liner_L_flag&&Impulse_R_Flag==1)
//  {
//    //    Beep_Once(&Image_Island_Test_Beep);
//    return Right_Island_pre;
//  }
//  else if(Liner_R_flag&&Impulse_L_Flag==1)
//  {
//    //    Beep_Once(&Image_Island_Test_Beep);
//    return Left_Island_pre;
//  }
//  else 
//    return 0;
//}
//
//
//int Stay2Out_test()
//{
//  u8 Far_Lie[30];
//  static int Diff_Far_Lie[29];//一阶差分
//  static int DDiff_Far_Lie[28];//二阶差分
//  static int Edge_Hang[30];
//  static int Diff_Hang[29];
//  static int DDiff_Hang[28];
//  
//  int Liner_cnt  = 0;
//  u8 Impulse_flag = 0;
//  int Liner_H_cnt= 0;
//  
//  u8 i;
//  if(Island.State==Right_Island_in)
//  {
//    for(i=0;i<30;i++)
//    {
//      Far_Lie[i] = ROADY.Y[CCD_START+i*4];
//      Edge_Hang[i] = ROAD.Left[Y_START - 4 - i*2];
//    }
//    for(i=0;i<29;i++)
//    {
//      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
//      Diff_Hang[i] = Edge_Hang[i+1] - Edge_Hang[i];
//    }
//    for(i=0;i<28;i++)
//    {
//      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
//      DDiff_Hang[i] = Diff_Hang[i+1] - Diff_Hang[i];
//      if(Abs_(DDiff_Far_Lie[i])<Liner_threshold)//线性标志
//        Liner_cnt++;
//      if(Abs_(DDiff_Hang[i])<Liner_threshold)
//        Liner_H_cnt++;
//      if(DDiff_Far_Lie[i]<-15
//         &&Liner_cnt>i/3
//           &&Liner_cnt>0)//之前线性，出现冲激
//      {
//        Impulse_flag = 1;
//        break;
//      }
//      if(Diff_Hang[i] < -1
//         &&Liner_H_cnt > i/2
//           &&Liner_H_cnt>0
//            &&Y_START - 4 - i*2 > ROADY.far+8)
//      {
//        Impulse_flag = 1;
//        break;
//      }
//      
//    }
//  }
//  else if(Island.State==Left_Island_in)
//  {
//    for(i=0;i<30;i++)
//    {
//      Far_Lie[i] = ROADY.Y[CCD_END-i*4];
//      Edge_Hang[i] = ROAD.Right[Y_START - 4 - i*2];
//    }
//    for(i=0;i<29;i++)
//    {
//      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
//      Diff_Hang[i] = Edge_Hang[i+1] - Edge_Hang[i];
//    }
//    for(i=0;i<28;i++)
//    {
//      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
//      DDiff_Hang[i] = Diff_Hang[i+1] - Diff_Hang[i];
//      if(Abs_(DDiff_Far_Lie[i])<Liner_threshold)//线性标志
//        Liner_cnt++;
//      if(Abs_(DDiff_Hang[i])<Liner_threshold)
//        Liner_H_cnt++;
//      if(DDiff_Far_Lie[i]<-15
//         &&Liner_cnt>i/3
//           &&Liner_cnt>0)//之前线性，出现冲激
//      {
//        Impulse_flag = 1;
//        break;
//      }
//      if(Diff_Hang[i] > 1
//         &&Liner_H_cnt > i/2
//           &&Liner_H_cnt>0
//             &&Y_START - 4 - i*2 > ROADY.far+8)
//      {
//        Impulse_flag = 1;
//        break;
//      }
//    }
//  }
//  return Impulse_flag;
//}
//
//int Wait_Next_Island()
//{
//  if(Island.State!=Left_Wait_Next&&Island.State!=Right_Wait_Next)//不在此状态下
//    return 1;//直接返回
//  
//  CenterlineToDiff(get_Center(Main_Line-5));
//  
//  return 0;
//}
//
//int detect_startline(int line)
//{
//  int start_Flag=0;
//  int i=0,j=0;
//  int size_flag=0;
//  //start_data.status=0;
//  j=0;
// 
//  for(i=CCD_START;i<CCD_END;i++)
//  {
//    if(j<260)
//    {
//      if(IMG_PIXEL(i,line)>=1)
//      {
//        start_data.black_white[j]=1;
//      }
//      else
//        start_data.black_white[j]=0;
//    }
//    j++;
//
//  }
//   for(int i=0;i<20;i++)
//  {
//    start_data.continuous[i]=0;
//  }
//  start_data.color_flag=start_data.black_white[0];
//  for(i=1;i<260;i++)
//  {
//    if(start_data.black_white[i]==start_data.black_white[i-1])
//    {
//      start_data.continuous[start_data.status]++;
//    }
//    else if(start_data.black_white[i]!=start_data.black_white[i-1])
//    {
//      start_data.status+=1;
//    }
//  }
//  for(int i=0;i<20;i++)
//{
//  if (start_data.continuous[i]>4000)
//    start_data.continuous[i]=4000;
//}
//   size_flag=0;
//  for(int i=0;i<20;i++)
//  {
//    if(start_data.continuous[i]>=5)
//    {
//      size_flag+=1;
//    }
//  }
//  if(start_data.status>=15&&size_flag>=15)
//  {
//    start_Flag=1;
//  }
//  else 
//    start_Flag=0;
//  return start_Flag;
//  
//}
//void start_operate(void)
//{
//  start_data.detect_flag=0;
//  start_data.confirm_flag=0;
// 
//  start_data.detect_line=test_start;
//  start_data.confirm_line=low_line;
//  start_data.status=0;//每次进如函数则对status清零
//  start_data.detect_flag=detect_startline(start_data.detect_line);
//  start_data.status=0;//每次进如函数则对status清零
//  start_data.confirm_flag=detect_startline(start_data.confirm_line);
//  if(start_data.detect_flag==1)
//  {
//   start_data.stop_flag=1;
//   start_data.toelc=1;
//    Dir_mode = Elec_Mode;
//  }
//  if(start_data.confirm_flag==1)
//  {
//    start_data.stop_flag=2;
//   }
//
//
//
// }
//
//
//
//int All_White_Line(int hang,int *Left,int *Right)
//{
//  int Left_start=0,Right_start=0;
//  int Left_end=0 ,Right_end=0;
//  int Left_max_start = 0,Right_max_start = 0;
//  int Left_max_end = 0 ,Right_max_end = 0;
//  int Left_longest = 0,Right_longest = 0;
//  int Left_cnt = 0    ,Right_cnt = 0;
//  if(LCD_DISPLAY_FLAG)
//  {
//    Draw_single_line('H',hang-IMAGE_CORRECT,COLOR_CYAN);
//  }
//  
//  
//  for (int i = hang + 2;i<(Y_START - 2);i++)
//  {
//    
////    if(ROAD.Left[i] < CCD_START+3)
//    if(IMG_PIXEL(CCD_START+3,i)==0)
//    {
//      if(Left_cnt == 0)//新的开始
//      {
//        Left_start = i;
//      }
//      Left_cnt ++;
//    }
//    else
//    {
//      if(Left_cnt > 0)
//      {
//        Left_end = i;
//        if((Left_cnt)>Left_longest)//比之前的大
//        {
//          Left_longest = Left_cnt;
//          Left_max_start = Left_start;
//          Left_max_end   = Left_end;
//        }
//      }
//      Left_cnt = 0;
//    }
//    
////    if(ROAD.Right[i] > CCD_END-3)
//    if(IMG_PIXEL(CCD_END-3,i)==0)
//    {
//      if(Right_cnt == 0)//新的开始
//      {
//        Right_start = i;
//      }
//      Right_cnt ++;
//    }
//    else
//    {
//      if(Right_cnt > 0)
//      {
//        Right_end = i;
//        if((Right_cnt)>Right_longest)//比之前的大
//        {
//          Right_longest = Right_cnt;
//          Right_max_start = Right_start;
//          Right_max_end   = Right_end;
//        }
//      }
//      Right_cnt = 0;
//    }
//  }
//    if(Left_cnt > 0)
//    {
//      Left_end = Y_START - 2;
//      if((Left_cnt)>Left_longest)//比之前的大
//      {
//        Left_longest = Left_cnt;
//        Left_max_start = Left_start;
//        Left_max_end   = Left_end;
//      }
//    }
//    if(Right_cnt > 0)
//    {
//      Right_end = Y_START - 2;
//      if((Right_cnt)>Right_longest)//比之前的大
//      {
//        Right_longest = Right_cnt;
//        Right_max_start = Right_start;
//        Right_max_end   = Right_end;
//      }
//    }
//  
//  if((Left_longest > 10)&&(Right_longest > 10))
//  {
//    *Left  = (Left_max_start + Left_max_end)/2;
//    *Right = (Right_max_start + Right_max_end)/2;
//    return 1;
//  }
//  else
//    return 0;
//  
//}
//
//int Find_Quanbai_Xie(int y1,int y2)
//{
//  int xm1=0,ym2=0,model_x,model_y,model,  mcx,mcy,mRow,mCol;  
//  int Cnt = 0;
//  int x1 = CCD_START+3,x2 = CCD_END-3;
//  model_x=x2-x1;                 
//  model_y=y2-y1; 
//  mRow=x1; 
//  mCol=y1; 
//  if(model_x>0)mcx=1;       
//  else if(model_x==0)mcx=0;      
//  else {mcx=-1;model_x=-model_x;} 
//  if(model_y>0)mcy=1; 
//  else if(model_y==0)mcy=0;    
//  else{mcy=-1;model_y=-model_y;} 
//  if( model_x>model_y)model=model_x;  
//  else model=model_y; 
//  for(int i=0;i<=model+1;i++ )     
//  {
//    if(IMG_PIXEL(mRow,mCol)==0)
//    {
//      Cnt ++;
//    }
//    xm1+=model_x ; 
//    ym2+=model_y ; 
//    if(xm1>model) 
//    { 
//      xm1-=model; 
//      mRow+=mcx; 
//    } 
//    if(ym2>model) 
//    { 
//      ym2-=model; 
//      mCol+=mcy; 
//    } 
//  }  
//  if(Cnt>(model+1)*0.95f)
//  {
//    return 1;
//  }
//  else
//    return 0;
//}
