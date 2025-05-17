#ifndef __ACTION_LIBRARY_H__
#define __ACTION_LIBRARY_H__
#include "stdint.h"

#define ACTION_COUNT_MAX 50
// 示教总步进数设置（默认定时器为5次/秒，例如35次对应了7s总时长）
#define TEACH_TOTAL_STEP 35
// 定义了每个动作最大的步进数
#define MAX_TOTAL_STEP 60
// 定义了一个动作最多分为几步
#define MAX_NUM_MOTION 5

// 坐标点结构体（3维空间坐标）（用于反解）
typedef struct
{
    float x;
    float y;
    float z;
} Coordinate;

// 情绪类型枚举
typedef enum
{
    EMOTION_NEUTRAL = 0, // 中性
    EMOTION_HAPPY,       // 开心
    EMOTION_SAD,         // 悲伤
    EMOTION_ANGRY,       // 愤怒
    EMOTION_SURPRISED,   // 惊讶
    EMOTION_CUSTOM       // 自定义类型
} EmotionType;

// 动作序列结构体（单步动作）
typedef struct
{
    const int16_t servoAngles[14]; // 该动作下舵机角度值 保持 const，但通过外部常量初始化
} ServoActionStep;

// 舵机动作系列结构体
typedef struct
{
    uint16_t actionId;
    const ServoActionStep *actions; // 改为指针，指向 Flash 数据
    EmotionType emotionType;        // 对应的情绪类型
    uint16_t total_step;
    float totalDuration;          // 总执行时间（单位：ms）
    int16_t startservoAngles[14]; // 该动作开始时14个舵机角度值
    int16_t endservoAngles[14];   // 该动作结束时14个舵机角度值
    uint8_t ifNeedBezier;         // 是否需要贝塞尔曲线插值
} ServoActionSeries;

typedef struct
{
    ServoActionSeries motion[MAX_NUM_MOTION];
    uint32_t point_total;
    uint32_t point_iter;
} Motion_t;

typedef struct
{
    int16_t servoAngles[14]; // 该动作下舵机角度值
} ServoActionStep_ram;

typedef struct
{
    uint16_t actionId;                           // 系列动作唯一标识
    ServoActionStep_ram actions[MAX_TOTAL_STEP]; // 假定每一个表达情绪的整个动作被拆分为10个离散的单步动作
    EmotionType emotionType;                     // 对应的情绪类型
    uint16_t total_step;
    float totalDuration;          // 总执行时间（单位：ms）
    int16_t startservoAngles[14]; // 该动作开始时14个舵机角度值
    int16_t endservoAngles[14];   // 该动作结束时14个舵机角度值
    uint8_t ifNeedBezier;         // 是否需要贝塞尔曲线插值
} ServoActionSeries_ram;

typedef struct
{
    ServoActionSeries_ram motion[MAX_NUM_MOTION];
    uint32_t point_total;
    uint32_t point_iter;
} Motion_t_ram;

// #pragma pack(pop)
void Action_init(void);
void Action_Teachmode_Init(void);
void Action_Teachmode(void);
extern ServoActionSeries_ram *Action_index[50];
extern int TEACHMODE;
extern ServoActionSeries_ram Action_TEACH;
extern int TEACH_OK;
extern int TEACH_FINISH;

extern Motion_t_ram _Action_TEACH;
extern Motion_t_ram _Action_Standup;
extern Motion_t_ram _Action_LieProne;
extern Motion_t_ram _Action_BigLie;
extern Motion_t_ram _Action_PronetoSit; // 分两步

extern Motion_t _Action_Walk;
extern Motion_t _Active_Wave;
extern Motion_t _Active_Sit;
extern Motion_t _Active_SittoEat;
extern Motion_t _Action_Hug;
extern Motion_t _Action_Sit2Prone;
extern Motion_t _Action_Test;
extern Motion_t _Action_Hello;
extern Motion_t _Action_ScratchHead;
extern Motion_t _Action_Worship;
extern Motion_t _Action_ShakeHead;
extern Motion_t _Action_Pouting;
extern Motion_t _Action_TurnThings;
extern Motion_t _Action_SleepTilt;
extern Motion_t _Action_WashFace;
extern Motion_t _Action_SideLieScratch;
extern Motion_t _Action_SitLegsOpen;
extern Motion_t _Action_StandToSit;
extern Motion_t _Action_SideLie;
extern Motion_t _Action_WagHips;

extern Motion_t _Action_HighFive;
extern Motion_t _Action_HugKiss;
extern Motion_t _Action_Handshake;
extern Motion_t _Action_ScratchButt;
extern Motion_t _Action_Bow;
extern Motion_t _Action_Cheer;
extern Motion_t _Action_DrawHeart;
extern Motion_t _Action_StompFoot;
extern Motion_t _Action_Drum;
extern Motion_t _Action_RubEyes;
extern Motion_t _Action_Yawn;
extern Motion_t _Action_SitPatButt;
extern Motion_t _Action_Eat;
extern Motion_t _Action_Stretch;
extern Motion_t _Action_StretchLying;
extern Motion_t _Action_WaveStanding;
extern Motion_t _Action_ScratchButtStanding;
extern Motion_t _Action_SplitStanding;
extern Motion_t _Action_PatTummy;

extern Motion_t M1;
extern Motion_t M3;
extern Motion_t M4;
extern Motion_t M5;
extern Motion_t M6;
extern Motion_t M7;
extern Motion_t M8;
extern Motion_t M10;
extern Motion_t M11;
extern Motion_t M14;
extern Motion_t M16;
extern Motion_t M17;
extern Motion_t M18;
extern Motion_t M19;
extern Motion_t M20;
extern Motion_t M21;
extern Motion_t M24;
extern Motion_t M26;
extern Motion_t M28;
extern Motion_t M31;
extern Motion_t M32;
extern Motion_t M33;
extern Motion_t M34;
extern Motion_t M37;
extern Motion_t M38;
extern Motion_t M46;
extern Motion_t M49;

extern Motion_t M61;
extern Motion_t M64;
extern Motion_t M65;
extern Motion_t M67;
extern Motion_t M70;
extern Motion_t M71;
extern Motion_t M73;
extern Motion_t M75;
extern Motion_t M76;
extern Motion_t M78;
extern Motion_t M80;
extern Motion_t M83;
extern Motion_t M87;
extern Motion_t M89;
extern Motion_t M90;
extern Motion_t M91;
extern Motion_t M92;
extern Motion_t M94;
extern Motion_t M121;
extern Motion_t M123;
extern Motion_t M125;
extern Motion_t M126;
extern Motion_t M136;
extern Motion_t M138;
extern Motion_t M151;
extern Motion_t M152;
extern Motion_t M155;
extern Motion_t M156;
extern Motion_t M157;
extern Motion_t M162;
extern Motion_t M163;
extern Motion_t M164;
extern Motion_t M166;
extern Motion_t M167;
extern Motion_t M168;
extern Motion_t M169;
extern Motion_t M170;
extern Motion_t M171;
extern Motion_t M172;
extern Motion_t M173;
extern Motion_t M176;
extern Motion_t M178;
extern Motion_t M182;
extern Motion_t M184;
extern Motion_t M191;
extern Motion_t M195;
extern Motion_t M196;
extern Motion_t M202;
extern Motion_t M204;
extern Motion_t M205;

#endif
