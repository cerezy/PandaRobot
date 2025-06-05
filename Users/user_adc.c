#include "user_adc.h"
#include "adc.h"

USER_ADC_TYPE USER_ADC; 

uint8_t workflag = 0;
void User_AdcBatVoltGet(void)
{
	USER_ADC.adc3_dma_buf[0] = HAL_ADC_GetValue(&hadc3);
	static uint8_t cnt = 0;
	static float sum = 0;
	
	cnt++;
	if(cnt <= 100)
	{
		sum += USER_ADC.adc3_dma_buf[0]*3.3/4095;
	}
	if(cnt == 100)
	{
		workflag = 1;
		USER_ADC.bat_volt = sum/100*6;
		USER_ADC.bat_power = sum/100/2*100;
		cnt = 0;
		sum = 0;
	}
}
void User_AdcInit(void)
{
	//HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);//默认是开启DMA中断的

//	HAL_ADCEx_Calibration_Start(&hadc3,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);//F4无需自校准！！！
//	
//	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)USER_ADC.adc3_dma_buf, ADC3_CH_NUM);
	USER_ADC.bat_volt = 12.0;
	USER_ADC.bat_power = 100;
	
}


void User_AdcDmaIRQHandler(void)
{

}






