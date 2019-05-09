#include "start_line.h"
#include "include.h"
Start_Data start_info;
void start_init(void)
{
    start_info.detect_line=150;
    start_info.confirm_line=195;
}
int detect_startline(int using_line) {
    int start_flag = 0;
    int i = 0, j = 0;
    int size_flag = 0;
    j = 0;
    for (i = CCD_START; i < CCD_END; i++) {
        if (j < 260) {
            if (IMG_PIXEL(i, using_line) >= 1) {
                start_info.black_white[j] = 1;
            } else
                start_info.black_white[j] = 0;
        }
        j++;
    }
    for (i = 0; i < 20; i++) {
        start_info.continuous[i] = 0;//每次进入都清零
    }
    start_info.color_flag = start_info.black_white[0];
    start_info.status = 0;//每次进入函数状态清零;
    for (i = 1; i < 260; i++) {
        if (start_info.status >= 40)//防止数组越界
        {
            break;
        }
        if (start_info.black_white[i] == start_info.black_white[i - 1]) {
            start_info.continuous[start_info.status]++;
        } else if (start_info.black_white[i] != start_info.black_white[i - 1]) {
            start_info.status += 1;
        }
    }
    size_flag = 0;//记录每个颜色带长度大于一定值时的数目
    for (i = 0; i < 40; i++) {
        if (start_info.continuous[i] > 4000) {
            start_info.continuous[i] = 4000;//限幅，防止越界
        }
        if (start_info.continuous[i] >= 5) {
            size_flag += 1;
        }
    }
    if (start_info.status >= 15 && size_flag >= 15) {
        start_flag = 1;
    } else
        start_flag = 0;
    return start_flag;
}
void start_operate(void)
{
    start_info.detect_flag=0;
    start_info.confirm_flag=0;
    start_info.detect_flag=detect_startline(start_info.detect_line);
    start_info.confirm_flag=detect_startline(start_info.confirm_line);
    if(start_info.detect_flag==1)
    {
        start_info.stop_count=1;
        start_info.final_stop=1;
    }
     if(((SWITCH_STATUS>>1)&1)==0)
     {
          if(start_info.confirm_flag==1)
       {
        start_info.stop_count=2;
       }
     }
   
}
void double_start_move(void)
{
    if(start_info.stop_count==1)
    {
        Dir.Dir_mode=Elec_Mode;
        Speed.using_speed=40;
    }
    else if(start_info.stop_count==2)
    {
        if(start_info.twice_stop==0||start_info.final_stop==2)
        {
            Speed.using_speed=0;
        }
        else if(start_info.twice_stop==1&&start_info.final_stop==1)
        {
            Speed.using_speed=45;
            start_info.dismove+=(int)((Speed.Speed_L+Speed.Speed_R)/2);
            if(start_info.dismove>=5845)
            {
                start_info.final_stop=2;
            }
        }
    }
}
void single_start_move(void)
{
     static unsigned int TimeCnt_stop=0;
    if(start_info.final_stop==1) 
    {
        if (start_info.stop_count == 1)
        {
            TimeCnt_stop++;
            Dir.Dir_mode=Elec_Mode;
            Speed.using_speed = 40;
        }
        if (TimeCnt_stop >= 300) 
        {
            TimeCnt_stop = 0;
            Speed.using_speed = 0;
            start_info.final_stop = 2;
        }
    }
        else if(start_info.final_stop==2)
        {
            Speed.using_speed=0;
        }
}
void start_led(void)
{
  if(start_info.stop_count!=0)
  {
    LED_GREEN;
  }
    
}
  
