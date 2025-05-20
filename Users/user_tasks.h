#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

//上电之后 打哈欠，揉眼睛，洗脸，吃东西，比爱心

typedef enum
{
    ACTION_TEACH = 0, // 示教
    // 4.21
    ACTION_WALK,           // 1 走
    ACTION_WAVE,           // 2 挥手
    ACTION_STANDUP,        // 3 站
    ACTION_SIT,            // 4 坐
    ACTION_SITTOEAT,       // 5 坐着吃东西
    ACTION_HUG,            // 6 拥抱
    ACTION_LIEPRONE,       // 7 卧
    ACTION_BIGLIE,         // 8 大字躺
    ACTION_SIT2PRONE,      // 9 坐->卧
    ACTION_PRONETOSIT,     // 10 卧->坐
    ACTION_TEST,           // 11
    IDLE,                  // 12 空闲
    ACTION_HELLO,          // 13 你好
    ACTION_ScratchHead,    // 14 挠头
    ACTION_Worship,        // 15 拜一拜
    ACTION_ShakeHead,      // 16 摇头
    ACTION_Pouting,        // 17 撅屁股
    ACTION_TurnThings,     // 18翻东西
    ACTION_SleepTilt,      // 19歪头睡觉
    ACTION_WashFace,       // 20洗脸
    ACTION_SideLieScratch, // 21 侧躺挠痒
    ACTION_SitLegsOpen,    // 22开腿坐
    ACTION_StandToSit,     // 23站->坐
    ACTION_SideLie,        // 24侧躺
    ACTION_WagHips,        // 25扭屁股
    // 4.27
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
    ACTION_SitPatButt,  // 37坐着撅\拍屁股动作

    ACTION_Eat,                 // 38 吃东西
    ACTION_Stretch,             // 39 坐着伸懒腰
    ACTION_StretchLying,        // 40 躺着伸懒腰
    ACTION_WaveStanding,        // 41 站立挥手
    ACTION_ScratchButtStanding, // 42 站立挠屁股
    ACTION_SplitStanding,       // 43 站立劈叉
    ACTION_PatTummy,            // 44 单手拍肚子

    // 5.11补充的动作
    ACTION_M1,  // M1动作45
    ACTION_M3,  // M3动作46
	ACTION_M4,  // M4动作47
    ACTION_M5,  // M5动作48
    ACTION_M6,  // M6动作49
    ACTION_M7,  // M7动作50
    ACTION_M8,  // M8动作51
    ACTION_M10, // M10动作52
    ACTION_M11, // M11动作53
    ACTION_M14, // M14动作54
	ACTION_M16, // M16动作55
	ACTION_M17, // M17动作56
    ACTION_M18, // M18动作57
    ACTION_M19, // M19动作58
	ACTION_M20, // M20动作59
    ACTION_M21, // M21动作60
    ACTION_M24, // M24动作61.
	ACTION_M26, // M26动作62
    ACTION_M28, // M28动作63.
    ACTION_M31, // M31动作64
    ACTION_M32, // M32动作65
    ACTION_M33, // M33动作66
    ACTION_M34, // M34动作67
    ACTION_M37, // M37动作68
    ACTION_M38, // M38动作69.
    ACTION_M46, // M46动作70
    ACTION_M49, // M49动作71
	
	//5.14
	ACTION_M61,  // M61动作72
	ACTION_M64,  // M64动作73
	ACTION_M65,  // M65动作74,
	ACTION_M67,  // M67动作75	
	ACTION_M70,  // M70动作76
	ACTION_M71,  // M71动作77
	ACTION_M73,  // M73动作78
	ACTION_M75,  // M75动作79.
	ACTION_M76,  // M76动作80.
	ACTION_M78,  // M78动作81
	ACTION_M80,  // M80动作82。直接一个最终角度
	ACTION_M83,  // M83动作83。
	ACTION_M87,  // M87动作84.直接一个最终角度
	ACTION_M89,  // M89动作85，直接最终角度
	ACTION_M90,  // M90动作86
	ACTION_M91,  // M91动作87，
	ACTION_M92,  // M92动作88
	ACTION_M94,  // M94动作89
	ACTION_M121, // M121动作90	
	ACTION_M123, // M123动作91
	ACTION_M125, // M125动作92
	ACTION_M126, // M126动作93
	ACTION_M136, // M136动作94，
	ACTION_M138, // M138动作95，
	ACTION_M151, // M151动作96
	ACTION_M152, // M152动作97
	ACTION_M155, // M155动作98
	ACTION_M156, // M156动作99
	ACTION_M157, // M157动作100
	ACTION_M162, // M162动作101
	ACTION_M163, // M163动作102
	ACTION_M164, // M164动作103
	ACTION_M166, // M166动作104
	ACTION_M167, // M167动作105
	ACTION_M168, // M168动作106
	ACTION_M169, // M169动作107
	ACTION_M170, // M170动作108
	ACTION_M171, // M171动作109
	ACTION_M172, // M172动作110
	ACTION_M173, // M173动作111
	ACTION_M176, // M176动作112
	ACTION_M178, // M178动作113
	ACTION_M182, // M182动作114
	ACTION_M184, // M184动作115
	ACTION_M191, // M191动作116
	ACTION_M195, // M195动作117
	ACTION_M196, // M196动作118
	ACTION_M202, // M202动作119
	ACTION_M204, // M204动作120
	ACTION_M205, // M205动作121

} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif
