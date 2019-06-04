#include "include.h"
#include "process.h"
#include "mymath.h"

ROAD_LINE ROAD;
ROAD_Y_STR ROADY = {.Middle_Correct = 0.0038,
                    };
int Main_Line;
u8 Liner_threshold = 4;

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
  Island_process();
  Cross_process();
//   if( start_process==1)
//       ramp();
//  if(break_info.meeting_flag==0&&Island.State==NoIsland&&ramp_flag==0)
//    pattern_shift();
//  if(start_info.final_stop!=2&&break_info.meeting_flag==1&&ramp_flag==0)
//    start_operate();
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

u8 Find_All_White(int hang)
{
  u8 Quanbai_flag = 0;
  int Quanbai_Left = 0,Quanbai_Right = 0;
  if(All_White_Line(hang,&Quanbai_Left,&Quanbai_Right)==1)
  {
    if(LCD_DISPLAY_FLAG)
    {
      LCD_DrawBigPoint(CCD_START+3,Quanbai_Left  - IMAGE_CORRECT,COLOR_BLUE);
      LCD_DrawBigPoint(CCD_END-  3,Quanbai_Right - IMAGE_CORRECT,COLOR_BLUE);
    }
    if(Find_Quanbai_Xie(Quanbai_Left,Quanbai_Right)==1)
    {
      Quanbai_flag = 1;
    }
    else 
      Quanbai_flag = 0;
  }
  else
  {
    Quanbai_flag = 0;
  }
  return Quanbai_flag;
}

int All_White_Line(int hang,int *Left,int *Right)
{
  int Left_start=0,Right_start=0;
  int Left_end=0 ,Right_end=0;
  int Left_max_start = 0,Right_max_start = 0;
  int Left_max_end = 0 ,Right_max_end = 0;
  int Left_longest = 0,Right_longest = 0;
  int Left_cnt = 0    ,Right_cnt = 0;
  if(LCD_DISPLAY_FLAG)
  {
    Draw_single_line('H',hang-IMAGE_CORRECT,COLOR_CYAN);
  }
  
  
  for (int i = hang + 2;i<(Y_START - 2);i++)
  {
    
//    if(ROAD.Left[i] < CCD_START+3)
    if(IMG_PIXEL(10,i)==0)
    {
      if(Left_cnt == 0)//新的开始
      {
        Left_start = i;
      }
      Left_cnt ++;
    }
    else
    {
      if(Left_cnt > 0)
      {
        Left_end = i;
        if((Left_cnt)>Left_longest)//比之前的大
        {
          Left_longest = Left_cnt;
          Left_max_start = Left_start;
          Left_max_end   = Left_end;
        }
      }
      Left_cnt = 0;
    }
    
//    if(ROAD.Right[i] > CCD_END-3)
    if(IMG_PIXEL(310,i)==0)
    {
      if(Right_cnt == 0)//新的开始
      {
        Right_start = i;
      }
      Right_cnt ++;
    }
    else
    {
      if(Right_cnt > 0)
      {
        Right_end = i;
        if((Right_cnt)>Right_longest)//比之前的大
        {
          Right_longest = Right_cnt;
          Right_max_start = Right_start;
          Right_max_end   = Right_end;
        }
      }
      Right_cnt = 0;
    }
  }
    if(Left_cnt > 0)
    {
      Left_end = Y_START - 2;
      if((Left_cnt)>Left_longest)//比之前的大
      {
        Left_longest = Left_cnt;
        Left_max_start = Left_start;
        Left_max_end   = Left_end;
      }
    }
    if(Right_cnt > 0)
    {
      Right_end = Y_START - 2;
      if((Right_cnt)>Right_longest)//比之前的大
      {
        Right_longest = Right_cnt;
        Right_max_start = Right_start;
        Right_max_end   = Right_end;
      }
    }
  
  if((Left_longest > 12)&&(Right_longest > 12))
  {
    *Left  = (Left_max_start + Left_max_end)/2;
    *Right = (Right_max_start + Right_max_end)/2;
    return 1;
  }
  else
    return 0;
  
}

int Find_Quanbai_Xie(int y1,int y2)
{
  int xm1=0,ym2=0,model_x,model_y,model,  mcx,mcy,mRow,mCol;  
  int Cnt = 0;
  int x1 = 10,x2 = 310;
  model_x=x2-x1;                 
  model_y=y2-y1; 
  mRow=x1; 
  mCol=y1; 
  if(model_x>0)mcx=1;       
  else if(model_x==0)mcx=0;      
  else {mcx=-1;model_x=-model_x;} 
  if(model_y>0)mcy=1; 
  else if(model_y==0)mcy=0;    
  else{mcy=-1;model_y=-model_y;} 
  if( model_x>model_y)model=model_x;  
  else model=model_y; 
  for(int i=0;i<=model+1;i++ )     
  {
    if(IMG_PIXEL(mRow,mCol)==0)
    {
      Cnt ++;
    }
    xm1+=model_x ; 
    ym2+=model_y ; 
    if(xm1>model) 
    { 
      xm1-=model; 
      mRow+=mcx; 
    } 
    if(ym2>model) 
    { 
      ym2-=model; 
      mCol+=mcy; 
    } 
  }  
  if(Cnt>(model+1)*0.95f)
  {
    return 1;
  }
  else
    return 0;
}
int left_x[20];
int right_x[20];
int delat_x[20];
int test_delat;
int test_delat_l;
int ramp_sum;
int delat_x_l[20];
int straight_ramp=0;

