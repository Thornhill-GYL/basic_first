#include "mpu9250.h"
#include "include.h"

#define USE_HARDWARE_SPI    (1)

float Kp =1.25f ;		//比例增益
float Ki = 0.001f;		//积分增益
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;		//积分误差累积

static float q0 = 1.0f;	        //四元数
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	

CarThreeNum  Acc_ADC_Data={0,0,0};
CarThreeNum  Acc_ADC={0,0,0};
CarThreeNum  Gyro_ADC={0,0,0};

int Ave_Cnt = 0;
s32 mpu9250_Gyro_x_ave = 40,mpu9250_Gyro_y_ave = 9,mpu9250_Gyro_z_ave = 6;
CarAngle Angle={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,{0},{0},{0},0,0,0,0,0,0,0};
CarGyro Gyro={0,0,0,0,0,0};

float Yoll,Pitch,Yaw;
float Correct_gyro_y;

//--------------------- 
//作者：dyl74500196 
//来源：CSDN 
//原文：https://blog.csdn.net/dyl74500196/article/details/59108807 
//版权声明：本文为博主原创文章，转载请附上博文链接！

unsigned char mpu9250_buffer[14];
s16 mpu9250_Acc_x,mpu9250_Acc_y,mpu9250_Acc_z;
s16 mpu9250_Gyro_x,mpu9250_Gyro_y,mpu9250_Gyro_z;
//short mpu9250_Tempreature;

float mpu9250_Acc_x_ms2,mpu9250_Acc_y_ms2,mpu9250_Acc_z_ms2;//×a?a?×???t′?·???
float mpu9250_Gyro_x_as,mpu9250_Gyro_y_as,mpu9250_Gyro_z_as;//×a?a???è????

unsigned char outputbuf[6] = {0};
unsigned char inputbuf[6] = {0};


void MPU9250_PORT_init()
{
//  GPIO_Init(PORTE,4, GPO, LOW);//cs
#if USE_HARDWARE_SPI
  spi_init(MPU_SPIn,MPU_CS_PCSn,MASTER,1000000);//12500*1000);
#else
  GPIO_Init(MPU_SDI_PORT, GPO, HIGH);//sda
  GPIO_Init(MPU_SCL_PORT, GPO, HIGH);//scl
  GPIO_Init(MPU_SDO_PORT, GPI, HIGH);
#endif
}
void  mpu_write_date(unsigned char address,unsigned char  c)
{
  address &= MPU_WRITE_BIT;

#if USE_HARDWARE_SPI
//  mpu_cs = 0;
  outputbuf[0] = address;
  outputbuf[1] = c;
  spi_mosi(MPU_SPIn,MPU_CS_PCSn,outputbuf,inputbuf,2);
//  mpu_cs = 1;
#else  
  spi_date_trans(address);
  spi_date_trans(c);
#endif
}


char  mpu_read_data(unsigned char address)
{
  address |= MPU_READ_BIT;
  outputbuf[0] = address;
  outputbuf[1] = 0xff;
#if USE_HARDWARE_SPI
//  mpu_cs = 0;
  spi_mosi(MPU_SPIn,MPU_CS_PCSn,outputbuf,inputbuf,2);

//  mpu_cs = 1;
#else
  spi_date_trans(address);
  date = spi_date_trans(NULL);
#endif
  return inputbuf[1];
}

void  mpu_read_str(unsigned char address,unsigned char *str,int len)
{
  address |= MPU_READ_BIT;
#if USE_HARDWARE_SPI
//  mpu_cs = 0;
  spi_mosi(MPU_SPIn,MPU_CS_PCSn,&address,NULL,1);
  for(int i=0;i<len;i++)
    spi_mosi(MPU_SPIn,MPU_CS_PCSn,NULL,&str[i],1);
//  mpu_cs = 1;
#else
  spi_date_trans(address);
  for(int i=0;i<len;i++)
    str[i] = spi_date_trans(NULL);
#endif
}


void Init_MPU9250(void)
{
//  u8 who_am_i = 0;
//  u8 date[6];
  MPU9250_PORT_init();
  mpu_write_date(PWR_MGMT_1, 0x80);
  delayms(30);
  mpu_write_date(SMPLRT_DIV, 0x00);
  mpu_write_date(PWR_MGMT_1, 0x03);
//  mpu_write_date(INT_PIN_CFG,1<<1);
  mpu_write_date(CONFIG, 0x03);
  mpu_write_date(GYRO_CONFIG, 0x18);//满量程2000DPS
  mpu_write_date(ACCEL_CONFIG1, 2<<3);//满量程8g
//  while(who_am_i!=115)
//  {
//    who_am_i = mpu_read_data(WHO_AM_I);
//    date[0] = mpu_read_data(PWR_MGMT_1);
//    date[1] = mpu_read_data(SMPLRT_DIV);
//    date[2] = mpu_read_data(CONFIG);
//    date[3] = mpu_read_data(GYRO_CONFIG);
//    date[4] = mpu_read_data(ACCEL_CONFIG1);
//    date[5] = mpu_read_data(ODR_CONTROL);
//    delayms(50);
//  }
}


void READ_DATE(void)
{
    for(int i=0;i<14;i++)
  {
    mpu9250_buffer[i] = mpu_read_data(ACCEL_XOUT_H+i);
  }
  mpu9250_Acc_x = ((((int16_t)mpu9250_buffer[0]) << 8) | mpu9250_buffer[1]) ;
  mpu9250_Acc_y = ((((int16_t)mpu9250_buffer[2]) << 8) | mpu9250_buffer[3]) ;
  mpu9250_Acc_z = ((((int16_t)mpu9250_buffer[4]) << 8) | mpu9250_buffer[5]) ;
  mpu9250_Gyro_x = ((((int16_t)mpu9250_buffer[ 8]) << 8) | mpu9250_buffer[ 9]) ;
  mpu9250_Gyro_y = ((((int16_t)mpu9250_buffer[10]) << 8) | mpu9250_buffer[11]) ;
  mpu9250_Gyro_z = ((((int16_t)mpu9250_buffer[12]) << 8) | mpu9250_buffer[13]) ;
  
//  if(Ave_Cnt>Ave_Cnt_const)
//  {
    mpu9250_Gyro_x=mpu9250_Gyro_x-mpu9250_Gyro_x_ave;
    mpu9250_Gyro_y=mpu9250_Gyro_y-mpu9250_Gyro_y_ave;
    mpu9250_Gyro_z=mpu9250_Gyro_z-mpu9250_Gyro_z_ave;//调零
//  }
  
  mpu9250_Gyro_x_as=(float)(((float)mpu9250_Gyro_x)*0.001065f);         //0.06103515625     4000/65536         
  mpu9250_Gyro_y_as=(float)(((float)mpu9250_Gyro_y)*0.001065f);
  mpu9250_Gyro_z_as=(float)(((float)mpu9250_Gyro_z)*0.001065f);//转化为角度每秒         /164    
  Acc_ADC.x = (float)mpu9250_Acc_x;
  Acc_ADC.y = (float)mpu9250_Acc_y;
  Acc_ADC.z = (float)mpu9250_Acc_z;
  Gyro_ADC.x = (float)mpu9250_Gyro_x;
  Gyro_ADC.y = (float)mpu9250_Gyro_y;
  Gyro_ADC.z = (float)mpu9250_Gyro_z;
  
//  mpu9250_Acc_x_ms2=Ave_filter((float)(mpu9250_Acc_x*0.00239285f),&Acc_x_filter);///16.4);      //0.0023928466796875     8/32768*9.8011   
//  mpu9250_Acc_y_ms2=Ave_filter((float)(mpu9250_Acc_y*0.00239285f),&Acc_y_filter);///16.4);
//  mpu9250_Acc_z_ms2=Ave_filter((float)(mpu9250_Acc_z*0.00239285f),&Acc_z_filter);///16.4);//转化为米每二次方秒
  
//  mpu9250_Acc_x_ms2=(float)(mpu9250_Acc_x*0.00239285f);///16.4);      //0.0023928466796875     8/32768*9.8011   
//  mpu9250_Acc_y_ms2=(float)(mpu9250_Acc_y*0.00239285f);///16.4);
//  mpu9250_Acc_z_ms2=(float)(mpu9250_Acc_z*0.00239285f);///16.4);//转化为米每二次方秒
//  
//  mpu9250_Gyro_x_as=(float)(((float)mpu9250_Gyro_x)*0.001065f);         //0.06103515625     4000/65536         
//  mpu9250_Gyro_y_as=(float)(((float)mpu9250_Gyro_y)*0.001065f);
//  mpu9250_Gyro_z_as=(float)(((float)mpu9250_Gyro_z)*0.001065f);//转化为弧度每秒         /164
  
//  imuUpdate(
//            mpu9250_Gyro_x_as,
//            mpu9250_Gyro_y_as,
//            mpu9250_Gyro_z_as,
//            mpu9250_Acc_x_ms2,
//            mpu9250_Acc_y_ms2,
//            mpu9250_Acc_z_ms2,
//            dt,
//            &Yoll,
//            &Pitch,
//            &Yaw);
}


