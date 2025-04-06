#ifndef __ACTION_LIBRARY_H__
#define __ACTION_LIBRARY_H__
#include "stdint.h"

#define ACTION_COUNT_MAX 50
//示教总步进数设置（默认定时器为5次/秒，例如35次对应了7s总时长）
#define TEACH_TOTAL_STEP 35
//定义了每个动作最大的步进数
#define MAX_TOTAL_STEP 100



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
    ServoActionStep actions[MAX_TOTAL_STEP];     // 假定每一个表达情绪的整个动作被拆分为10个离散的单步动作
    EmotionType     emotionType;     // 对应的情绪类型
	uint16_t        total_step;
    float           totalDuration;   // 总执行时间（单位：ms）
} ServoActionSeries;
//#pragma pack(pop)
void Action_init(void);
void Action_Teachmode_Init(void);
void Action_Teachmode(void);
extern ServoActionSeries* Action_index[50];
extern int TEACHMODE;
extern ServoActionSeries Action_TEACH;
extern int TEACH_OK;
extern int TEACH_FINISH;


#endif
