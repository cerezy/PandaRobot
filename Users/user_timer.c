#include "user_timer.h"

//����ģʽ
#define _CMD_TYPE_SET 		0
//��ȡģʽ
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
				1ms->������DMA���͸ô��������ϸ������6�����ǶȺ�ִ��ʱ�䣨1.5ms��
				3ms->������DMA���͸ô��������ϵ�1������Ƕȶ�ȡ����ָ�400us��
				4ms->������DMA���͸ô��������ϵ�2������Ƕȶ�ȡ����ָ�400us��
				5ms->������DMA���͸ô��������ϵ�3������Ƕȶ�ȡ����ָ�400us��
				6ms->������DMA���͸ô��������ϵ�4������Ƕȶ�ȡ����ָ�400us��
				7ms->������DMA���͸ô��������ϵ�5������Ƕȶ�ȡ����ָ�400us��
				8ms->������DMA���͸ô��������ϵ�6������Ƕȶ�ȡ����ָ�400us��
				
				9ms->������DMA���͸ô��������ϸ������6�����ǶȺ�ִ��ʱ�䣨1.5ms��
				11ms->������DMA���͸ô��������ϵ�1��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ�10���е�ѹ1���¶�1�ε���8�Σ���400us��
				12ms->������DMA���͸ô��������ϵ�2��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ�10���е�ѹ1���¶�1�ε���8�Σ���400us��
				13ms->������DMA���͸ô��������ϵ�3��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ�10���е�ѹ1���¶�1�ε���8�Σ���400us��
				14ms->������DMA���͸ô��������ϵ�4��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ�10���е�ѹ1���¶�1�ε���8�Σ���400us��
				15ms->������DMA���͸ô��������ϵ�5��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ�10���е�ѹ1���¶�1�ε���8�Σ���400us��
				16ms->������DMA���͸ô��������ϵ�6��������ݣ���ѹ/����/����/�¶ȣ���ȡ����ָ�10���е�ѹ1���¶�1�ε���8�Σ���400us��
ÿ������Ƕȿ������ڣ�8ms
ÿ������Ƕȶ�ȡ���ڣ�16ms
ÿ�����������ȡ���ڣ�Լ20ms
ÿ�������ѹ��ȡ���ڣ�160ms
ÿ�������ѹ��ȡ���ڣ�160ms
*/
void User_TimerServoIRQ(void)
{
	static uint8_t cmd_type = _CMD_TYPE_SET;
	static uint8_t read_type = _READ_TYPE_ANG;
	static uint8_t data_type = _DATA_TYPE_CUR;
	static uint8_t cnt_servo_id = 0;	
	static uint8_t cnt_data_read = 0;//�����ȡ���ݶ�ȡ�˶��ٴ���
	static uint8_t wait_to_set = 0;
	if (__HAL_TIM_GET_IT_SOURCE(&USER_htim_servo, TIM_IT_UPDATE) != RESET)
	{
		SERVO_COMM_BUSY = 1;
		switch(cmd_type)
		{
			case _CMD_TYPE_SET:	//�Ȳ�12���������һ�ο���ָ�����ͨ��ʱ��Լ750us
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
			case _CMD_TYPE_READ://����һ�ζ�ָ��ӷ��͸�ָ�����������ݴ����ܹ�ͨ��ʱ��Լ400us������100us���ȴ���Ӧ100us���ش�200us��
			{			
				if(read_type == _READ_TYPE_ANG)//���Ƕ�
				{
					cnt_servo_id++;	
					User_UsartReadServoAng(cnt_servo_id);
					User_UsartReadServoAng(cnt_servo_id+6);						
				}
				else//�����ݣ�1-��ѹ,2-������3-���ʣ�4-�¶�
				{
					cnt_servo_id++;
					User_UsartReadServoData(cnt_servo_id,data_type);
					User_UsartReadServoData(cnt_servo_id+6,data_type);	
				}
				if(cnt_servo_id == 6)//6��1ms�ֱ��ȡ��6����������ݺ󣬽����µ����ڲ��л���ȡ���ݣ����Ƕ�/�����ݣ�
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


