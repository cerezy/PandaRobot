#ifndef _user_adc_h
#define _user_adc_h

#include "user_includes.h"

#define ADC3_CH_NUM	1

typedef struct
{
	uint16_t adc3_dma_buf[ADC3_CH_NUM];	
	
	float bat_volt;
	uint16_t bat_power;
	
}USER_ADC_TYPE;

extern USER_ADC_TYPE USER_ADC;


void User_AdcInit(void);
void User_AdcDmaIRQHandler(void);
void User_AdcBatVoltGet(void);

#endif