//void READ_DATE(void)
//{
//  for(int i=0;i<14;i++)
//  {
//    mpu9250_buffer[i] = mpu_read_data(ACCEL_XOUT_H+i);
//  }
//  mpu9250_Acc_x = ((((int16_t)mpu9250_buffer[0]) << 8) | mpu9250_buffer[1]) ;
//  mpu9250_Acc_y = ((((int16_t)mpu9250_buffer[2]) << 8) | mpu9250_buffer[3]) ;
//  mpu9250_Acc_z = ((((int16_t)mpu9250_buffer[4]) << 8) | mpu9250_buffer[5]) ;
//  mpu9250_Gyro_x = ((((int16_t)mpu9250_buffer[ 8]) << 8) | mpu9250_buffer[ 9]) ;
//  mpu9250_Gyro_y = ((((int16_t)mpu9250_buffer[10]) << 8) | mpu9250_buffer[11]) ;
//  mpu9250_Gyro_z = ((((int16_t)mpu9250_buffer[12]) << 8) | mpu9250_buffer[13]) ;
//  
////  if(Ave_Cnt>Ave_Cnt_const)
////  {
//    mpu9250_Gyro_x=mpu9250_Gyro_x-mpu9250_Gyro_x_ave;
//    mpu9250_Gyro_y=mpu9250_Gyro_y-mpu9250_Gyro_y_ave;
//    mpu9250_Gyro_z=mpu9250_Gyro_z-mpu9250_Gyro_z_ave;//调零
////  }
//  
//  mpu9250_Gyro_x_as=(float)(((float)mpu9250_Gyro_x)*0.001065f);         //0.06103515625     4000/65536         
//  mpu9250_Gyro_y_as=(float)(((float)mpu9250_Gyro_y)*0.001065f);
//  mpu9250_Gyro_z_as=(float)(((float)mpu9250_Gyro_z)*0.001065f);//转化为角度每秒         /164    
//  Acc_ADC.x = (float)mpu9250_Acc_x;
//  Acc_ADC.y = (float)mpu9250_Acc_y;
//  Acc_ADC.z = (float)mpu9250_Acc_z;
//  Gyro_ADC.x = (float)mpu9250_Gyro_x;
//  Gyro_ADC.y = (float)mpu9250_Gyro_y;
//  Gyro_ADC.z = (float)mpu9250_Gyro_z;
//  
////  if(Ave_Cnt<Ave_Cnt_const)
////  {
////    mpu9250_Gyro_x_ave += mpu9250_Gyro_x;
////    mpu9250_Gyro_y_ave += mpu9250_Gyro_y;
////    mpu9250_Gyro_z_ave += mpu9250_Gyro_z;
////    Ave_Cnt++;
////  }
////  else if(Ave_Cnt==Ave_Cnt_const)
////  {
////    Ave_Cnt++;
////    mpu9250_Gyro_x_ave/=Ave_Cnt_const;
////    mpu9250_Gyro_y_ave/=Ave_Cnt_const;
////    mpu9250_Gyro_z_ave/=Ave_Cnt_const;
////  }
//}

u8 spi_date_trans(u8 out)
{
  u8 in = 0;
  mpu_cs = 0;
  for(int i=0;i<8;i++)
  {
    mpu_scl = 0;
    in = in << 1;
    mpu_sdi = (out>>(7-i)&1);
    mpudelay();
    mpu_scl = 1;
    mpudelay();
    in |= mpu_sdo;
  }
  mpu_cs = 1;
  return in;
}

void  mpudelay(void)
{
    int us = 1000;
    uint32 j;
    for(j = (int)(bus_clk_khz*us/10000.0); j > 0; j--)
    {
      asm("nop");
    }
}


void MPU_selftest(void)
{

    for(int i=0;i<14;i++)
    {
      mpu9250_buffer[i] = mpu_read_data(ACCEL_XOUT_H+i);
    }
    mpu9250_Gyro_x = ((((int16_t)mpu9250_buffer[ 8]) << 8) | mpu9250_buffer[ 9]) ;
    mpu9250_Gyro_y = ((((int16_t)mpu9250_buffer[10]) << 8) | mpu9250_buffer[11]) ;
    mpu9250_Gyro_z = ((((int16_t)mpu9250_buffer[12]) << 8) | mpu9250_buffer[13]) ;
    
    mpu9250_Gyro_x_ave += mpu9250_Gyro_x;
    mpu9250_Gyro_y_ave += mpu9250_Gyro_y;
    mpu9250_Gyro_z_ave += mpu9250_Gyro_z;

}


