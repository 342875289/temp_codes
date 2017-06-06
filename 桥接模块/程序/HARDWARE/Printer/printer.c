#include "printer.h"
#include "config_variables.h"
#include "usart.h"
#include "delay.h"


#define coef 0.57735026918963

u8 fac_us=72000000/8000000;	
u16 fac_ms=72000000/8000000*1000;

u8 Cmd_check_printer_state[3]={0x1B,0x76,0x01};
u8 Cmd_print_test_paper[2]={0x12,0x54};
u8 Cmd_ptinter_reset[2]={0x1B,0x40};
u8 Cmd_Set_bold_true[3]={0x1B,0x45,0x01};//进入粗体打印模式
u8 Cmd_Set_bold_false[3]={0x1B,0x45,0x00};//退出粗体打印莫使
u8 Cmd_Set_printer_parameter[5]={0x1B,0x37,0x09,0x80,0x02};

u8 Cmd_Output_Enter[1]={0x0A};


void Printer_Init(void)
{
	check_PrinterState();
	 //输出--打印机总状态-连接状态-纸量-机芯
//  RS485_TX_MODE_ENABLE();
//	
//	USART_SendData(USART3,0x55);while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
//	 if(printer_state.isAllOK == 0){while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x00);}
//	 else{while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x01);}	 
//	 if(printer_state.isConnect == 0){while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x00);}
//	 else{while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x01);}	 
//	 if(printer_state.isHavePaper == 0){while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x00);}
//	 else{while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x01);}	 
//	 if(printer_state.isHardwareOK == 0){while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x00);}
//	 else{while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);USART_SendData(USART3,0x01);}	 
//	 //进入接收模式
//	 RS485_RX_MODE_ENABLE();
}

void check_PrinterState(void)
{
	u8 data=0xff; 
	usart1_send_string(Cmd_check_printer_state,3);
	if( ReceivePrinterData(3,&data)  )
	{
		printer_state.isConnect = 1;
		
		//检查打印机机芯是否连接
		if(  (data&0x01)   ){printer_state.isHardwareOK = 0;}
		else{printer_state.isHardwareOK = 1;}
		//检查打印机是否缺纸
		if(  (data&0x04)   ){printer_state.isHavePaper = 0;}
		else{printer_state.isHavePaper = 1;}                                                                              
		//判断打印机是否可以使用
		if( printer_state.isHardwareOK == 1  && printer_state.isHavePaper == 1)
		{
			printer_state.isAllOK = 1;
		}
	}
	else
	{
		printer_state.isConnect = 0;
		printer_state.isHavePaper = 0;
		printer_state.isHardwareOK = 0;
		printer_state.isAllOK = 0;
	}
}
int ReceivePrinterData(int seconds,u8 *data)
{
	u32 temp;
	u16 nms=1000;
	while(seconds--)
	{
		SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
		SysTick->VAL =0x00;							//清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
		do
		{
			if( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
			{
				(*data) = USART_ReceiveData(USART1);
				//接收成功
				return 1;
			}
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
		SysTick->VAL =0X00;       					//清空计数器	
	}
	return 0;
}

void printSampleData(float xy[256][2] , int num)
{
	int i;
//	double average_x=0.0,average_y=0.0;
	float result_Pin4[2],result_Pin3[2];
	if(num >255 ){num = 255;}
	
	cal_result(xy,result_Pin4 ,result_Pin3);
	
	
	usart1_send_string(Cmd_Output_Enter,1);
	printf("开始打印采样数据,计划采样%d个数据点,已采样%d个数据点.\r\n",self_state.sample_num,num);
	
	for(i=1;i<=num;i++)
	{
		//usart1_send_string(Cmd_Output_Enter,1);
		//printf("第%d个采样点数据:\r\nX:%.4f  Y:%.4f  \r\n",i,xy[i][0],xy[i][1]);
		printf("%d:  %.4f °\r\n",i,xy[i][1]);
		//average_x += xy[i][0];
		//average_y += xy[i][1];
	}
	
	usart1_send_string(Cmd_Output_Enter,1);
	
	//average_x = average_x / num;
	//average_y = average_y / num;
	//printf("平均数据为:\r\n");
	//printf("X:%.4f    Y:%.4f    \r\n",average_x,average_y);
	//printf("X:%.4f\r\n",average_x);
	
	
	printf("平台调整方案如下：\r\n");
	if( self_state.isSampleByClockwise == 1){printf("结果按照 顺时针 采集顺序计算\r\n");}
	else{printf("结果按照 逆时针 采集顺序计算\r\n");}
	
	printf("一、十字型平台调整方法：\r\n");

	printf(" ╭───────╮\r\n");
	printf("│       ╋A      │\r\n");
	printf("│    ↑X正方向   │\r\n");
	printf("│                │\r\n");
	printf("│╋D          C╋│\r\n");
	printf("│    →Y正方向   │\r\n");
	printf("│                │\r\n");
	printf("│       ╋B      │\r\n");
	printf("╰────────╯\r\n");

	printf("A-B连线方向调整：\r\n%.1f 密位 ( %.4f 度)\r\n",result_Pin4[0]/0.06,result_Pin4[0]);
	printf("C-D连线方向调整：\r\n%.1f 密位 ( %.4f 度)\r\n",result_Pin4[1]/0.06,result_Pin4[1]);
	
	usart1_send_string(Cmd_Output_Enter,1);
	
	printf("二、三角型平台调整方法：\r\n");
	printf(" ╭───────╮\r\n");
	printf("│       ╋       │\r\n");
	printf("│    ↑X正方向   │\r\n");
	printf("│                │\r\n");
	printf("│    →Y正方向   │\r\n");
	printf("│                │\r\n");
	printf("│                │\r\n");
	printf("│╋A         ╋B │\r\n");
	printf("╰────────╯\r\n");

	printf("A点调整：%.1f 密位  %.4f 度\r\n",result_Pin3[0]/0.06,result_Pin3[0]);
	printf("B点调整：%.1f 密位  %.4f 度\r\n",result_Pin3[1]/0.06,result_Pin3[1]);

	printf("数据打印完毕\r\n");
	usart1_send_string(Cmd_Output_Enter,1);
	usart1_send_string(Cmd_Output_Enter,1);
	usart1_send_string(Cmd_Output_Enter,1);
	usart1_send_string(Cmd_Output_Enter,1);
}

void cal_result(float xy[256][2],float result_Pin4[2] , float result_Pin3[2])
{
	//
	switch(self_state.sample_num)
	{
		case  4 :
							result_Pin4[0] = xy[1][1] -  xy[3][1];
							result_Pin4[1] = xy[2][1] -  xy[4][1];
							break;
		case  8 :
							result_Pin4[0] = xy[1][1] -  xy[5][1];
							result_Pin4[1] = xy[3][1] -  xy[7][1];
							break;
		case 12 :
							result_Pin4[0] = xy[1][1] -  xy[7][1];
							result_Pin4[1] = xy[4][1] -  xy[10][1];
							break;
	}
	
	result_Pin3[0] = - result_Pin4[0]  -  coef*result_Pin4[1]  ;
	result_Pin3[1] = - result_Pin4[0]  + coef*result_Pin4[1]  ;
	
	if( self_state.isSampleByClockwise != 1){ result_Pin4[1] = - result_Pin4[1] ; result_Pin3[1] = - result_Pin3[1] ;}

	
	
	result_Pin4[0] = - result_Pin4[0] ;
	result_Pin4[1] = - result_Pin4[1] ;
	result_Pin3[0] = - result_Pin3[0] ;
	result_Pin3[1] = - result_Pin3[1] ;
	
	
}
