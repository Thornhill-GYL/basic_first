#include "Island.h"
#include "Process.h"
#include "include.h"

Island_Data    Island={
  .State = NoIsland,
  .Correct_hang = 180,//���߸�����
  .Stay_hang_use = 150,//�������Ϲ����
  .Image_Start_hang = 124,//����̽��ǰհ
  .ADdouble =      {.delay_const = 5000 ,.dec = DIS_DEC},
  .Out2doubleFAR = {.delay_const = 5500 ,.dec = DIS_DEC},
  .Next_Island   = {.delay_const = 25000,.dec = DIS_DEC},
  .Stay2Out      = {.delay_const = 10800,.dec = DIS_DEC},
//  .Next_Island_flag_delay_const = 2000/PIT_PERIOD,
//  .Stay2Out_flag_delay_const = 500/PIT_PERIOD,//�������ʱ��
//  .Out2doubleFAR_flag_delay_const = 300/PIT_PERIOD
};


u8 Island_process(void)
{ 
  Elec_Island();//��ż�⣬ֻ���뻷�ͳ�����ʱ����
  In_Island();//�뻷��
  Stay_Island();//�ڻ�����
  Out_Island();//������
  Wait_Next_Island();//��ֹ�ٴν��뻷��
  
  return 0;
}


u8 In_double_AD(void)
{
  if(((Dir.Elec_Left>3400)&&(Dir.Elec_Right>1600))
     ||((Dir.Elec_Left>1600)&&(Dir.Elec_Right>3400))
     ||((Dir.Elec_Left>3900)&&(Dir.Elec_Right>1200))
     ||((Dir.Elec_Left>1200)&&(Dir.Elec_Right>3900))
       )
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}


int go_in_island=0;
u8 Elec_Island(void)
{
  u8 doublt_island = 0;
  
  if(Island.State == NoIsland)//�޻���ʱ��⻷��
  {
    if(In_double_AD()==1)
    {
      Time_delay_start(&Island.ADdouble);
    }
    if(Island.ADdouble.flag)
      doublt_island = Image_Island_Test();
    if(doublt_island!=0)
    {
//      Beep_Once(&normal_beep);
    }
    
    
    if(doublt_island!=0)
    {
      //LED_CYAN;
      Island.State = doublt_island;
      if(Island.State==Left_Island_pre)
      {
        Island.In_Center = ROADY.Three_Lie[1] - 25;
      }
      else if(Island.State==Right_Island_pre)
      {
        Island.In_Center = ROADY.Three_Lie[1] + 25;
      }
    }
  }
  
  if(Island.State == Left_Island_out)
  {
    if(Island.doubleFAR_Allow_flag
       &&ROADY.farC<ROADY.Three_Lie[1]-50)
    {
      Island.State = Left_Wait_Next;//�ȴ���һ��������ʱ����
      
    }
  }
  else if(Island.State == Right_Island_out)
  {
    
    if(Island.doubleFAR_Allow_flag
       &&ROADY.farC>ROADY.Three_Lie[1]+50)
    {
      Island.State = Right_Wait_Next;//�ȴ���һ��������ʱ����
    }
  }
  return 0;
}


