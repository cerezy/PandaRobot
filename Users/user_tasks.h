#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

// �ϵ�֮�� ���Ƿ�����۾���ϴ�����Զ������Ȱ���

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
    IDLE,                  // 10 ����
    ACTION_HELLO,          // 11 ���
    ACTION_ScratchHead,    // 12 ��ͷ
    ACTION_Worship,        // 13 ��һ��
    ACTION_ShakeHead,      // 14 ҡͷ
    ACTION_Pouting,        // 15 ��ƨ��
    ACTION_TurnThings,     // 16 ������
    ACTION_SleepTilt,      // 17 ��ͷ˯��
    ACTION_WashFace,       // 18 ϴ��
    ACTION_SideLieScratch, // 19 ��������
    ACTION_SitLegsOpen,    // 20 ������
    ACTION_StandToSit,     // 21 վ->��
    ACTION_SideLie,        // 22 ����
    ACTION_WagHips,        // 23 Ťƨ��
    // 4.27
    ACTION_HighFive,    // 24 ���ƶ���
    ACTION_HugKiss,     // 25 ����kiss����
    ACTION_Handshake,   // 26 ���ֶ���
    ACTION_ScratchButt, // 27 ��������ƨ�ɶ���
    ACTION_Bow,         // 28 �Ϲ�����
    ACTION_Cheer,       // 29 �ӱۼ��Ͷ���
    ACTION_DrawHeart,   // 30 �����Ķ���
    ACTION_StompFoot,   // 31 ������Ŷ���
    ACTION_Drum,        // 32 ���Ķ���
    ACTION_RubEyes,     // 33 ���۾�����
    ACTION_Yawn,        // 34 ���Ƿ����
    ACTION_SitPatButt,  // 35 ���ž�\��ƨ�ɶ���

    ACTION_Eat,                 // 36 �Զ���
    ACTION_Stretch,             // 37 ����������
    ACTION_StretchLying,        // 38 ����������
    ACTION_WaveStanding,        // 39 վ������
    ACTION_ScratchButtStanding, // 40 վ����ƨ��
    ACTION_SplitStanding,       // 41 վ������
    ACTION_PatTummy,            // 42 �����Ķ���

    // 5.11����Ķ���
    ACTION_M1,  // M1����43
    ACTION_M3,  // M3����44
    ACTION_M4,  // M4����45
    ACTION_M5,  // M5����46
    ACTION_M6,  // M6����47
    ACTION_M7,  // M7����48
    ACTION_M8,  // M8����49
    ACTION_M10, // M10����50
    ACTION_M11, // M11����51
    ACTION_M14, // M14����52
    ACTION_M16, // M16����53
    ACTION_M17, // M17����54
    ACTION_M18, // M18����55
    ACTION_M19, // M19����56
    ACTION_M20, // M20����57
    ACTION_M21, // M21����58
    ACTION_M24, // M24����59.
    ACTION_M26, // M26����60
    ACTION_M28, // M28����61.
    ACTION_M31, // M31����62
    ACTION_M32, // M32����63
    ACTION_M33, // M33����64
    ACTION_M34, // M34����65
    ACTION_M37, // M37����66
    ACTION_M38, // M38����67.
    ACTION_M46, // M46����68
    ACTION_M49, // M49����69

    // 5.14
    ACTION_M61,  // M61����70
    ACTION_M63,  // M63����71 �� ����
    ACTION_M64,  // M64����72
    ACTION_M65,  // M65����73
    ACTION_M67,  // M67����74
    ACTION_M70,  // M70����75
    ACTION_M71,  // M71����76
    ACTION_M73,  // M73����77
    ACTION_M75,  // M75����78
    ACTION_M76,  // M76����79
    ACTION_M77,  // M77����80 �� ����
    ACTION_M78,  // M78����81
    ACTION_M80,  // M80����82
    ACTION_M83,  // M83����83
    ACTION_M87,  // M87����84
    ACTION_M89,  // M89����85
    ACTION_M90,  // M90����86
    ACTION_M91,  // M91����87
    ACTION_M92,  // M92����88
    ACTION_M94,  // M94����89
    ACTION_M98,  // M98����90 �� ����
    ACTION_M99,  // M99����91 �� ����
    ACTION_M102, // M102����92 �� ����
    ACTION_M121, // M121����93
    ACTION_M123, // M123����94
    ACTION_M125, // M125����95
    ACTION_M126, // M126����96
    ACTION_M130, // M130����97 �� ����
    ACTION_M136, // M136����98
    ACTION_M137, // M137����99 �� ����
    ACTION_M138, // M138����100
    ACTION_M148, // M148����101 �� ����
    ACTION_M151, // M151����102
    ACTION_M152, // M152����103
    ACTION_M155, // M155����104
    ACTION_M156, // M156����105
    ACTION_M157, // M157����106
    ACTION_M160, // M160����107 �� ����
    ACTION_M162, // M162����108
    ACTION_M163, // M163����109
    ACTION_M164, // M164����110
    ACTION_M166, // M166����111
    ACTION_M167, // M167����112
    ACTION_M168, // M168����113
    ACTION_M169, // M169����114
    ACTION_M170, // M170����115
    ACTION_M171, // M171����116
    ACTION_M172, // M172����117
    ACTION_M173, // M173����118
    ACTION_M176, // M176����119
    ACTION_M177, // M177����120 �� ����
    ACTION_M178, // M178����121
    ACTION_M182, // M182����122
    ACTION_M184, // M184����123
    ACTION_M191, // M191����124
    ACTION_M195, // M195����125
    ACTION_M196, // M196����126
    ACTION_M202, // M202����127
    ACTION_M204, // M204����128
    ACTION_M205, // M205����129
	
    ACTION_M179, // M179����130
    ACTION_M181, // M181����131
    ACTION_M189, // M189����132
    ACTION_M193, // M193����133
    ACTION_M197, // M197����134
    ACTION_M199, // M199����135
    ACTION_M207, // M207����136
    ACTION_M210, // M210����137
    ACTION_MCRAWL_TO_SIT,    // ſ->�� ����138
    ACTION_MSIT_TO_CRAWL,    // ��->ſ ����139
    ACTION_MSIT_TO_STAND,    // ��->վ ����140
    ACTION_MSTAND_TO_SIT,    // վ->�� ����141
    ACTION_MSTAND_TO_CRAWL,  // վ->ſ ����142
    ACTION_MCRAWL_TO_STAND,  // ſ->վ ����143
} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif
