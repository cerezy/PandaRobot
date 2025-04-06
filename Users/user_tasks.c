#include "user_tasks.h"
#include "cmsis_os.h" 
#include "user_servo.h"
#include "user_timer.h"
#include "user_states.h"

int16_t ang_goal[15] = {0}; 
uint16_t ms_goal[15] = {0}; 
uint8_t Action_done[50] = {0};
uint8_t ActionNowFlag = 14;
uint8_t speed = 2;//�����Ĳ����ٶȣ���λ0.1�㣩
int32_t step_counter = 0;//��ǰ�������ö������еĵڼ���
uint8_t vis[15] = {0};//�ж϶���Ƿ�ִ�λ��
uint8_t Servo_Reset_Flag = 0;
uint8_t Init_OK = 0;

void User_Init(void)
{	
	Action_init();//�������ʼ����������Ϣ��
	User_ServoInit();//�����Ϣ��ʼ������Ӧ���ڵȣ�
	User_TimerInit();//ͳ��ȫ�ֵĶ�ʱ��
	User_TeachTimerInit();//ʾ��ģʽר�ö�ʱ��
	Init_OK = 1;
}

void ActionRUN(void)
{
	//�жϵ�ǰ�ڽ��еĶ���ActionNowFlag��û�н����Ĳ�����֤ID����ȷ��
	if(Action_done[ActionNowFlag] == 0 && Action_index[ActionNowFlag]->actionId == ActionNowFlag)
	{
		//���������������һʱ�̵�Ŀ��ֵ
		for(int i = 1;i <= 14;i++)
		{
		    if(ang_goal[i] <= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i-1] - speed)
				ang_goal[i] += speed;
			else if(ang_goal[i] >= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i-1] + speed)
				ang_goal[i] -= speed;
			//�ж��Ƿ�ִ�Ŀ��λ��
			if(ang_goal[i] > Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i-1] - speed && ang_goal[i] < Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i-1] + speed)
				vis[i] = 1;
		}
		for(int i = 1;i <= 14;i++)
		{
			if(vis[i] == 0)
				break;
			if(i == 14)
			{
				//δִ�������в���
				if(step_counter <  Action_index[ActionNowFlag]->total_step -1)
				{
					step_counter ++;
					for(int i = 1;i <= 14;i++)
						vis[i] = 0;
				}
				//ִ�����
				else if(step_counter == Action_index[ActionNowFlag]->total_step - 1)
				{
					//��ǰ����Done
					Action_done[Action_index[ActionNowFlag]->actionId] = 1;
					//��ز�����λ
					step_counter = 0;
					for(int i = 1;i <= 14;i++)
						vis[i] = 0;
				}
			}	
		 }
	 }
}

void TeachmodeRUN(void)
{
	//�������������ϵĶ����������ģʽ��һ����ʾ��ʱʹ��
	if(Servo_Reset_Flag == 1)
	{
		User_SetDamping(1,1000);
		User_SetDamping(2,1000);
		User_SetDamping(3,1000);
		Servo_Reset_Flag = 0;
	}
	if(TEACHMODE == 1)
	{
		//ʾ��ģʽ����������������λ
		if(TEACH_FINISH == 1)
		{
			/*ʾ�̽���*/
			TEACHMODE = 0;
			TEACH_OK = 0;
			TEACH_FINISH = 0;
			
			/*�������̸��ָոյĶ���*/
			ActionNowFlag = 0;
			Action_done[Action_index[ActionNowFlag]->actionId] = 0;
			step_counter = 0;
			
			/*��������λ����������*/
			Action_Teachmode();
		 }
	}
}

void StartTaskHigh(void const * argument)
{
	User_Init();
	for(;;)
	{
		osDelay(1);
	}
}

void StartTaskMid(void const * argument)
{
	for(;;)
	{	
		if(TEACHMODE == 1)
			TeachmodeRUN();
		else
			if(Init_OK == 1)
				ActionRUN();
		osDelay(10);	
	}
}
void StartTaskLow(void const * argument)
{
	
	for(;;)
	{			
		osDelay(20); 
	}
}



