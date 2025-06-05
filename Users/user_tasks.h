#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

// 上电之后 打哈欠，揉眼睛，洗脸，吃东西，比爱心

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
    IDLE,                  // 10 空闲
    ACTION_HELLO,          // 11 你好
    ACTION_ScratchHead,    // 12 挠头
    ACTION_Worship,        // 13 拜一拜
    ACTION_ShakeHead,      // 14 摇头
    ACTION_Pouting,        // 15 撅屁股
    ACTION_TurnThings,     // 16 翻东西
    ACTION_SleepTilt,      // 17 歪头睡觉
    ACTION_WashFace,       // 18 洗脸
    ACTION_SideLieScratch, // 19 侧躺挠痒
    ACTION_SitLegsOpen,    // 20 开腿坐
    ACTION_StandToSit,     // 21 站->坐
    ACTION_SideLie,        // 22 侧躺
    ACTION_WagHips,        // 23 扭屁股
    // 4.27
    ACTION_HighFive,    // 24 击掌动作
    ACTION_HugKiss,     // 25 环抱kiss动作
    ACTION_Handshake,   // 26 握手动作
    ACTION_ScratchButt, // 27 坐着手挠屁股动作
    ACTION_Bow,         // 28 鞠躬动作
    ACTION_Cheer,       // 29 挥臂加油动作
    ACTION_DrawHeart,   // 30 画爱心动作
    ACTION_StompFoot,   // 31 掐腰跺脚动作
    ACTION_Drum,        // 32 击鼓动作
    ACTION_RubEyes,     // 33 揉眼睛动作
    ACTION_Yawn,        // 34 打哈欠动作
    ACTION_SitPatButt,  // 35 坐着撅\拍屁股动作

    ACTION_Eat,                 // 36 吃东西
    ACTION_Stretch,             // 37 坐着伸懒腰
    ACTION_StretchLying,        // 38 躺着伸懒腰
    ACTION_WaveStanding,        // 39 站立挥手
    ACTION_ScratchButtStanding, // 40 站立挠屁股
    ACTION_SplitStanding,       // 41 站立劈叉
    ACTION_PatTummy,            // 42 单手拍肚子

    // 5.11补充的动作
    ACTION_M1,  // M1动作43
    ACTION_M3,  // M3动作44
    ACTION_M4,  // M4动作45
    ACTION_M5,  // M5动作46
    ACTION_M6,  // M6动作47
    ACTION_M7,  // M7动作48
    ACTION_M8,  // M8动作49
    ACTION_M10, // M10动作50
    ACTION_M11, // M11动作51
    ACTION_M14, // M14动作52
    ACTION_M16, // M16动作53
    ACTION_M17, // M17动作54
    ACTION_M18, // M18动作55
    ACTION_M19, // M19动作56
    ACTION_M20, // M20动作57
    ACTION_M21, // M21动作58
    ACTION_M24, // M24动作59.
    ACTION_M26, // M26动作60
    ACTION_M28, // M28动作61.
    ACTION_M31, // M31动作62
    ACTION_M32, // M32动作63
    ACTION_M33, // M33动作64
    ACTION_M34, // M34动作65
    ACTION_M37, // M37动作66
    ACTION_M38, // M38动作67.
    ACTION_M46, // M46动作68
    ACTION_M49, // M49动作69

    // 5.14
    ACTION_M61,  // M61动作70
    ACTION_M63,  // M63动作71 ← 新增
    ACTION_M64,  // M64动作72
    ACTION_M65,  // M65动作73
    ACTION_M67,  // M67动作74
    ACTION_M70,  // M70动作75
    ACTION_M71,  // M71动作76
    ACTION_M73,  // M73动作77
    ACTION_M75,  // M75动作78
    ACTION_M76,  // M76动作79
    ACTION_M77,  // M77动作80 ← 新增
    ACTION_M78,  // M78动作81
    ACTION_M80,  // M80动作82
    ACTION_M83,  // M83动作83
    ACTION_M87,  // M87动作84
    ACTION_M89,  // M89动作85
    ACTION_M90,  // M90动作86
    ACTION_M91,  // M91动作87
    ACTION_M92,  // M92动作88
    ACTION_M94,  // M94动作89
    ACTION_M98,  // M98动作90 ← 新增
    ACTION_M99,  // M99动作91 ← 新增
    ACTION_M102, // M102动作92 ← 新增
    ACTION_M121, // M121动作93
    ACTION_M123, // M123动作94
    ACTION_M125, // M125动作95
    ACTION_M126, // M126动作96
    ACTION_M130, // M130动作97 ← 新增
    ACTION_M136, // M136动作98
    ACTION_M137, // M137动作99 ← 新增
    ACTION_M138, // M138动作100
    ACTION_M148, // M148动作101 ← 新增
    ACTION_M151, // M151动作102
    ACTION_M152, // M152动作103
    ACTION_M155, // M155动作104
    ACTION_M156, // M156动作105
    ACTION_M157, // M157动作106
    ACTION_M160, // M160动作107 ← 新增
    ACTION_M162, // M162动作108
    ACTION_M163, // M163动作109
    ACTION_M164, // M164动作110
    ACTION_M166, // M166动作111
    ACTION_M167, // M167动作112
    ACTION_M168, // M168动作113
    ACTION_M169, // M169动作114
    ACTION_M170, // M170动作115
    ACTION_M171, // M171动作116
    ACTION_M172, // M172动作117
    ACTION_M173, // M173动作118
    ACTION_M176, // M176动作119
    ACTION_M177, // M177动作120 ← 新增
    ACTION_M178, // M178动作121
    ACTION_M182, // M182动作122
    ACTION_M184, // M184动作123
    ACTION_M191, // M191动作124
    ACTION_M195, // M195动作125
    ACTION_M196, // M196动作126
    ACTION_M202, // M202动作127
    ACTION_M204, // M204动作128
    ACTION_M205, // M205动作129
	
    ACTION_M179, // M179动作130
    ACTION_M181, // M181动作131
    ACTION_M189, // M189动作132
    ACTION_M193, // M193动作133
    ACTION_M197, // M197动作134
    ACTION_M199, // M199动作135
    ACTION_M207, // M207动作136
    ACTION_M210, // M210动作137
    ACTION_MCRAWL_TO_SIT,    // 趴->坐 动作138
    ACTION_MSIT_TO_CRAWL,    // 坐->趴 动作139
    ACTION_MSIT_TO_STAND,    // 坐->站 动作140
    ACTION_MSTAND_TO_SIT,    // 站->坐 动作141
    ACTION_MSTAND_TO_CRAWL,  // 站->趴 动作142
    ACTION_MCRAWL_TO_STAND,  // 趴->站 动作143
} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif
