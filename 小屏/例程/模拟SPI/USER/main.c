
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"

//说明: 
//-此lcd不带触摸接口，BLK为背光开关，默认可以不接
//GND    电源地
//VCC    3.3v电源
//CLK    接PD6 	PA5 --sck
//MOSI   接PD7	PA7 --mosi
//RES    接PD4 	PA6 --miso
//DC     接PD5 	PB0 --pb0   高为发送数据  低为发送命令
//BLK可以不接
//MISO不接


u8 ref=0;//刷新显示
u16 vx=15542,vy=11165;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
u16 chx=140,chy=146;//默认像素点坐标为0时的AD起始值
void xianshi()//显示信息
{   
	u16 lx,ly;
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
	showhanzi(10,0,0);  //中
	showhanzi(45,0,1);  //景
	showhanzi(80,0,2);  //园
	showhanzi(115,0,3);  //电
	showhanzi(150,0,4);  //子
  LCD_ShowString(10,35,"2.4 TFT SPI 240*320");
	LCD_ShowString(10,55,"LCD_W:");	LCD_ShowNum(70,55,LCD_W,3);
	LCD_ShowString(110,55,"LCD_H:");LCD_ShowNum(160,55,LCD_H,3);	
	lx=10;ly=75;			
}



void showimage() //显示40*40图片
{
  	int i,j,k; 

	for(k=3;k<8;k++)
	{
	   	for(j=0;j<6;j++)
		{	
			Address_set(40*j,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<1600;i++)
			 { 	
				 		
			  	 LCD_WR_DATA8(image[i*2+1]);	 
				 LCD_WR_DATA8(image[i*2]);				
			 }	
		 }
	}
	ref=0;				
}
 int main(void)
 {	u8 t;
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化

	 Lcd_Init();			//初始化OLED  
	
	LCD_Clear(WHITE); //清屏
	BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 

	xianshi(); //显示信息
    showimage(); //显示40*40图片


	while(1)
	{
	  ;
    
}
}
