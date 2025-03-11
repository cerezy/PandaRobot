#include "user_gait.h"

int16_t SERVO_STAND_ANG[12] = {-40,-100,300,40,100,-300,40,100,-300,-40,-100,300};


void User_GaitInit(void)
{

}

void User_GaitZeroPoint(void)
{
	SERVO[1].ang_set = 0;//���������п���ʱ���������ang_set������ֵ������Ŀ��Ʒ��ڶ�ʱ���ж����Զ���ɡ�
	SERVO[2].ang_set = 0;
	SERVO[3].ang_set = 0;
	SERVO[4].ang_set = 0;
	SERVO[5].ang_set = 0;
	SERVO[6].ang_set = 0;
	SERVO[7].ang_set = 0;
	SERVO[8].ang_set = 0;
	SERVO[9].ang_set = 0;
	SERVO[10].ang_set = 0;
	SERVO[11].ang_set = 0;
	SERVO[12].ang_set = 0;

	User_SeovoActTimeCalcu();//���ϽǶȲ������¸�ֵ����øú����Զ�������ת��ʱ��������Ƕȸñ���Խ����Ҫ��ʱ��Խ����
}

void User_GaitStand(void)
{
	SERVO[1].ang_set = SERVO_STAND_ANG[0];
	SERVO[2].ang_set = SERVO_STAND_ANG[1];
	SERVO[3].ang_set = SERVO_STAND_ANG[2];
	SERVO[4].ang_set = SERVO_STAND_ANG[3];
	SERVO[5].ang_set = SERVO_STAND_ANG[4];
	SERVO[6].ang_set = SERVO_STAND_ANG[5];
	SERVO[7].ang_set = SERVO_STAND_ANG[6];
	SERVO[8].ang_set = SERVO_STAND_ANG[7];
	SERVO[9].ang_set = SERVO_STAND_ANG[8];
	SERVO[10].ang_set = SERVO_STAND_ANG[9];
	SERVO[11].ang_set = SERVO_STAND_ANG[10];
	SERVO[12].ang_set = SERVO_STAND_ANG[11];

	User_SeovoActTimeCalcu();
}
int16_t ANG_SET = 200;
void User_GaitRaiseLeft(void)
{
	SERVO[1].ang_set = SERVO_STAND_ANG[0]+0;
	SERVO[2].ang_set = SERVO_STAND_ANG[1]-ANG_SET;
	SERVO[3].ang_set = SERVO_STAND_ANG[2]+3*ANG_SET;
	SERVO[4].ang_set = SERVO_STAND_ANG[3]+0;
	SERVO[5].ang_set = SERVO_STAND_ANG[4]+0;
	SERVO[6].ang_set = SERVO_STAND_ANG[5]+0;
	SERVO[7].ang_set = SERVO_STAND_ANG[6]+0;
	SERVO[8].ang_set = SERVO_STAND_ANG[7]+0;
	SERVO[9].ang_set = SERVO_STAND_ANG[8]+0;
	SERVO[10].ang_set = SERVO_STAND_ANG[9]+0;
	SERVO[11].ang_set = SERVO_STAND_ANG[10]-ANG_SET;
	SERVO[12].ang_set = SERVO_STAND_ANG[11]+3*ANG_SET;
	
	User_SeovoActTimeCalcu();
}

void User_GaitRaiseRight(void)
{
	SERVO[1].ang_set = SERVO_STAND_ANG[0]+0;
	SERVO[2].ang_set = SERVO_STAND_ANG[1];
	SERVO[3].ang_set = SERVO_STAND_ANG[2];
	SERVO[4].ang_set = SERVO_STAND_ANG[3]+0;
	SERVO[5].ang_set = SERVO_STAND_ANG[4]+ANG_SET;
	SERVO[6].ang_set = SERVO_STAND_ANG[5]-3*ANG_SET;
	SERVO[7].ang_set = SERVO_STAND_ANG[6]+0;
	SERVO[8].ang_set = SERVO_STAND_ANG[7]+ANG_SET;
	SERVO[9].ang_set = SERVO_STAND_ANG[8]-3*ANG_SET;
	SERVO[10].ang_set = SERVO_STAND_ANG[9]+0;
	SERVO[11].ang_set = SERVO_STAND_ANG[10];
	SERVO[12].ang_set = SERVO_STAND_ANG[11];
	
	User_SeovoActTimeCalcu();
}

