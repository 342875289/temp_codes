#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

//����봮���жϽ��գ��벻Ҫע�����º궨��


void RS485_ControlIO_Init(void);
void RS485_TX_MODE_ENABLE(void);
void RS485_RX_MODE_ENABLE(void);

void usart3_send_cmd(u8 USART_BUFF[]);
void usart_data_process(u8 usartData,u8 USART_BUFF[]);
void getInformationToOLED(u8 USART_BUFF[]);
void dispalyAngleByDegree(float angle,unsigned char string[]);

void UART1_Init(int bound);
void UART3_Init(int bound);
#endif


