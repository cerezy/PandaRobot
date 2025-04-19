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
ACTION_STATE ActionNow = IDLE; // 当前动作状态
ACTION_STATE ActionLast = IDLE; // 上一个动作状态
uint8_t speed = 5;		  // 动作的步进速度（单位0.1°）
int32_t step_counter = 0; // 当前步进到该动作序列的第几步
uint8_t vis[15] = {0};	  // 判断舵机是否抵达位置
uint8_t Servo_Reset_Flag = 0;
uint8_t Init_OK = 0;
int16_t LookPos[14] = {0};
uint8_t actionStandup_getStartAngle = 0; // 执行动作站立时获取当前舵机位置为初始位置

void User_Init(void)
{
	Action_init();		   // 动作库初始化（动作信息）
	User_ServoInit();	   // 舵机信息初始化（对应串口等）
	User_TimerInit();	   // 统领全局的定时器
	User_TeachTimerInit(); // 示教模式专用定时器
	TEACHMODE = 1;
	Init_OK = 1;
}

// 判断单独动作是否到位
bool _SingleAction_CheckApproch(ServoActionSeries *action)
{
	if (action->ifNeedBezier == 1)
	{
		if (actionStandup_getStartAngle == 0)
		{
			for (int i = 1; i <= 12; i++)
				action->startservoAngles[i] = SERVO[i].pos_read; // 获取当前角度
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
		// 判断是否抵达目标位置
		if (goal_pos[i] > action->actions[step_counter].servoAngles[i] - speed && goal_pos[i] < action->actions[step_counter].servoAngles[i] + speed)
			vis[i] = 1;
	}
	for (int i = 1; i <= 12; i++)
	{
		if (vis[i] == 0)
			break;
		if (i == 12)
		{
			// 未执行完所有步数
			if (step_counter < action->total_step - 1)
			{
				step_counter++;
				for (int i = 1; i <= 12; i++)
					vis[i] = 0;
				return false;
			}
			// 执行完毕
			else if (step_counter == action->total_step - 1)
			{
				// 当前动作Done
				Action_done[action->actionId] = 1;
				// 相关参数复位
				step_counter = 0;
				for (int i = 1; i <= 12; i++)
					vis[i] = 0;
				return true;
			}
		}
	}
	return false;
}

// 将iter清零，方便再次做这个动作
void Motion_Reset(Motion_t *motion_)
{
	for(int i = 0;i<motion_->point_total;i++)
	{
		Action_done[motion_->motion[i].actionId] = 0; // 重置动作完成标志
	}
	motion_->point_iter = 0; // 重置动作点序号
}

// 运动函数，通过判断iter返回true或false
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
			else motion_->point_iter++; // 切换下一个动作点
			return false;
		}
			
		else if (motion_->point_iter == motion_->point_total - 1)
		{
			return true;
		}
		return false;
	} // 检测是否完成整个动作
	return false;
}

void robotRun()
{
	switch (ActionNow)
	{
	case ACTION_TEACH://0
		// 示教
		if (Motion_Run(&_Action_TEACH) == true)
		{
			Motion_Reset(&_Action_TEACH); // 重新使能该动作，便于下次再次跑
			step_counter = 1;//测爬行需要重复
			//ActionNow = IDLE;
		}
		break;

	case ACTION_WALK://1
		// 走路
		if (Motion_Run(&_Action_Walk) == true)
		{
			Motion_Reset(&_Action_Walk); // 重新使能该动作，便于下次再次跑
		}

		break;

	case ACTION_WAVE://2
		// 挥手
		if (Motion_Run(&_Action_Walk) == true)
		{
			Motion_Reset(&_Action_Walk); // 重新使能该动作，便于下次再次跑
			ActionNow = IDLE;
		}
		break;

	case ACTION_STANDUP://3
		// 站立
		if (Motion_Run(&_Action_Standup) == true)
		{
			Motion_Reset(&_Action_Standup); // 重新使能该动作，便于下次再次跑
			ActionNow = IDLE;
		}
		break;

	case ACTION_SIT://4
		// 坐下
		if (Motion_Run(&_Active_Sit) == true)
		{
			Motion_Reset(&_Active_Sit); // 重新使能该动作，便于下次再次跑
			ActionNow = IDLE;
		}
		break;

	case ACTION_SITTOEAT://5
		// 坐下吃东西
		if (Motion_Run(&_Active_SittoEat) == true)
		{
			Motion_Reset(&_Active_SittoEat); // 重新使能该动作，便于下次再次跑
			ActionNow = IDLE;
		}
		break;

	case ACTION_HUG://64.18 23：04，参数还没改，要达瓦明天改
		// 拥抱
		if (Motion_Run(&_Action_Hug) == true)
		{
			Motion_Reset(&_Action_Hug); // 重新使能该动作，便于下次再次跑
		}
		break;

	case ACTION_LIEDOWN:
		if (Motion_Run(&_Action_LieDown) == true)
		{
			Motion_Reset(&_Action_LieDown); // 重新使能该动作，便于下次再次跑
			ActionNow = IDLE;
		}
		break;

	case ACTION_SIT2PRONE:
		// 坐下
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
		// 空闲
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
	// 用于三个串口上的舵机进入阻尼模式，一般在示教时使用
	if (Servo_Reset_Flag == 1)
	{
		Servo_Reset_Flag = 0;
	}
	if (TEACHMODE == 1)
	{
		// 示教模式结束，各个参数复位
		if (TEACH_FINISH == 1)
		{
			/*示教结束*/
			for (int i = 1; i <= 12; i++)
			{
				goal_pos[i] = _Action_TEACH.motion[0].actions[1].servoAngles[i];
			}
			TEACHMODE = 0;
			TEACH_OK = 0;
			TEACH_FINISH = 0;
			
			/*用于立刻复现刚刚的动作*/
			ActionNowFlag = 0;
			Action_done[_Action_TEACH.motion[0].actionId] = 0;
			step_counter = 1;
			ActionNow = ACTION_TEACH;

			/*用于向上位机发送数据*/
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
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port, Servo_Power_Pin, GPIO_PIN_SET); // 舵机供电
		else
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port, Servo_Power_Pin, GPIO_PIN_RESET); // 舵机供电
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
