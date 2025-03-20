#ifndef __ACTION_LIBRARY_H__
#define __ACTION_LIBRARY_H__
#include "stdint.h"
//#pragma pack(push, 1)
//坐标点结构体（3维空间坐标）（用于反解）
typedef struct {
    float x;
    float y;
    float z;
} Coordinate;

// 动作序列结构体（单步动作）
typedef struct {
    //Coordinate discretePoints[14];   // 14个舵机对应的坐标点
    int16_t    servoAngles[14];      // 该动作下14个舵机角度值
    float      stepDuration;         // 本步执行时间（单位：ms）
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
    ServoActionStep actions[20];     // 假定每一个表达情绪的整个动作被拆分为10个离散的单步动作
    EmotionType     emotionType;     // 对应的情绪类型
	uint16_t        total_step;
    float           totalDuration;   // 总执行时间（单位：ms）
} ServoActionSeries;
//#pragma pack(pop)
void Action_init(void);
extern ServoActionSeries* Action_index[50];
extern ServoActionSeries Action_Hello;
extern ServoActionSeries Action_Hug;
extern ServoActionSeries Action_Standup;
extern ServoActionSeries Action_Sit2Prone;
extern ServoActionSeries Action_Lie2Standup;;



#endif
