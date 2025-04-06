#ifndef _user_states_h
#define _user_states_h


#include "main.h"


#define _STATE_ZhanLi_SiZu 			1
#define _STATE_ZhanLi_ZuoQian 	2
#define _STATE_ZhanLi_YouQian 	3
#define _STATE_ZhanLi_ZuoHou 		4
#define _STATE_ZhanLi_YouHou 		5

#define _STATE_PaXing_XiangQian 6
#define _STATE_PaXing_XiangHou 	7
#define _STATE_PaXing_XiangZuo 	8
#define _STATE_PaXing_XiangYou 	9
#define _STATE_PaXing_ZuoZhuan 	10
#define _STATE_PaXing_YouZhuan 	11

#define _STATE_ZuoLi 						12
#define _STATE_FuPa 						13
#define _STATE_YangTang 				14



typedef struct
{
	uint8_t state_now;
	uint8_t state_last;
	uint8_t state_next;
				
}STATE_TYPEDEF;



void User_StatesInit(void);
void User_ActionStateRun(void);

#endif




