#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

extern u8 usart_readXY[];
extern u8 modbus_readXY[];
extern u8 auchCRCHi[] ;
extern u8 auchCRCLo[];
void UART1_Init(u32 bound);
void UART2_Init(u32 bound);
void UART3_Init(u32 bound);
void RS485_ControlIO_Init(void);
void RS485_TX_MODE_ENABLE(void);
void RS485_RX_MODE_ENABLE(void);

void usart1_send_string(u8 USART_BUFF[],u8 datanum);
void usart3_send_string(u8 USART_BUFF[],u8 datanum);
void usart2_send_string(u8 USART_BUFF[],u8 datanum);
void usart1_send_cmd(u8 USART_BUFF[]);
void usart3_send_cmd(u8 USART_BUFF[]);

void process_static_level_data(void);
void receive_data_from_static_level(u8 usartData);
void receive_data_from_control_bus(u8 usartData);


int string_cmp_8b(u8 sourceString[],u8 targetString[],u8 cmpnum);
int modbus_Cmd_To_usart(u8 modbusCmd[]);
int modbus_Cmd_To_usart2(u8 modbusCmd[]);
int usart_feedback_To_modbus(u8 USART_BUFF[]);
void usart3_send_modbus_feedback(u8 USART_BUFF[]);
void usart1_send_modbus_feedback(u8 USART_BUFF[]);
void processSelfCmd(u8 USART_BUFF[]);
void getRusult(void);
u16 CRC16(u8 *puchMsgg,u8 usDataLen);
#endif


