#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "config_variables.h"
//说明: 
//-此lcd不带触摸接口，BLK为背光开关，默认可以不接
//GND    电源地
//VCC    3.3v电源
//CLK    接PA5
//MOSI   接PA7
//RES    接PA6
//DC     接PB0	高为发送数据  低为发送命令
//BLK可以不接
//MISO不接
 int main(void)
 {	
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
		  
	 
	//	TIM1_Init(20000,7200);//2000ms 定时器
		UART3_Init(9600);
	  UART1_Init(9600);
	  
	 
		Lcd_Init();			//初始化OLED
		LCD_Clear(WHITE); //清屏
		BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
		xianshi(); //显示信息
		showimage(); //显示40*40图片
	 
		RS485_ControlIO_Init();
	 	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM1中断,允许更新中断
		TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设		
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