//四元数解算
//数据融合，互补滤波
//gx,gy,gz单位为弧度每秒
//ax,ay,az单位为米每二次方秒
//dt为周期，单位为ms

void imuUpdate(float gx, float gy, float gz, float ax, float ay, float az, float dt_,float *roll,float *pitch,float *yaw)
{
	float normalise;
	float ex, ey, ez;
	float q0s, q1s, q2s, q3s;	//四元数的平方
	static float R11,R21;		//矩阵(1,1)(2,1)项
	static float vecxZ, vecyZ, veczZ;	//机体坐标系下的z方向向量
	float halfT =0.5f * dt;
	


	//某一个方向加速度不为0
	if((ax != 0.0f) || (ay != 0.0f) || (az != 0.0f))
	{
		//单位化加速度计测量值
		normalise = myInvSqrt(ax * ax + ay * ay + az * az);
		ax *= normalise;
		ay *= normalise;
		az *= normalise;

		//加速度计读取的方向与重力加速计方向的差值，用向量叉乘计算
		ex = (ay * veczZ - az * vecyZ);
		ey = (az * vecxZ - ax * veczZ);
		ez = (ax * vecyZ - ay * vecxZ);
		
		//误差累计，与积分常数相乘
		exInt += Ki * ex * dt ;  
		eyInt += Ki * ey * dt ;
		ezInt += Ki * ez * dt ;
		
		//用叉积误差来做PI修正陀螺仪零偏，即抵消陀螺仪读数中的偏移量
		gx += Kp * ex + exInt;
		gy += Kp * ey + eyInt;
                Correct_gyro_y = gy / 0.001065f;
		gz += Kp * ez + ezInt;
	}
	//一阶近似算法，四元数运动学方程的离散化形式和积分
	q0 += (-q1 * gx - q2 * gy - q3 * gz) * halfT;
	q1 += (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2 += (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3 += (q0 * gz + q1 * gy - q2 * gx) * halfT;
	
	//单位化四元数
	normalise = myInvSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	//四元数平方
	q0s = q0 * q0;
	q1s = q1 * q1;
	q2s = q2 * q2;
	q3s = q3 * q3;
	
	R11 = q0s + q1s - q2s - q3s;	//矩阵(1,1)项
	R21 = 2 * (q1 * q2 + q0 * q3);	//矩阵(2,1)项

	//机体坐标系下的z方向向量
	vecxZ = 2 * (q1 * q3 - q0 * q2);        //矩阵(3,1)项
	vecyZ = 2 * (q0 * q1 + q2 * q3);        //矩阵(3,2)项
	veczZ = q0s - q1s - q2s + q3s;	        //矩阵(3,3)项
	
	if (vecxZ>1) vecxZ=1;
	if (vecxZ<-1) vecxZ=-1;
	
	//计算欧拉角
//	*pitch =  -asinf(vecxZ) * RAD2DEG;//+PITCH_ERROR;       //公式
        *pitch =  90 + asinf(vecxZ) * RAD2DEG;//+PITCH_ERROR; 
	*roll = atan2f(vecyZ, veczZ) * RAD2DEG;//+ROLL_ERROR;
	*yaw = atan2f(R21, R11) * RAD2DEG;//+YAW_ERROR;
	
	//test_yaw+=	((Gyro_z-GyrooffsetX)*MPU6050G_s2000dps*0.0174)*RAD2DEG*dt;
	

  // az= ax*vecxZ + ay * vecyZ + az * veczZ - baseZacc;	/*Z?á?ó?ù?è(è￥3y??á|?ó?ù?è)*/
	
	
}


/*
二阶滤波器的基本差分方程
*/
//截止频率:30Hz 采样频率:500Hz
#define b0 0.1883633f
#define b1 0
#define a1 1.023694f
#define a2 0.2120577f

CarThreeNum LastIn={0,0,0};
CarThreeNum PreOut={0,0,0};
CarThreeNum LastOut={0,0,0};

void Filter_2nd_LPF2ndFilter()//这个函数用于加速度计数据滤波，使用直立车滤波也效果一样
{
    Acc_ADC_Data.x = b0 * Acc_ADC.x + b1 * LastIn.x -  a1 * LastOut.x - a2 * PreOut.x;
    Acc_ADC_Data.y = b0 * Acc_ADC.y + b1 * LastIn.y -  a1 * LastOut.y - a2 * PreOut.y;
    Acc_ADC_Data.z = b0 * Acc_ADC.z + b1 * LastIn.z -  a1 * LastOut.z - a2 * PreOut.z;
    
    LastIn.x = Acc_ADC.x;
    LastIn.y = Acc_ADC.y;
    LastIn.z = Acc_ADC.z;
    
    PreOut.x = LastOut.x;
    PreOut.y = LastOut.y;
    PreOut.z = LastOut.z;
    
    LastOut.x = Acc_ADC_Data.x;
    LastOut.y = Acc_ADC_Data.y;
    LastOut.z = Acc_ADC_Data.z;
}


/*************************************************************************
*  函数名称：Measure_Acc
*  功能说明：测量加速度计值
*  参数说明：无
*  函数返回：无（但包含全局变量a_sin和a_cos）
*  修改时间：2012-2-14    已测试
*************************************************************************/
float Angle_a_sin=0 ;
void Measure_Acc()
{
    static float a_sin_new=0,a_sin[3];
    
    for(int i = 0;i < 2;i++)
    {   
        a_sin[i+1] = a_sin[i];
    }
        
    a_sin[0] = Acc_ADC_Data.z;
    a_sin_new = (a_sin[0] + a_sin[1] + a_sin[2])/3.0;
    
      //低通滤波
    Angle_a_sin = Angle_a_sin*0.5 + a_sin_new*0.5;
}


/*************************************************************************
*  函数名称：Angle_control
*  功能说明：角度融合，计算出角度，计算角度pwm输出
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/

void Angle_control()
{
    //直立

#define k_Gyro 0.07   //角速度放大倍数
#define tg 1         //**跟踪时间:1~4
    static float mG_delta_y=0;      //和angle_speed一个级别
    static float Acc_angle_z=0;

    
    Acc_angle_z = Angle_a_sin / 350;   //纵向
    if(Acc_angle_z > 1)
        Acc_angle_z = 1;
    else if(Acc_angle_z<-1)
        Acc_angle_z = -1;
    
    Angle.m_angle = asin(Acc_angle_z)*57.295779513;
    if(Angle.m_angle >90) Angle.m_angle = 90;
    else if(Angle.m_angle <-90) Angle.m_angle = -90;
//    if(Real_time_ms<100)
      Angle.G_angle = Angle.m_angle;
    
//    if(Angle.G_angle>18||Angle.G_angle<-31)
//      deadFlag =1 ;
      
    //得到Angle.G_angle
    Angle.Gyro = (float)(Gyro_ADC.y);
    Angle.speed =0 - MPU6050G_s2000dps * Angle.Gyro;
    Angle.sspeed=Angle.speed*0.9f+Angle.prespeed*0.1f;
    Angle.prespeed=Angle.speed;
    mG_delta_y = (Angle.m_angle - Angle.G_angle) / tg;
    Angle.G_angle += (Angle.speed + mG_delta_y) * dt;
    
    
    //得到Angle.PWM   
//    Angle.PWM = (Angle.OFFSET - Angle.G_angle + Carspeed.expect_angle)*Angle.P - Angle.speed * Angle.D;
//    Angle.PWM = (Angle.OFFSET - Angle.G_angle)*Angle.P - Angle.speed * Angle.D;
    
    if(Angle.PWM<=Angle.PWM_min)
      Angle.PWM=Angle.PWM_min;
    if(Angle.PWM>=Angle.PWM_max)
      Angle.PWM=Angle.PWM_max;
}


//void Angle_control()
//{
//  Angle.speed =0 - MPU6050G_s2000dps * Correct_gyro_y;
//  Angle.sspeed=Angle.speed*0.9f+Angle.prespeed*0.1f;
//  Angle.prespeed=Angle.speed;
//  Angle.PWM = (Angle.OFFSET - Pitch + Carspeed.expect_angle)*Angle.P - Angle.speed * Angle.D;
//  if(Angle.PWM<=Angle.PWM_min)
//      Angle.PWM=Angle.PWM_min;
//  if(Angle.PWM>=Angle.PWM_max)
//      Angle.PWM=Angle.PWM_max;
//}
