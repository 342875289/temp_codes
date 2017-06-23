#define M_PI 3.1415926

typedef unsigned char u8;

u8 jizhun_1[]="=2298799";
u8 jizhun_2[]="=2298899";

u8 celiang_1[]=":5521<6=";//053>2=84  
u8 celiang_2[]=":4521<6=";//0:3>3384

float cal(u8 BUF[],int i)
{
	int temp_data1,temp_data2,target,result_i,result_f;
	double result,temp_angle,param_a,param_b,param_c;
	static double standard_point_delta;
	
	temp_data1 = ((int)(BUF[1] - '0')<<4) + ((int)(BUF[2] - '0')) + ((int)(BUF[3] - '0')<<12) + ((int)(BUF[4] - '0')<<8);
	temp_data2 = ((int)(BUF[5] - '0')<<4) + ((int)(BUF[6] - '0')) + ((int)(BUF[7] - '0')<<12) + ((int)(BUF[8] - '0')<<8);
	
	temp_angle = asin( ( temp_data2/10000.0  - temp_data1/10000.0 ) /16 ) / M_PI *180;
	
	switch(i)
	{
		case 1:param_a = 0.00744;param_b = 2.15329;param_c = 27.63270;break;
		case 2:param_a = 0.00737;param_b = 2.16623;param_c = 27.93397;break;
		case 3:param_a = 0.00714;param_b = 2.12315;param_c = 27.76165;break;
	}
	
	result = param_a * temp_angle * temp_angle + param_b * temp_angle + param_c;
	return result;
}

void main()
{
	float result;
	result = (cal(celiang_2,1) - cal(celiang_1,1) ) -  (cal(jizhun_2,3) - cal(jizhun_1,3));
	printf("%f\n",result);
	
	
}
