#include "user_tasks.h"
#include "cmsis_os.h" 
#include "user_servo.h"
#include "user_timer.h"
#include "user_states.h"

int16_t ang_goal[15] = {0}; 
uint16_t ms_goal[15] = {0}; 
uint8_t Action_done[50] = {0};
uint8_t ActionNowFlag = 1;
uint8_t speed = 5;//�����Ĳ����ٶȣ���λ0.1�㣩
int32_t step_counter = 0;//��ǰ�������ö������еĵڼ���
uint8_t vis[15] = {0};//�ж϶���Ƿ�ִ�λ��
uint8_t Servo_Reset_Flag = 0;
uint8_t Init_OK = 0;
int16_t LookPos[14] = {0};

void User_Init(void)
{	
	Action_init();//�������ʼ����������Ϣ��
	User_ServoInit();//�����Ϣ��ʼ������Ӧ���ڵȣ�
	User_TimerInit();//ͳ��ȫ�ֵĶ�ʱ��
	User_TeachTimerInit();//ʾ��ģʽר�ö�ʱ��
	TEACHMODE = 1;
	Init_OK = 1;
}

void ActionRUN(void)
{
	/*for(int i = 1;i <= 12;i++)
		goal_pos[i] = 0;*/
	/*if(step_counter<14)
		step_counter++;
	if(step_counter==14)
		step_counter=0;*/
	
	//�жϵ�ǰ�ڽ��еĶ���ActionNowFlag��û�н����Ĳ�����֤ID����ȷ��
	/*if(Action_done[ActionNowFlag] == 0 && Action_index[ActionNowFlag]->actionId == ActionNowFlag)
	{
		//���������������һʱ�̵�Ŀ��ֵ
		for(int i = 1;i <= 12;i++)
		{
		    if(goal_pos[i] <= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] - speed)
				goal_pos[i] += speed;
			else if(goal_pos[i] >= Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] + speed)
				goal_pos[i] -= speed;
			//�ж��Ƿ�ִ�Ŀ��λ��
			if(goal_pos[i] > Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] - speed && goal_pos[i] < Action_index[ActionNowFlag]->actions[step_counter].servoAngles[i] + speed)
				vis[i] = 1;
		}
		for(int i = 1;i <= 12;i++)
		{
			if(vis[i] == 0)
				break;
			if(i == 12)
			{
				//δִ�������в���
				if(step_counter <  Action_index[ActionNowFlag]->total_step -1)
				{
					step_counter ++;
					for(int i = 1;i <= 12;i++)
						vis[i] = 0;
				}
				//ִ�����
				else if(step_counter == Action_index[ActionNowFlag]->total_step - 1)
				{
					//��ǰ����Done
					Action_done[Action_index[ActionNowFlag]->actionId] = 1;
					//��ز�����λ
					step_counter = 0;
					for(int i = 1;i <= 12;i++)
						vis[i] = 0;
					if(ActionNowFlag == 1)
					{
						Action_done[Action_index[ActionNowFlag]->actionId] = 0;
						step_counter = 1;
					}
				}
			}	
		 }
	 }*/
}

void TeachmodeRUN(void)
{
	//�������������ϵĶ����������ģʽ��һ����ʾ��ʱʹ��
	if(Servo_Reset_Flag == 1)
	{
		//User_SetDamping(1,1000);
		//User_SetDamping(2,1000);
		//User_SetDamping(3,1000);
		Servo_Reset_Flag = 0;
	}
	if(TEACHMODE == 1)
	{
		//ʾ��ģʽ����������������λ
		if(TEACH_FINISH == 1)
		{
			/*ʾ�̽���*/
			for(int i = 1;i <= 12;i++)
			{
				goal_pos[i] = Action_index[0]->actions[1].servoAngles[i];
			}
			TEACHMODE = 0;
			TEACH_OK = 0;
			TEACH_FINISH = 0;
			
			/*�������̸��ָոյĶ���*/
			ActionNowFlag = 0;
			Action_done[Action_index[ActionNowFlag]->actionId] = 0;
			step_counter = 1;
			
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
uint8_t OPEN = 1;
void StartTaskMid(void const * argument)
{
	for(;;)
	{	
		if(TEACHMODE == 1)
			TeachmodeRUN();
		else
			if(Init_OK == 1)
				ActionRUN();
		for(int i=1;i<=12;i++)
			LookPos[i] = SERVO[i].ang_read;
		if(OPEN == 1)
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port,Servo_Power_Pin,GPIO_PIN_SET);//�������
		else 
			HAL_GPIO_WritePin(Servo_Power_GPIO_Port,Servo_Power_Pin,GPIO_PIN_RESET);//�������
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



