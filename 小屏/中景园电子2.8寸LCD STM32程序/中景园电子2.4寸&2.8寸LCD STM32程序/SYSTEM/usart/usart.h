#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

//如果想串口中断接收，请不要注释以下宏定义
void RS485_ControlIO_Init(void);
void RS485_TX_MODE_ENABLE(void);
void RS485_RX_MODE_ENABLE(void);
void UART3_Init(u32 bound);
void usart3_send_cmd(u8 USART_BUFF[]);
void usart_data_process(u8 usartData,u8 USART_BUFF[]);
void getInformationToOLED(u8 USART_BUFF[]);
void anglef_to_anglechars(float angle,u8 string[4]);
#endif