u8 In_Island(void)
{
  int center;
  static int center_use;
  u8  Impulse_hang = 0;
  if(Island.State!=Left_Island_pre&&Island.State!=Right_Island_pre)//���ڴ�״̬��
    return 1;//ֱ�ӷ���
  else 
  {
    center = In_Island_center(&Impulse_hang);//Ѱ�����ĵ�
    In2Stay_Island(center,Impulse_hang);
    if(center == -1)//Ѱ��ʧ�ܻ�������ȫ���뻷��
    {
      CenterlineToDiff(Island.In_Center);//ʹ����һ�εľ�ֵ
    }
    else
    {
      if(Island.State==Left_Island_pre)
      {
        center_use = (int)(((center - (center - CCD_END)*(Impulse_hang - Island.Stay_hang_use)*1.0/(Impulse_hang - Island.Correct_hang)) + CCD_START)/2);//���ߣ����������ƣ�
        if(center_use<CCD_START)
          center_use = CCD_START;
        Island.In_Center = center_use;//������һ�ε����ĵ�
        CenterlineToDiff(center_use);
      }
      else if(Island.State==Right_Island_pre)
      {
        center_use = (int)(((center - (center - CCD_START)*(Impulse_hang - Island.Stay_hang_use)*1.0/(Impulse_hang - Island.Correct_hang)) + CCD_END)/2);//���ߣ����������ƣ�
        if(center_use>CCD_END)
          center_use = CCD_END;
        
        Island.In_Center = center_use;//������һ�ε����ĵ�
        CenterlineToDiff(center_use);
      }
      //�����ã���ʾ���ĵ�
      if(LCD_DISPLAY_FLAG==1)
      {
        //        LCD_DrawBigPoint(center_use,Main_Line,Blue);//���еߵ�
      }
    }
  }
  return 0;
}

int In_Island_center(u8* hang)//�뻷��ʱѰ��ͻ���+����
{
  int Middle;
  int center = 160;
  int ccd_start=10,ccd_end=310;  //ccdɨ�����10���յ�310   
  int Left_Count=0,Right_Count=0;//���Ҽ���Ϊ0
  static int Diff_L[19],Diff_R[19];//һ�ײ��
  static u16 In_black_L[20];
  static u16 In_black_R[20];
  u8  Impulse_L_Flag = 0,Impulse_R_Flag = 0;//һ�ײ���г��ֽ�Ծ
  u8 i = 0;
  
  Middle = ROADY.farC;
  if(Island.State == Right_Island_pre)
  {
    if(Middle>ROADY.Three_Lie[1]+40)return -1;//ǰ��ֱ���Ѿ���������
  }
  else if(Island.State == Left_Island_pre)
  {
    if(Middle<ROADY.Three_Lie[1]-40)return -1;//ǰ��ֱ���Ѿ���������
  }
  
  for(i=0;i<20;i++)//20��
  {
    if(Island.State == Right_Island_pre)
    {
      Right_Count = ROAD.Right[i*3+Island.Image_Start_hang];
      if(Right_Count<ccd_end)//�������Ч��Χ��
      {
        In_black_R[i] = Right_Count;
      }
      else if(Right_Count<ROADY.Three_Lie[1]+10)
      {
        In_black_R[i] = ccd_end;
      }
      else
      {
        In_black_R[i] = ccd_end;
      }
    }
    else if(Island.State == Left_Island_pre)
    {
      Left_Count = ROAD.Left[i*3+Island.Image_Start_hang];
      if(Left_Count > ccd_start)
      {
        In_black_L[i] = Left_Count; 
      }
      else if(Left_Count>ROADY.Three_Lie[1]-10)
      {
        In_black_L[i] = ccd_start;
      }
      else
      {
        In_black_L[i] = ccd_start;
      }
    }
  }
  
  if(Island.State == Right_Island_pre)
  {
    for(i=0;i<19;i++)
    {
      Diff_R[i] = In_black_R[i+1] - In_black_R[i];
      if(Diff_R[i]>30)
      {
        Impulse_R_Flag = 1;
        center = In_black_R[i];//������ת����
        *hang   = i*3+Island.Image_Start_hang;
        break;
      }
    }
  }
  else if(Island.State == Left_Island_pre)
  {
    for(i=0;i<19;i++)
    {
      Diff_L[i] = In_black_L[i+1] - In_black_L[i];
      if(Diff_L[i]<-30)
      {
        Impulse_L_Flag = 1;//���ֳ弤
        center = In_black_L[i];//������ת����
        *hang   = i*3+Island.Image_Start_hang;
        break;
      }
    }
  }
  if(Impulse_R_Flag==0&&Impulse_L_Flag==0)
  {
    return -1;//û�г���
  }
  else 
  {
    return center;
  }
}

