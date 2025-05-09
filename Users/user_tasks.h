#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

typedef enum
{
    ACTION_TEACH = 0, //示教
	// 4.21
    ACTION_WALK,        // 1 走
    ACTION_WAVE,        // 2 挥手
    ACTION_STANDUP,     // 3 站
    ACTION_SIT,         // 4 坐
    ACTION_SITTOEAT,    // 5 坐着吃东西
    ACTION_HUG,         // 6 拥抱
    ACTION_LIEPRONE,    // 7 卧
    ACTION_BIGLIE,      // 8 大字躺
    ACTION_SIT2PRONE,   // 9 坐->卧
    ACTION_PRONETOSIT,  // 10 卧->坐
    ACTION_TEST,        // 11
    IDLE,               // 12 空闲
    ACTION_HELLO,       // 13 你好
    ACTION_ScratchHead, // 14 挠头
    ACTION_Worship,     // 15 拜一拜
    ACTION_ShakeHead,   // 16 摇头
    ACTION_Pouting,     // 17 撅屁股
    ACTION_TurnThings,  // 18翻东西
    ACTION_SleepTilt,   // 19歪头睡觉
    ACTION_WashFace,    // 20洗脸
    ACTION_SideLieScratch, //21 侧躺挠痒
    ACTION_SitLegsOpen, // 22开腿坐
    ACTION_StandToSit,  // 23站->坐
    ACTION_SideLie,     // 24侧躺
    ACTION_WagHips,     // 25扭屁股
	//4.27
	ACTION_HighFive,    // 26击掌动作
    ACTION_HugKiss,     // 27环抱kiss动作
    ACTION_Handshake,   // 28握手动作
    ACTION_ScratchButt, // 29坐着手挠屁股动作
    ACTION_Bow,         // 30鞠躬动作
    ACTION_Cheer,       // 31挥臂加油动作
    ACTION_DrawHeart,   // 32画爱心动作
    ACTION_StompFoot,   // 33掐腰跺脚动作
    ACTION_Drum,        // 34击鼓动作
    ACTION_RubEyes,     // 35揉眼睛动作
    ACTION_Yawn,        // 36打哈欠动作
    ACTION_SitPatButt,   // 37坐着撅\拍屁股动作
	
	ACTION_Eat,         // 38 吃东西
    ACTION_Stretch,     // 39 坐着伸懒腰
    ACTION_StretchLying,// 40 躺着伸懒腰
    ACTION_WaveStanding,// 41 站立挥手
    ACTION_ScratchButtStanding, // 42 站立挠屁股
    ACTION_SplitStanding, // 43 站立劈叉
    ACTION_PatTummy,    // 44 拍肚子
	
} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif
