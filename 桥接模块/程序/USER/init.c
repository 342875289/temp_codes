#include "stm32f10x.h"
#include "init.h"
#include "usart.h"
#include "config_variables.h"
void start_up(void)
{
		USART_Cmd(USART1, ENABLE);
		USART_Cmd(USART2, ENABLE);
		USART_Cmd(USART3, ENABLE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3����	 
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
		//RS485����ģʽʹ��
		//RS485_RX_MODE_ENABLE();
}
	
void SysTick_Configuration(void)
{	
  	/* Setup SysTick Timer for 10 msec interrupts  */
  	if (SysTick_Config(SystemCoreClock / 1000))	    /*SysTick����Ϊ10ms�ж�һ��*/
  	{ 
    	/* Capture error */ 
    	while (1);
  	}
  
  	/* Configure the SysTick handler priority */
  	NVIC_SetPriority(SysTick_IRQn, 0x0);		         /*����SysTick���ж���ѡ��*/
														 /* n=0x00~0x03 ����Systick��ռʽ���ȼ�0 
															n=0x04~0x07 ����Systick��ռʽ���ȼ�1 
															n=0x08~0x0B ����Systick��ռʽ���ȼ�2 
															n=0x0C~0x0F ����Systick��ռʽ���ȼ�3  */	
}
void RCC_Configuration(void)
{	
	ErrorStatus HSEStartUpStatus;
  	
	/* ������������RCC�����ã����ƼĴ�����λ */
	RCC_DeInit();
	/* ʹ���ⲿ���پ��� */
	RCC_HSEConfig(RCC_HSE_ON);
	/* �ȴ����پ����ȶ� */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if (HSEStartUpStatus == SUCCESS)
	{
	    /* ʹ��flashԤ��ȡ������ */
	    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	    /* ��Flash���ڵȴ�״̬��2����Ը�Ƶʱ�ӵģ��������RCCûֱ�ӹ�ϵ�����������Թ� */
	    FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK ���ø�������ʱ��=ϵͳʱ��*/
	    RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK ���õ�������2ʱ��=��������ʱ��*/
	    RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 ���õ�������1��ʱ��=����ʱ�ӵĶ���Ƶ*/
	    RCC_PCLK1Config(RCC_HCLK_Div2);

		/* ADCCLK = PCLK2/6 ����ADC����ʱ��=��������2ʱ�ӵ�����Ƶ*/
	    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

		/* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */ 
	    //�������������ȱʧ�ˣ���ȴ�ܹؼ�
		/* �������໷���ⲿ8Mhz����9��Ƶ��72Mhz */ 
	    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL ʹ�����໷*/
	    RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready �ȴ����໷����ȶ�*/
	    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	    {}

		/* Select PLL as system clock source �����໷�������Ϊϵͳʱ�� */
	    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source �ȴ�У��ɹ�*/
	    while (RCC_GetSYSCLKSource() != 0x08)
	    {}
	}
}

