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
void User_TeachTimerInit(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
}
/*
舵机控制：
定时器周期：1ms
控制步骤：
				1ms->各串口DMA发送该串口总线上各舵机（6个）角度和执行时间（1.5ms）
				3ms->各串口DMA发送该串口总线上第1个舵机状态读取请求指令（400us）
				4ms->各串口DMA发送该串口总线上第2个舵机状态读取请求指令（400us）
				5ms->各串口DMA发送该串口总线上第3个舵机状态读取请求指令（400us）
				6ms->各串口DMA发送该串口总线上第4个舵机状态读取请求指令（400us）
				7ms->各串口DMA发送该串口总线上第5个舵机状态读取请求指令（400us）
每个舵机角度控制周期：7ms
每个舵机角度读取周期：7ms
*/

extern uint8_t OPEN;
void User_TimerServoIRQ(void)
{
	static uint8_t cmd_type = _CMD_TYPE_READ;
	static uint8_t read_type = _READ_TYPE_ANG;
	static uint8_t cnt_servo_id = 0;	
	static uint8_t wait_to_set = 0;
	if (__HAL_TIM_GET_IT_SOURCE(&USER_htim_servo, TIM_IT_UPDATE) != RESET && OPEN == 1)
	{
		SERVO_COMM_BUSY = 1;
		switch(cmd_type)
		{
			case _CMD_TYPE_SET:	//总共12个舵机发送一次控制指令，串口通信时间约750us
			{
				wait_to_set ++;
				if(wait_to_set == 1)
				{
					if(TEACHMODE != 1) //示教模式下，电机不动
						User_AllSetAngTime();
				}					
				else if(wait_to_set == 2) //2
				{
					cmd_type = _CMD_TYPE_READ;
					wait_to_set = 0;
				}
				
			}break;
			case _CMD_TYPE_READ://发送一次读指令，从发送该指令到舵机返回数据串口总共通信时间约400us（发送100us，等待响应100us，回传200us）
			{			
				if(read_type == _READ_TYPE_ANG)//读角度
				{
					cnt_servo_id++;	
					FEETECH_ReadServoPos(cnt_servo_id%2 + 11);//头部两个舵机轮流读取
					FEETECH_ReadServoPos(cnt_servo_id);
					FEETECH_ReadServoPos(cnt_servo_id + 5);
				}
				if(cnt_servo_id == 5)//5个1ms分别读取了5个舵机的数据后，进入新的周期并切换读取内容（读角度/读数据）
				{
					cnt_servo_id = 0;
					cmd_type = _CMD_TYPE_SET;						
				}						
			}break;	
			default:break;
		}
		SERVO_COMM_BUSY = 0;	
	}
		
}
//用于示教的定时器中断，如果不需要示教模式可以关闭
int T_COUNTER = 0;//记录示教过程中进入中断的次数
int step_record = 0;//记录此次示教模式的初始时刻
int last_step_record = 0;//记录上次示教模式的结束时刻
void User_TimerTeachIRQ(void)
{
	
	if (__HAL_TIM_GET_IT_SOURCE(&USER_htim_teach, TIM_IT_UPDATE) != RESET)
	{
		if(TEACHMODE == 1)//如果是示教模式
		{
			T_COUNTER++;//进入计数
			if(step_record == last_step_record && TEACH_OK == 1 && TEACH_FINISH != 1)//如果示教模式刚刚开始
			{
				step_record = T_COUNTER;//记录此次示教模式的初始时刻
			}
			else if(step_record != last_step_record && TEACH_OK == 1 && TEACH_FINISH != 1)
			{
				if(T_COUNTER - step_record >= TEACH_TOTAL_STEP)
				{
					//结束本次示教
					TEACH_FINISH = 1;
					last_step_record = step_record;
				}
				else if((T_COUNTER - step_record)>=0 && (T_COUNTER - step_record) <= TEACH_TOTAL_STEP - 1){
					//存储示教过程中的角度数据
					for(int i = 0;i<14; i++)
						Action_TEACH.actions[T_COUNTER - step_record].servoAngles[i] = SERVO[i].pos_read;
				}
			}
		}
	}
}

