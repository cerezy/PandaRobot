#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

typedef enum
{
	ACTION_TEACH = 0,
	ACTION_WALK,//1
	ACTION_HUG,//2
	ACTION_STANDUP,	//3
	ACTION_SIT2PRONE,	//4
	ACTION_PRONETOSIT,	//5
	ACTION_LIE2STANDUP,	//6
	ACTION_CRAWL,	//7
	ACTION_CRAWLREPEAT,	//8
	ACTION_TEST,	//9
	ACTION_UNIT1,	//10
    ACTION_UNITTEST,//11
	ACTION_LIEDOWN //12
} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif



