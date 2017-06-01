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
		//ʱ������
		RCC_Configuration();
		//system clock����
		SysTick_Configuration();
		//����NVIC�жϷ���2 : 2λ��ռ���ȼ�(�ж�Ƕ�״���)��2λ��Ӧ���ȼ�(�ж���Ӧ�Ⱥ�˳��)
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
		//��ʱ������ʼ��	
		DelayInit();
		TIM3_Init(10000,72000);
		//��ʼ�� ����USART1  ��9600 8λ����λ 1λֹͣλ 0λУ��λ
		UART1_Init(9600);//RS232
		UART3_Init(2400);//RS485
		//RS485ģ���ʼ��
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
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	u8 cmd[]="#+712566";
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			//USART_SendData(USART1,0x01);
			
			
			self_state.target_address = 1;
			usart3_send_string(cmd,8);
		}
} 
