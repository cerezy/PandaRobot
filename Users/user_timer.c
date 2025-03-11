#include "user_timer.h"

#define _CMD_TYPE_SET 		0
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
������ƣ�
��ʱ�����ڣ�1ms
���Ʋ��裺
				1ms->������DMA���͸ô��������ϸ�����ǶȺ�ִ��ʱ��
				2ms->������DMA���͸ô��������ϵ�1������Ƕȶ�ȡ����ָ��
				3ms->������DMA���͸ô��������ϵ�2������Ƕȶ�ȡ����ָ��
				4ms->������DMA���͸ô��������ϵ�3������Ƕȶ�ȡ����ָ��
				
				5ms->������DMA���͸ô��������ϸ�����ǶȺ�ִ��ʱ��
				6ms->������DMA���͸ô��������ϵ�1��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ���ѹ1���¶�1�ε���199�Σ�
				7ms->������DMA���͸ô��������ϵ�2��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ���ѹ1���¶�1�ε���199�Σ�
				8ms->������DMA���͸ô��������ϵ�3��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ���ѹ1���¶�1�ε���199�Σ�
ÿ������Ƕȿ������ڣ�4ms
ÿ������Ƕȶ�ȡ���ڣ�8ms
ÿ�����������ȡ���ڣ�198��/16s
ÿ�������ѹ��ȡ���ڣ�16s
ÿ�������ѹ��ȡ���ڣ�16s
*/
void User_TimerServoIRQ(void)
{
	static uint8_t cmd_type = 0;
	static uint8_t read_type = 0;
	static uint8_t data_type = _DATA_TYPE_CUR;
	static uint8_t cnt_servo_id = 0;	
	static uint8_t cnt_data_read = 0;
	if (__HAL_TIM_GET_IT_SOURCE(&USER_htim_servo, TIM_IT_UPDATE) != RESET)
	{
		SERVO_COMM_BUSY = 1;
		switch(cmd_type)
		{
			case _CMD_TYPE_SET:	//�Ȳ�12���������һ�ο���ָ�����ͨ��ʱ��Լ750us
			{
				User_LegAllSetAngTime(); 
				cmd_type = _CMD_TYPE_READ;
			}break;
			case _CMD_TYPE_READ://����һ�ζ�ָ��ӷ��͸�ָ�����������ݴ����ܹ�ͨ��ʱ��Լ400us������100us���ȴ���Ӧ100us���ش�200us��
			{			
				if(read_type == _READ_TYPE_ANG)//���Ƕ�
				{
					cnt_servo_id++;
					User_UsartReadServoAng(cnt_servo_id);
					User_UsartReadServoAng(cnt_servo_id+3);
					User_UsartReadServoAng(cnt_servo_id+6);
					User_UsartReadServoAng(cnt_servo_id+9);				
				}
				else//�����ݣ�1-��ѹ,2-������3-���ʣ�4-�¶�
				{
					cnt_servo_id++;
					User_UsartReadServoData(cnt_servo_id,data_type);
					User_UsartReadServoData(cnt_servo_id+3,data_type);
					User_UsartReadServoData(cnt_servo_id+6,data_type);
					User_UsartReadServoData(cnt_servo_id+9,data_type);		
				}
				if(cnt_servo_id == 3)//3��1ms�ֱ��ȡ��3����������ݣ������µ����ڲ��л���ȡ���ݣ����Ƕ�/�����ݣ�
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
							case 99:data_type = _DATA_TYPE_VOLT;break;
							case 100:data_type = _DATA_TYPE_CUR;break;
							case 199:data_type = _DATA_TYPE_TEMP;break;
							case 200:data_type = _DATA_TYPE_CUR;cnt_data_read = 0;break;
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


