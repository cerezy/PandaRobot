#include "user_tasks.h"


void User_Init(void)
{	
	User_AdcInit();
	User_LedInit();
	User_ImuInit();
	User_CommInit();
	User_ServoInit();
	osDelay(500);
	User_TimerInit();//����ϵ����Ҫ��ʱһ��ʱ����ܿ��ƣ��������쳣
}

////////////////////////////////////////////////////////
void StartTaskHigh(void *argument)//���ڸ��߳�����Ӳ�̬���룬����StartTaskMid���޸�Ҳ��
{
	User_Init();
	for(;;)
  {				
		osDelay(2);			
  } 
}

uint16_t ACTION_DELAY = 200;//�����ö������ν���ʱ
void StartTaskMid(void *argument)//���Զ�����
{
	User_GaitZeroPoint();//�ؽ�0�㶯��
	osDelay(5000);
	User_GaitStand();//�Ȳ�����վ��
	osDelay(3000);
	for(;;)
  {												
		while(SERVO_COMM_BUSY == 1)//����ûɶ���ˣ�����ɾ��
		{			

		}
		User_GaitStand();//�Ȳ�����վ��
		osDelay(ACTION_DELAY);
		
		User_GaitRaiseLeft();//̧��ǰ+�Һ���
		while((User_IsLegAngArrive(1)==0)||(User_IsLegAngArrive(4)==0))//�ȴ���1���Ⱥ͵�4���ȶ��ִ�нǶȵ�λ
		{
			osDelay(1);	
		}
		
		User_GaitStand();//�Ȳ�����վ��
		osDelay(ACTION_DELAY);
		
		User_GaitRaiseRight();//̧��ǰ+�����
		while((User_IsLegAngArrive(2)==0)||(User_IsLegAngArrive(3)==0))//�ȴ���2���Ⱥ͵�3���ȶ��ִ�нǶȵ�λ
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



