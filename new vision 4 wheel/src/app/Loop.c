#include "include.h"
#include "String.h"
#include "fsl_debug_console.h"
u8 Image_Flag=0;
u8 LCD_DISPLAY_FLAG = 0;
int start_process=0;
void Setup(void)
{
  //LCD_SetPos(0,319,0,239);
}

void Loop(void)
{
  u8 old_Memory_use_Flag = 0;
  unsigned char buf[40];
  detect_led();//检测断路的灯
//  start_led();
  if(ramp_flag==1)
  {
    LED_BLUE;
  }
  if(Image_Flag==1)
  {
    old_Memory_use_Flag = Memory_use_Flag;
    Image_Flag=0;
    OV7725_cnt++;
    image_process();
    if(LCD_DISPLAY_FLAG==1)
    {
      Send_Image_to_LCD(Image_fire);
//       Draw_single_line('H',break_info.breakage_line-IMAGE_CORRECT,COLOR_BLUE);
//       Draw_single_line('H',break_info.breakage_line+5-IMAGE_CORRECT,COLOR_YELLOW);
//       Draw_single_line('H',break_info.angle_line-IMAGE_CORRECT,COLOR_RED);
       //Draw_single_line('H',start_info.detect_line-IMAGE_CORRECT,COLOR_RED);
        Draw_single_line('H',Y_END+30-IMAGE_CORRECT,COLOR_BLUE);
        Draw_single_line('H',Y_START-10-IMAGE_CORRECT,COLOR_RED);
         Draw_single_line('H',Main_Line-5-IMAGE_CORRECT,COLOR_RED);
         Draw_single_line('L',CCD_START,COLOR_BLUE);
        Draw_single_line('L',CCD_END,COLOR_RED);
         Draw_single_line('L',153,COLOR_RED);
       sprintf((char *)buf,"test_delat=%d   ramp_sum =%d ",test_delat,ramp_sum);
       Gui_DrawFont_GBK24(10,150, COLOR_BLACK,COLOR_WHITE,buf);
       sprintf((char *)buf,"black_flag=%d straight_flag=%d  ",break_info.black_flag,break_info.combine_flag);
       Gui_DrawFont_GBK24(10,170, COLOR_BLACK,COLOR_WHITE,buf);
       sprintf((char *)buf,"angle_flag=%d  ",break_info.angle_flag);
       Gui_DrawFont_GBK24(10,190, COLOR_BLACK,COLOR_WHITE,buf);
//       sprintf((char *)buf,"far_x=%d   far_y=%d    ",scan_info.far_x,scan_info.far_y);
//       Gui_DrawFont_GBK24(10,210, COLOR_BLACK,COLOR_WHITE,buf);
      sprintf((char *)buf,"tof_distance=%d      ",TOF_Distance);
       Gui_DrawFont_GBK24(10,210, COLOR_BLACK,COLOR_WHITE,buf);
         
    }
    
    
    
    if(old_Memory_use_Flag != Memory_use_Flag)
    {
      if(Memory_use_Flag==1)//改变缓冲区
      {
        Image_fire=&Image_fire_Memory2[0];
      }
      else if(Memory_use_Flag==2)
      {
        Image_fire=&Image_fire_Memory1[0];
      }
    }
    else                        //说明程序执行较快，此时下一场的场中断还未到
    {
      if(Memory_use_Flag==1)//改变缓冲区
      {
        Image_fire=&Image_fire_Memory1[0];
      }
      else if(Memory_use_Flag==2)
      {
        Image_fire=&Image_fire_Memory2[0];
      }
    }
  }

        
}