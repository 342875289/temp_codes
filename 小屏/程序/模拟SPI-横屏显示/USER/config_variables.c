#include "config_variables.h"
#include "usart.h"
#include "delay.h"

u8 USART_RX_BUF[USART_BEFFER_SIZE];
u8 USART_TX_BUF[USART_BEFFER_SIZE];

u8 newCmd = 0;//接收标志位
u8 cmdNum = 0;//串口数据量

float x_angle,y_angle,z_angle;

struct Slave_State slave_state;
u8 sensor_address=0x00;
void confirm_Sensor_Connect()
{
	slave_state.last_sensor_connect_Time=TIME_FOR_CONNECT_RESERVE;
	if(slave_state.isSensorConnect==0)
	{
		//OLED_ShowCN(111,0,10);//√
		slave_state.isSensorConnect=1;
	}	
}

void Check_Connect()
{
	if(slave_state.isSensorConnect==1){slave_state.last_sensor_connect_Time --;}
	
	if(slave_state.last_sensor_connect_Time<=0 && slave_state.isSensorConnect==1)
	{
		slave_state.isSensorConnect=0;
	}
	
}

void searchSensorAddr()
{
	static u8 searchSensorAddrCmd[] = {0x77,0x04,0x00,0x1F,0x23};
	usart3_send_cmd(searchSensorAddrCmd);
}


void readSensorXY()
{
	static u8 readCmd[] = {0x77,0x04,0x00,0x04,0x08};
	u8 i,temp;
	readCmd[2] = sensor_address;
	for(i=1,temp=0;i<4;i++)
	{
		temp += readCmd[i];
	}
	readCmd[i] = temp;
	usart3_send_cmd(readCmd);
}
