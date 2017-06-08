#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
void TIM1_Init(u16 arr,u16 psc);
void TIM1_SPWM_Init(unsigned int Prescaler);
void TIM8_PWM_Init(void );
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM3_SPWM_Init(u16 squareFrequence ,u16 psc);
void TIM_MasterSlaveModeInit(void);
void LED_GPIO_Config(void);
void TIM2_Configuration(void);
void TIM2_NVIC_Configuration(void);
#endif
