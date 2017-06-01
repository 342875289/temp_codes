#include "others.h"
#include "math.h"
#include "config_variables.h"
#include "math.h"

#define M_PI 3.1415926535898

void process_static_level_data(void)
{
	int temp_data1,temp_data2,target;
	double result,temp_angle,param_a,param_b,param_c;
		 
	temp_data1 = ((int)(STATIC_LEVEL_RX_BUF[1] - '0')<<4) + ((int)(STATIC_LEVEL_RX_BUF[2] - '0')) + ((int)(STATIC_LEVEL_RX_BUF[3] - '0')<<12) + ((int)(STATIC_LEVEL_RX_BUF[4] - '0')<<8);
	temp_data2 = ((int)(STATIC_LEVEL_RX_BUF[5] - '0')<<4) + ((int)(STATIC_LEVEL_RX_BUF[6] - '0')) + ((int)(STATIC_LEVEL_RX_BUF[7] - '0')<<12) + ((int)(STATIC_LEVEL_RX_BUF[8] - '0')<<8);
	
	temp_angle = asin( ( temp_data2/10000.0  - temp_data1/10000.0 ) /16 ) / M_PI *180;
	
	//printf("data1:%d\tdata2:%d\tangle:%lf\t",temp_data1,temp_data2,temp_angle);
	
	target = self_state.target_address - 1;
	param_a = static_level_sensor_profile[target].parameter[0];
	param_b = static_level_sensor_profile[target].parameter[1];
	param_c = static_level_sensor_profile[target].parameter[2];
	
	result = param_a * temp_angle * temp_angle + param_b * temp_angle + param_c;
	
	//printf("result:%lf\n",result);
}