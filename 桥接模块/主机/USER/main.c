#include "stm32f10x.h"
#include "config_variables.h"
#include "init.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "bsp_usart1.h"
#include "others.h"
u8 status;
u8 usartdata;

int main(void)
{	
		//时钟配置
		RCC_Configuration();
		//system clock配置
		SysTick_Configuration();
		//设置NVIC中断分组2 : 2位抢占优先级(中断嵌套次数)，2位响应优先级(中断响应先后顺序)
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
		//延时函数初始化	
		DelayInit();
		TIM3_Init(10000,72000);
		//初始化 串口USART1  波9600 8位数据位 1位停止位 0位校验位
		UART1_Init(9600);//RS232
		UART3_Init(2400);//RS485
		//RS485模块初始化
		RS485_ControlIO_Init();
	
		start_up();
	
	while(1)
	{
		//if(newSelfCmd == 1){processSelfCmd(USART_RX_BUF);}
		if( static_level_data_received == 1 )
		{
				process_static_level_data();
				static_level_data_received = 0;
		}
		
	}
}
void TIM3_IRQHandler(void)   //TIM3中断
{
	u8 cmd[]="#+712566";
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
			//USART_SendData(USART1,0x01);
			
			
			self_state.target_address = 1;
			usart3_send_string(cmd,8);
		}
} 
