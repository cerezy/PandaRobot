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
ACTION_STATE ActionNow = IDLE;	// ��ǰ����״̬
ACTION_STATE ActionLast = IDLE; // ��һ������״̬
uint8_t speed = 8;				// �����Ĳ����ٶȣ���λ0.1�㣩
int32_t step_counter = 0;		// ��ǰ�������ö������еĵڼ���
uint8_t vis[15] = {0};			// �ж϶���Ƿ�ִ�λ��
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
	for (int i = 0; i < motion_->point_total; i++)
	{
		Action_done[motion_->motion[i].actionId] = 0; // ���ö�����ɱ�־
	}
	motion_->point_iter = 0; // ���ö��������
	step_counter = 0;
	actionStandup_getStartAngle = 0; // �������»�ȡ��ǰ�ǶȴӶ������µı��������߼���
}

int delTim = 0;
// �˶�������ͨ���ж�iter����true��false
bool Motion_Run(Motion_t *motion_)
{
	if (_SingleAction_CheckApproch(&motion_->motion[motion_->point_iter]))
	{
		if (motion_->point_iter < motion_->point_total - 1)
		{
//			if (motion_ == &_Action_Hug)
//			{
//				delTim++;
//				if (delTim > 100)
//				{
//					delTim = 0;
//					motion_->point_iter++;
//				}
//			}
//			else
				motion_->point_iter++; // �л���һ��������
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
	case ACTION_TEACH: // 0
		// ʾ��
	_Action_TEACH.motion[0].total_step = TEACH_TOTAL_STEP;
		if (Motion_Run(&_Action_TEACH) == true)
		{
			Motion_Reset(&_Action_TEACH); // ����ʹ�ܸö����������´��ٴ���
			step_counter = 1;
			ActionNow = IDLE;
		}
		break;

	case ACTION_WALK: // 1
		// ��·
		if (Motion_Run(&_Action_Walk) == true)
		{
			Motion_Reset(&_Action_Walk); // ����ʹ�ܸö����������´��ٴ���
		}

		break;

//	case ACTION_WAVE: // 2
//		// ����
//		if (Motion_Run(&_Active_Wave) == true)
//		{
//			Motion_Reset(&_Active_Wave); // ����ʹ�ܸö����������´��ٴ���
//			ActionNow = IDLE;
//		}
//		break;

	case ACTION_STANDUP: // 3
		// վ��
		if (Motion_Run(&_Action_Standup) == true)
		{
			Motion_Reset(&_Action_Standup); // ����ʹ�ܸö����������´��ٴ���
			ActionNow = IDLE;
		}
		break;

	case ACTION_SIT: // 4
		// ����
		if (Motion_Run((Motion_t*)&_Active_Sit) == true)
		{
			Motion_Reset((Motion_t*)&_Active_Sit); // ����ʹ�ܸö����������´��ٴ���
			ActionNow = IDLE;
		}
		break;

//	case ACTION_SITTOEAT: // 5 ���ųԶ���
//        // ���³Զ���
//        if (Motion_Run(&_Active_SittoEat) == true)
//        {
//            Motion_Reset(&_Active_SittoEat); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_HUG: // 6 ӵ��
//        // ӵ��
//        if (Motion_Run(&_Action_Hug) == true)
//        {
//            Motion_Reset(&_Action_Hug); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

    case ACTION_LIEPRONE: // 7 ��
        if (Motion_Run(&_Action_LieProne) == true)
        {
            Motion_Reset(&_Action_LieProne); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

//    case ACTION_BIGLIE: // 8 ������
//        if (Motion_Run(&_Action_BigLie) == true)
//        {
//            Motion_Reset(&_Action_BigLie); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

    case ACTION_SIT2PRONE: // 9 ��->��
        if (Motion_Run(&_Action_Sit2Prone) == true)
        {
            Motion_Reset(&_Action_Sit2Prone); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_PRONETOSIT: // 10 ��->��
        if (Motion_Run(&_Action_PronetoSit) == true)
        {
            Motion_Reset(&_Action_PronetoSit); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    // case ACTION_TEST: // 11 ���Զ���
    //     if (Motion_Run(&_Action_Test) == true)
    //     {
    //         Motion_Reset(&_Action_Test); // ����ʹ�ܸö����������´��ٴ���
    //         ActionNow = IDLE;
    //     }
    //     break;

//    case ACTION_HELLO: // 13 ���
//        if (Motion_Run(&_Action_Hello) == true)
//        {
//            Motion_Reset(&_Action_Hello); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_ScratchHead: // 14 ��ͷ
//        if (Motion_Run(&_Action_ScratchHead) == true)
//        {
//            Motion_Reset(&_Action_ScratchHead); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_Worship: // 15 ��һ��
//        if (Motion_Run(&_Action_Worship) == true)
//        {
//            Motion_Reset(&_Action_Worship); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_ShakeHead: // 16 ҡͷ
//        if (Motion_Run(&_Action_ShakeHead) == true)
//        {
//            Motion_Reset(&_Action_ShakeHead); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_Pouting: // 17 ��ƨ��
//        if (Motion_Run(&_Action_Pouting) == true)
//        {
//            Motion_Reset(&_Action_Pouting); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_TurnThings: // 18 ������
//        if (Motion_Run(&_Action_TurnThings) == true)
//        {
//            Motion_Reset(&_Action_TurnThings); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_SleepTilt: // 19 ��ͷ˯��
//        if (Motion_Run(&_Action_SleepTilt) == true)
//        {
//            Motion_Reset(&_Action_SleepTilt); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_WashFace: // 20 ϴ��
//        if (Motion_Run(&_Action_WashFace) == true)
//        {
//            Motion_Reset(&_Action_WashFace); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_SideLieScratch: // 21 ��������
//        if (Motion_Run(&_Action_SideLieScratch) == true)
//        {
//            Motion_Reset(&_Action_SideLieScratch); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_SitLegsOpen: // 22 ������
//        if (Motion_Run(&_Action_SitLegsOpen) == true)
//        {
//            Motion_Reset(&_Action_SitLegsOpen); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_StandToSit: // 23 վ->��
//        if (Motion_Run(&_Action_StandToSit) == true)
//        {
//            Motion_Reset(&_Action_StandToSit); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_SideLie: // 24 ����
//        if (Motion_Run(&_Action_SideLie) == true)
//        {
//            Motion_Reset(&_Action_SideLie); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

//    case ACTION_WagHips: // 25 Ťƨ��
//        if (Motion_Run(&_Action_WagHips) == true)
//        {
//            Motion_Reset(&_Action_WagHips); // ����ʹ�ܸö����������´��ٴ���
//            ActionNow = IDLE;
//        }
//        break;

    case ACTION_HighFive: // 26 ���ƶ���
        if (Motion_Run(&_Action_HighFive) == true)
        {
            Motion_Reset(&_Action_HighFive); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_HugKiss: // 27 ����kiss����
        if (Motion_Run(&_Action_HugKiss) == true)
        {
            Motion_Reset(&_Action_HugKiss); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_Handshake: // 28 ���ֶ���
        if (Motion_Run(&_Action_Handshake) == true)
        {
            Motion_Reset(&_Action_Handshake); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_ScratchButt: // 29 ��ƨ�ɶ���
        if (Motion_Run(&_Action_ScratchButt) == true)
        {
            Motion_Reset(&_Action_ScratchButt); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_Bow: // 30 �Ϲ�����
        if (Motion_Run(&_Action_Bow) == true)
        {
            Motion_Reset(&_Action_Bow); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_Cheer: // 31 �ӱۼ��Ͷ���
        if (Motion_Run(&_Action_Cheer) == true)
        {
            Motion_Reset(&_Action_Cheer); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_DrawHeart: // 32 �����Ķ���
        if (Motion_Run(&_Action_DrawHeart) == true)
        {
            Motion_Reset(&_Action_DrawHeart); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_StompFoot: // 33 ������Ŷ���
        if (Motion_Run(&_Action_StompFoot) == true)
        {
            Motion_Reset(&_Action_StompFoot); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_Drum: // 34 ���Ķ���
        if (Motion_Run(&_Action_Drum) == true)
        {
            Motion_Reset(&_Action_Drum); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_RubEyes: // 35 ���۾�����
        if (Motion_Run(&_Action_RubEyes) == true)
        {
            Motion_Reset(&_Action_RubEyes); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_Yawn: // 36 ���Ƿ����
        if (Motion_Run(&_Action_Yawn) == true)
        {
            Motion_Reset(&_Action_Yawn); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;

    case ACTION_SitPatButt: // 37 ������ƨ�ɶ���
        if (Motion_Run(&_Action_SitPatButt) == true)
        {
            Motion_Reset(&_Action_SitPatButt); // ����ʹ�ܸö����������´��ٴ���
            ActionNow = IDLE;
        }
        break;
		 case ACTION_Eat: // 38 �Զ���
        if (Motion_Run(&_Action_Eat) == true)
        {
            Motion_Reset(&_Action_Eat); 
            ActionNow = IDLE;
        }
        break;

    case ACTION_Stretch: // 39 ������
        if (Motion_Run(&_Action_Stretch) == true)
        {
            Motion_Reset(&_Action_Stretch); 
            ActionNow = IDLE;
        }
        break;

    case ACTION_StretchLying: // 40 ����������
        if (Motion_Run(&_Action_StretchLying) == true)
        {
            Motion_Reset(&_Action_StretchLying); 
            ActionNow = IDLE;
        }
        break;

    case ACTION_WaveStanding: // 41 վ������
        if (Motion_Run(&_Action_WaveStanding) == true)
        {
            Motion_Reset(&_Action_WaveStanding); 
            ActionNow = IDLE;
        }
        break;

    case ACTION_ScratchButtStanding: // 42 վ����ƨ��
        if (Motion_Run(&_Action_ScratchButtStanding) == true)
        {
            Motion_Reset(&_Action_ScratchButtStanding); 
            ActionNow = IDLE;
        }
        break;

    case ACTION_SplitStanding: // 43 վ������
        if (Motion_Run(&_Action_SplitStanding) == true)
        {
            Motion_Reset(&_Action_SplitStanding); 
            ActionNow = IDLE;
        }
        break;

    case ACTION_PatTummy: // 44 �Ķ���
        if (Motion_Run(&_Action_PatTummy) == true)
        {
            Motion_Reset(&_Action_PatTummy); 
            ActionNow = IDLE;
        }
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
			_Action_TEACH.motion[0].total_step = TEACH_TOTAL_STEP - 1;
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
