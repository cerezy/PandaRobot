#include "user_states.h"
#include "user_servo.h"


//STATE_TYPEDEF ACTION_STATE = {0};

void User_StatesInit(void)
{
	
}
 
/*
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
*/
/*void User_ActionStateRun(void)
{
	
	switch(ACTION_STATE.state_now)
	{
		case 0:
		{
			if(ACTION_STATE.state_last != 0)
			{
				if(User_ZuoLi())
					ACTION_STATE.state_last = 0;
			}			
		}break;
		///////////////////////////////////////////////
		case _STATE_ZhanLi_SiZu:
		{
			if(ACTION_STATE.state_last != _STATE_ZhanLi_SiZu)
			{
				if(User_ZhanLi())
					ACTION_STATE.state_last = _STATE_ZhanLi_SiZu;
			}			
			else if(ACTION_STATE.state_next == _STATE_ZuoLi)
			{
				if(User_ZhanLi_ZuoLi())
				{
					ACTION_STATE.state_now = ACTION_STATE.state_next;
					ACTION_STATE.state_next = 0;
				}
				else ;
			}
		}break;	
		case _STATE_ZhanLi_ZuoQian:
		{
			
		}break;
		case _STATE_ZhanLi_YouQian:
		{
			
		}break;
		case _STATE_ZhanLi_ZuoHou:
		{
			
		}break;
		case _STATE_ZhanLi_YouHou:
		{
			
		}break;
		///////////////////////////////////////////////
		case _STATE_PaXing_XiangQian:
		{
			User_PaXing_XiangQian();
		}break;
		case _STATE_PaXing_XiangHou:
		{
			
		}break;
		case _STATE_PaXing_XiangZuo:
		{
			
		}break;
		case _STATE_PaXing_XiangYou:
		{
			
		}break;
		case _STATE_PaXing_ZuoZhuan:
		{
			
		}break;
		case _STATE_PaXing_YouZhuan:
		{
			
		}break;
		///////////////////////////////////////////////	
		case _STATE_ZuoLi:
		{
			if(ACTION_STATE.state_last != _STATE_ZuoLi)
			{
				User_ZuoLi();	
				ACTION_STATE.state_last = _STATE_ZuoLi;				
			}
			else if(ACTION_STATE.state_next == _STATE_ZhanLi_SiZu)
			{
				if(User_ZuoLi_ZhanLi())
				{
					ACTION_STATE.state_now = ACTION_STATE.state_next;
					ACTION_STATE.state_next = 0;
				}
				else ;
			}	
		}break;
		///////////////////////////////////////////////	
		case _STATE_FuPa:
		{
			
		}break;
		///////////////////////////////////////////////	
		case _STATE_YangTang:
		{
			
		}break;
		///////////////////////////////////////////////	
		
		default:ACTION_STATE.state_next = 0;break;
	}
}*/


