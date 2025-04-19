#ifndef _user_tasks_h
#define _user_tasks_h

#include "main.h"

typedef enum
{
	ACTION_TEACH = 0,
	ACTION_WALK,		// 1
	ACTION_WAVE,		// 2
	ACTION_STANDUP,		// 3
	ACTION_SIT,			// 4
	ACTION_SITTOEAT,	// 5
	ACTION_HUG,			// 6
	ACTION_LIEPRONE,	// 7
	ACTION_BIGLIE,		// 8
	ACTION_SIT2PRONE,	// 9
	ACTION_PRONETOSIT,	// 10
	ACTION_LIE2STANDUP, // 11
	ACTION_CRAWL,		// 12
	ACTION_CRAWLREPEAT, // 13
	ACTION_TEST,		// 14
	IDLE,				// 15
	ACTION_HELLO		// 16
} ACTION_STATE;

void ActionRUN(void);
void TeachmodeRUN(void);

#endif
