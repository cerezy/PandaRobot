#ifndef _USER_SERVO_H
#define _USER_SERVO_H

#include "user_includes.h"

#define USART_SERVO_TX_SIZE	90
#define USART_SERVO_RX_SIZE	50



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
void User_ServoLegLEFT_IRQHandler(void);
void User_ServoLegRIGHT_IRQHandler(void);
void User_ServoHead_IRQHandler(void);
void User_ServoHeadIRQHandler(void);
void User_UsartServoDataParas(USART_SERVO_TYPEDEF* p_usart_servo_x);
void User_UsartReadServoAng(uint8_t servo_id);
void User_UsartReadServoData(uint8_t servo_id,uint8_t data_id);
void User_UsartSetServoAngTime(uint8_t servo_id,int16_t ang,uint16_t ms);
void User_UsartSetLegAngTime(uint8_t leg_id,int16_t ang[6],uint16_t ms[6]);
void User_UsartSetHeadAngTime(int16_t ang[2],uint16_t ms[2]);
void User_LegAllSetAngTime(void);
void User_SetOriginPoint(uint8_t leg_id);
void User_SeovoActTimeCalcu(void);
void User_SetDamping(uint8_t leg_id,uint16_t Power);
uint8_t User_IsLegAngArrive(uint8_t leg_id);
uint8_t User_IsLegTrigFloor(uint8_t leg_id);

//FEETECH MOTOR
void FEETECH_UsartSetServoPos(uint8_t servo_id,int16_t pos,uint16_t ms,int16_t speed);
void FEETECH_LEGSYNCWRITE(uint8_t leg_id,int16_t pos[5],int16_t ms[5],int16_t speed[5]);
void FEETECH_HEADSYNCWRITE(int16_t pos[5],int16_t ms[5],int16_t speed[5]);
void FEETECH_ReadServoPos(uint8_t servo_id);
void FEETECH_LEGSYNCRead(uint8_t servo_id);
extern int16_t goal_pos[15];//FEETECH POS GOAL
extern int16_t goal_speed[15];//FEETECH SPEED GOAL
extern int16_t goal_ms[15];//FEETECH MS GOAL

#endif










