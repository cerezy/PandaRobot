#include "user_tasks.h"
#include "cmsis_os.h"
#include "user_servo.h"
#include "user_timer.h"
#include "user_states.h"
#include <stdbool.h>
int16_t ang_goal[15] = {0};
uint16_t ms_goal[15] = {0};
uint8_t Action_done[50] = {0};
uint8_t ActionNowFlag = 1;
ACTION_STATE ActionNow = IDLE; // ��ǰ����״̬
ACTION_STATE ActionLast = IDLE; // ��һ������״̬
uint8_t speed = 5;		  // �����Ĳ����ٶȣ���λ0.1�㣩
int32_t step_counter = 0; // ��ǰ�������ö������еĵڼ���
uint8_t vis[15] = {0};	  // �ж϶���Ƿ�ִ�λ��
uint8_t Servo_Reset_Flag = 0;
uint8_t Init_OK = 0;
int16_t LookPos[14] = {0};
uint8_t actionStandup_getStartAngle = 0; // ִ�ж���վ��ʱ��ȡ��ǰ���λ��Ϊ��ʼλ��

void User_Init(void)
{
	Action_init();		   // �������ʼ����������Ϣ��
	User_ServoInit();	   // �����Ϣ��ʼ������Ӧ���ڵȣ�
	User_TimerInit();	   // ͳ��ȫ�ֵĶ�ʱ��
	User_TeachTimerInit(); // ʾ��ģʽר�ö�ʱ��
	TEACHMODE = 1;
	Init_OK = 1;
}

// �жϵ��������Ƿ�λ
bool _SingleAction_CheckApproch(ServoActionSeries *action)
{
	if (action->ifNeedBezier == 1)
	{
		if (actionStandup_getStartAngle == 0)
		{
			for (int i = 1; i <= 12; i++)
				action->startservoAngles[i] = SERVO[i].pos_read; // ��ȡ��ǰ�Ƕ�
			actionStandup_getStartAngle = 1;
		}
		User_BezierCurve(action->total_step, action);
	}

	for (int i = 1; i <= 12; i++)
	{
		if (goal_pos[i] <= action->actions[step_counter].servoAngles[i] - speed)
			goal_pos[i] += speed;
		else if (goal_pos[i] >= action->actions[step_counter].servoAngles[i] + speed)
			goal_pos[i] -= speed;
		// �ж��Ƿ�ִ�Ŀ��λ��
		if (goal_pos[i] > action->actions[step_counter].servoAngles[i] - speed && goal_pos[i] < action->actions[step_counter].servoAngles[i] + speed)
			vis[i] = 1;
	}
	for (int i = 1; i <= 12; i++)
	{
		if (vis[i] == 0)
			break;
		if (i == 12)
		{
			// δִ�������в���
			if (step_counter < action->total_step - 1)
			{
				step_counter++;
				for (int i = 1; i <= 12; i++)
					vis[i] = 0;
				return false;
			}
			// ִ�����
			else if (step_counter == action->total_step - 1)
			{
				// ��ǰ����Done
				Action_done[action->actionId] = 1;
				// ��ز�����λ
				step_counter = 0;
				for (int i = 1; i <= 12; i++)
					vis[i] = 0;
				return true;
			}
		}
	}
	return false;
}

// ��iter���㣬�����ٴ����������
void Motion_Reset(Motion_t *motion_)
{
	for(int i = 0;i<motion_->point_total;i++)
	{
		Action_done[motion_->motion[i].actionId] = 0; // ���ö�����ɱ�־
	}
	motion_->point_iter = 0; // ���ö��������
}

// �˶�������ͨ���ж�iter����true��false
bool Motion_Run(Motion_t *motion_)
{
	if (_SingleAction_CheckApproch(&motion_->motion[motion_->point_iter]))
	{
		if (motion_->point_iter < motion_->point_total - 1)
		{
			if(motion_ == & _Action_Hug)
			{
				static int delTim = 0;
				delTim++;
				if(delTim > 100)
				{
					delTim = 0;
					motion_->point_iter++;
				}
			}
			else motion_->point_iter++; // �л���һ��������
			return false;
		}
			
		else if (motion_->point_iter == motion_->point_total - 1)
		{
			return true;
		}
		return false;
	} // ����Ƿ������������
	return false;
}