u8 In2Stay_Island(int center,int hang)
{
  u8 i;
  u8  Curve_cnt = 0;
  if(Island.State!=Left_Island_pre&&Island.State!=Right_Island_pre)
    return 1;
  
  if(Island.State==Left_Island_pre)
  {
    for(i=0;i<29;i++)
    {
      if(ROADY.Y[40+(i+1)*8]-ROADY.Y[40+i*8]>0)
        Curve_cnt ++;
    }
  }
  else if(Island.State==Right_Island_pre)
  {
    for(i=0;i<29;i++)
    {
      if(ROADY.Y[40+(i+1)*8]-ROADY.Y[40+i*8]<0)
        Curve_cnt ++;
    }
  }
  
  if(Island.State==Left_Island_pre)
  {
    if(
       ROADY.farC<ROADY.Three_Lie[1]-60
         &&((hang>(Island.Stay_hang_use+5)&&center>ROADY.Three_Lie[1]+60)
            ||Curve_cnt>22))
    {
      Island.In2Stay_cnt++;
    }
  }
  else if(Island.State==Right_Island_pre)
  {
    if(
       ROADY.farC>ROADY.Three_Lie[1]+60
         &&((hang>(Island.Stay_hang_use+5)&&center>ROADY.Three_Lie[1]-60)
            ||Curve_cnt>22))
    {
      Island.In2Stay_cnt++;
    }
  }
  if(Island.In2Stay_cnt>1)
  {
//    Beep_Once(&Test_100ms);
    if(Island.State==Left_Island_pre)
    {
      Island.State = Left_Island_in;
      Island.Stay_Center = 50;
    }
    else if(Island.State==Right_Island_pre)
    {
      Island.State = Right_Island_in;
      Island.Stay_Center = 269;
    }
    Time_delay_start(&Island.Stay2Out);
  }
  return 0;
}

u8 Stay_Island(void)
{
  if(Island.State!=Left_Island_in&&Island.State!=Right_Island_in)
    return 1;
  
  //������ͨ���Ѱ������
  Island.Stay_Center = get_Center(Island.Stay_hang_use);//ROAD.Middle[Island.Stay_hang_use];
  CenterlineToDiff(Island.Stay_Center);
  
  Island.Out_Center_ += Island.Stay_Center;
  Island.Stay_Island_center_cnt++;
  
  if(Island.Out_Allow_flag==1)//����ı�״̬
  {
    if(Stay2Out_test()==1)
    {
      Island.Stay2Out_cnt ++;
    }
    else
    {
      Island.Stay2Out_cnt = 0;
    }
    if(Island.Stay2Out_cnt > 1)//�������β⵽ͻ���״̬�ı�
    {
      Island.Out_Center = (Island.Out_Center_ / Island.Stay_Island_center_cnt);
      if(Island.State==Left_Island_in)
      {
//        Island.Out_Center += (Center_Stand - Island.Out_Center)*0.2;
        Island.State = Left_Island_out;
      }
      else if(Island.State==Right_Island_in)
      {
//        Island.Out_Center += (Center_Stand - Island.Out_Center)*0.2;
        Island.State = Right_Island_out;
      }
      Time_delay_start(&Island.Out2doubleFAR);
      Time_delay_start(&Island.Next_Island);
//      Island.Out2doubleFAR_flag = 1;
//      Island.Out2doubleFAR_flag_delay = Island.Out2doubleFAR_flag_delay_const;
//      Island.Next_Island_flag = 1;
//      Island.Next_Island_flag_delay = Island.Next_Island_flag_delay_const;
    }
  }
  
  return 0;
}

u8 Out_Island(void)
{
  int center_use;
  
  //����������־
  if(Island.State!=Right_Island_out&&Island.State!=Left_Island_out)
    return 1;
  
  if(Island.State==Right_Island_out)//����
  {
    if(Out_Island_find_Quanbai()==1)
    {
      center_use = Island.Out_Center ;
    }
    else//����ʧ�ܣ�ʹ��֮ǰ��������ĵ�
    {
      center_use = Island.Out_Center;
    }
    CenterlineToDiff(center_use);
  }
  else if(Island.State==Left_Island_out)
  {
    if(Out_Island_find_Quanbai()==1)
    {
      //      Beep_Once(&Image_Island_Test_Beep);
      center_use = Island.Out_Center ;
    }
    else//����ʧ�ܣ�ʹ��֮ǰ��������ĵ�
    {
      center_use = Island.Out_Center;
    }
    CenterlineToDiff(center_use);
  }
  return 0;
}


u8 Image_Island_Test(void)//��׽����  
{
  int ccd_start=CCD_START,ccd_end=CCD_END;  //ccdɨ�����10���յ�310   
  int Left_Count=0,Right_Count=0;//���Ҽ���Ϊ0
  int black_L[40],black_R[40];
  int Diff_L[39],Diff_R[39];//һ�ײ��
  int DDiff_L[38],DDiff_R[38];//���ײ��
  int   Liner_L_cnt  = 0,Liner_R_cnt  = 0;
  u8    Liner_L_flag = 0,Liner_R_flag = 0;
  u8    Impulse_L_Flag = 0,Impulse_R_Flag = 0;//һ�ײ���г��ֽ�Ծ
  u8 i = 0;
  u8 Quanbai_flag = 0;
  u8 Start_line = 0;
  
  Start_line = max_s16(&ROADY.Y[120],80);///////////////////////////////////////////////////
  Quanbai_flag = Find_All_White(Start_line);
  
  for(i=0;i<40;i++)//10��
  {
    Right_Count = ROAD.Right[i+Island.Image_Start_hang];
    if(Right_Count<ccd_end)//�������Ч��Χ��
    {
      black_R[i] = Right_Count;
    }
    else
    {
      black_R[i] = ccd_end;
    }
    Left_Count = ROAD.Left[i+Island.Image_Start_hang];
    if(Left_Count > ccd_start)
    {
      black_L[i] = Left_Count; 
    }
    else
    {
      black_L[i] = ccd_start;
    }
  }
  for(i=0;i<39;i++)
  {
    Diff_L[i] = black_L[i+1] - black_L[i];
    Diff_R[i] = black_R[i+1] - black_R[i];
  }
  for(i=0;i<38;i++)
  {
    DDiff_L[i] = Diff_L[i+1] - Diff_L[i];
    DDiff_R[i] = Diff_R[i+1] - Diff_R[i];
    if(Abs_(DDiff_L[i])<Liner_threshold
       &&(Abs_(Diff_L[i])>0
          ||Abs_(Diff_L[i+1])>0))
      Liner_L_cnt++;
    if(Abs_(DDiff_R[i])<Liner_threshold
       &&(Abs_(Diff_R[i])>0
          ||Abs_(Diff_R[i+1])>0))
      Liner_R_cnt++;
    if(DDiff_L[i]<-40
         &&Liner_L_cnt>i/2
           &&Liner_L_cnt>0
             )
    {
      Impulse_L_Flag=1;//���ֳ弤
    }
    if(DDiff_R[i]>40
         &&Liner_R_cnt>i/2
           &&Liner_R_cnt>0
             )
    {
      Impulse_R_Flag=1;
    }
  }
  if(Liner_L_cnt>29)Liner_L_flag = 1;
  if(Liner_R_cnt>29)Liner_R_flag = 1;
  
  if((Impulse_R_Flag&&Impulse_L_Flag)
  ||Quanbai_flag == 1)
    return 0;
  else if(Liner_L_flag&&Impulse_R_Flag==1)
  {
    return Right_Island_pre;
  }
  else if(Liner_R_flag&&Impulse_L_Flag==1)
  {
    return Left_Island_pre;
  }
  else 
    return 0;
}


