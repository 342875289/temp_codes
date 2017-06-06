#include "others.h"
#include "math.h"
#include "config_variables.h"
#include "math.h"
#include "string.h"
#include "usart.h"

#define M_PI 3.1415926535898

void process_static_level_data(void)
{
	int temp_data1,temp_data2,target,result_i,result_f;
	double result,temp_angle,param_a,param_b,param_c;
	static double standard_point_delta;
	static u8 cmd[8]="#+";
	u16 CRC_temp;

	temp_data1 = ((int)(STATIC_LEVEL_RX_BUF[1] - '0')<<4) + ((int)(STATIC_LEVEL_RX_BUF[2] - '0')) + ((int)(STATIC_LEVEL_RX_BUF[3] - '0')<<12) + ((int)(STATIC_LEVEL_RX_BUF[4] - '0')<<8);
	temp_data2 = ((int)(STATIC_LEVEL_RX_BUF[5] - '0')<<4) + ((int)(STATIC_LEVEL_RX_BUF[6] - '0')) + ((int)(STATIC_LEVEL_RX_BUF[7] - '0')<<12) + ((int)(STATIC_LEVEL_RX_BUF[8] - '0')<<8);
	
	temp_angle = asin( ( temp_data2/10000.0  - temp_data1/10000.0 ) /16 ) / M_PI *180;
	
	//printf("data1:%d\tdata2:%d\tangle:%lf\t",temp_data1,temp_data2,temp_angle);
	
	target = self_state.target_address;
	param_a = static_level_sensor_profile[target].parameter[0];
	param_b = static_level_sensor_profile[target].parameter[1];
	param_c = static_level_sensor_profile[target].parameter[2];
	
	result = param_a * temp_angle * temp_angle + param_b * temp_angle + param_c;
	//printf("result:%lf\n",result);
	
	if( self_state.is_doing_init != 0 )//采集初始值
	{
//			printf("\n------------\n");
//			printf("\n%d\t%d\n",self_state.is_doing_init,self_state.is_static_level_init);
//			printf("\n------------\n");
		
		 self_state.static_level_original_value[ self_state.is_doing_init - 1 ] = result;
		 self_state.is_doing_init ++;
		 if( self_state.is_doing_init <= 3 )//依次采集初始值
		 {
			  memcpy(&cmd[2] , static_level_sensor_profile[self_state.is_doing_init-1].address ,sizeof(u8)*6);
				usart2_send_string(cmd,8);
		 }
		 else
		 {
			 self_state.is_doing_init = 0;
			 self_state.is_static_level_init = 1;
		 }
	}  
	else if(self_state.is_static_level_init == 1)//初始化已完毕，开始采集数据
	{
			if( self_state.is_sampling_standard_point == 1 )//处理基准点数据
			{
					standard_point_delta = result - self_state.static_level_original_value[2];
					memcpy(&cmd[2] , static_level_sensor_profile[self_state.target_address].address ,sizeof(u8)*6);
					usart2_send_string(cmd,8);
					self_state.is_sampling_standard_point = 0;
			}
			else
			{
					result = (result - self_state.static_level_original_value[target]) - standard_point_delta;
					CONTROL_BUS_TX_BUF[0] = target + 0x04;
					CONTROL_BUS_TX_BUF[1] = 0x03;
					CONTROL_BUS_TX_BUF[2] = 0x04;
					result_i = (int)result;
					result_f = (int)((result - result_i)*10000);

					result_i += 10000;
					result_f += 10000;
					CONTROL_BUS_TX_BUF[3]=(result_i&0xFF00)>>8;//i_high
					CONTROL_BUS_TX_BUF[4]=(result_i&0x00FF);//i_low
					CONTROL_BUS_TX_BUF[5]=(result_f&0xFF00)>>8;//f_high
					CONTROL_BUS_TX_BUF[6]=(result_f&0x00FF);//f_low
					
					CRC_temp = CRC16(CONTROL_BUS_TX_BUF,7);
					CONTROL_BUS_TX_BUF[7]= CRC_temp >> 8 ;//CRC_high
					CONTROL_BUS_TX_BUF[8]= (CRC_temp & 0xFF );//CRC_low
					
					memcpy(static_level_sensor_profile[target].data,CONTROL_BUS_TX_BUF,sizeof(u8)*(8+5));
					static_level_sensor_profile[target].is_new = 1;
					
					
					usart1_send_string(CONTROL_BUS_TX_BUF,9);
			}
			
	}

	
}

void process_control_bus_data(void)
{
	static u8 target_angle_sensor,cmd[8]="#+";
		if(CONTROL_BUS_RX_BUF[0]==self_state.self_address)//模块参数设置命令
		{
				if(  (CONTROL_BUS_RX_BUF[1]==0x06) && (CONTROL_BUS_RX_BUF[3]==0x0A) && (CONTROL_BUS_RX_BUF[5]==0x01) )
				{
						memcpy(&cmd[2] , static_level_sensor_profile[0].address ,sizeof(u8)*6);
						usart2_send_string(cmd,8);
						self_state.is_static_level_init = 0;
						self_state.is_doing_init = 1;
				}
		}
		else if(   ((CONTROL_BUS_RX_BUF[0]>=0x01) || (CONTROL_BUS_RX_BUF[0]<=0x03) )   &&   
								(CONTROL_BUS_RX_BUF[1] == 0x03)   	&&
								(CONTROL_BUS_RX_BUF[2] == 0x08)  )//传感器倾角数据
		{
			target_angle_sensor = CONTROL_BUS_RX_BUF[0];
			memcpy(angle_sensor_profile[target_angle_sensor].data,CONTROL_BUS_RX_BUF,sizeof(u8)*(8+5));
			angle_sensor_profile[target_angle_sensor].is_new = 1;
			
		}
		else if(  	( self_state.is_static_level_init == 1 )  &&
								((CONTROL_BUS_RX_BUF[0]>=0x04) || (CONTROL_BUS_RX_BUF[0]<=0x06) )   &&   
								(CONTROL_BUS_RX_BUF[1] == 0x03)   	&&
								(CONTROL_BUS_RX_BUF[3] == 0x01)  )//读取静力水准仪的命令
		{

				self_state.target_address = CONTROL_BUS_RX_BUF[0]-4;
				memcpy(&cmd[2] , static_level_sensor_profile[self_state.target_address].address ,sizeof(u8)*6);
				usart2_send_string(cmd,8);
				self_state.is_sampling_standard_point = 1;
		}
}
