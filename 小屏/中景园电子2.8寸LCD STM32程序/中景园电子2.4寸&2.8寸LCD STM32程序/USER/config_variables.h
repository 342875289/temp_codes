#ifndef __CONFIG_VARIABLES_H
#define __CONFIG_VARIABLES_H

#include "stm32f10x.h"

#define USART_BEFFER_SIZE 40
#define   TIME_FOR_CONNECT_RESERVE 5

extern u8 USART_RX_BUF[];
extern u8 USART_TX_BUF[];
extern u8 newCmd;
extern u8 cmdNum;
extern u8 sensor_address;
extern struct Slave_State slave_state;

void confirm_Sensor_Connect(void);
void Check_Connect(void);
void searchSensorAddr(void);
void readSensorXY(void);
struct Slave_State
{
	int isSensorConnect;
	int last_sensor_connect_Time;
};

#endif
