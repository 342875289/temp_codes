#include "timer.h"
#include "usart.h"
					  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3
void TIM3_Init(int arr,int psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	//TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设	 
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
}


void TIM1_Init(unsigned int Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
						 
	/* TIM1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/* GPIO Configuration */

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Period = 3600-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =Prescaler-1; //设置用来作为TIMx时钟频率除数的预分频值  为0 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;  //TIM中央对齐模式1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;//设置周期重复次数为1
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	//TIM1更新中断
  //TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM1中断,允许更新中断
	
  //设置TIME1中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级(最高级)
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级最高级)
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);        
	/* TIM1 counter disable */
	TIM_Cmd(TIM1, DISABLE);
}
