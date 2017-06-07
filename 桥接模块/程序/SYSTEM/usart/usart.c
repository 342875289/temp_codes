#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "config_variables.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 



//void RS485_TX_MODE_ENABLE(void)
//{
//	GPIO_SetBits(GPIOB,GPIO_Pin_1);
//	DelayUs(1);
//}
//void RS485_RX_MODE_ENABLE(void)
//{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
//	DelayUs(1);
//}

//void RS485_ControlIO_Init(void)
//{
//		GPIO_InitTypeDef GPIO_InitStructure;
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//		//进入接收模式
//		//RS485_RX_MODE_ENABLE();
//		//进入发送模式
//		//RS485_TX_MODE_ENABLE();
//}

void UART1_Init(u32 bound)
{
  	
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
		USART_Cmd(USART1, ENABLE);
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
		//关闭中断
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		USART_Cmd(USART1, DISABLE);
}


void UART2_Init(u32 bound)
{
  	
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		/* config USART1 clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		// USART2 GPIO config
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		USART_Cmd(USART2, ENABLE);
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.01) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		/* Configure USART1 Rx (PA.03) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
		USART_Init(USART2, &USART_InitStructure); 
		
		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
		//关闭中断
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		
		USART_Cmd(USART2, DISABLE);
}

void UART3_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		// config USART3 clock 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		// USART3 GPIO config
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		USART_Cmd(USART3, ENABLE);
		// Configure USART3 Tx (PB.10) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);    
		// Configure USART3 Rx (PB.11) as input floating 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		// USART3 mode config 
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART3, &USART_InitStructure); 
		//开启中断
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	  //使能USART3
		USART_Cmd(USART3, DISABLE);
}







void receive_data_from_control_bus(u8 usartData)
{
	static u8 data_count = 0 , data_num = 8;
	u16 CRC_temp;
		
	if( (control_bus_data_received == 0) && ( data_count < data_num) )
	{
		
		switch(data_count)
		{
			case 0:if(!( usartData>=0x01  ||  usartData<=0x06 || usartData==self_state.self_address ))
						{return;}
						break;
			case 2:if( (usartData == 0x04) || (usartData == 0x08) )
						{data_num = usartData + 5;}
						break;
		}
		CONTROL_BUS_RX_BUF[data_count] = usartData ;
		data_count ++;
		if(data_count == data_num )
		{			
			CRC_temp = CRC16(CONTROL_BUS_RX_BUF,data_num-2);
			if( (CONTROL_BUS_RX_BUF[data_num-2]== CRC_temp >> 8)  &&  (CONTROL_BUS_RX_BUF[data_num-1]== (CRC_temp & 0xFF ))  )
			{
					control_bus_data_received = 1;
			}
			data_count = 0;
			data_num = 8;
		}
	}
}


// 函数功能CRC校验用函数 
// 函数输入puchMsgg是要进行CRC校验的消息usDataLen是消息中字节数
// 函 数 输 出  计 算 出 来 的CRC校 验 码 。                                                                                      

/************************************************************/ 
u16 CRC16(u8 *puchMsgg,u8 usDataLen)       //puchMsgg是要进行CRC校验的消息usDataLen是消息中字节数
{   
		u8 uchCRCHi = 0xFF ; /* 高CRC字节初始化*/ 
    u8 uchCRCLo = 0xFF ; /* 低CRC 字节初始化*/ 
    u8 uIndex ; /* CRC循环中的索引*/ 
    while (usDataLen--) /* 传输消息缓冲区*/ 
    { 
				uIndex = uchCRCHi ^ *puchMsgg++ ; /* 计算CRC */ 
				uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
				uchCRCLo = auchCRCLo[uIndex] ; 
    } 
		return ((uchCRCHi<< 8) | uchCRCLo) ; 
} 


void usart1_send_string(u8 USART_BUFF[],u8 datanum)
{
	u8 i = 0;
	while(i<datanum)
	{
		USART_SendData(USART1,USART_BUFF[i]);
		i++;
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
void usart2_send_string(u8 USART_BUFF[],u8 datanum)
{
	u8 i = 0;
	while(i<datanum)
	{
		USART_SendData(USART2,USART_BUFF[i]);
		i++;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
void usart3_send_string(u8 USART_BUFF[],u8 datanum)
{
	u8 i = 0;
	while(i<datanum)
	{
		USART_SendData(USART3,USART_BUFF[i]);
		i++;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}
}


	
void USART1_IRQHandler(void)
{ 
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
		receive_data_from_control_bus(USART_ReceiveData(USART1));
	}
}

void USART2_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
		receive_data_from_static_level(USART_ReceiveData(USART2));
	}
} 

//处理静力水准仪数据
void receive_data_from_static_level(u8 usartData)
{
	//$12345678
	static u8 data_count = 0 , data_num = 9 ;
	
	if(static_level_data_received == 0)
	{
		if(data_count == data_num ){data_count = 0;}
		
		if( data_count == 0 )
		{
			if( usartData=='$' )
			{
				STATIC_LEVEL_RX_BUF[0] = usartData  ;
				data_count ++;
			}
		}
		else if( data_count < data_num )
		{
			STATIC_LEVEL_RX_BUF[data_count] = usartData ;
			data_count ++;
			if(data_count == data_num )
			{
				static_level_data_received = 1;
			}
		}
	}
	
}


//void processSelfCmd(u8 USART_BUFF[])
//{
//	u8 i=0,temp=0,datanum; 
//	static u8 old_sample_times=0,print_cmd_num=0;
//	
//	//usart1_send_string(USART_BUFF,8);
//	
//	if(USART_BUFF[1] == 0x03)//Read
//	{
//		switch(USART_BUFF[3])
//		{
//			case ReturnSampleData:
//																RS485_TX_MODE_ENABLE();
//																datanum = MASTER_TX_BUF[2] + 5;
//																while(i<datanum)
//																{
//																	USART_SendData(USART3,MASTER_TX_BUF[i]);
//																	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
//																	i++;
//																}
//																RS485_RX_MODE_ENABLE();
//																break;
//			case ReturnResult_Pin4:
//																getRusult();
//																RS485_TX_MODE_ENABLE();
//																datanum = Result_Pin4_TX_BUF[2] + 5;
//																while(i<datanum)
//																{
//																	USART_SendData(USART3,Result_Pin4_TX_BUF[i]);
//																	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
//																	i++;
//																}
//																RS485_RX_MODE_ENABLE();
//																break;
//			case ReturnResult_Pin3:
//																RS485_TX_MODE_ENABLE();
//																datanum = Result_Pin3_TX_BUF[2] + 5;
//																while(i<datanum)
//																{
//																	USART_SendData(USART3,Result_Pin3_TX_BUF[i]);
//																	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
//																	i++;
//																}
//																RS485_RX_MODE_ENABLE();			
//																break;
//		}
//	}
//	else if(USART_BUFF[1] == 0x06)//Write
//	{
//		
//		switch(USART_BUFF[3])
//		{
//			case SetAutoSampleModel_targetAddress:
//																self_state.target_address = USART_BUFF[5];
//																self_state.saved_num = 0;
//																//printf("开始采集数据,地址:%d,点数:%d\r\n",self_state.target_address,self_state.sample_num);
//																break;
//			case SetAutoSampleModel_sampleNumber:
//																if(USART_BUFF[5] != 0)
//																{
//																	self_state.sample_num = USART_BUFF[5];
//																	self_state.saved_num = 0;
//																}
//																//printf("开始采集数据,地址:%d,点数:%d\r\n",self_state.target_address,self_state.sample_num);
//																break;
//			case SampleData:
//																//读取传感器数据的命令
//																NRF_TX_BUF_Point = readXY_usart;
//																NRF_TX_BUF_Point[2] =  self_state.target_address;
//																for(i=1,temp=0;i<NRF_TX_BUF_Point[1];i++)
//																{
//																	temp += NRF_TX_BUF_Point[i];
//																}
//																NRF_TX_BUF_Point[i] = temp;
//			
//																
//																self_state.saved_num = USART_BUFF[5];
//																
//																//只对新的次数请求做回应
//																if(self_state.saved_num != old_sample_times)
//																{
//																	newCmd = 1;
//																	self_state.isSampling = 1;
//																	old_sample_times = self_state.saved_num;
//																}
//																//对超过总次数的请求不回应
//																if( self_state.saved_num > self_state.sample_num )
//																{
//																	self_state.saved_num = self_state.sample_num;
//																	newCmd = 0;
//																	self_state.isSampling = 0;
//																}
//																//self_state.saved_num++;
//																//printf("收到第%d条采集命令\r\n",self_state.saved_num);
//																break;
//			
//			case PrintData:
//																print_cmd_num++;
//																if(self_state.sample_num != 0 && print_cmd_num == 3 && self_state.sample_num == self_state.saved_num)
//																{
//																	printSampleData(xy,self_state.saved_num);
//																}
//																if(print_cmd_num==3){print_cmd_num = 0;}
//																break;
//			case StartCorrect:
//			case EndCorrect:
//			case SamplePoint1:
//			case SamplePoint2:
//			case ChangeOutputMode:
//																//usart1_send_string(USART_BUFF,8);
//																//
//																NRF_TX_BUF_Point = correct_usart;
//																NRF_TX_BUF_Point[3] =  USART_BUFF[3];//命令字
//																NRF_TX_BUF_Point[2] =  USART_BUFF[5];//地址
//																for(i=1,temp=0;i<NRF_TX_BUF_Point[1];i++)
//																{
//																	temp += NRF_TX_BUF_Point[i];
//																}
//																NRF_TX_BUF_Point[i] = temp;		
//																
//																if(NRF_TX_BUF_Point[2] == 0x00){break;}
//																//usart1_send_cmd(NRF_TX_BUF_Point);
//																newCmd = 1;
//																break;
//			case SampleByClockwise:		self_state.isSampleByClockwise = 1;break;
//			case SampleNotByClockwise:self_state.isSampleByClockwise = 0;break;
//	
//																
//		}
//	}
//	
//	newSelfCmd = 0;
//}
//CRC校验查表用参数 
/* CRC 高位字节值表*/ 
static u8 auchCRCHi[] = { 

    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0, 

    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41, 

    0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0, 

    0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40, 

    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1, 

    0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41, 

    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1, 

    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41, 

    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0, 

    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40, 

    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1, 

    0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40, 

    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
	    
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40, 

    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0, 

    0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40, 

    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0, 

    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41, 

    0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0, 

    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41, 

    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0, 

    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40, 

    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1, 

    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41, 

    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0, 

    0x80,0x41,0x00,0xC1,0x81,0x40 

    } ; 

    /* CRC低位字节值表*/ 
static u8 auchCRCLo[] = { 

    0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06, 

    0x07,0xC7,0x05,0xC5,0xC4,0x04,0xCC,0x0C,0x0D,0xCD, 

    0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09, 

    0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A, 

    0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4, 

    0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3, 

    0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3, 

    0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4, 

    0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A, 

    0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29, 

    0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED, 

    0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26, 

    0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60, 

    0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
	    
		0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F, 

    0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68, 

    0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E, 

    0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5, 

    0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71, 

    0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92, 

    0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C, 

    0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B, 

    0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B, 

    0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C, 

    0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42, 

    0x43,0x83,0x41,0x81,0x80,0x40 

    } ; 
		

///*
//void USART3_IRQHandler(void)                	//串口3中断服务程序
//{
//	u8 Res;
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//		{
//		Res =USART_ReceiveData(USART3);	//读取接收到的数据
//		
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
//     } 
//} 
//*/
