#ifndef __ACTION_LIBRARY_H__
#define __ACTION_LIBRARY_H__
#include "stdint.h"
//#pragma pack(push, 1)
//�����ṹ�壨3ά�ռ����꣩�����ڷ��⣩
typedef struct {
    float x;
    float y;
    float z;
} Coordinate;

// �������нṹ�壨����������
typedef struct {
    //Coordinate discretePoints[14];   // 14�������Ӧ�������
    int16_t    servoAngles[14];      // �ö�����14������Ƕ�ֵ
    float      stepDuration;         // ����ִ��ʱ�䣨��λ��ms��
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
    ServoActionStep actions[20];     // �ٶ�ÿһ������������������������Ϊ10����ɢ�ĵ�������
    EmotionType     emotionType;     // ��Ӧ����������
	uint16_t        total_step;
    float           totalDuration;   // ��ִ��ʱ�䣨��λ��ms��
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
