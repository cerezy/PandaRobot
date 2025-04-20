#ifndef __ACTION_LIBRARY_H__
#define __ACTION_LIBRARY_H__
#include "stdint.h"

#define ACTION_COUNT_MAX 50
//ʾ���ܲ��������ã�Ĭ�϶�ʱ��Ϊ5��/�룬����35�ζ�Ӧ��7s��ʱ����
#define TEACH_TOTAL_STEP 35
//������ÿ���������Ĳ�����
#define MAX_TOTAL_STEP 100
//������һ����������Ϊ����
#define MAX_NUM_MOTION 5


//�����ṹ�壨3ά�ռ����꣩�����ڷ��⣩
typedef struct {
    float x;
    float y;
    float z;
} Coordinate;

// �������нṹ�壨����������
typedef struct {
    int16_t    servoAngles[14];      // �ö����¶���Ƕ�ֵ
} ServoActionStep;

// ��������ö��
typedef enum {
    EMOTION_NEUTRAL = 0,  // ����
    EMOTION_HAPPY,        // ����
    EMOTION_SAD,          // ����
    EMOTION_ANGRY,        // ��ŭ
    EMOTION_SURPRISED,    // ����
    EMOTION_CUSTOM        // �Զ�������
} EmotionType;

// �������ϵ�нṹ��

typedef struct {
    uint16_t        actionId;        // ϵ�ж���Ψһ��ʶ
    ServoActionStep actions[MAX_TOTAL_STEP];     // �ٶ�ÿһ������������������������Ϊ10����ɢ�ĵ�������
    EmotionType     emotionType;     // ��Ӧ����������
	uint16_t        total_step;
    float           totalDuration;   // ��ִ��ʱ�䣨��λ��ms��
	int16_t         startservoAngles[14];  // �ö�����ʼʱ14������Ƕ�ֵ
    int16_t         endservoAngles[14];    // �ö�������ʱ14������Ƕ�ֵ
    uint8_t         ifNeedBezier; // �Ƿ���Ҫ���������߲�ֵ
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
