#include "user_tasks.h"
#include "cmsis_os.h"
#include "user_servo.h"
#include "user_timer.h"
#include "user_states.h"

int16_t ang_goal[15] = {0};
uint16_t ms_goal[15] = {0};
uint8_t Action_done[50] = {0};
uint8_t ActionNowFlag = 1;
ACTION_STATE ActionNow = ACTION_LIEDOWN; // 当前动作状态
uint8_t speed = 10;						 // 动作的步进速度（单位0.1°）
int32_t step_counter = 0;				 // 当前步进到该动作序列的第几步
uint8_t vis[15] = {0};					 // 判断舵机是否抵达位置
uint8_t Servo_Reset_Flag = 0;
uint8_t Init_OK = 0;
int16_t LookPos[14] = {0};
uint8_t actionStandup_getStartAngle = 0; // 执行动作站立时获取当前舵机位置为初始位置
uint8_t state[25] = {0};				 // 用于跑每个动作的分段

void User_Init(void)
{
	Action_init();		   // 动作库初始化（动作信息）
	User_ServoInit();	   // 舵机信息初始化（对应串口等）
	User_TimerInit();	   // 统领全局的定时器
	User_TeachTimerInit(); // 示教模式专用定时器
	TEACHMODE = 1;
	Init_OK = 1;
}

// 跑动作函数
void ActionRUN(void)
{
	/*for(int i = 1;i <= 12;i++)
		goal_pos[i] = Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i];
	if(step_counter<14)
		step_counter++;
	if(step_counter==14)
		step_counter=0;*/

	// 判断当前在进行的动作ActionNowFlag是没有结束的并且验证ID的正确性
	if (Action_done[ActionNowFlag] == 0 && Action_index[ActionNowFlag]->actionId == ActionNowFlag)
	{
		if (Action_index[ActionNowFlag]->ifNeedBezier == 1)
		{
			if (actionStandup_getStartAngle == 0)
			{
				for (int i = 1; i <= 12; i++)
					Action_index[ActionNowFlag]->startservoAngles[i] = SERVO[i].ang_read;//获取当前角度
				actionStandup_getStartAngle = 1;
			}
			User_BezierCurve(Action_index[ActionNowFlag]->total_step, Action_index[ActionNowFlag]);
		}

		// 遍历舵机并更新下一时刻的目标值
		for (int i = 1; i <= 12; i++)
		{
			if (goal_pos[i] <= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] - speed)
				goal_pos[i] += speed;
			else if (goal_pos[i] >= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] + speed)
				goal_pos[i] -= speed;
			// 判断是否抵达目标位置
			if (goal_pos[i] > Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] - speed && goal_pos[i] < Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] + speed)
				vis[i] = 1;
		}
		for (int i = 1; i <= 12; i++)
		{
			if (vis[i] == 0)
				break;
			if (i == 12)
			{
				// 未执行完所有步数
				if (step_counter < Action_index[ActionNowFlag]->total_step - 1)
				{
					step_counter++;
					for (int i = 1; i <= 12; i++)
						vis[i] = 0;
				}
				// 执行完毕
				else if (step_counter == Action_index[ActionNowFlag]->total_step - 1)
				{
					// 当前动作Done
					Action_done[Action_index[ActionNowFlag]->actionId] = 1;
					// 相关参数复位
					step_counter = 0;
					actionStandup_getStartAngle = 0;

					for (int i = 1; i <= 12; i++)
						vis[i] = 0;
					if (ActionNowFlag == 1)
					{
						Action_done[Action_index[ActionNowFlag]->actionId] = 0;
						step_counter = 1;
					}
				}
			}
		}
	}
}

// 机器人运行函数
void robotRun()
{
	switch (ActionNow)
	{
	case ACTION_WALK:

		break;
	case ACTION_HUG:
		//拥抱
		if (state[ACTION_HUG] == 0)
		{
			static int timDel = 0;
			ActionNowFlag = 2;
			ActionRUN();
			if (Action_done[ActionNowFlag] == 1)
			{
				timDel++;
			}
			if (timDel > 100)
			{
				ActionNowFlag = 16;
				state[ACTION_HUG]++;
				Action_done[2] = 0;
				timDel = 0;
			}
		}
		else if (state[ACTION_HUG] == 1)
		{
			ActionRUN();
			if (Action_done[ActionNowFlag] == 1)
			{
				ActionNowFlag = 2;
				state[ACTION_HUG]--;
				Action_done[16] = 0;
			}
		}

		break;
	case ACTION_STANDUP:
		// 站立动作
		if (state[ACTION_STANDUP] == 0)
		{
			ActionNowFlag = 13; // 1_stant
			ActionRUN();
			if (Action_done[ActionNowFlag] == 1)
			{
				ActionNowFlag = ACTION_STANDUP;
				state[ACTION_STANDUP]++;
			}
		}
		else if (state[ACTION_STANDUP] == 1)
			ActionRUN();
		else
			break;
	case ACTION_SIT2PRONE:
		// 坐
		if (state[ACTION_SIT2PRONE] == 0)
		{
			ActionNowFlag = 15; // 1_sit
			ActionRUN();
			if (Action_done[ActionNowFlag] == 1)
			{
				ActionNowFlag = 14;
				state[ACTION_SIT2PRONE]++;
			}
		}
		else if (state[ACTION_SIT2PRONE] == 1)
		{
			ActionRUN();
			if (Action_done[ActionNowFlag] == 1)
			{
				ActionNowFlag = ACTION_SIT2PRONE;
				state[ACTION_SIT2PRONE]++;
			}
		}
		else
			ActionRUN();

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
	case ACTION_UNIT1:

		break;
	case ACTION_UNITTEST:

		break;
	case ACTION_LIEDOWN:
		ActionNowFlag = 12; //
		ActionRUN();
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
		// User_SetDamping(1,1000);
		// User_SetDamping(2,1000);
		// User_SetDamping(3,1000);
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
				goal_pos[i] = Action_index[0]->actions[1].servoAngles[i];
			}
			TEACHMODE = 0;
			TEACH_OK = 0;
			TEACH_FINISH = 0;

			/*用于立刻复现刚刚的动作*/
			ActionNowFlag = 0;
			Action_done[Action_index[ActionNowFlag]->actionId] = 0;
			step_counter = 1;

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
		{
			robotRun();
			// ActionRUN();
		}

		for (int i = 1; i <= 12; i++)
			LookPos[i] = SERVO[i].ang_read;
		if (OPEN == 1)
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port, Servo_Power_Pin, GPIO_PIN_SET); // 舵机供电
		else
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port, Servo_Power_Pin, GPIO_PIN_RESET); // 舵机供电
		osDelay(20);
	}
}
void StartTaskLow(void const *argument)
{

	for (;;)
	{
		osDelay(20);
	}
}
