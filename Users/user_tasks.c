#include "user_tasks.h"
#include "cmsis_os.h"
#include "user_servo.h"
#include "user_timer.h"
#include "user_states.h"
#include <stdbool.h>
#include "user_communication.h"

int16_t ang_goal[15] = {0};
uint16_t ms_goal[15] = {0};
uint8_t Action_done[50] = {0};
uint8_t ActionNowFlag = 1;
ACTION_STATE ActionNow = IDLE;  // 当前动作状态
ACTION_STATE ActionLast = IDLE; // 上一个动作状态
uint8_t speed = 8;              // 动作的步进速度（单位0.1°）
int32_t step_counter = 1;       // 当前步进到该动作序列的第几步
uint8_t vis[15] = {0};          // 判断舵机是否抵达位置
uint8_t Servo_Reset_Flag = 0;
uint8_t Init_OK = 0;
int16_t LookPos[14] = {0};
uint8_t actionStandup_getStartAngle = 0; // 执行动作站立时获取当前舵机位置为初始位置
Motion_t_ram* motion_ram_last = &_Action_TEACH;
Motion_t*  motion_last = &M1;
uint8_t ifStartAct = 0;
uint8_t PoweronAction;//上电开始做一系列动作

void User_Init(void)
{
	#if 1
		PoweronAction = 0;//
		TEACHMODE = 1;
	#else
		PoweronAction = 1;//上电开始做一系列动作
		TEACHMODE = 0;
	#endif
	
    Action_init();         // 动作库初始化（动作信息）
    User_ServoInit();      // 舵机信息初始化（对应串口等）
    User_TimerInit();      // 统领全局的定时器
    User_TeachTimerInit(); // 示教模式专用定时器
    User_CommunicationInit(); // 通信协议初始化
    Init_OK = 1;
	if(PoweronAction == 1)
	ActionNow = ACTION_Yawn;
	else ActionNow = IDLE;
}

//// 示教用
//void ActionRUN(void)
//{
//    // 判断当前在进行的动作ActionNowFlag是没有结束的并且验证ID的正确性
//    if (Action_done[ActionNowFlag] == 0)
//    {
//        // 遍历舵机并更新下一时刻的目标值
//        for (int i = 1; i <= 12; i++)
//        {
//            if (goal_pos[i] <= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] - speed)
//                goal_pos[i] += speed;
//            else if (goal_pos[i] >= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] + speed)
//                goal_pos[i] -= speed;
//            // 判断是否抵达目标位置
//            if (goal_pos[i] > Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] - speed && goal_pos[i] < Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] + speed)
//                vis[i] = 1;
//        }
//        for (int i = 1; i <= 12; i++)
//        {
//            if (vis[i] == 0)
//                break;
//            if (i == 12)
//            {
//                // 未执行完所有步数
//                if (step_counter < Action_index[ActionNowFlag]->total_step - 1)
//                {
//                    step_counter++;
//                    for (int i = 1; i <= 12; i++)
//                        vis[i] = 0;
//                }
//                // 执行完毕
//                else if (step_counter == Action_index[ActionNowFlag]->total_step - 1)
//                {
//                    // 当前动作Done
//                    Action_done[Action_index[ActionNowFlag]->actionId] = 1;
//                    // 相关参数复位
//                    step_counter = 1;
//                    actionStandup_getStartAngle = 0;

//                    for (int i = 1; i <= 12; i++)
//                        vis[i] = 0;
//                }
//            }
//        }
//    }
//}

// 判断单独动作是否到位
bool _SingleAction_CheckApproch(ServoActionSeries *action)
{
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
                for (int i = 1; i <= 12; i++)
                    vis[i] = 0;
                return true;
            }
        }
    }
    return false;
}

