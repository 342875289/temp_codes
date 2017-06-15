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


void RS485_TX_MODE_ENABLE(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	DelayUs(1);
}
void RS485_RX_MODE_ENABLE(void)
{
	//等待发送结束
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	DelayUs(1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}


void RS485_ControlIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		//进入接收模式
		//RS485_RX_MODE_ENABLE();
		//进入发送模式
		//RS485_TX_MODE_ENABLE();
}

void UART1_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
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
	/*	
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  	// Configure USARTy 
  	USART_Init(USART1, &USART_InitStructure);
		*/
		//开启中断
		//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART1, ENABLE);
}


void UART3_Init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		// config USART3 clock 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		// USART3 GPIO config
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
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
		//Usart3 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  	// Configure USART3 
  	USART_Init(USART3, &USART_InitStructure);
		//开启中断
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	  //使能USART3
		USART_Cmd(USART3, ENABLE);
}


void usart_data_process(u8 usartData,u8 USART_BUFF[])
{
	static u8 data_count = 0 , data_num = 14;
	if(newCmd == 0)
	{
		if(data_count == data_num )
		{
					data_count = 0;
		}
		if( data_count == 0 )
		{
			if( usartData==0x77 )
			{
				USART_BUFF[0] = usartData  ;
				data_count ++;
			}
		}
		else if( data_count == 1 )
		{
			USART_BUFF[1] = usartData  ;
			data_num = usartData + 1 ;
			//防止数据格式过短
			if(data_num < 3){data_num=3;}
			cmdNum = data_num;
			data_count ++;
		}
		else if( data_count < data_num )
		{
			USART_BUFF[data_count] = usartData ;
			data_count ++;
			if(data_count == data_num )
			{
					getInformationToOLED( USART_BUFF);
					newCmd = 1;
			}
		}
	}
	
}

void getInformationToOLED(u8 USART_BUFF[])
{
	int x_i,x_f,y_i,y_f,z_i,z_f;
	if(( USART_BUFF[0] == 0x77 )  )
	{
		if( USART_BUFF[1] == 0x0D )
		{
			x_i = (USART_BUFF[4]&0x0F)*100 + (USART_BUFF[5]>>4)*10 + (USART_BUFF[5]&0x0F);
			x_f = (int)(USART_BUFF[6]>>4)*1000 + (int)(USART_BUFF[6]&0x0F)*100;
				
			y_i = (USART_BUFF[7]&0x0F)*100 + (USART_BUFF[8]>>4)*10 + (USART_BUFF[8]&0x0F);
			y_f = (int)(USART_BUFF[9]>>4)*1000 + (int)(USART_BUFF[9]&0x0F)*100 ;
				
			z_i = (USART_BUFF[10]&0x0F)*100 + (USART_BUFF[11]>>4)*10 + (USART_BUFF[11]&0x0F);
			z_f = (int)(USART_BUFF[12]>>4)*1000 + (int)(USART_BUFF[12]&0x0F)*100 ;
			
			
			
			if(USART_BUFF[4]&0xF0)//X为负数
			{
				x_f = - x_f;
				x_i = - x_i;
			}
			if(USART_BUFF[8]&0xF0)//Y为负数
			{
				y_f = - y_f;
				y_i = - y_i;
			}
			//slave_state.x_angle =  x_i + x_f/10000.0;
			//slave_state.y_angle =  y_i + y_f/10000.0;
			
		}
		else if( USART_BUFF[1] == 0x04 && USART_BUFF[3] == 0x1F)
		{
				sensor_address = USART_BUFF[4];
		}
	}
		
}
void anglef_to_anglechars(float angle,u8 string[4])
{
	
	int angle_i,angle_f,temp;
	
	if(angle>=0){ string[0] = 0x00; }
	else        { string[0] = 0x10; angle = - angle;}
	
	angle_i = (int)angle;
	angle_f = (int)((angle - angle_i)*10000);
	
	temp = angle_i /100;
	string[0] = temp  + string[0];
	angle_i -= temp*100;
	
	temp = angle_i /10;
	string[1] = temp 	<< 4;
	angle_i -= temp*10;
	
	string[1] = angle_i + string[1] ;
	
	temp = angle_f /1000;
	string[2] = temp << 4;
	angle_f -= temp*1000;
	
	temp = angle_f /100;
	string[2] = temp  + string[2];
	angle_f -= temp*100;
	
	temp = angle_f /10;
	string[3] = temp  << 4 ;
	angle_f -= temp*10;
	
	string[3] = angle_f  + string[3];
	
}


void usart3_send_cmd(u8 USART_BUFF[])
{
	u8 i = 0,datanum = USART_BUFF[1] +1 ;
	//防止发送数据个数超过缓冲区大小
	RS485_TX_MODE_ENABLE();
	if( datanum > (u8)USART_BEFFER_SIZE )
	{
		datanum = (u8)USART_BEFFER_SIZE;
	}
	while(i<datanum)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART3,USART_BUFF[i]);
		i++;
	}
	RS485_RX_MODE_ENABLE();
}


void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		 usart_data_process( USART_ReceiveData(USART3) , USART_RX_BUF);
  } 
} 
