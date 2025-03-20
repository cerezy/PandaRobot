#ifndef _USER_SERVO_H
#define _USER_SERVO_H

#include "user_includes.h"

#define USART_SERVO_TX_SIZE	50
#define USART_SERVO_RX_SIZE	40



typedef struct
{
	UART_HandleTypeDef* p_usart_n;
	DMA_HandleTypeDef*	p_hdma_usart_n_rx;
	uint8_t usart_tx_buf[USART_SERVO_TX_SIZE];
	uint8_t usart_rx_buf[USART_SERVO_RX_SIZE];				
	uint8_t rx_data_len;
}USART_SERVO_TYPEDEF;


typedef struct
{
	int16_t ang_set;
	uint16_t ms_set;
	int16_t ang_read;
	uint16_t current_read;
	uint16_t temper_read;
	uint16_t volt_read;
	uint16_t power_read;
	int16_t zero_ang;
}SERVO_INFO_TYPEDEF;
extern SERVO_INFO_TYPEDEF SERVO[17];

extern uint8_t SERVO_COMM_BUSY;

void User_ServoInit(void);
void User_ServoLeg1IRQHandler(void);
void User_ServoLeg2IRQHandler(void);
void User_ServoLeg3IRQHandler(void);
void User_ServoLeg4IRQHandler(void);
void User_ServoHeadIRQHandler(void);
void User_UsartServoDataParas(USART_SERVO_TYPEDEF* p_usart_servo_x);
void User_UsartReadServoAng(uint8_t servo_id);
void User_UsartReadServoData(uint8_t servo_id,uint8_t data_id);
void User_UsartSetServoAngTime(uint8_t servo_id,int16_t ang,uint16_t ms);
void User_UsartSetLegAngTime(uint8_t leg_id,int16_t ang[3],uint16_t ms[3]);
void User_LegAllSetAngTime(void);
void User_SetOriginPoint(uint8_t leg_id);
void User_SeovoActTimeCalcu(void);
uint8_t User_IsLegAngArrive(uint8_t leg_id);
uint8_t User_IsLegTrigFloor(uint8_t leg_id);
#endif










