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
    Get_column_end(i);//����߽�
  }
  ROADY.Three_lie_end[0] = ROADY.Y[ROADY.Three_Lie[0]];
  ROADY.Three_lie_end[1] = ROADY.Y[ROADY.Three_Lie[1]];
  ROADY.Three_lie_end[2] = ROADY.Y[ROADY.Three_Lie[2]];
  Road_XY_Rebuild();
  Island_process();
  Cross_process();
  if(break_info.meeting_flag==0&&Island.State==NoIsland)
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
    line--;                                                           //̽�ⶥ��
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
  for(int i = ROADY.far + 5;i <= Y_START-2;i++)//����ɨ��
  {
    get_black_line(i);
  }

}
int get_Center(int hang)
{
  int count;
  int Middle;
  
  Middle = ROADY.farC;//���ĸ�ֵ
  
  count = Middle;//�Ѻ����м�ֵ�����������
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
  
  count = Middle;//�Ѻ����м�ֵ�����������
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
  if(ROADY.Center>Center_Stand)//�Ҳಹ��
  {
    if(ROADY.far<hang)
      ROADY.Center -= (int)((ROADY.far-hang)*(ROADY.far-hang)*ROADY.Middle_Correct);
    else 
      ROADY.Center += (int)((ROADY.far-hang)*(ROADY.far-hang)*ROADY.Middle_Correct);
    if(ROADY.Center<Center_Stand)
      ROADY.Center = Center_Stand;
  }
  else if(ROADY.Center<Center_Stand)//��ಹ��
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
  int Middle=160;  //�����м�Ĭ��ΪCENNTER
  int count;
  int _black_R,_black_L;//�������Ҷ�
  
  Middle = ROADY.farC;//���ĸ�ֵ
  count = Middle;//�Ѻ����м�ֵ�����������
  while(!((IMG_PIXEL(count+3,hang)) 
          && (IMG_PIXEL(count+2,hang))
            && (IMG_PIXEL(count+1,hang)))
        && count < CCD_END)//�������Ч����û���ҵ����������ڵ�
    count++;//���м�λ�ÿ�ʼ���������������������㶼�Ǻڵ�ͣ
  ROAD.Right[hang] = _black_R = count;
  
  if(_black_R<CCD_END)//�������Ч��Χ��
  {
    ROAD.Find[hang] |= RIGHT;
  }
  else
  {
    ROAD.Find[hang] &= ~RIGHT;
  }
  
  count = Middle;//�Ѻ����м�ֵ�����������
  while(!((IMG_PIXEL(count-3,hang)) 
          && (IMG_PIXEL(count-2,hang))
            && (IMG_PIXEL(count-1,hang)))
        && count > CCD_START)//�������Ч����û���ҵ����������ڵ�
    count--;//���м�λ�ÿ�ʼ���������������������㶼�Ǻڵ�ͣ
  ROAD.Left[hang] = _black_L = count;
  
  if(_black_L>CCD_START)//�������Ч��Χ��
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
      if(Left_cnt == 0)//�µĿ�ʼ
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
        if((Left_cnt)>Left_longest)//��֮ǰ�Ĵ�
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
      if(Right_cnt == 0)//�µĿ�ʼ
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
        if((Right_cnt)>Right_longest)//��֮ǰ�Ĵ�
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
      if((Left_cnt)>Left_longest)//��֮ǰ�Ĵ�
      {
        Left_longest = Left_cnt;
        Left_max_start = Left_start;
        Left_max_end   = Left_end;
      }
    }
    if(Right_cnt > 0)
    {
      Right_end = Y_START - 2;
      if((Right_cnt)>Right_longest)//��֮ǰ�Ĵ�
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