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
		TIM_Cmd(TIM3, ENABLE);  //使能TIM3外设	 
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
		//RS485接收模式使能
		//RS485_RX_MODE_ENABLE();
}
	
void SysTick_Configuration(void)
{	
  	/* Setup SysTick Timer for 10 msec interrupts  */
  	if (SysTick_Config(SystemCoreClock / 1000))	    /*SysTick配置为10ms中断一次*/
  	{ 
    	/* Capture error */ 
    	while (1);
  	}
  
  	/* Configure the SysTick handler priority */
  	NVIC_SetPriority(SysTick_IRQn, 0x0);		         /*设置SysTick的中断优选级*/
														 /* n=0x00~0x03 设置Systick抢占式优先级0 
															n=0x04~0x07 设置Systick抢占式优先级1 
															n=0x08~0x0B 设置Systick抢占式优先级2 
															n=0x0C~0x0F 设置Systick抢占式优先级3  */	
}
void RCC_Configuration(void)
{	
	ErrorStatus HSEStartUpStatus;
  	
	/* 这里是重置了RCC的设置，类似寄存器复位 */
	RCC_DeInit();
	/* 使能外部高速晶振 */
	RCC_HSEConfig(RCC_HSE_ON);
	/* 等待高速晶振稳定 */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if (HSEStartUpStatus == SUCCESS)
	{
	    /* 使能flash预读取缓冲区 */
	    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	    /* 令Flash处于等待状态，2是针对高频时钟的，这两句跟RCC没直接关系，可以暂且略过 */
	    FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK 设置高速总线时钟=系统时钟*/
	    RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK 设置低速总线2时钟=高速总线时钟*/
	    RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 设置低速总线1的时钟=高速时钟的二分频*/
	    RCC_PCLK1Config(RCC_HCLK_Div2);

		/* ADCCLK = PCLK2/6 设置ADC外设时钟=低速总线2时钟的六分频*/
	    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

		/* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */ 
	    //上面这句例程中缺失了，但却很关键
		/* 利用锁相环讲外部8Mhz晶振9倍频到72Mhz */ 
	    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL 使能锁相环*/
	    RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready 等待锁相环输出稳定*/
	    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	    {}

		/* Select PLL as system clock source 将锁相环输出设置为系统时钟 */
	    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source 等待校验成功*/
	    while (RCC_GetSYSCLKSource() != 0x08)
	    {}
	}
}

