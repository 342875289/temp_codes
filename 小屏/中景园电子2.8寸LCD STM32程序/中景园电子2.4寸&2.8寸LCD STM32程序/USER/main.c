#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
//˵��: 
//-��lcd���������ӿڣ�BLKΪ���⿪�أ�Ĭ�Ͽ��Բ���
//GND    ��Դ��
//VCC    3.3v��Դ
//CLK    ��PD6 	PA5
//MOSI   ��PD7	PA7
//RES    ��PD4 	PA6
//DC     ��PD5 	PA4
//BLK���Բ���
//MISO����
 int main(void)
 {	
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		//Lcd_Init();			//��ʼ��OLED  
		//TIM1_Init(2000,7200);//200ms ��ʱ��
		//LCD_Clear(WHITE); //����
		//BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
		//xianshi(); //��ʾ��Ϣ
		//showimage(); //��ʾ40*40ͼƬ
		while(1)
		{
			;

		}
}