void ramp_straight(void)
{
  int scan_middle=153;
  int scan_start_left;
  int scan_start_right;
  int k=3;
  test_delat=0;
  k=3;
  for(int i=0;i<5;i++)
  {
    
    scan_start_left=scan_middle;
    while(!((IMG_PIXEL(scan_start_left-3,Main_Line-k))
           &&(IMG_PIXEL(scan_start_left-2,Main_Line-k))
           &&(IMG_PIXEL(scan_start_left-1,Main_Line-k)))
           &&scan_start_left>CCD_START)
      scan_start_left--;
     if(scan_start_left>CCD_START&&scan_start_left<153)
     {
       left_x[i]=scan_start_left;
     }
     else
     {
       left_x[i]=-1;
     }
     scan_start_right=scan_middle;
      while(!((IMG_PIXEL(scan_start_right+3,Main_Line-k))
           &&(IMG_PIXEL(scan_start_right+2,Main_Line-k))
           &&(IMG_PIXEL(scan_start_right+1,Main_Line-k)))
           &&scan_start_right<CCD_END)
      scan_start_right++;
      if(scan_start_right<CCD_END&&scan_start_right>153)
      {
        right_x[i]=scan_start_right;
      }
      else
        right_x[i]=-1;    
    if(LCD_DISPLAY_FLAG==1)
    {
      
     POINT(right_x[i]-10,Main_Line-k-90,COLOR_RED);
     POINT(left_x[i]+10,Main_Line-k-90,COLOR_RED);
    }
   k=k+1;
  }
  ramp_sum=0;
  for(int i=1;i<5;i++)
  {
    if(right_x[i]!=-1)
    {
      delat_x[i-1]=right_x[i]-right_x[i-1];
      delat_x[i-1]=abs_s16(delat_x[i-1]);
      
      if(delat_x[i-1]<=5)
       ramp_sum++;
    }
     else
    {
     delat_x[i-1]=20;
    }
   if(left_x[i]!=-1)
    {
      delat_x_l[i-1]=left_x[i]-left_x[i-1];
      delat_x_l[i-1]=abs_s16(delat_x_l[i-1]);
    }                                 
   else
      delat_x_l[i-1]=20;
    
  }
   test_delat=delat_x[0];
  test_delat_l=delat_x_l[0];
  
  for(int i=1;i<5;i++)
  {
    if(test_delat<delat_x[i])
    {
      test_delat=delat_x[i];
    }
    
    if(test_delat_l<delat_x_l[i])
    {
      test_delat_l=delat_x_l[i];
    }
  }
  
  if(test_delat_l<=5||test_delat<=5)
  {
    straight_ramp=1;
  }
  else
    straight_ramp=0;
                      
}
int ramp_flag=0;
int ramp_wrsum=0;
int ramp_over=0;
int ave_dc=0;
int beep_ramp=0;
int ramp_whl=0;
int ramp_elec=0;
void ramp(void)
{
   ramp_straight();
  // ramp_whl=0;
  int sum_straight=0;
  
  for(int i=Main_Line;i>Main_Line-10;i--)
  {
    if(part_straight(i)==1)
    {
      sum_straight++;
    }
    else
    {
      sum_straight=0;
    }
  }
  if(ramp_over!=1)
  {
     if(straight_ramp==1&&TOF_Distance<=400&&TOF_Distance>100)
     {
       ramp_flag=1;
       // Dir.Dir_mode=Elec_Mode;
       Main_Line=170;
    
      }
      if(ramp_flag==1)
     {
         if(ave_dc>130&&ave_dc<160&&beep_ramp==0)
       {
           beep_ramp=1;
           Dir.Dir_mode=Elec_Mode;
            Beep_Once(&Beep_100ms);
            ramp_elec=1;
         }
         
         if(ramp_elec==1)
         {
           if(TOF_Distance==2000)
           {
             ramp_whl=1;
           }
    
          if(straight_ramp==1&&TOF_Distance<=400&&TOF_Distance>100&&ramp_whl==1)
            {
                ramp_over=1;
             }
         }
       }
  }
  else if(ramp_over==1)
  {
    Main_Line=152;
    Dir.Dir_mode=Camera_Mode;
    LED_NO;
    ramp_flag=0;
  }

  
}
int ramp_cnt=0;
int ramp_diff[20];
int ramp_delat[20];
int ramp_ave[10];

void ramp_filter(void)
{
  static unsigned char TimeCnt_20ms = 0;
   TimeCnt_20ms++;
   if(TimeCnt_20ms==4)
   {
      ramp_cnt++;
      TimeCnt_20ms=0;
   }
  
//   if(ramp_cnt<20)
//   {
//     ramp_diff[ramp_cnt]=TOF_Distance;
//     
//   }
//   else
//   {
//     ramp_cnt=0;
//   }
//   for(int i=1;i<20;i++)
//   {
//     ramp_delat[i-1]=ramp_diff[i]-ramp_diff[i-1];
//     ramp_delat[i-1]=abs_s16(ramp_delat[i-1]);
//   }
//   ramp_wrsum=0;
//   for(int i=0;i<20;i++)
//   {
//     if(ramp_delat[i]>10)
//        ramp_wrsum++;
//    }    
   if(ramp_cnt<5)
     ramp_ave[ramp_cnt]=ROADY.Center;
   else
     ramp_cnt=0;
   ave_dc=0;
   for(int i=0;i<5;i++)
   {
     ave_dc+=ramp_ave[i];
   }
   ave_dc=ave_dc/5;
   
   
}

