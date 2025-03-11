#include "user_servo.h"
#include "stdio.h"
#include "string.h"


USART_SERVO_TYPEDEF USART_LEG1 = {0};
USART_SERVO_TYPEDEF USART_LEG2 = {0};
USART_SERVO_TYPEDEF USART_LEG3 = {0};//usart2
USART_SERVO_TYPEDEF USART_LEG4 = {0};//usart2

SERVO_INFO_TYPEDEF SERVO[17] = {0};

uint8_t SERVO_COMM_BUSY = 0;

//伺服舵机初始化
void User_ServoInit(void)
{               
	//各组舵机对应串口
	USART_LEG1.p_usart_n = &huart5;
	USART_LEG1.p_hdma_usart_n_rx = &hdma_uart5_rx;
	USART_LEG2.p_usart_n = &huart3;
	USART_LEG2.p_hdma_usart_n_rx = &hdma_usart3_rx;
	USART_LEG3.p_usart_n = &huart2;
	USART_LEG3.p_hdma_usart_n_rx = &hdma_usart2_rx;
	USART_LEG4.p_usart_n = &huart2;
	USART_LEG4.p_hdma_usart_n_rx = &hdma_usart2_rx;
	
	//各个伺服舵机的初始角度（*10）
	SERVO[1].zero_ang = 0;
	
	
	//__HAL_UART_ENABLE_IT(USART_LEG1.p_usart_n, UART_IT_IDLE);         
	//HAL_UART_Receive_DMA(USART_LEG1.p_usart_n, (uint8_t*)USART_LEG1.usart_rx_buf, USART_SERVO_RX_SIZE);   
	
}

void User_ServoLeg1IRQHandler(void)
{
	if(RESET != __HAL_UART_GET_FLAG(USART_LEG1.p_usart_n, UART_FLAG_IDLE))   
	{
		__HAL_UART_CLEAR_IDLEFLAG(USART_LEG1.p_usart_n);    		
		HAL_UART_DMAStop(USART_LEG1.p_usart_n);                                                        
    USART_LEG1.rx_data_len  = USART_SERVO_RX_SIZE - __HAL_DMA_GET_COUNTER(USART_LEG1.p_hdma_usart_n_rx);  
		User_UsartServoDataParas(&USART_LEG1);///////
		HAL_UART_Receive_DMA(USART_LEG1.p_usart_n, (uint8_t*)USART_LEG1.usart_rx_buf, USART_SERVO_RX_SIZE);
	}
}
 
void User_UsartServoDataParas(USART_SERVO_TYPEDEF* p_usart_servo_x)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	
	for(i=0;i<p_usart_servo_x->rx_data_len-1;i++)
	{
		sum += p_usart_servo_x->usart_rx_buf[i];
	}
	if((p_usart_servo_x->usart_rx_buf[0] == 0x05) && (p_usart_servo_x->usart_rx_buf[1] == 0x1c) && (p_usart_servo_x->usart_rx_buf[i] == sum))
	{
		switch(p_usart_servo_x->usart_rx_buf[2])
		{
			case 0x0a://cmd_id:read_angle
			{
				SERVO[p_usart_servo_x->usart_rx_buf[4]].ang_read = (p_usart_servo_x->usart_rx_buf[5]+(((int16_t)p_usart_servo_x->usart_rx_buf[6])<<8)) - SERVO[p_usart_servo_x->usart_rx_buf[4]].zero_ang;
			}break;
			case 0x03://cmd_id:read_data
			{
				switch(p_usart_servo_x->usart_rx_buf[5])
				{
					case 0x01://data_id:volt
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].volt_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);
					}break;
					case 0x02://data_id:current
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].current_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);					
					}break;
					case 0x03://data_id:power
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].power_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);
					}break;
					case 0x04://data_id:temper
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].temper_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);					
					}break;
				}
			}break;
		}
	}
}

void User_UsartReadServoAng(uint8_t servo_id)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		default:break;
	}
	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// 帧头
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// 帧头
	p_usart_servo_x->usart_tx_buf[2] = 0x0a;// 读取舵机角度的指令
	p_usart_servo_x->usart_tx_buf[3] = 0x01;// 数据包内容的字节长度
	p_usart_servo_x->usart_tx_buf[4] = servo_id;// 数据包内容 舵机ID

	for(i=0;i<=4;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[5] = sum;// 数据包校验和（对0到n-1的字节数据求和，然后跟256取余数）
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,6);
}
void User_UsartReadServoData(uint8_t servo_id,uint8_t data_id)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		default:break;
	}
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// 帧头
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// 帧头
	p_usart_servo_x->usart_tx_buf[2] = 0x03;// 读取舵机数据的指令
	p_usart_servo_x->usart_tx_buf[3] = 0x02;// 数据包内容的字节长度
	p_usart_servo_x->usart_tx_buf[4] = servo_id;// 数据包内容 舵机ID
	p_usart_servo_x->usart_tx_buf[5] = data_id;//1-电压,2-电流，3-功率，4-温度
	for(i=0;i<=5;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[6] = sum;// 数据包校验和（对0到n-1的字节数据求和，然后跟256取余数）
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,7);
}

void User_UsartSetServoAngTime(uint8_t servo_id,int16_t ang,uint16_t ms)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		default:break;
	}
	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// 帧头
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// 帧头
	p_usart_servo_x->usart_tx_buf[2] = 0x08;// 角度控制模式
	p_usart_servo_x->usart_tx_buf[3] = 0x07;// 数据包内容的字节长度
	p_usart_servo_x->usart_tx_buf[4] = servo_id;// 数据包内容 舵机ID
	p_usart_servo_x->usart_tx_buf[5] = ang&0xff;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[6] = ang>>8;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[7] = ms&0xff;//执行时间低8位
	p_usart_servo_x->usart_tx_buf[8] = ms>>8;//执行时间高8位
	p_usart_servo_x->usart_tx_buf[9] = 0;//一般设0
	p_usart_servo_x->usart_tx_buf[10] = 0;//一般设0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[11] = sum;// 数据包校验和（对0到n-1的字节数据求和，然后跟256取余数）
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,12);
}

void User_UsartSetLegAngTime(uint8_t leg_id,int16_t ang[3],uint16_t ms[3])
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(leg_id)
	{
		case 1:p_usart_servo_x = &USART_LEG1;break;
		case 2:p_usart_servo_x = &USART_LEG2;break;
		case 3:p_usart_servo_x = &USART_LEG3;break;
		case 4:p_usart_servo_x = &USART_LEG4;break;
		default:break;
	}	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// 帧头
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// 帧头
	p_usart_servo_x->usart_tx_buf[2] = 0x08;// 角度控制模式
	p_usart_servo_x->usart_tx_buf[3] = 0x07;// 数据包内容的字节长度
	p_usart_servo_x->usart_tx_buf[4] = leg_id*3-2;// 数据包内容 舵机ID
	p_usart_servo_x->usart_tx_buf[5] = ang[0]&0xff;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[6] = ang[0]>>8;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[7] = ms[0]&0xff;//执行时间低8位
	p_usart_servo_x->usart_tx_buf[8] = ms[0]>>8;//执行时间高8位
	p_usart_servo_x->usart_tx_buf[9] = 0;//一般设0
	p_usart_servo_x->usart_tx_buf[10] = 0;//一般设0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[11] = sum;// 数据包校验和（对0到n-1的字节数据求和，然后跟256取余数）	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[12+0] = 0x12;// 帧头
	p_usart_servo_x->usart_tx_buf[12+1] = 0x4c;// 帧头
	p_usart_servo_x->usart_tx_buf[12+2] = 0x08;// 角度控制模式
	p_usart_servo_x->usart_tx_buf[12+3] = 0x07;// 数据包内容的字节长度
	p_usart_servo_x->usart_tx_buf[12+4] = leg_id*3-1;// 数据包内容 舵机ID
	p_usart_servo_x->usart_tx_buf[12+5] = ang[1]&0xff;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[12+6] = ang[1]>>8;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[12+7] = ms[1]&0xff;//执行时间低8位
	p_usart_servo_x->usart_tx_buf[12+8] = ms[1]>>8;//执行时间高8位
	p_usart_servo_x->usart_tx_buf[12+9] = 0;//一般设0
	p_usart_servo_x->usart_tx_buf[12+10] = 0;//一般设0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[12+i];
	}
	p_usart_servo_x->usart_tx_buf[12+11] = sum;// 数据包校验和（对0到n-1的字节数据求和，然后跟256取余数）	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[24+0] = 0x12;// 帧头
	p_usart_servo_x->usart_tx_buf[24+1] = 0x4c;// 帧头
	p_usart_servo_x->usart_tx_buf[24+2] = 0x08;// 角度控制模式
	p_usart_servo_x->usart_tx_buf[24+3] = 0x07;// 数据包内容的字节长度
	p_usart_servo_x->usart_tx_buf[24+4] = leg_id*3;// 数据包内容 舵机ID
	p_usart_servo_x->usart_tx_buf[24+5] = ang[2]&0xff;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[24+6] = ang[2]>>8;//舵机目标角度 低8位
	p_usart_servo_x->usart_tx_buf[24+7] = ms[2]&0xff;//执行时间低8位
	p_usart_servo_x->usart_tx_buf[24+8] = ms[2]>>8;//执行时间高8位
	p_usart_servo_x->usart_tx_buf[24+9] = 0;//一般设0
	p_usart_servo_x->usart_tx_buf[24+10] = 0;//一般设0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[24+i];
	}
	p_usart_servo_x->usart_tx_buf[24+11] = sum;// 数据包校验和（对0到n-1的字节数据求和，然后跟256取余数）	
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,36);
}

void User_LegAllSetAngTime(void) 
{
	int16_t ang_set[3] = {0}; 
	uint16_t ms_set[3] = {0}; 
	
	ang_set[0] = SERVO[1].ang_set + SERVO[1].zero_ang;
	//ang_set[1] = SERVO[2].ang_set + SERVO[2].zero_ang;
	//ang_set[2] = SERVO[3].ang_set + SERVO[3].zero_ang;
	//User_SeovoActTimeCalcu();
	ms_set[0] = 2;//SERVO[1].ms_set;
	//ms_set[1] = SERVO[2].ms_set;
	//ms_set[2] = SERVO[3].ms_set;
	User_UsartSetLegAngTime(1,ang_set,ms_set);
}


uint16_t User_AbsAngErr(int16_t ang)
{
	return (ang>=0)?ang:-ang;
}
uint16_t ACT_TIME_DIV = 100;
void User_SeovoActTimeCalcu(void)
{
	uint8_t i = 0;
	for(i=1;i<=12;i++)
	{
		SERVO[i].ms_set = User_AbsAngErr(SERVO[i].ang_set - SERVO[i].ang_read)/ACT_TIME_DIV;
	}
}
uint16_t ACT_ERR = 20;
uint8_t User_IsLegAngArrive(uint8_t leg_id) 
{
	uint8_t sub_id = 0;
	for(sub_id = 1;sub_id <=3;sub_id++)
	{
		if(User_AbsAngErr(SERVO[(leg_id-1)*3+sub_id].ang_read - SERVO[(leg_id-1)*3+sub_id].ang_set) > ACT_ERR)
			return 0;
	}
	return 1;
}
uint8_t User_IsLegTrigFloor(uint8_t leg_id) 
{
	return 0;
}



