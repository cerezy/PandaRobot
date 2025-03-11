#include "user_tasks.h"


void User_Init(void)
{	
	User_AdcInit();
	User_LedInit();
	User_ImuInit();
	User_CommInit();
	User_ServoInit();
	osDelay(500);
	User_TimerInit();//舵机上电后需要延时一段时间才能控制，否则工作异常
}

////////////////////////////////////////////////////////
void StartTaskHigh(void *argument)//可在该线程中添加步态代码，或在StartTaskMid中修改也行
{
	User_Init();
	for(;;)
  {				
		osDelay(2);			
  } 
}

uint16_t ACTION_DELAY = 200;//测试用动作间衔接延时
void StartTaskMid(void *argument)//测试动作组
{
	User_GaitZeroPoint();//关节0点动作
	osDelay(5000);
	User_GaitStand();//腿部外括站立
	osDelay(3000);
	for(;;)
  {												
		while(SERVO_COMM_BUSY == 1)//好像没啥用了，可以删掉
		{			

		}
		User_GaitStand();//腿部外括站立
		osDelay(ACTION_DELAY);
		
		User_GaitRaiseLeft();//抬左前+右后腿
		while((User_IsLegAngArrive(1)==0)||(User_IsLegAngArrive(4)==0))//等待第1条腿和第4条腿舵机执行角度到位
		{
			osDelay(1);	
		}
		
		User_GaitStand();//腿部外括站立
		osDelay(ACTION_DELAY);
		
		User_GaitRaiseRight();//抬右前+左后腿
		while((User_IsLegAngArrive(2)==0)||(User_IsLegAngArrive(3)==0))//等待第2条腿和第3条腿舵机执行角度到位
		{
			osDelay(1);	
		}		
  }
}
void StartTaskLow(void *argument)
{
	User_LedShowSet(0,5,2,1);
	
	for(;;)
  {			
		User_AdcBatVoltGet();
		User_LedTaskShow();  
		osDelay(_LED_TASK_TIME); 
  }
}



