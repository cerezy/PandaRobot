#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

typedef enum
{
	ACTION_TEACH = 0,
	ACTION_WALK,//1
	ACTION_WAVE,//2
	ACTION_STANDUP,//3
	ACTION_SIT,//4
	ACTION_SITTOEAT,//5
	ACTION_HUG,//6
	ACTION_LIEDOWN, //7
	ACTION_SIT2PRONE,	//8
	ACTION_PRONETOSIT,	//9
	ACTION_LIE2STANDUP,	//10
	ACTION_CRAWL,	//11
	ACTION_CRAWLREPEAT,	//12
	ACTION_TEST,	//13
	IDLE//14
} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif



