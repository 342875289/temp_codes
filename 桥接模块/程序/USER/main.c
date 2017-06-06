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
u8 tt[]={0xff,0x11,0x33};
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
		//TIM3_Init(10000,36000);//5s  
		TIM3_Init(36000,60000);//30s 
		//初始化 串口
		UART1_Init(9600);//RS485
		UART2_Init(2400);//RS485
		UART3_Init(9600);//RS485
		//RS485模块初始化
		//RS485_ControlIO_Init();
	
		start_up();
	
	while(1)
	{
		
		usart1_send_string(tt,3);
		
		usart2_send_string(tt,3);
		
		usart3_send_string(tt,3);
		
		
		if( static_level_data_received == 1 )
		{
				process_static_level_data();
				static_level_data_received = 0;
		}
		if( control_bus_data_received == 1 )
		{
				process_control_bus_data();
			  control_bus_data_received = 0;
		}
	}
}
void TIM3_IRQHandler(void)   //TIM3中断
{
	u8 i;
	static u8 time_count = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
			
			time_count++;
			if(time_count >= 1)//10--5分钟
			{
				time_count = 0;
				for(i=0;i<3;i++)
				{
						if(angle_sensor_profile[i].is_new == 1)
						{
							usart3_send_string(angle_sensor_profile[i].data,13);
							angle_sensor_profile[i].is_new = 0;
						}
						}
						if(self_state.is_static_level_init == 1)
						{
						for(i=0;i<2;i++)
						{
							if(static_level_sensor_profile[i].is_new == 1)
							{
								usart3_send_string(static_level_sensor_profile[i].data,9);
								static_level_sensor_profile[i].is_new = 0;
							}
						}
				}
			}
					
			
		}
} 