// 判断单独动作是否到位
bool _SingleAction_CheckApproch_Bezier(ServoActionSeries_ram *action)
{
    // 需要
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
    for (int i = 0; i < motion_->point_total; i++)
    {
        Action_done[motion_->motion[i].actionId] = 0; // 重置动作完成标志
    }
	step_counter = 1;
    motion_->point_iter = 0; // 重置动作点序号
    actionStandup_getStartAngle = 0; // 用于重新获取当前角度从而进行新的贝塞尔曲线计算
}
// 将iter清零，方便再次做这个动作
void Motion_Reset_Bezier(Motion_t_ram *motion_)
{
    for (int i = 0; i < motion_->point_total; i++)
    {
        Action_done[motion_->motion[i].actionId] = 0; // 重置动作完成标志
    }
    motion_->point_iter = 0; // 重置动作点序号
    step_counter = 1;
    actionStandup_getStartAngle = 0; // 用于重新获取当前角度从而进行新的贝塞尔曲线计算
}

extern uint8_t flag_act;
// 运动函数，通过判断iter返回true或false
bool Motion_Run(Motion_t *motion_)
{
	motion_last = motion_;
	if(ifStartAct == 0)
	{
		for (int i = 1; i <= 12; i++)
            {
                goal_pos[i] = motion_->motion[0].actions[1].servoAngles[i];
            }
			flag_act = 1;
			ifStartAct = 1;
	}
    if (_SingleAction_CheckApproch(&motion_->motion[motion_->point_iter]))
    {
        if (motion_->point_iter < motion_->point_total - 1)
        {
            motion_->point_iter++; // 切换下一个动作点
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

// 运动函数，通过判断iter返回true或false
bool Motion_Run_Bezier(Motion_t_ram *motion_)
{
	motion_ram_last = motion_;
	if(ifStartAct == 0)
	{
		for (int i = 1; i <= 12; i++)
            {
                goal_pos[i] = _Action_TEACH.motion[0].actions[1].servoAngles[i];
            }
			ifStartAct = 1;
			flag_act = 1;
	}
    if (_SingleAction_CheckApproch_Bezier(&motion_->motion[motion_->point_iter]))
    {
        if (motion_->point_iter < motion_->point_total - 1)
        {
            motion_->point_iter++; // 切换下一个动作点
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
    case ACTION_TEACH: // 0
                       // 示教
		_Action_TEACH.motion[0].total_step = TEACH_TOTAL_STEP;
		if (Motion_Run_Bezier(&_Action_TEACH) == true)
		{
			Motion_Reset_Bezier(&_Action_TEACH); // 重新使能该动作，便于下次再次跑
			ActionNow = IDLE;
		}
//        ActionNowFlag = 0;
//        ActionRUN();
//		ActionNow = IDLE;
	
	
        break;

    case ACTION_WALK: // 1
        // 走路
        if (Motion_Run(&_Action_Walk) == true)
        {
            Motion_Reset(&_Action_Walk); // 重新使能该动作，便于下次再次跑
        }

        break;

    case ACTION_WAVE: // 2
        // 挥手
        if (Motion_Run(&_Active_Wave) == true)
        {
            Motion_Reset(&_Active_Wave); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_STANDUP: // 3
        // 站立
        if (Motion_Run_Bezier(&_Action_Standup) == true)
        {
            Motion_Reset_Bezier(&_Action_Standup); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_SIT: // 4
        // 坐下
        if (Motion_Run((Motion_t *)&_Active_Sit) == true)
        {
            Motion_Reset((Motion_t *)&_Active_Sit); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_SITTOEAT: // 5 坐着吃东西
        // 坐下吃东西
        if (Motion_Run(&_Active_SittoEat) == true)
        {
            Motion_Reset(&_Active_SittoEat); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_HUG: // 6 拥抱
        // 拥抱
        if (Motion_Run(&_Action_Hug) == true)
        {
            Motion_Reset(&_Action_Hug); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_LIEPRONE: // 7 卧
		if(PoweronAction == 1) PoweronAction = 0;
        if (Motion_Run_Bezier(&_Action_LieProne) == true)
        {
            Motion_Reset_Bezier(&_Action_LieProne); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_BIGLIE: // 8 大字躺
        if (Motion_Run_Bezier(&_Action_BigLie) == true)
        {
            Motion_Reset_Bezier(&_Action_BigLie); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_SIT2PRONE: // 9 坐->卧
        if (Motion_Run(&_Action_Sit2Prone) == true)
        {
            Motion_Reset(&_Action_Sit2Prone); // 重新使能该动作，便于下次再次跑
            if(PoweronAction == 1)
				ActionNow = ACTION_WagHips;
			else
            ActionNow = IDLE;
        }
        break;

    case ACTION_PRONETOSIT: // 10 卧->坐
        if (Motion_Run_Bezier(&_Action_PronetoSit) == true)
        {
            Motion_Reset_Bezier(&_Action_PronetoSit); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_HELLO: // 13 你好
        if (Motion_Run(&_Action_Hello) == true)
        {
            Motion_Reset(&_Action_Hello); // 重新使能该动作，便于下次再次跑
            if(PoweronAction == 1)
				ActionNow = ACTION_M1;
			else
            ActionNow = IDLE;
        }
        break;

    case ACTION_ScratchHead: // 14 挠头
        if (Motion_Run(&_Action_ScratchHead) == true)
        {
            Motion_Reset(&_Action_ScratchHead); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_Worship: // 15 拜一拜
        if (Motion_Run(&_Action_Worship) == true)
        {
            Motion_Reset(&_Action_Worship); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_ShakeHead: // 16 摇头
        if (Motion_Run(&_Action_ShakeHead) == true)
        {
            Motion_Reset(&_Action_ShakeHead); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_Pouting: // 17 撅屁股
        if (Motion_Run(&_Action_Pouting) == true)
        {
            Motion_Reset(&_Action_Pouting); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_TurnThings: // 18 翻东西
        if (Motion_Run(&_Action_TurnThings) == true)
        {
            Motion_Reset(&_Action_TurnThings); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_SleepTilt: // 19 歪头睡觉
        if (Motion_Run(&_Action_SleepTilt) == true)
        {
            Motion_Reset(&_Action_SleepTilt); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_WashFace: // 20 洗脸
        if (Motion_Run(&_Action_WashFace) == true)
        {
            Motion_Reset(&_Action_WashFace); // 重新使能该动作，便于下次再次跑
            if(PoweronAction == 1)
				ActionNow = ACTION_Eat;
			else
            ActionNow = IDLE;
        }
        break;

    case ACTION_SideLieScratch: // 21 侧躺挠痒
        if (Motion_Run(&_Action_SideLieScratch) == true)
        {
            Motion_Reset(&_Action_SideLieScratch); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_SitLegsOpen: // 22 开腿坐
        if (Motion_Run(&_Action_SitLegsOpen) == true)
        {
            Motion_Reset(&_Action_SitLegsOpen); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_StandToSit: // 23 站->坐
        if (Motion_Run(&_Action_StandToSit) == true)
        {
            Motion_Reset(&_Action_StandToSit); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_SideLie: // 24 侧躺
        if (Motion_Run(&_Action_SideLie) == true)
        {
            Motion_Reset(&_Action_SideLie); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_WagHips: // 25 扭屁股
        if (Motion_Run(&_Action_WagHips) == true)
        {
            Motion_Reset(&_Action_WagHips); // 重新使能该动作，便于下次再次跑
            if(PoweronAction == 1)
				ActionNow = ACTION_LIEPRONE;
			else
            ActionNow = IDLE;
        }
        break;

    case ACTION_HighFive: // 26 击掌动作
        if (Motion_Run(&_Action_HighFive) == true)
        {
            Motion_Reset(&_Action_HighFive); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_HugKiss: // 27 环抱kiss动作
        if (Motion_Run(&_Action_HugKiss) == true)
        {
            Motion_Reset(&_Action_HugKiss); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_Handshake: // 28 握手动作
        if (Motion_Run(&_Action_Handshake) == true)
        {
            Motion_Reset(&_Action_Handshake); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_ScratchButt: // 29 挠屁股动作
        if (Motion_Run(&_Action_ScratchButt) == true)
        {
            Motion_Reset(&_Action_ScratchButt); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_Bow: // 30 鞠躬动作
        if (Motion_Run(&_Action_Bow) == true)
        {
            Motion_Reset(&_Action_Bow); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_Cheer: // 31 挥臂加油动作
        if (Motion_Run(&_Action_Cheer) == true)
        {
            Motion_Reset(&_Action_Cheer); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_DrawHeart: // 32 画爱心动作
        if (Motion_Run(&_Action_DrawHeart) == true)
        {
            Motion_Reset(&_Action_DrawHeart); // 重新使能该动作，便于下次再 
            ActionNow = IDLE;
        }
        break;

    case ACTION_StompFoot: // 33 掐腰跺脚动作
        if (Motion_Run(&_Action_StompFoot) == true)
        {
            Motion_Reset(&_Action_StompFoot); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_Drum: // 34 击鼓动作
        if (Motion_Run(&_Action_Drum) == true)
        {
            Motion_Reset(&_Action_Drum); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;

    case ACTION_RubEyes: // 35 揉眼睛动作
        if (Motion_Run(&_Action_RubEyes) == true)
        {
            Motion_Reset(&_Action_RubEyes); // 重新使能该动作，便于下次再次跑
            if(PoweronAction == 1)
				ActionNow = ACTION_WashFace;
			else
            ActionNow = IDLE;
        }
        break;

    case ACTION_Yawn: // 36 打哈欠动作
        if (Motion_Run(&_Action_Yawn) == true)
        {
            Motion_Reset(&_Action_Yawn); // 重新使能该动作，便于下次再次跑
			if(PoweronAction == 1)
				ActionNow = ACTION_RubEyes;
			else
            ActionNow = IDLE;
        }
        break;

    case ACTION_SitPatButt: // 37 坐着拍屁股动作
        if (Motion_Run(&_Action_SitPatButt) == true)
        {
            Motion_Reset(&_Action_SitPatButt); // 重新使能该动作，便于下次再次跑
            ActionNow = IDLE;
        }
        break;
    case ACTION_Eat: // 38 吃东西
        if (Motion_Run(&_Action_Eat) == true)
        {
            Motion_Reset(&_Action_Eat);
            if(PoweronAction == 1)
				ActionNow = ACTION_HELLO;
			else
            ActionNow = IDLE;
        }
        break;

    case ACTION_Stretch: // 39 伸懒腰
        if (Motion_Run(&_Action_Stretch) == true)
        {
            Motion_Reset(&_Action_Stretch);
            ActionNow = IDLE;
        }
        break;

    case ACTION_StretchLying: // 40 躺着伸懒腰
        if (Motion_Run(&_Action_StretchLying) == true)
        {
            Motion_Reset(&_Action_StretchLying);
            ActionNow = IDLE;
        }
        break;

    case ACTION_WaveStanding: // 41 站立挥手
        if (Motion_Run(&_Action_WaveStanding) == true)
        {
            Motion_Reset(&_Action_WaveStanding);
            ActionNow = IDLE;
        }
        break;

    case ACTION_ScratchButtStanding: // 42 站立挠屁股
        if (Motion_Run(&_Action_ScratchButtStanding) == true)
        {
            Motion_Reset(&_Action_ScratchButtStanding);
            ActionNow = IDLE;
        }
        break;

    case ACTION_SplitStanding: // 43 站立劈叉
        if (Motion_Run(&_Action_SplitStanding) == true)
        {
            Motion_Reset(&_Action_SplitStanding);
            ActionNow = IDLE;
        }
        break;

    case ACTION_PatTummy: // 44 拍肚子
        if (Motion_Run(&_Action_PatTummy) == true)
        {
            Motion_Reset(&_Action_PatTummy);
            if(PoweronAction == 1)
				ActionNow = ACTION_SIT2PRONE;
			else
            ActionNow = IDLE;
        }
        break;
    case ACTION_M1:
        if (Motion_Run(&M1) == true)
        {
            Motion_Reset(&M1);
            if(PoweronAction == 1)
				ActionNow = ACTION_PatTummy;
			else
            ActionNow = IDLE;
        }
        break;
    case ACTION_M3:
        if (Motion_Run(&M3) == true)
        {
            Motion_Reset(&M3);
            ActionNow = IDLE;
        }
        break;
		case ACTION_M4:
        if (Motion_Run(&M4) == true)
        {
            Motion_Reset(&M4);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M5:
        if (Motion_Run(&M5) == true)
        {
            Motion_Reset(&M5);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M6:
        if (Motion_Run(&M6) == true)
        {
            Motion_Reset(&M6);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M7:
        if (Motion_Run(&M7) == true)
        {
            Motion_Reset(&M7);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M8:
        if (Motion_Run(&M8) == true)
        {
            Motion_Reset(&M8);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M10:
        if (Motion_Run(&M10) == true)
        {
            Motion_Reset(&M10);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M11:
        if (Motion_Run(&M11) == true)
        {
            Motion_Reset(&M11);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M14:
        if (Motion_Run(&M14) == true)
        {
            Motion_Reset(&M14);
            ActionNow = IDLE;
        }
        break;
		case ACTION_M16:
        if (Motion_Run(&M16) == true)
        {
            Motion_Reset(&M16);
            ActionNow = IDLE;
        }
		break;
		case ACTION_M17:
        if (Motion_Run(&M17) == true)
        {
            Motion_Reset(&M17);
            ActionNow = IDLE;
        }
		break;
    case ACTION_M18:
        if (Motion_Run(&M18) == true)
        {
            Motion_Reset(&M18);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M19:
        if (Motion_Run(&M19) == true)
        {
            Motion_Reset(&M19);
            ActionNow = IDLE;
        }
        break;
		case ACTION_M20:
        if (Motion_Run(&M20) == true)
        {
            Motion_Reset(&M20);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M21:
        if (Motion_Run(&M21) == true)
        {
            Motion_Reset(&M21);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M24:
        if (Motion_Run(&M24) == true)
        {
            Motion_Reset(&M24);
            ActionNow = IDLE;
        }
        break;
		case ACTION_M26:
        if (Motion_Run(&M26) == true)
        {
            Motion_Reset(&M26);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M28:
        if (Motion_Run(&M28) == true)
        {
            Motion_Reset(&M28);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M31:
        if (Motion_Run(&M31) == true)
        {
            Motion_Reset(&M31);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M32:
        if (Motion_Run(&M32) == true)
        {
            Motion_Reset(&M32);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M33:
        if (Motion_Run(&M33) == true)
        {
            Motion_Reset(&M33);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M34:
        if (Motion_Run(&M34) == true)
        {
            Motion_Reset(&M34);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M37:
        if (Motion_Run(&M37) == true)
        {
            Motion_Reset(&M37);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M38:
        if (Motion_Run(&M38) == true)
        {
            Motion_Reset(&M38);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M46:
        if (Motion_Run(&M46) == true)
        {
            Motion_Reset(&M46);
            ActionNow = IDLE;
        }
        break;
    case ACTION_M49:
        if (Motion_Run(&M49) == true)
        {
            Motion_Reset(&M49);
            ActionNow = IDLE;
        }
        break;
		case ACTION_M61:
    if (Motion_Run(&M61) == true)
    {
        Motion_Reset(&M61);
        ActionNow = IDLE;
    }
    break;

case ACTION_M64:
    if (Motion_Run(&M64) == true)
    {
        Motion_Reset(&M64);
        ActionNow = IDLE;
    }
    break;

case ACTION_M65:
    if (Motion_Run(&M65) == true)
    {
        Motion_Reset(&M65);
        ActionNow = IDLE;
    }
    break;

case ACTION_M67:
    if (Motion_Run(&M67) == true)
    {
        Motion_Reset(&M67);
        ActionNow = IDLE;
    }
    break;

case ACTION_M70:
    if (Motion_Run(&M70) == true)
    {
        Motion_Reset(&M70);
        ActionNow = IDLE;
    }
    break;

case ACTION_M71:
    if (Motion_Run(&M71) == true)
    {
        Motion_Reset(&M71);
        ActionNow = IDLE;
    }
    break;

case ACTION_M73:
    if (Motion_Run(&M73) == true)
    {
        Motion_Reset(&M73);
        ActionNow = IDLE;
    }
    break;

case ACTION_M75:
    if (Motion_Run(&M75) == true)
    {
        Motion_Reset(&M75);
        ActionNow = IDLE;
    }
    break;

case ACTION_M76:
    if (Motion_Run(&M76) == true)
    {
        Motion_Reset(&M76);
        ActionNow = IDLE;
    }
    break;

case ACTION_M78:
    if (Motion_Run(&M78) == true)
    {
        Motion_Reset(&M78);
        ActionNow = IDLE;
    }
    break;

case ACTION_M80:
    if (Motion_Run(&M80) == true)
    {
        Motion_Reset(&M80);
        ActionNow = IDLE;
    }
    break;

case ACTION_M83:
    if (Motion_Run(&M83) == true)
    {
        Motion_Reset(&M83);
        ActionNow = IDLE;
    }
    break;

case ACTION_M87:
    if (Motion_Run(&M87) == true)
    {
        Motion_Reset(&M87);
        ActionNow = IDLE;
    }
    break;

case ACTION_M89:
    if (Motion_Run(&M89) == true)
    {
        Motion_Reset(&M89);
        ActionNow = IDLE;
    }
    break;

case ACTION_M90:
    if (Motion_Run(&M90) == true)
    {
        Motion_Reset(&M90);
        ActionNow = IDLE;
    }
    break;

case ACTION_M91:
    if (Motion_Run(&M91) == true)
    {
        Motion_Reset(&M91);
        ActionNow = IDLE;
    }
    break;

case ACTION_M92:
    if (Motion_Run(&M92) == true)
    {
        Motion_Reset(&M92);
        ActionNow = IDLE;
    }
    break;

case ACTION_M94:
    if (Motion_Run(&M94) == true)
    {
        Motion_Reset(&M94);
        ActionNow = IDLE;
    }
    break;

case ACTION_M121:
    if (Motion_Run(&M121) == true)
    {
        Motion_Reset(&M121);
        ActionNow = IDLE;
    }
    break;

case ACTION_M123:
    if (Motion_Run(&M123) == true)
    {
        Motion_Reset(&M123);
        ActionNow = IDLE;
    }
    break;

case ACTION_M125:
    if (Motion_Run(&M125) == true)
    {
        Motion_Reset(&M125);
        ActionNow = IDLE;
    }
    break;

case ACTION_M126:
    if (Motion_Run(&M126) == true)
    {
        Motion_Reset(&M126);
        ActionNow = IDLE;
    }
    break;

case ACTION_M136:
    if (Motion_Run(&M136) == true)
    {
        Motion_Reset(&M136);
        ActionNow = IDLE;
    }
    break;
case ACTION_M138:
    if (Motion_Run(&M138) == true)
    {
        Motion_Reset(&M138);
        ActionNow = IDLE;
    }
    break;

case ACTION_M151:
    if (Motion_Run(&M151) == true)
    {
        Motion_Reset(&M151);
        ActionNow = IDLE;
    }
    break;

case ACTION_M152:
    if (Motion_Run(&M152) == true)
    {
        Motion_Reset(&M152);
        ActionNow = IDLE;
    }
    break;

case ACTION_M155:
    if (Motion_Run(&M155) == true)
    {
        Motion_Reset(&M155);
        ActionNow = IDLE;
    }
    break;

case ACTION_M156:
    if (Motion_Run(&M156) == true)
    {
        Motion_Reset(&M156);
        ActionNow = IDLE;
    }
    break;

case ACTION_M157:
    if (Motion_Run(&M157) == true)
    {
        Motion_Reset(&M157);
        ActionNow = IDLE;
    }
    break;

case ACTION_M162:
    if (Motion_Run(&M162) == true)
    {
        Motion_Reset(&M162);
        ActionNow = IDLE;
    }
    break;

case ACTION_M163:
    if (Motion_Run(&M163) == true)
    {
        Motion_Reset(&M163);
        ActionNow = IDLE;
    }
    break;

case ACTION_M164:
    if (Motion_Run(&M164) == true)
    {
        Motion_Reset(&M164);
        ActionNow = IDLE;
    }
    break;

case ACTION_M166:
    if (Motion_Run(&M166) == true)
    {
        Motion_Reset(&M166);
        ActionNow = IDLE;
    }
    break;

case ACTION_M167:
    if (Motion_Run(&M167) == true)
    {
        Motion_Reset(&M167);
        ActionNow = IDLE;
    }
    break;

case ACTION_M168:
    if (Motion_Run(&M168) == true)
    {
        Motion_Reset(&M168);
        ActionNow = IDLE;
    }
    break;

case ACTION_M169:
    if (Motion_Run(&M169) == true)
    {
        Motion_Reset(&M169);
        ActionNow = IDLE;
    }
    break;

case ACTION_M170:
    if (Motion_Run(&M170) == true)
    {
        Motion_Reset(&M170);
        ActionNow = IDLE;
    }
    break;

case ACTION_M171:
    if (Motion_Run(&M171) == true)
    {
        Motion_Reset(&M171);
        ActionNow = IDLE;
    }
    break;

case ACTION_M172:
    if (Motion_Run(&M172) == true)
    {
        Motion_Reset(&M172);
        ActionNow = IDLE;
    }
    break;

case ACTION_M173:
    if (Motion_Run(&M173) == true)
    {
        Motion_Reset(&M173);
        ActionNow = IDLE;
    }
    break;

case ACTION_M176:
    if (Motion_Run(&M176) == true)
    {
        Motion_Reset(&M176);
        ActionNow = IDLE;
    }
    break;

case ACTION_M178:
    if (Motion_Run(&M178) == true)
    {
        Motion_Reset(&M178);
        ActionNow = IDLE;
    }
    break;

case ACTION_M182:
    if (Motion_Run(&M182) == true)
    {
        Motion_Reset(&M182);
        ActionNow = IDLE;
    }
    break;

case ACTION_M184:
    if (Motion_Run(&M184) == true)
    {
        Motion_Reset(&M184);
        ActionNow = IDLE;
    }
    break;

case ACTION_M191:
    if (Motion_Run(&M191) == true)
    {
        Motion_Reset(&M191);
        ActionNow = IDLE;
    }
    break;

case ACTION_M195:
    if (Motion_Run(&M195) == true)
    {
        Motion_Reset(&M195);
        ActionNow = IDLE;
    }
    break;

case ACTION_M196:
    if (Motion_Run(&M196) == true)
    {
        Motion_Reset(&M196);
        ActionNow = IDLE;
    }
    break;

case ACTION_M202:
    if (Motion_Run(&M202) == true)
    {
        Motion_Reset(&M202);
        ActionNow = IDLE;
    }
    break;

case ACTION_M204:
    if (Motion_Run(&M204) == true)
    {
        Motion_Reset(&M204);
        ActionNow = IDLE;
    }
    break;

case ACTION_M205:
    if (Motion_Run(&M205) == true)
    {
        Motion_Reset(&M205);
        ActionNow = IDLE;
    }
    break;

		
	case IDLE:
		
		Motion_Reset(motion_last);
		Motion_Reset_Bezier(motion_ram_last);
		step_counter = 1;
		ifStartAct = 0;
	
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
            
            TEACH_OK = 0;
            TEACH_FINISH = 0;
			TEACHMODE = 0;

            /*用于立刻复现刚刚的动作*/
            ActionNowFlag = 0;
            Action_done[0] = 0;
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
