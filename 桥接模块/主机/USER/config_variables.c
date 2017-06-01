#include "config_variables.h"

u8 STATIC_LEVEL_RX_BUF[USART_BEFFER_SIZE];
u8 CONTROL_BUS_RX_BUF[USART_BEFFER_SIZE];

u8 USART_RX_BUF[USART_BEFFER_SIZE];
u8 USART_TX_BUF[USART_BEFFER_SIZE];


u8 MASTER_TX_BUF[USART_BEFFER_SIZE];

u8 Result_Pin4_TX_BUF[USART_BEFFER_SIZE];
u8 Result_Pin3_TX_BUF[USART_BEFFER_SIZE];


struct Self_State self_state;
struct sensor_data static_level_sensor_profile[3]={ 
{"712566",{0.00744,2.15329,27.63270}},
{"712911",{0.00737,2.16623,27.93397}},
{"712915",{0.00714,2.12315,27.76165}}};
struct sensor_data angle_sensor_profile[3]={{0x01},{0x02},{0x03}};
	
u8 newCmd = 0;//接收标志位
u8 newSelfCmd = 0;
u8 cmdNum = 0;//串口数据量

u8 static_level_data_received = 0;
u8 control_bus_data_received = 0;

float xy[256][2];
float result_Pin4[2];
float result_Pin3[2];
