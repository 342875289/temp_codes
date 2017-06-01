#ifndef __CONFIG_VARIABLES_H
#define __CONFIG_VARIABLES_H

#include "stm32f10x.h"

#define USART_BEFFER_SIZE 40
#define   NRF_BEFFER_SIZE 40

enum SelfCmd{
	SetAutoSampleModel_targetAddress = 0x01,
	SetAutoSampleModel_sampleNumber ,
	SampleData,
	PrintData,
	ReturnSampleData,
	StartCorrect = 0x09,
	EndCorrect,
	SamplePoint1,
	SamplePoint2,
	ChangeOutputMode,
	SampleByClockwise,
	SampleNotByClockwise,
	ReturnResult_Pin4 = 0x10,
	ReturnResult_Pin3 = 0x14
};

extern enum workstates workstate;
extern u8 STATIC_LEVEL_RX_BUF[];
extern u8 CONTROL_BUS_RX_BUF[];
extern u8 USART_RX_BUF[];
extern u8 USART_TX_BUF[];
extern u8 NRF_RX_BUF[];
extern u8 NRF_TX_BUF[];
extern u8 MASTER_TX_BUF[];
extern u8 Result_Pin4_TX_BUF[];
extern u8 Result_Pin3_TX_BUF[];

extern u8 newCmd;
extern u8 newSelfCmd;
extern u8 cmdNum ;

extern u8 static_level_data_received;
extern u8 control_bus_data_received;

extern float xy[256][2];
extern float result_Pin4[2];
extern float result_Pin3[2];

extern struct sensor_data static_level_sensor_profile[3];
extern struct sensor_data angle_sensor_profile[3];
extern struct Self_State self_state;
struct Self_State{
	u8 self_address;
	u8 target_address;
};

struct sensor_data{
	u8 address[6];
	double parameter[3];
	u8 data[13];
	u8 is_new ;
	
};

#endif
