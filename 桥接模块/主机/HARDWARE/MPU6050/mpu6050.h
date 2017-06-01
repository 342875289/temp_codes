#ifndef    __MPU6050_H
#define    __MPU6050_H

#define Gravity_X_Zero 360
#define Gravity_Y_Zero -270
#define Gravity_Z_Zero 0 //����

void delayM(unsigned int t); 
void sda_high(void);  //SDA�ߵ�ƽ�Ĳ��� 
void sda_low(void);  //SDA�͵�ƽ�Ĳ��� 
void scl_high(void);  //SCL�ߵ�ƽ���� 
void scl_low(void); //SCL�͵�ƽ�Ĳ��� 
void I2C_Start(void); 
void I2C_Stop(void); 
void I2C_SendACK(char ack); 
void I2C_RecvACK(void); 
void I2C_SendByte(unsigned char dat);    
unsigned char I2C_RecvByte(void); 
void Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data); 
unsigned char Single_ReadI2C(unsigned char REG_Address); 
void InitMPU6050(void); 
short GetData(unsigned char REG_Address);
float MPU6050_Get_Angle(float,float); 
void trans (int a,unsigned char b[3]);
void trans_four(float a, char b[4]);
void trans_five(float a, char b[5]);

void Kalman_Filter(float,float);

void Angle_Calcu(void);
void Get_Gyro_Zero(void);
void Get_Gravity_Zero(void);
void Show_Angle(void);
void Angle_Control(void);
void Speed_Adjust(float,float);



extern int Angle_Speed;
extern int Accel_x,Accel_y,Accel_z;
extern float Gyro_y ;
extern float x_zero,y_zero,z_zero;

#endif

/*   ��������
void Get_Gravity(void)
void Get_Gyro(void); 
*/
