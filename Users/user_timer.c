#include "user_timer.h"

//设置模式
#define _CMD_TYPE_SET 		0
//读取模式
#define _CMD_TYPE_READ 		1

#define _READ_TYPE_ANG 		0
#define _READ_TYPE_DATA 	1

#define _DATA_TYPE_VOLT 	1
#define _DATA_TYPE_CUR 		2
#define _DATA_TYPE_POWER 	3
#define _DATA_TYPE_TEMP 	4

TIMER_TYPEDEF TIMER = {0};

void User_TimerInit(void)
{
	HAL_TIM_Base_Start_IT(&USER_htim_servo);//  	
}
/*
舵机控制：
定时器周期：1ms
控制步骤：
				1ms->各串口DMA发送该串口总线上各舵机（6个）角度和执行时间（1.5ms）
				3ms->各串口DMA发送该串口总线上第1个舵机角度读取请求指令（400us）
				4ms->各串口DMA发送该串口总线上第2个舵机角度读取请求指令（400us）
				5ms->各串口DMA发送该串口总线上第3个舵机角度读取请求指令（400us）
				6ms->各串口DMA发送该串口总线上第4个舵机角度读取请求指令（400us）
				7ms->各串口DMA发送该串口总线上第5个舵机角度读取请求指令（400us）
				8ms->各串口DMA发送该串口总线上第6个舵机角度读取请求指令（400us）
				
				9ms->各串口DMA发送该串口总线上各舵机（6个）角度和执行时间（1.5ms）
				11ms->各串口DMA发送该串口总线上第1个舵机数据（电压/电流/功率/温度）读取请求指令（10次中电压1次温度1次电流8次）（400us）
				12ms->各串口DMA发送该串口总线上第2个舵机数据（电压/电流/功率/温度）读取请求指令（10次中电压1次温度1次电流8次）（400us）
				13ms->各串口DMA发送该串口总线上第3个舵机数据（电压/电流/功率/温度）读取请求指令（10次中电压1次温度1次电流8次）（400us）
				14ms->各串口DMA发送该串口总线上第4个舵机数据（电压/电流/功率/温度）读取请求指令（10次中电压1次温度1次电流8次）（400us）
				15ms->各串口DMA发送该串口总线上第5个舵机数据（电压/电流/功率/温度）读取请求指令（10次中电压1次温度1次电流8次）（400us）
				16ms->各串口DMA发送该串口总线上第6个舵机数据（电压/电流/功率/温度）读取请求指令（10次中电压1次温度1次电流8次）（400us）
每个舵机角度控制周期：8ms
每个舵机角度读取周期：16ms
每个舵机电流读取周期：约20ms
每个舵机电压读取周期：160ms
每个舵机电压读取周期：160ms
*/
void User_TimerServoIRQ(void)
{
	static uint8_t cmd_type = _CMD_TYPE_SET;
	static uint8_t read_type = _READ_TYPE_ANG;
	static uint8_t data_type = _DATA_TYPE_CUR;
	static uint8_t cnt_servo_id = 0;	
	static uint8_t cnt_data_read = 0;//计算读取数据读取了多少次了
	static uint8_t wait_to_set = 0;
	if (__HAL_TIM_GET_IT_SOURCE(&USER_htim_servo, TIM_IT_UPDATE) != RESET)
	{
		SERVO_COMM_BUSY = 1;
		switch(cmd_type)
		{
			case _CMD_TYPE_SET:	//腿部12个舵机发送一次控制指令，串口通信时间约750us
			{
				wait_to_set ++;
				if(wait_to_set == 1)
				{
					User_LegAllSetAngTime();
				}					
				else if(wait_to_set == 2)
				{
					//for(int i = 0;i< 10000;i++);
					cmd_type = _CMD_TYPE_READ;
					wait_to_set = 0;
				}
				
			}break;
			case _CMD_TYPE_READ://发送一次读指令，从发送该指令到舵机返回数据串口总共通信时间约400us（发送100us，等待响应100us，回传200us）
			{			
				if(read_type == _READ_TYPE_ANG)//读角度
				{
					cnt_servo_id++;	
					User_UsartReadServoAng(cnt_servo_id);
					User_UsartReadServoAng(cnt_servo_id+6);						
				}
				else//读数据：1-电压,2-电流，3-功率，4-温度
				{
					cnt_servo_id++;
					User_UsartReadServoData(cnt_servo_id,data_type);
					User_UsartReadServoData(cnt_servo_id+6,data_type);	
				}
				if(cnt_servo_id == 6)//6个1ms分别读取了6个舵机的数据后，进入新的周期并切换读取内容（读角度/读数据）
				{
					cnt_servo_id = 0;
					cmd_type = _CMD_TYPE_SET;					
					if(read_type == _READ_TYPE_ANG)
						read_type = _READ_TYPE_DATA;
					else
					{
						read_type = _READ_TYPE_ANG;
						cnt_data_read++;
						switch(cnt_data_read)
						{
							case 8:data_type = _DATA_TYPE_VOLT;break;
							case 9:data_type = _DATA_TYPE_TEMP;break;
							case 10:data_type = _DATA_TYPE_CUR;cnt_data_read = 0;break;
							default:break;
						}					
					}									
				}						
			}break;	
			default:break;
		}
		SERVO_COMM_BUSY = 0;	
	}
		
}


