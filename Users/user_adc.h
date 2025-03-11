#ifndef _user_adc_h
#define _user_adc_h

#include "user_includes.h"

#define ADC1_CH_NUM	2

typedef struct
{
	uint16_t adc1_dma_buf[ADC1_CH_NUM];	
	
	float bat_volt;
	
}USER_ADC_TYPE;
extern USER_ADC_TYPE USER_ADC;


void User_AdcInit(void);
void User_AdcDmaIRQHandler(void);
void User_AdcBatVoltGet(void);

#endif










