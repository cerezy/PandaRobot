#include "user_timer.h"
#include "user_communication.h"

// ����ģʽ
#define _CMD_TYPE_SET 0
// ��ȡģʽ
#define _CMD_TYPE_READ 1

#define _READ_TYPE_ANG 0
#define _READ_TYPE_DATA 1

#define _DATA_TYPE_VOLT 1
#define _DATA_TYPE_CUR 2
#define _DATA_TYPE_POWER 3
#define _DATA_TYPE_TEMP 4

TIMER_TYPEDEF TIMER = {0};
uint8_t flag_act = 0;

void User_TimerInit(void)
{
	HAL_TIM_Base_Start_IT(&USER_htim_servo); //
}
void User_TeachTimerInit(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
}
/*
������ƣ�
��ʱ�����ڣ�1ms
���Ʋ��裺
				1ms->������DMA���͸ô��������ϸ������6�����ǶȺ�ִ��ʱ�䣨1.5ms��
				3ms->������DMA���͸ô��������ϵ�1�����״̬��ȡ����ָ�400us��
				4ms->������DMA���͸ô��������ϵ�2�����״̬��ȡ����ָ�400us��
				5ms->������DMA���͸ô��������ϵ�3�����״̬��ȡ����ָ�400us��
				6ms->������DMA���͸ô��������ϵ�4�����״̬��ȡ����ָ�400us��
				7ms->������DMA���͸ô��������ϵ�5�����״̬��ȡ����ָ�400us��
ÿ������Ƕȿ������ڣ�7ms
ÿ������Ƕȶ�ȡ���ڣ�7ms
*/

extern uint8_t OPEN;
extern uint8_t Init_OK;
extern uint8_t touchTopofHead_Downside;
extern uint8_t touchChin_Downside;
void User_TimerServoIRQ(void)
{
	Key_Downside_Record();
	if(touchTopofHead_Downside == 1 && Init_OK == 1 && ActionNow == IDLE)//���Դ�
	{
		ActionNow = ACTION_M61;//M61. ͷ��������ҡ�����º󱣳־�ֹ��

	}
	if(touchChin_Downside == 1 && Init_OK == 1 && ActionNow == IDLE)//���°�
	{
		ActionNow = ACTION_M65;//M65. ̧����������䣬��󻺻����¡�
	}
	static uint8_t cmd_type = _CMD_TYPE_READ;
	static uint8_t read_type = _READ_TYPE_ANG;
	static uint8_t cnt_servo_id = 0;
	static uint8_t wait_to_set = 0;
	if (__HAL_TIM_GET_IT_SOURCE(&USER_htim_servo, TIM_IT_UPDATE) != RESET && OPEN == 1)
	{
		SERVO_COMM_BUSY = 1;
		switch (cmd_type)
		{
		case _CMD_TYPE_SET: // �ܹ�12���������һ�ο���ָ�����ͨ��ʱ��Լ750us
		{
			wait_to_set++;
			if (wait_to_set == 1)
			{
				if (TEACHMODE != 1 && flag_act == 1) // ʾ��ģʽ�£��������
					User_AllSetAngTime();
			}
			else if (wait_to_set == 2) // 2
			{
				cmd_type = _CMD_TYPE_READ;
				wait_to_set = 0;
			}
		}
		break;
		case _CMD_TYPE_READ: // ����һ�ζ�ָ��ӷ��͸�ָ�����������ݴ����ܹ�ͨ��ʱ��Լ400us������100us���ȴ���Ӧ100us���ش�200us��
		{
			if (read_type == _READ_TYPE_ANG) // ���Ƕ�
			{
				cnt_servo_id++;
				FEETECH_ReadServoPos(cnt_servo_id % 2 + 11); // ͷ���������������ȡ
				FEETECH_ReadServoPos(cnt_servo_id);
				FEETECH_ReadServoPos(cnt_servo_id + 5);
			}
			if (cnt_servo_id == 5) // 5��1ms�ֱ��ȡ��5����������ݺ󣬽����µ����ڲ��л���ȡ���ݣ����Ƕ�/�����ݣ�
			{
				cnt_servo_id = 0;
				cmd_type = _CMD_TYPE_SET;
			}
		}
		break;
		default:
			break;
		}
		SERVO_COMM_BUSY = 0;
	}
}
// ����ʾ�̵Ķ�ʱ���жϣ��������Ҫʾ��ģʽ���Թر�
int T_COUNTER = 0;		  // ��¼ʾ�̹����н����жϵĴ���
int step_record = 0;	  // ��¼�˴�ʾ��ģʽ�ĳ�ʼʱ��
int last_step_record = 0; // ��¼�ϴ�ʾ��ģʽ�Ľ���ʱ��
void User_TimerTeachIRQ(void)
{

	if (__HAL_TIM_GET_IT_SOURCE(&USER_htim_teach, TIM_IT_UPDATE) != RESET)
	{
		if (TEACHMODE == 1) // �����ʾ��ģʽ
		{
			T_COUNTER++;															   // �������
			if (step_record == last_step_record && TEACH_OK == 1 && TEACH_FINISH != 1) // ���ʾ��ģʽ�ոտ�ʼ
			{
				step_record = T_COUNTER; // ��¼�˴�ʾ��ģʽ�ĳ�ʼʱ��
			}
			else if (step_record != last_step_record && TEACH_OK == 1 && TEACH_FINISH != 1)
			{
				if (T_COUNTER - step_record > TEACH_TOTAL_STEP)
				{
					// ��������ʾ��
					TEACH_FINISH = 1;
					last_step_record = step_record;
				}
				else if ((T_COUNTER - step_record) >= 0 && (T_COUNTER - step_record) <= TEACH_TOTAL_STEP)
				{
					// �洢ʾ�̹����еĽǶ�����
					for (int i = 0; i < 14; i++)
						_Action_TEACH.motion[0].actions[T_COUNTER - step_record - 1].servoAngles[i] = SERVO[i].pos_read;
				}
			}
		}
	}
}
