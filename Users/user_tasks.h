#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

typedef enum
{
    ACTION_TEACH = 0, //ʾ��
	// 4.21
    ACTION_WALK,        // 1 ��
    ACTION_WAVE,        // 2 ����
    ACTION_STANDUP,     // 3 վ
    ACTION_SIT,         // 4 ��
    ACTION_SITTOEAT,    // 5 ���ųԶ���
    ACTION_HUG,         // 6 ӵ��
    ACTION_LIEPRONE,    // 7 ��
    ACTION_BIGLIE,      // 8 ������
    ACTION_SIT2PRONE,   // 9 ��->��
    ACTION_PRONETOSIT,  // 10 ��->��
    ACTION_TEST,        // 11
    IDLE,               // 12 ����
    ACTION_HELLO,       // 13 ���
    ACTION_ScratchHead, // 14 ��ͷ
    ACTION_Worship,     // 15 ��һ��
    ACTION_ShakeHead,   // 16 ҡͷ
    ACTION_Pouting,     // 17 ��ƨ��
    ACTION_TurnThings,  // 18������
    ACTION_SleepTilt,   // 19��ͷ˯��
    ACTION_WashFace,    // 20ϴ��
    ACTION_SideLieScratch, //21 ��������
    ACTION_SitLegsOpen, // 22������
    ACTION_StandToSit,  // 23վ->��
    ACTION_SideLie,     // 24����
    ACTION_WagHips,     // 25Ťƨ��
	//4.27
	ACTION_HighFive,    // 26���ƶ���
    ACTION_HugKiss,     // 27����kiss����
    ACTION_Handshake,   // 28���ֶ���
    ACTION_ScratchButt, // 29��������ƨ�ɶ���
    ACTION_Bow,         // 30�Ϲ�����
    ACTION_Cheer,       // 31�ӱۼ��Ͷ���
    ACTION_DrawHeart,   // 32�����Ķ���
    ACTION_StompFoot,   // 33������Ŷ���
    ACTION_Drum,        // 34���Ķ���
    ACTION_RubEyes,     // 35���۾�����
    ACTION_Yawn,        // 36���Ƿ����
    ACTION_SitPatButt,   // 37���ž�\��ƨ�ɶ���
	
	ACTION_Eat,         // 38 �Զ���
    ACTION_Stretch,     // 39 ����������
    ACTION_StretchLying,// 40 ����������
    ACTION_WaveStanding,// 41 վ������
    ACTION_ScratchButtStanding, // 42 վ����ƨ��
    ACTION_SplitStanding, // 43 վ������
    ACTION_PatTummy,    // 44 �Ķ���
	
} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif
