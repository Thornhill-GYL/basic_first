#include "include.h"
#include "String.h"
#include "fsl_debug_console.h"
u8 Image_Flag=0;
u8 LCD_DISPLAY_FLAG = 0;
void Setup(void)
{
  //LCD_SetPos(0,319,0,239);
}

void Loop(void)
{
  u8 old_Memory_use_Flag = 0;

  if(Image_Flag==1)
  {
    old_Memory_use_Flag = Memory_use_Flag;
    Image_Flag=0;
    image_process();
    if(LCD_DISPLAY_FLAG==1)
    {
      Send_Image_to_LCD(Image_fire);
    }
    
    
    
    if(old_Memory_use_Flag != Memory_use_Flag)
    {
      if(Memory_use_Flag==1)//�ı仺����
      {
        Image_fire=&Image_fire_Memory2[0];
      }
      else if(Memory_use_Flag==2)
      {
        Image_fire=&Image_fire_Memory1[0];
      }
    }
    else                        //˵������ִ�нϿ죬��ʱ��һ���ĳ��жϻ�δ��
    {
      if(Memory_use_Flag==1)//�ı仺����
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