void robotRun()
{
	switch (ActionNow)
	{
	case ACTION_TEACH://0
		// ʾ��
		if (Motion_Run(&_Action_TEACH) == true)
		{
			Motion_Reset(&_Action_TEACH); // ����ʹ�ܸö����������´��ٴ���
			step_counter = 1;//��������Ҫ�ظ�
			//ActionNow = IDLE;
		}
		break;

	case ACTION_WALK://1
		// ��·
		if (Motion_Run(&_Action_Walk) == true)
		{
			Motion_Reset(&_Action_Walk); // ����ʹ�ܸö����������´��ٴ���
		}

		break;

	case ACTION_WAVE://2
		// ����
		if (Motion_Run(&_Action_Walk) == true)
		{
			Motion_Reset(&_Action_Walk); // ����ʹ�ܸö����������´��ٴ���
			ActionNow = IDLE;
		}
		break;

	case ACTION_STANDUP://3
		// վ��
		if (Motion_Run(&_Action_Standup) == true)
		{
			Motion_Reset(&_Action_Standup); // ����ʹ�ܸö����������´��ٴ���
			ActionNow = IDLE;
		}
		break;

	case ACTION_SIT://4
		// ����
		if (Motion_Run(&_Active_Sit) == true)
		{
			Motion_Reset(&_Active_Sit); // ����ʹ�ܸö����������´��ٴ���
			ActionNow = IDLE;
		}
		break;

	case ACTION_SITTOEAT://5
		// ���³Զ���
		if (Motion_Run(&_Active_SittoEat) == true)
		{
			Motion_Reset(&_Active_SittoEat); // ����ʹ�ܸö����������´��ٴ���
			ActionNow = IDLE;
		}
		break;

	case ACTION_HUG://64.18 23��04��������û�ģ�Ҫ���������
		// ӵ��
		if (Motion_Run(&_Action_Hug) == true)
		{
			Motion_Reset(&_Action_Hug); // ����ʹ�ܸö����������´��ٴ���
		}
		break;

	case ACTION_LIEDOWN:
		if (Motion_Run(&_Action_LieDown) == true)
		{
			Motion_Reset(&_Action_LieDown); // ����ʹ�ܸö����������´��ٴ���
			ActionNow = IDLE;
		}
		break;

	case ACTION_SIT2PRONE:
		// ����
		break;

	case ACTION_PRONETOSIT:

		break;

	case ACTION_LIE2STANDUP:

		break;

	case ACTION_CRAWL:

		break;

	case ACTION_CRAWLREPEAT:

		break;

	case ACTION_TEST:

		break;

	case IDLE:
		// ����
		for (int i = 1; i <= 12; i++)
		{
			goal_pos[i] = SERVO[i].pos_read;
		}
		step_counter = 0;
		break;
	default:
		break;
	}
}

void TeachmodeRUN(void)
{
	// �������������ϵĶ����������ģʽ��һ����ʾ��ʱʹ��
	if (Servo_Reset_Flag == 1)
	{
		Servo_Reset_Flag = 0;
	}
	if (TEACHMODE == 1)
	{
		// ʾ��ģʽ����������������λ
		if (TEACH_FINISH == 1)
		{
			/*ʾ�̽���*/
			for (int i = 1; i <= 12; i++)
			{
				goal_pos[i] = _Action_TEACH.motion[0].actions[1].servoAngles[i];
			}
			TEACHMODE = 0;
			TEACH_OK = 0;
			TEACH_FINISH = 0;
			
			/*�������̸��ָոյĶ���*/
			ActionNowFlag = 0;
			Action_done[_Action_TEACH.motion[0].actionId] = 0;
			step_counter = 1;
			ActionNow = ACTION_TEACH;

			/*��������λ����������*/
			Action_Teachmode();
		}
	}
}

void StartTaskHigh(void const *argument)
{
	User_Init();
	for (;;)
	{
		osDelay(1);
	}
}
uint8_t OPEN = 1;
void StartTaskMid(void const *argument)
{
	for (;;)
	{
		if (TEACHMODE == 1)
			TeachmodeRUN();
		else if (Init_OK == 1)
			robotRun();
		for (int i = 1; i <= 12; i++)
			LookPos[i] = SERVO[i].pos_read;
		if (OPEN == 1)
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port, Servo_Power_Pin, GPIO_PIN_SET); // �������
		else
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port, Servo_Power_Pin, GPIO_PIN_RESET); // �������
		osDelay(10);
	}
}
void StartTaskLow(void const *argument)
{

	for (;;)
	{
		osDelay(20);
	}
}
