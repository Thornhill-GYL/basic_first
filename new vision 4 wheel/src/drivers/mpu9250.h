#ifndef __MPU9250_H__
#define __MPU9250_H__
#include "common.h"

#define      Ave_Cnt_const    (0)
#define      dt               (0.005f)
#define DEG2RAD		      (0.017453293f)	//�Ƕ�ת����
#define RAD2DEG		      (57.29578f)	//����ת�Ƕ�
#define MPU6050G_s2000dps     ((float)0.0609756f)  // 0.0700000 dps/LSB

#define MPU_RST_PORT PORTE,27
#define MPU_RS_PORT PORTE,0
#define MPU_SDI_PORT PORTE,1
#define MPU_SCL_PORT PORTE,2
#define MPU_SDO_PORT PORTE,3

#define MPU_SPIn kSPI1
#define MPU_CS_PCSn SPIn_PCS0

//#define mpu_rs PTE0_OUT
#define mpu_sdi PTE1_OUT
#define mpu_scl PTE2_OUT
#define mpu_sdo PTE3_IN
#define mpu_cs  PTE4_OUT
//#define reset PTE27_OUT

// ?����?MPU9250?��2?��??��
//****************************************
#define	SMPLRT_DIV		0x19	//����?Y��?2��?��?����?��?D��?�̡�o0x07(125Hz)
#define	CONFIG			0x1A	//�̨�����??2��?��?����?��?D��?�̡�o0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//����?Y��?��??��?��2a��?��??���?��?D��?�̡�o0x18(2?��??����?2000deg/s)
#define ODR_CONTROL             0x1E
#define	ACCEL_CONFIG1	0x1C	//?��?��??��??��?��2a��?��??��?��??����??2��?��?����?��?D��?�̡�o0x01(2?��??����?2G��?5Hz)
#define	ACCEL_CONFIG2	0x1D

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

#define INT_PIN_CFG             0x37

#define	PWR_MGMT_1		0x6B	//��??��1��������?��?D��?�̡�o0x00(?y3��??��?)
#define	WHO_AM_I		  0x75	//IIC��??��??��??��(??��?��y?��0x68��????��)

#define MPU_READ_BIT            (0X80)
#define MPU_WRITE_BIT           (0X7F)

//****************************

#define	GYRO_ADDRESS   0xD0	  //����?Y��??��
#define MAG_ADDRESS    0x18   //��?3?��??��
#define ACCEL_ADDRESS  0xD0 


typedef struct
{
    float x;
    float y;
    float z;    
}CarThreeNum;

typedef struct{
  float Angle;
  float Q_bias;
  float PP[2][2];
}Kalman_Filter_Str_for_9250;

typedef struct  
{
    float G_angle_min;
    int getup_time;
    
    float roll;   //����
    float pitch;  //ƫ��
    float yaw;    
    float rotation;  
    float vertical;
    
    float error;
    float Now;
    float Goal;
    float Gyro;
    float speed;
    float sspeed;
    float prespeed;
    //����ĽǶȣ�m_angleΪmma845x��õĽǶȣ�G_angleΪ�����ǻ��ֳ����ĽǶ�
    float m_angle;
    float G_angle;
    float OFFSET;//OFFSET_angleΪ��ֹʱ�ĽǶ�important    				
    
    int set;//��������
    int set_angle;
    int radius;
    float Rad[21];
    float angle_x[21];
    float angle_y[21];
    
    
    float P;              //�ٶȿ���P����
    float I;              //�ٶȿ���I����
    float D;              //�ٶȿ���I����
    float PWM;
    float Pwm_P;
    float Pwm_I;
    float Pwm_D;
    int PWM_max;
    int PWM_min;
}CarAngle;

typedef struct 
{
  float Delta;
  float DDelta;
  float preDelta;
  float P;
  float D;
  float pwm;
  long pwm_min;
  long pwm_max;
}CarGyro;

void MPU9250_PORT_init();
u8 spi_date_trans(u8 out);
void  mpu_write_date(unsigned char address,unsigned char  c);
char  mpu_read_data(unsigned char address);
void  mpu_read_str(unsigned char address,unsigned char *str,int len);
void Init_MPU9250(void);
void READ_DATE(void);
float Kalman_Filter(Kalman_Filter_Str_for_9250* Str,float Gyro,float Accel);
void  mpudelay(void);
void MPU_selftest(void);
void mix_gyrAcc_crossMethod(float * attitude,int gyr[3],int acc[3]);
void imuUpdate(float gx, float gy, float gz, float ax, float ay, float az, float dt_,float *roll,float *pitch,float *yaw);
void Filter_2nd_LPF2ndFilter();
void Measure_Acc();
void Angle_control();

#endif