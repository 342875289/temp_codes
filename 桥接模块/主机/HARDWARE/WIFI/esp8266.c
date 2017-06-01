#include "esp8266.h"

int rn_count = 0;//�Խ�����Ϣ�еĻ��з��������յ�3��\r\n��Ϊ��������
char wifi_feedback[200];//���ڴ�Ž������ݵ�����
int data_i=0;//�����������������
int time_wait=3;//�������ݵȴ���ʱ��(3��)
int receive_state=0;//Ϊ0��ʾ��û���յ�����  Ϊ1��ʾ���ڽ������� Ϊ2��ʾ�������ݵĹ�����������
u8 fac_us=72000000/8000000;	
u16 fac_ms=72000000/8000000*1000;
//ʹ��esp8266
void wifi_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTBʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;		//PB.12
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//ʹ��esp8266
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
//��������WiFi�������ɹ�����1��ʧ�ܷ���0
int Connect_master()
{
	//�ȴ�ģ�������������
	delay_ms(1000);
	delay_ms(1000);

	//�ر��������
	printf("ATE0\r\n");
	if(Re(2)==1){;}
	else{return 0;}
	
	
	//�ӻ�����AP�ȵ�
	printf("AT+CWJAP=\"lift-project\",\"labcat127\"\r\n");
	if(Re(3)==1){;}
	else{return 0;}
	
	//�ӻ�����STAģʽ�µ�IP
	printf("AT+CIPSTA=\"192.168.4.3\"\r\n");
	if(Re(2)==1){;}
	else{return 0;}
	
	//�ӻ�����Ϊ������
	printf("AT+CIPMUX=0\r\n");
	if(Re(2)==1){;}
	else{return 0;}
	
	//�ӻ���������
	printf("AT+CIPSTART=\"TCP\",\"192.168.4.1\",8088\r\n");
	if(Re(2)==1)
	{
		//delay_ms(1000);delay_ms(1000);delay_ms(1000);
		/* Clear the Overrun Error interrupt pending bit */
		//USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�;
	}
	else{return 0;}
	
	/*
	//�ӻ�����������ָ���ֽ�������Ϣ
	printf("AT+CIPSEND=XX\r\n");
	if(Re(10)==1){;}
	else{return 0;}
	delay_ms(1000);
	*/
	
	/*
	//�ӻ��Ͽ�������������
	printf("AT+CIPCLOSE\r\n");
	if(Re(10)==1){;}
	else{return 0;}
	delay_ms(1000);
	delay_ms(1000);
	*/
	
	
	return 1;
	
}

int Re(int seconds)
{
	u32 temp;
	u8 data;
	u16 nms=1000;
	int times=seconds;
	data_i=0;
	
	while( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET );//�ȴ���һ���ֽ�
	
	while(times--)
	{
		SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
		SysTick->VAL =0x00;							//��ռ�����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
		do
		{
			if( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
			{
				data = USART_ReceiveData(USART3);
				wifi_feedback[data_i]=data ;
				//OK
				if( wifi_feedback[data_i] == 'K' && data_i>=1 && 
					wifi_feedback[data_i-1] == 'O' )
				{
					SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
					SysTick->VAL =0X00;       					//��ռ�����	
					return 1;//���سɹ���Ϣ
				}
				//ERROR
				if( wifi_feedback[data_i] == 'R' && data_i>=4 && 
					wifi_feedback[data_i-1] == 'O' &&
					wifi_feedback[data_i-2] == 'R' &&
					wifi_feedback[data_i-3] == 'R' &&
					wifi_feedback[data_i-4] == 'E' 
				)
				{
					SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
					SysTick->VAL =0X00;       					//��ռ�����	
					return 0;//����ʧ����Ϣ
				}
				
				//���ճɹ�-����װ�ص���ʱ����
				times=seconds;
				data_i++;
			}
			
			
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
		SysTick->VAL =0X00;       					//��ռ�����	
	}
	return 0;
}



void USART3_IRQHandler(void)                	//����3�жϷ������
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//ʧ�ܽ����ж�--׼�����������ַ�
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	}
} 
