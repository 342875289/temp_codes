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
		  
	 
		//TIM1_Init(2000,7200);//200ms ��ʱ��
		//UART3_Init(9600);
	  //RS485_ControlIO_Init();
	 
		//Lcd_Init();			//��ʼ��OLED
		//LCD_Clear(WHITE); //����
		//BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
		//xianshi(); //��ʾ��Ϣ
		//showimage(); //��ʾ40*40ͼƬ
	 
	 
		//RS485_TX_MODE_ENABLE();
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
