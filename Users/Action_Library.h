#ifndef __ACTION_LIBRARY_H__
#define __ACTION_LIBRARY_H__
#include "stdint.h"

#define ACTION_COUNT_MAX 50
//示教总步进数设置（默认定时器为5次/秒，例如35次对应了7s总时长）
#define TEACH_TOTAL_STEP 35
//定义了每个动作最大的步进数
#define MAX_TOTAL_STEP 100
//定义了一个动作最多分为几步
#define MAX_NUM_MOTION 5


//坐标点结构体（3维空间坐标）（用于反解）
typedef struct {
    float x;
    float y;
    float z;
} Coordinate;

// 动作序列结构体（单步动作）
typedef struct {
    int16_t    servoAngles[14];      // 该动作下舵机角度值
} ServoActionStep;

// 情绪类型枚举
typedef enum {
    EMOTION_NEUTRAL = 0,  // 中性
    EMOTION_HAPPY,        // 开心
    EMOTION_SAD,          // 悲伤
    EMOTION_ANGRY,        // 愤怒
    EMOTION_SURPRISED,    // 惊讶
    EMOTION_CUSTOM        // 自定义类型
} EmotionType;

// 舵机动作系列结构体

typedef struct {
    uint16_t        actionId;        // 系列动作唯一标识
    ServoActionStep actions[MAX_TOTAL_STEP];     // 假定每一个表达情绪的整个动作被拆分为10个离散的单步动作
    EmotionType     emotionType;     // 对应的情绪类型
	uint16_t        total_step;
    float           totalDuration;   // 总执行时间（单位：ms）
	int16_t         startservoAngles[14];  // 该动作开始时14个舵机角度值
    int16_t         endservoAngles[14];    // 该动作结束时14个舵机角度值
    uint8_t         ifNeedBezier; // 是否需要贝塞尔曲线插值
} ServoActionSeries;

typedef struct 
{
    ServoActionSeries motion[MAX_NUM_MOTION];
    uint32_t point_total;
    uint32_t point_iter;
} Motion_t;


//#pragma pack(pop)
void Action_init(void);
void Action_Teachmode_Init(void);
void Action_Teachmode(void);
extern ServoActionSeries* Action_index[50];
extern int TEACHMODE;
extern ServoActionSeries Action_TEACH;
extern int TEACH_OK;
extern int TEACH_FINISH;


extern Motion_t _Action_TEACH;
extern Motion_t _Action_Walk;
extern Motion_t _Active_Wave;
extern Motion_t _Action_Standup;
extern Motion_t _Active_Sit;
extern Motion_t _Active_SittoEat;
extern Motion_t _Action_Hug;
extern Motion_t _Action_LieProne;
extern Motion_t _Action_BigLie;
extern Motion_t _Action_Sit2Prone;
extern Motion_t _Action_PronetoSit;
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
#endif
