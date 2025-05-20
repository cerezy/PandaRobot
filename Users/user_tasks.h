#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

//�ϵ�֮�� ���Ƿ�����۾���ϴ�����Զ������Ȱ���

typedef enum
{
    ACTION_TEACH = 0, // ʾ��
    // 4.21
    ACTION_WALK,           // 1 ��
    ACTION_WAVE,           // 2 ����
    ACTION_STANDUP,        // 3 վ
    ACTION_SIT,            // 4 ��
    ACTION_SITTOEAT,       // 5 ���ųԶ���
    ACTION_HUG,            // 6 ӵ��
    ACTION_LIEPRONE,       // 7 ��
    ACTION_BIGLIE,         // 8 ������
    ACTION_SIT2PRONE,      // 9 ��->��
    ACTION_PRONETOSIT,     // 10 ��->��
    ACTION_TEST,           // 11
    IDLE,                  // 12 ����
    ACTION_HELLO,          // 13 ���
    ACTION_ScratchHead,    // 14 ��ͷ
    ACTION_Worship,        // 15 ��һ��
    ACTION_ShakeHead,      // 16 ҡͷ
    ACTION_Pouting,        // 17 ��ƨ��
    ACTION_TurnThings,     // 18������
    ACTION_SleepTilt,      // 19��ͷ˯��
    ACTION_WashFace,       // 20ϴ��
    ACTION_SideLieScratch, // 21 ��������
    ACTION_SitLegsOpen,    // 22������
    ACTION_StandToSit,     // 23վ->��
    ACTION_SideLie,        // 24����
    ACTION_WagHips,        // 25Ťƨ��
    // 4.27
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
    ACTION_SitPatButt,  // 37���ž�\��ƨ�ɶ���

    ACTION_Eat,                 // 38 �Զ���
    ACTION_Stretch,             // 39 ����������
    ACTION_StretchLying,        // 40 ����������
    ACTION_WaveStanding,        // 41 վ������
    ACTION_ScratchButtStanding, // 42 վ����ƨ��
    ACTION_SplitStanding,       // 43 վ������
    ACTION_PatTummy,            // 44 �����Ķ���

    // 5.11����Ķ���
    ACTION_M1,  // M1����45
    ACTION_M3,  // M3����46
	ACTION_M4,  // M4����47
    ACTION_M5,  // M5����48
    ACTION_M6,  // M6����49
    ACTION_M7,  // M7����50
    ACTION_M8,  // M8����51
    ACTION_M10, // M10����52
    ACTION_M11, // M11����53
    ACTION_M14, // M14����54
	ACTION_M16, // M16����55
	ACTION_M17, // M17����56
    ACTION_M18, // M18����57
    ACTION_M19, // M19����58
	ACTION_M20, // M20����59
    ACTION_M21, // M21����60
    ACTION_M24, // M24����61.
	ACTION_M26, // M26����62
    ACTION_M28, // M28����63.
    ACTION_M31, // M31����64
    ACTION_M32, // M32����65
    ACTION_M33, // M33����66
    ACTION_M34, // M34����67
    ACTION_M37, // M37����68
    ACTION_M38, // M38����69.
    ACTION_M46, // M46����70
    ACTION_M49, // M49����71
	
	//5.14
	ACTION_M61,  // M61����72
	ACTION_M64,  // M64����73
	ACTION_M65,  // M65����74,
	ACTION_M67,  // M67����75	
	ACTION_M70,  // M70����76
	ACTION_M71,  // M71����77
	ACTION_M73,  // M73����78
	ACTION_M75,  // M75����79.
	ACTION_M76,  // M76����80.
	ACTION_M78,  // M78����81
	ACTION_M80,  // M80����82��ֱ��һ�����սǶ�
	ACTION_M83,  // M83����83��
	ACTION_M87,  // M87����84.ֱ��һ�����սǶ�
	ACTION_M89,  // M89����85��ֱ�����սǶ�
	ACTION_M90,  // M90����86
	ACTION_M91,  // M91����87��
	ACTION_M92,  // M92����88
	ACTION_M94,  // M94����89
	ACTION_M121, // M121����90	
	ACTION_M123, // M123����91
	ACTION_M125, // M125����92
	ACTION_M126, // M126����93
	ACTION_M136, // M136����94��
	ACTION_M138, // M138����95��
	ACTION_M151, // M151����96
	ACTION_M152, // M152����97
	ACTION_M155, // M155����98
	ACTION_M156, // M156����99
	ACTION_M157, // M157����100
	ACTION_M162, // M162����101
	ACTION_M163, // M163����102
	ACTION_M164, // M164����103
	ACTION_M166, // M166����104
	ACTION_M167, // M167����105
	ACTION_M168, // M168����106
	ACTION_M169, // M169����107
	ACTION_M170, // M170����108
	ACTION_M171, // M171����109
	ACTION_M172, // M172����110
	ACTION_M173, // M173����111
	ACTION_M176, // M176����112
	ACTION_M178, // M178����113
	ACTION_M182, // M182����114
	ACTION_M184, // M184����115
	ACTION_M191, // M191����116
	ACTION_M195, // M195����117
	ACTION_M196, // M196����118
	ACTION_M202, // M202����119
	ACTION_M204, // M204����120
	ACTION_M205, // M205����121

} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif
