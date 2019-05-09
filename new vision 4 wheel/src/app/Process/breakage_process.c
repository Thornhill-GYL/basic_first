#include "include.h"
#include "breakage_process.h"
Break_Data break_info;
Angle_Data angle_info;
Back_Data  back_info;
Move_Data move_info;
int AD_in=0;
void break_data_init(void)//断路参数初始化
{
    break_info.middle_stable=160;
    break_info.middle_with_black=20;
    break_info.middle_with_white=15;
    break_info.sum_black=0;
    break_info.meeting_flag=0;
    break_info.sure_flag=0;
    break_info.sum_white=0;
    break_info.angle_flag=0;
    break_info.breakage_flag=0;
    break_info.black_flag=0;
    break_info.combine_flag=0;
    break_info.limit_flag=0;
    break_info.part_flag=0;
    break_info.angle_line=Main_Line-2;
    break_info.breakage_line=Main_Line-10;
}
int black_straight(int using_line)//判断断路上的黑色区域
{
    int temp_flag;
    break_info.sum_black=0;
    break_info.black_flag=0;
    //右边小范围检测
    for(int i=break_info.middle_stable;i<break_info.middle_with_black+break_info.middle_stable;i++)
    {
        if(IMG_PIXEL(i,using_line)==0)
            break;
        else if(IMG_PIXEL(i,using_line)>=1)
        {
            break_info.sum_black++;
        }
    }
    //左边小范围检测
    for(int i=break_info.middle_stable;i>break_info.middle_stable-break_info.middle_with_black;i--)
    {
        if(IMG_PIXEL(i,using_line)==0)
            break;
        else if(IMG_PIXEL(i,using_line)>=1)
        {
            break_info.sum_black++;
        }
    }
    //综合检测
    if( break_info.sum_black==(break_info.middle_with_black*2))
    {

        temp_flag=1;
    }
    else
    {
      break_info.sum_black=0;
        temp_flag=0;
    }


    return temp_flag;
}
int part_straight(int using_line)//直道白色部分检测
{
    int temp_flag=0;
    break_info.sum_white=0;
    break_info.part_flag=0;
    //右边部分检测白色
    for(int i=break_info.middle_stable;i<break_info.middle_stable+break_info.middle_with_white;i++)
    {
        if(IMG_PIXEL(i,using_line)>=1)
            break;
        else if(IMG_PIXEL(i,using_line)==0)
        {
            break_info.sum_white++;
        }

    }
    //左边部分检测白色
    for(int i=break_info.middle_stable;i>break_info.middle_stable-break_info.middle_with_white;i--)
    {
        if(IMG_PIXEL(i,using_line)>=1)
            break;
        else if(IMG_PIXEL(i,using_line)==0)
        {
            break_info.sum_white++;
        }
    }
    //综合检测
    if(break_info.sum_white==(break_info.middle_with_white*2))
    {
        temp_flag=1;
    }
    else
    {
         break_info.sum_white=0;
        temp_flag=0;
    }
    return temp_flag;
}
int limit_straight(int using_line)//直到限制检测
{
    int temp_flag=0;
    int left_side=0,right_side=0;
    int left_flag=0,right_flag=0;
    int right_gets=0,left_gets=0;
    int down_line=20;
    int left_straight;
    int right_straight;
    int sum_right=0;
    int sum_left=0;


    right_side=160;//右侧在找到
    while(!((IMG_PIXEL(right_side+3,using_line))
            &&(IMG_PIXEL(right_side+2,using_line))
            &&(IMG_PIXEL(right_side+1,using_line)))
            &&right_side<CCD_END)
        right_side++;
    if(right_side<CCD_END)
    {
        right_gets=right_side;
        right_flag=1;
    }
    else
        right_flag=0;
    if(right_flag==1)//需要修改一下；
    {//向下探测五行，从160开始往右边检测，将向下五行的右边界的值记录，进行比较判断，是否在直道）
        for(int i=using_line;i<using_line+down_line;i++)
        {
            if(IMG_PIXEL(right_gets,i)==0)
            {
                sum_right++;
            }
            else
                break;
        }
    }
    else
        sum_right=0;
    if(sum_right==20)
        right_straight=1;
    else
        right_straight=0;

    //左边直道检测
    left_side=160;
    while(!((IMG_PIXEL(left_side-3,using_line))
           &&(IMG_PIXEL(left_side-2,using_line))
           &&(IMG_PIXEL(left_side-1,using_line)))
           &&left_side>CCD_START)
        left_side--;
    if(left_side>CCD_START)
    {
        left_gets = left_side;
        left_flag = 1;
    }
    else
        left_flag=0;
    sum_left=0;
    if(left_flag==1)
    {
        for(int i=using_line;i<using_line+down_line;i++)
        {
            if(IMG_PIXEL(left_gets,i)==0)
            {
                sum_left++;
            }
            else
                break;
        }
    }
    else
        sum_left=0;
    if(sum_left==20)
        left_straight=1;
    else
        left_straight=0;
    //综合检测
    if(right_straight==1&&left_straight==1)
        temp_flag=1;//全局变量
    else
        temp_flag=0;

    return temp_flag;
}
int angle_count(int using_line)
{
   angle_info.status_time=0;
   angle_info.sum_white=0;
   angle_info.sum_black=0;
   for(int i=0;i<300;i++)
   {
       angle_info.dot_color[i]=1;
   }
    for(int i=CCD_START;i<CCD_END;i++)
    {
        if(IMG_PIXEL(i,using_line)==0)
        {

            angle_info.sum_white++;
            angle_info.dot_color[i]=0;
        }
        else
        {
            angle_info.sum_black++;
        }
    }
    for(int i=0;i<300;i++)
    {
        if(angle_info.dot_color[i]==1&&angle_info.status_time==0)
        {
            angle_info.status_time=1;
        }
        else if(angle_info.status_time==1&&angle_info.dot_color[i]==0)
        {
            angle_info.status_time=2;
        }
        else if(angle_info.status_time==2&&angle_info.dot_color[i]==1)
        {
            angle_info.status_time=3;
        }
        else if(angle_info.status_time==3&&angle_info.dot_color[i]==0)
        {
            angle_info.status_time=4;
        }
        else if(angle_info.status_time==4&&angle_info.dot_color[i]==1)
        {
            angle_info.status_time=5;
        }
    }
    if(angle_info.status_time!=3)
        angle_info.sum_white=0;

    return angle_info.sum_white;
}
int angle_breakage(int using_line)
{
    int left_start=CCD_START;
    int right_start=CCD_END;
    angle_info.decrease=0;
    angle_info.maybe=0;
    angle_info.pre_left_may=0;
    angle_info.pre_right_may=0;
    for(int i=0;i<5;i++)
    {
        //检测左边的边界
        while ((IMG_PIXEL(left_start + 3, using_line - i) != 0)
               && (IMG_PIXEL(left_start + 2, using_line - i) != 0)
               && (IMG_PIXEL(left_start + 1, using_line - i) != 0)
               && (left_start < CCD_END))
            left_start++;
        if ((left_start < CCD_END) && (left_start != CCD_START)) {
            angle_info.left_line[i][0] = using_line - i;
            angle_info.left_line[i][1] = left_start;
        } else {
            angle_info.left_line[i][0] = 0;
            angle_info.left_line[i][1] = 0;
        }
        //检测右边的边界
        while ((IMG_PIXEL(right_start - 3, using_line - i) != 0)
               && (IMG_PIXEL(right_start - 2, using_line - i) != 0)
               && (IMG_PIXEL(right_start - 1, using_line - i) != 0)
               && (right_start > CCD_START))
            right_start--;
        if ((right_start > CCD_START) && (right_start != CCD_END)) {
            angle_info.right_line[i][0] = using_line - i;
            angle_info.right_line[i][1] = right_start;
        } else {
            angle_info.right_line[i][0] = 0;
            angle_info.right_line[i][1] = 0;
        }
    }
    angle_info.left_standard=angle_info.left_line[1][1]-angle_info.left_line[0][1];
    angle_info.right_standard=angle_info.right_line[1][1]-angle_info.right_line[0][1];
    for(int i=0;i<4;i++)
    {
        angle_info.left_delat_line[i]= angle_info.left_line[i+1][1]- angle_info.left_line[i][1];
        angle_info.right_delat_line[i]= angle_info.right_line[i+1][1]- angle_info.right_line[i][1];
    }
    for(int i=0;i<4;i++)
    {
        if(angle_info.left_delat_line[i]<0||angle_info.right_delat_line[i]>0)
        {
            angle_info.maybe=0;
            break;
        }
        //判断左侧是否有直线
        if(angle_info.left_standard!=0)
        {
            if(angle_info.left_delat_line[i]==angle_info.left_standard)
                angle_info.pre_left_may=1;
            else if(angle_info.left_delat_line[i]==angle_info.left_standard+1)
                angle_info.pre_left_may=1;
            else if(angle_info.left_delat_line[i]==angle_info.left_standard-1)
                angle_info.pre_left_may=1;
            else
                angle_info.pre_left_may=0;
        }
        else
        {
            angle_info.maybe=0;
            break;
        }
        //判断右边是否有直线
        if(angle_info.right_standard!=0)
        {
            if(angle_info.right_delat_line[i]==angle_info.right_standard)
                angle_info.pre_right_may=1;
            else if(angle_info.right_delat_line[i]==angle_info.right_standard-1)
                angle_info.pre_right_may=1;
            else if(angle_info.right_delat_line[i]==angle_info.right_standard+1)
                angle_info.pre_right_may=1;
            else
                angle_info.pre_right_may=0;
        }
        else
        {
            angle_info.maybe=0;
            break;
        }


    }

    for(int i=0;i<3;i++)
    {
        angle_info.line_count[i]=angle_count(using_line-i);
    }
    for(int i=0;i<3;i++)
    {
        if((angle_info.line_count[i]!=0)&&(angle_info.line_count[i]<70))
            angle_info.decrease=1;
        else
            angle_info.decrease=0;
    }
    if(angle_info.decrease==1&&angle_info.pre_right_may==1&&angle_info.pre_left_may==1)
    {
        angle_info.maybe=1;
    }
    else
        angle_info.maybe=0;

    if(angle_info.maybe==1)
    {
        if((angle_info.line_count[0]>angle_info.line_count[1])&&(angle_info.line_count[1]>angle_info.line_count[2]))
        {
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}
int combine_straight(int using_line)
{
    int temp_flag=0;
    break_info.part_flag=part_straight(using_line);
    break_info.limit_flag=limit_straight(using_line);
   if(break_info.part_flag==1&&break_info.limit_flag==1)
       temp_flag=1;
   else
       temp_flag=0;
   return temp_flag;
}
int normal_breakage(int using_line)//在正常的道路上判断断路
{
    int temp_flag;
    break_info.combine_flag=combine_straight(using_line+10);
    break_info.black_flag=black_straight(using_line);
    if(break_info.black_flag==1&&break_info.combine_flag==1)
        temp_flag=1;
    else
        temp_flag=0;
    return temp_flag;
}
void back_straight(int using_line)
{

    back_info.back_part=part_straight(using_line);
    back_info.back_straight=combine_straight(using_line);
    //back_info.back_limit=limit_straight(using_line);
    if(back_info.back_part==1&&(ROADY.Y[160]<(Y_START-20))&&back_info.back_straight==1)
       back_info.back_sure=1;
    else
        back_info.back_sure=0;
    if(back_info.back_sure==1)//（这里需要添加通讯信息和ad检测信息)
    {
        break_info.meeting_flag = 1;
        break_info.breakage_flag=0;
        Dir.Dir_mode=Camera_Mode;
    }

}
void pattern_shift(void)
{
    break_info.breakage_flag=normal_breakage(break_info.breakage_line);
    break_info.angle_flag=angle_breakage(break_info.angle_line);
    if(start_process==1&&break_info.sure_flag==0)
    {

        if(break_info.breakage_flag==1)
        {
            break_info.sure_flag=1;
        }
        if(break_info.angle_flag==1)
        {
            break_info.sure_flag=1;
        }
    }//判断是否改变循迹模式
    if(break_info.sure_flag==1)
    {
        Dir.Dir_mode=Elec_Mode;
        if(((SWITCH_STATUS>>1)&1)==1)
     {
       back_straight(break_info.breakage_line-5);
     }
     else if(((SWITCH_STATUS>>1)&1)==0)
     {
       if(move_info.normal_flag==1)
       {
           back_straight(break_info.breakage_line-5);
       }
     }
        
      
    }
    else
    {
        Dir.Dir_mode=Camera_Mode;
    }


}
void detect_led(void)
{
  if(start_process==1)
  {
    if(break_info.breakage_flag==1)
    {
      LED_BLUE;
    }
    else if(break_info.angle_flag==1)
    {
      LED_YELLOW;
    }

  }
}
void single_control(void)
{
   if(Dir.Dir_mode==Elec_Mode)
  {
    Speed.using_speed=60;
  }
  else if(Dir.Dir_mode==Camera_Mode)
    Speed.using_speed=Initialization.Speedtarget;
}
void double_control(void)
{
  breakage_move();
}
void breakage_move(void)
{
  
  static unsigned int Timestop=0;
  static unsigned int Timeback=0;
  //在无通信下的回来
  
    if(break_info.sure_flag==1)
    {
      ad_breakage();
      if(move_info.start_out_flag==1)
      {
        Timestop++;
        move_info.quit_safe=1;
        move_info.Dist_L+=Timestop*Speed.Speed_L;
        move_info.Dist_R+=Timestop*Speed.Speed_R;
        move_info.aver_D=(int)((move_info.Dist_L+move_info.Dist_R)/2);
      }
     if(move_info.aver_D>=400000&&move_info.go_back_flag==0)
      {
        move_info.diff_flag=1;
        move_info.start_out_flag=0;
        move_info.get_Dist_L=move_info.Dist_L;
        move_info.get_Dist_R=move_info.Dist_R;
        Speed.using_speed=0;
      }
      else if(move_info.go_back_flag==1&&move_info.normal_flag==0)
      {
        Timeback++;
        Speed.using_speed=-60;
        move_info.get_Dist_L+=Timeback*Speed.Speed_L;
        move_info.get_Dist_R+=Timeback*Speed.Speed_R;
       if(move_info.get_Dist_L<=0&&move_info.get_Dist_R<=0)
        {
          move_info.normal_flag=1;
          move_info.get_Dist_L=0;
          move_info.get_Dist_R=0;
          move_info.Dist_L=0;
          move_info.Dist_R=0;
        }
      }
     else if(move_info.normal_flag==1)
      {
          single_control();
          move_info.quit_safe=0;
//          back_straight(break_info.breakage_line-5);
      }
      else
        single_control();
     
     
     
    }
    else
      single_control();
}
void ad_breakage(void)
{
    int ad_detect=0;
    ad_detect=ADC0_Once(ADC0_SE5a,ADC_12bit);
    if(ad_detect<5)
        ad_detect=5;
    if(ad_detect>4050)
        ad_detect=4050;
    if(ad_detect>2800)
    {
        AD_in=1;
    }
}