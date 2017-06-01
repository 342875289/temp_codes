#include "esp8266.h"

int rn_count = 0;//对接收消息中的换行符计数，收到3个\r\n即为正常结束
char wifi_feedback[200];//用于存放接收数据的数组
int data_i=0;//接收数据数组的索引
int time_wait=3;//接收数据等待的时间(3秒)
int receive_state=0;//为0表示还没有收到数据  为1表示正在接收数据 为2表示接收数据的过程正常结束
u8 fac_us=72000000/8000000;	
u16 fac_ms=72000000/8000000*1000;
//使能esp8266
void wifi_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;		//PB.12
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//使能esp8266
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
//用于连接WiFi主机，成功返回1，失败返回0
int Connect_master()
{
	//等待模块自身启动完成
	delay_ms(1000);
	delay_ms(1000);

	//关闭命令回显
	printf("ATE0\r\n");
	if(Re(2)==1){;}
	else{return 0;}
	
	
	//从机连接AP热点
	printf("AT+CWJAP=\"lift-project\",\"labcat127\"\r\n");
	if(Re(3)==1){;}
	else{return 0;}
	
	//从机设置STA模式下的IP
	printf("AT+CIPSTA=\"192.168.4.3\"\r\n");
	if(Re(2)==1){;}
	else{return 0;}
	
	//从机设置为单连接
	printf("AT+CIPMUX=0\r\n");
	if(Re(2)==1){;}
	else{return 0;}
	
	//从机连接主机
	printf("AT+CIPSTART=\"TCP\",\"192.168.4.1\",8088\r\n");
	if(Re(2)==1)
	{
		//delay_ms(1000);delay_ms(1000);delay_ms(1000);
		/* Clear the Overrun Error interrupt pending bit */
		//USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断;
	}
	else{return 0;}
	
	/*
	//从机向主机发送指定字节数的信息
	printf("AT+CIPSEND=XX\r\n");
	if(Re(10)==1){;}
	else{return 0;}
	delay_ms(1000);
	*/
	
	/*
	//从机断开与主机的连接
	printf("AT+CIPCLOSE\r\n");
	if(Re(10)==1){;}
	else{return 0;}
	delay_ms(1000);
	delay_ms(1000);
	*/
	
	
	return 1;
	
}

int Re(int seconds)
{
	u32 temp;
	u8 data;
	u16 nms=1000;
	int times=seconds;
	data_i=0;
	
	while( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET );//等待第一个字节
	
	while(times--)
	{
		SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
		SysTick->VAL =0x00;							//清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
		do
		{
			if( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
			{
				data = USART_ReceiveData(USART3);
				wifi_feedback[data_i]=data ;
				//OK
				if( wifi_feedback[data_i] == 'K' && data_i>=1 && 
					wifi_feedback[data_i-1] == 'O' )
				{
					SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
					SysTick->VAL =0X00;       					//清空计数器	
					return 1;//返回成功信息
				}
				//ERROR
				if( wifi_feedback[data_i] == 'R' && data_i>=4 && 
					wifi_feedback[data_i-1] == 'O' &&
					wifi_feedback[data_i-2] == 'R' &&
					wifi_feedback[data_i-3] == 'R' &&
					wifi_feedback[data_i-4] == 'E' 
				)
				{
					SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
					SysTick->VAL =0X00;       					//清空计数器	
					return 0;//返回失败信息
				}
				
				//接收成功-重新装载倒计时秒数
				times=seconds;
				data_i++;
			}
			
			
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
		SysTick->VAL =0X00;       					//清空计数器	
	}
	return 0;
}



void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		//失能接收中断--准备处理连续字符
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	}
} 
