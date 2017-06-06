#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "oled.h"
#include "stdio.h"	
#include "usart.h"	
#include "esp8266.h"


//正对键盘从左往右各个针脚与单片机引脚的连接关系							
#define KEY1_GPIO		GPIOA
#define KEY1_GPIO_Pin	GPIO_Pin_11

#define KEY2_GPIO		GPIOA
#define KEY2_GPIO_Pin	GPIO_Pin_13

#define KEY3_GPIO		GPIOA
#define KEY3_GPIO_Pin	GPIO_Pin_15

#define KEY4_GPIO		GPIOC
#define KEY4_GPIO_Pin	GPIO_Pin_11

#define KEY5_GPIO		GPIOD
#define KEY5_GPIO_Pin	GPIO_Pin_2

#define KEY6_GPIO		GPIOB
#define KEY6_GPIO_Pin	GPIO_Pin_4

#define KEY7_GPIO		GPIOB
#define KEY7_GPIO_Pin	GPIO_Pin_6

#define KEY8_GPIO		GPIOB
#define KEY8_GPIO_Pin	GPIO_Pin_8



//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	/*GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4*/
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//只初始化KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}


//矩阵键盘初始化
void MatrixKey_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	//KEY 5 6 7 8 为输出线 //PA.1 PA.3 PC.1 PC.3 为输出线  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_Pin ;
 	GPIO_Init(KEY5_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY6_GPIO_Pin ;
 	GPIO_Init(KEY6_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY7_GPIO_Pin ;
 	GPIO_Init(KEY7_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY8_GPIO_Pin ;
 	GPIO_Init(KEY8_GPIO, &GPIO_InitStructure);
	
	//KEY 1 2 3 4 为输入线 //PA.5 PA.7 PC.5 PB.1 为输入线		
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_Pin ;
 	GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_Pin ;
 	GPIO_Init(KEY2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_Pin ;
 	GPIO_Init(KEY3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_Pin ;
 	GPIO_Init(KEY4_GPIO, &GPIO_InitStructure);
	
	GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);
	GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);
	GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);
	GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);
}
/*
	//矩阵键盘朝外放
	1-->D  2-->#  3-->0  4-->*
	5-->C  6-->9  7-->8  8-->7
	9-->B  10->6  11->5  12->4
	13->A  14->3  15->2  16->1
	*/
int MatrixKey_Scan(void)
{
	if( 
			GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0 ||
			GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0 ||
			GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0 ||
			GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0
		)
	{
		delay_ms(50);//去抖动 
		if( 
				GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0 ||
				GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0 ||
				GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0 ||
				GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0
		)
			{
				GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);//第一组
				GPIO_SetBits(KEY6_GPIO, KEY6_GPIO_Pin);
				GPIO_SetBits(KEY7_GPIO, KEY7_GPIO_Pin);
				GPIO_SetBits(KEY8_GPIO, KEY8_GPIO_Pin);
			
				if	   (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 1;}
				else if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 2;}
				else if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 3;}
				else if(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 4;}
				GPIO_SetBits(KEY5_GPIO, KEY5_GPIO_Pin);
				
				GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);//第二组
				if	   (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 5;}
				else if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 6;}
				else if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 7;}
				else if(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 8;}
				
				GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);//第三组
				if	   (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 9;}
				else if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 10;}
				else if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 11;}
				else if(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 12;}
				GPIO_SetBits(KEY7_GPIO, KEY7_GPIO_Pin);
			
				GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);//第四组
				if	   (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 13;}
				else if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 14;}
				else if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 15;}
				else if(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 16;}
				
				GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);
				GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);
				GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);
				GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);
			}
			
	}   
	GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);
	GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);
	GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);
	GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);
	return 0;
}

/*
矩阵键盘按下之后执行的函数
*/

void keyfunc(int key)
{
	if(key!=0)
	{
		printf("key: %d\r\n",key);
	}
	
	/*
	switch(key)
	{
		;
	}
	*/
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!

u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
}

	//	case  1:{/*step-*/impulse_step--;if(impulse_step<=1){impulse_step=1;}}break;
	//	case  2:{/*step+*/impulse_step++;if(impulse_step>=50){impulse_step=50;}}break;
		
	//	case  3:{/*num_impulse-*/impulse_num-=impulse_step;if(impulse_num<=1){impulse_num=1;}}break;
	//	case  4:{/*num_impulse+*/impulse_num+=impulse_step;if(impulse_num>=500){impulse_num=500;}}break;
	//	case  5:{/**/}break;
	//	case  6:{/*停止上传数据*/TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE );LCD_Print(0,6,"Send-Data-Closed");}break;
	//	case  7:{/**/}break;
	//	case  8:{/*启动脉冲模式*/if(outputstate==0){is_impulse=1;LCD_Print(35,0,"脉冲");TIM_CtrlPWMOutputs(TIM1, ENABLE);TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);}}break;
	//	case  9:{/*初始化WIFI*/Net_Init();}break;
	//	case 10:{/*开始定时接收数据*/TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );}break;
	//	case 11:{/*关闭输出*/TIM_CtrlPWMOutputs(TIM1, DISABLE);TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE );outputstate=0;LCD_Print(35,0,"停止");}break;
	//	case 12:{/*启动输出*/TIM_CtrlPWMOutputs(TIM1, ENABLE);TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE );outputstate=1;LCD_Print(35,0,"启动");}break;
	//	case 13:{/*f-*/sinFre-=0.5;U-=(int)(0.5*4.4);if(sinFre<=10){sinFre=10;}if(U<=10){U=10;}}break;
	//	case 14:{/*f+*/sinFre+=0.5;U+=(int)(0.5*4.4);if(sinFre>=50){sinFre=50;}if(U>=220){U=220;}}break;
	//	case 15:{/*U-*/U-=3;if(U<=130){U=130;}	sinFre = U /4.4; }break;
	//	case 16:{/*U+*/U+=3;if(U>=220){U=220;}	sinFre = U /4.4; }break;