int Stay2Out_test()
{
  u8 Far_Lie[30];
  static int Diff_Far_Lie[29];//һ�ײ��
  static int DDiff_Far_Lie[28];//���ײ��
  static int Edge_Hang[30];
  static int Diff_Hang[29];
  static int DDiff_Hang[28];
  
  int Liner_cnt  = 0;
  u8 Impulse_flag = 0;
  int Liner_H_cnt= 0;
  
  u8 i;
  if(Island.State==Right_Island_in)
  {
    for(i=0;i<30;i++)
    {
      Far_Lie[i] = ROADY.Y[CCD_START+i*4];
      Edge_Hang[i] = ROAD.Left[Y_START - 4 - i*2];
    }
    for(i=0;i<29;i++)
    {
      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
      Diff_Hang[i] = Edge_Hang[i+1] - Edge_Hang[i];
    }
    for(i=0;i<28;i++)
    {
      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
      DDiff_Hang[i] = Diff_Hang[i+1] - Diff_Hang[i];
      if(Abs_(DDiff_Far_Lie[i])<Liner_threshold)//���Ա�־
        Liner_cnt++;
      if(Abs_(DDiff_Hang[i])<Liner_threshold)
        Liner_H_cnt++;
      if(DDiff_Far_Lie[i]<-15
         &&Liner_cnt>i/3
           &&Liner_cnt>0)//֮ǰ���ԣ����ֳ弤
      {
        Impulse_flag = 1;
        break;
      }
      if(Diff_Hang[i] < -1
         &&Liner_H_cnt > i/2
           &&Liner_H_cnt>0
            &&Y_START - 4 - i*2 > ROADY.far+8)
      {
        Impulse_flag = 1;
        break;
      }
      
    }
  }
  else if(Island.State==Left_Island_in)
  {
    for(i=0;i<30;i++)
    {
      Far_Lie[i] = ROADY.Y[CCD_END-i*4];
      Edge_Hang[i] = ROAD.Right[Y_START - 4 - i*2];
    }
    for(i=0;i<29;i++)
    {
      Diff_Far_Lie[i] = Far_Lie[i+1] - Far_Lie[i];
      Diff_Hang[i] = Edge_Hang[i+1] - Edge_Hang[i];
    }
    for(i=0;i<28;i++)
    {
      DDiff_Far_Lie[i] = Diff_Far_Lie[i+1] - Diff_Far_Lie[i];
      DDiff_Hang[i] = Diff_Hang[i+1] - Diff_Hang[i];
      if(Abs_(DDiff_Far_Lie[i])<Liner_threshold)//���Ա�־
        Liner_cnt++;
      if(Abs_(DDiff_Hang[i])<Liner_threshold)
        Liner_H_cnt++;
      if(DDiff_Far_Lie[i]<-15
         &&Liner_cnt>i/3
           &&Liner_cnt>0)//֮ǰ���ԣ����ֳ弤
      {
        Impulse_flag = 1;
        break;
      }
      if(Diff_Hang[i] > 1
         &&Liner_H_cnt > i/2
           &&Liner_H_cnt>0
             &&Y_START - 4 - i*2 > ROADY.far+8)
      {
        Impulse_flag = 1;
        break;
      }
    }
  }
  return Impulse_flag;
}

int Wait_Next_Island()
{
  if(Island.State!=Left_Wait_Next&&Island.State!=Right_Wait_Next)//���ڴ�״̬��
    return 1;//ֱ�ӷ���
  
  CenterlineToDiff(get_Center(Main_Line-5));
  
  return 0;
}


u8 Out_Island_find_Quanbai(void)
{
  u8 i,cnt=0;
  u8 Start_line = 0;
  Start_line = max_s16(&ROADY.Y[120],80);
  for(i=0;i<20;i++)
  {
    if(Start_line+(i+1)*2 < Y_START
       &&sum_point(Image_fire[Start_line+(i+1)*2],40)<=5)
      cnt++;
  }
  if(cnt>5&&(ROADY.Three_lie_end[0]<140||ROADY.Three_lie_end[2]<140))//����û�м��
  {
    return 1;
  }
  else 
    return 0;
}

