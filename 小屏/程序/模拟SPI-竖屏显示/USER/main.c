#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "config_variables.h"
//˵��: 
//-��lcd���������ӿڣ�BLKΪ���⿪�أ�Ĭ�Ͽ��Բ���
//GND    ��Դ��
//VCC    3.3v��Դ
//CLK    ��PA5
//MOSI   ��PA7
//RES    ��PA6
//DC     ��PB0	��Ϊ��������  ��Ϊ��������
//BLK���Բ���
//MISO����
 int main(void)
 {	
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		  
	 
	//	TIM1_Init(20000,7200);//2000ms ��ʱ��
		UART3_Init(9600);
	  UART1_Init(9600);
	  
	 
		Lcd_Init();			//��ʼ��OLED
		LCD_Clear(WHITE); //����
		BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
		xianshi(); //��ʾ��Ϣ
		showimage(); //��ʾ40*40ͼƬ
	 
		RS485_ControlIO_Init();
	 	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж�
		TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����		
		while(1)
		{
//			if( newCmd == 1 )
//			{
//				confirm_Sensor_Connect();
//				newCmd = 0;
//			}
			;
		}
}
