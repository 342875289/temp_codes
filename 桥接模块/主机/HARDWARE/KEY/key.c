#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "oled.h"
#include "stdio.h"	
#include "usart.h"	
#include "esp8266.h"


//���Լ��̴������Ҹ�������뵥Ƭ�����ŵ����ӹ�ϵ							
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



//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	/*GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4*/
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//ֻ��ʼ��KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}


//������̳�ʼ��
void MatrixKey_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	//KEY 5 6 7 8 Ϊ����� //PA.1 PA.3 PC.1 PC.3 Ϊ�����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_Pin ;
 	GPIO_Init(KEY5_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY6_GPIO_Pin ;
 	GPIO_Init(KEY6_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY7_GPIO_Pin ;
 	GPIO_Init(KEY7_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY8_GPIO_Pin ;
 	GPIO_Init(KEY8_GPIO, &GPIO_InitStructure);
	
	//KEY 1 2 3 4 Ϊ������ //PA.5 PA.7 PC.5 PB.1 Ϊ������		
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
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
	//������̳����
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
		delay_ms(50);//ȥ���� 
		if( 
				GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0 ||
				GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0 ||
				GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0 ||
				GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0
		)
			{
				GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);//��һ��
				GPIO_SetBits(KEY6_GPIO, KEY6_GPIO_Pin);
				GPIO_SetBits(KEY7_GPIO, KEY7_GPIO_Pin);
				GPIO_SetBits(KEY8_GPIO, KEY8_GPIO_Pin);
			
				if	   (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 1;}
				else if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 2;}
				else if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 3;}
				else if(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 4;}
				GPIO_SetBits(KEY5_GPIO, KEY5_GPIO_Pin);
				
				GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);//�ڶ���
				if	   (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 5;}
				else if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 6;}
				else if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 7;}
				else if(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 8;}
				
				GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);//������
				if	   (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 9;}
				else if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 10;}
				else if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 11;}
				else if(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0){while(GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_GPIO_Pin)==0);GPIO_ResetBits(KEY5_GPIO, KEY5_GPIO_Pin);GPIO_ResetBits(KEY6_GPIO, KEY6_GPIO_Pin);GPIO_ResetBits(KEY7_GPIO, KEY7_GPIO_Pin);GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);	return 12;}
				GPIO_SetBits(KEY7_GPIO, KEY7_GPIO_Pin);
			
				GPIO_ResetBits(KEY8_GPIO, KEY8_GPIO_Pin);//������
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
������̰���֮��ִ�еĺ���
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

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!

u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}

	//	case  1:{/*step-*/impulse_step--;if(impulse_step<=1){impulse_step=1;}}break;
	//	case  2:{/*step+*/impulse_step++;if(impulse_step>=50){impulse_step=50;}}break;
		
	//	case  3:{/*num_impulse-*/impulse_num-=impulse_step;if(impulse_num<=1){impulse_num=1;}}break;
	//	case  4:{/*num_impulse+*/impulse_num+=impulse_step;if(impulse_num>=500){impulse_num=500;}}break;
	//	case  5:{/**/}break;
	//	case  6:{/*ֹͣ�ϴ�����*/TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE );LCD_Print(0,6,"Send-Data-Closed");}break;
	//	case  7:{/**/}break;
	//	case  8:{/*��������ģʽ*/if(outputstate==0){is_impulse=1;LCD_Print(35,0,"����");TIM_CtrlPWMOutputs(TIM1, ENABLE);TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);}}break;
	//	case  9:{/*��ʼ��WIFI*/Net_Init();}break;
	//	case 10:{/*��ʼ��ʱ��������*/TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );}break;
	//	case 11:{/*�ر����*/TIM_CtrlPWMOutputs(TIM1, DISABLE);TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE );outputstate=0;LCD_Print(35,0,"ֹͣ");}break;
	//	case 12:{/*�������*/TIM_CtrlPWMOutputs(TIM1, ENABLE);TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE );outputstate=1;LCD_Print(35,0,"����");}break;
	//	case 13:{/*f-*/sinFre-=0.5;U-=(int)(0.5*4.4);if(sinFre<=10){sinFre=10;}if(U<=10){U=10;}}break;
	//	case 14:{/*f+*/sinFre+=0.5;U+=(int)(0.5*4.4);if(sinFre>=50){sinFre=50;}if(U>=220){U=220;}}break;
	//	case 15:{/*U-*/U-=3;if(U<=130){U=130;}	sinFre = U /4.4; }break;
	//	case 16:{/*U+*/U+=3;if(U>=220){U=220;}	sinFre = U /4.4; }break;


