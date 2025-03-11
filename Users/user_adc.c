#include "user_adc.h"

USER_ADC_TYPE USER_ADC; 

void User_AdcInit(void)
{
//  HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);//默认是开启DMA中断的

//	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);F4无需自校准！！！
	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)USER_ADC.adc1_dma_buf, ADC1_CH_NUM);
	USER_ADC.bat_volt = 12.0;
}


void User_AdcDmaIRQHandler(void)
{

}



void User_AdcBatVoltGet(void)
{
	static uint8_t cnt = 0;
	static float sum = 0;
	cnt++;
	if(cnt <= 100)
	{
		sum += USER_ADC.adc1_dma_buf[1]*3.3/4096*8.13;
	}
	if(cnt == 100)
	{
		USER_ADC.bat_volt = sum/100;
		cnt = 0;
		sum = 0;
	}	
}





