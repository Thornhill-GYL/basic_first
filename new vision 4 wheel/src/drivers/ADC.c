#include "include.h"

//-------------------------------------------------------------------------*
//函数名: ADC_init                                                         *
//功  能: 初始化ADC                                                        * 
//参  数: adc_n:模块名ADC0或HSADC1                                         *
//返  回: 无                                                               *
//简  例: ADC_init（ADC0）初始化ADC0模块                                   *
//-------------------------------------------------------------------------*
void ADC0_Init(void)
{          
  SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //开启ADC0时钟
  SIM_SOPT7 &= ~(SIM_SOPT7_HSADC0AALTTRGEN_MASK  | SIM_SOPT7_HSADC0AALTTRGEN_MASK);
  SIM_SOPT7 |= SIM_SOPT7_HSADC0ATRGSEL(0);    
 }

//-------------------------------------------------------------------------*
//函数名: adc_start                                                        *
//功  能: 启动ADC转换                                                      * 
//参  数: adc_n:模块名ADC0或HSADC1                                         *
//        adc_ch:通道编号                                                  *
//        bit:精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit            * 
//返  回: 无                                                               *
//简  例: 一般由adc_once函数调用，不单独使用                               *
//-------------------------------------------------------------------------*
void ADC0_Start(ADC0_Ch_e adc_ch,ADC0_nbit bit)
{    
     int ch=0;
     
     ADC0_CFG1 = (0  | ADC_CFG1_ADIV(2)           //时钟分频选择,分频系数为 2^n,2bit
                     | ADC_CFG1_ADLSMP_MASK       //采样时间配置，0为短采样时间，1 为长采样时间
                     | ADC_CFG1_MODE(bit)         //精确度选择
                     | ADC_CFG1_ADICLK(0)         //0为总线时钟,1为总线时钟/2,2为交替时钟（ALTCLK），3为 异步时钟（ADACK）。
                 );

    ADC0_CFG2  = (0  | ADC_CFG2_ADHSC_MASK        //高速配置,0为正常转换序列，1为高速转换序列
                     | ADC_CFG2_ADLSTS(0)         //长采样时间选择，ADCK为4+n个额外循环，额外循环，0为20，1为12，2为6，3为2
                  );
    
    if((adc_ch>11)&&(adc_ch<16))
    {
      ch=adc_ch-8;
      ADC0_CFG2 |=ADC_CFG2_MUXSEL_MASK;           // 1选择b  1 ADxxb channels are selected.
    }     
    else
    {
      ch=adc_ch;
      ADC0_CFG2 &=~ADC_CFG2_MUXSEL_MASK;          // 0选择a  0 ADxxa channels are selected.
    }
    //写入 SC1A 启动转换
    ADC0_SC1(0) = (0 | ADC_SC1_AIEN_MASK          // 转换完成中断,0为禁止，1为使能
                     //| ADC_SC1_ADCH( 0x0c )     //ADC0 通道12
                     | ADC_SC1_ADCH(ch )          //ADC0 通道13
                  );
}

//-------------------------------------------------------------------------*
//函数名: adc_once                                                        
//功  能: 进行ADC单次转换                                                   
//参  数: adc_n:模块名ADC0或HSADC1                                           
//        adc_ch:通道编号                                                  
//        bit:精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit             
//返  回: result                                                              
//简  例: adc_once(ADC0,HSADC1_SE8,ADC_12bit) 通道编号为 HSADC1_SE8的引脚，进行
//          一次ADC转换。HSADC1_SE8对应引脚请查看ADC.H文件                    
//-------------------------------------------------------------------------*
u16 ADC0_Once(ADC0_Ch_e adc_ch,ADC0_nbit bit) //采集某路模拟量的AD值
{   
  int result = 0;
  
  ADC0_Start(adc_ch,bit);      //启动ADC转换
  while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK )!= ADC_SC1_COCO_MASK);   //只支持 AB通道
  result = ADC0_R(0);
  ADC0_SC1(0) &= ~ADC_SC1_COCO_MASK;
  return result;  
}

//-------------------------------------------------------------------------*
//函数名: adc_ave                                                        
//功  能: 进行ADC多次转换，取平均值                                                   
//参  数: adc_n:模块名ADC0或HSADC1                                           
//        adc_ch:通道编号                                                  
//        bit:精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit 
//        N  :均值滤波次数（1~65535）
//返  回: tmp                                                              
//简  例: adc_ave(ADC0,HSADC1_SE8,ADC_12bit,100) 通道编号为 HSADC1_SE8的引脚，进行
//          100次ADC转换，计算出平均值返回。                   
//-------------------------------------------------------------------------*
u16 ADC0_Ave(ADC0_Ch_e adc_ch,ADC0_nbit bit,u16 N) //均值滤波
{
    u32 tmp = 0;
    u8  i;
    
    for(i = 0; i < N; i++)
       tmp += ADC0_Once(adc_ch,bit); 
    
    tmp = tmp / N; 
    
    return (u16)tmp;
}