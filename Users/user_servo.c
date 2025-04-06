#include "user_servo.h"
#include "stdio.h"
#include "string.h"

//���ư���������ȡ��������෴��
USART_SERVO_TYPEDEF USART_LEG1 = {0};//usart1
USART_SERVO_TYPEDEF USART_LEG2 = {0};//usart1
USART_SERVO_TYPEDEF USART_LEG3 = {0};//usart2
USART_SERVO_TYPEDEF USART_LEG4 = {0};//usart2
USART_SERVO_TYPEDEF USART_RIGHT_LEG = {0};//�ұ��� ID:1 - 6 usart1
USART_SERVO_TYPEDEF USART_LEFT_LEG = {0};//����� ID: 7 - 12 usart2
USART_SERVO_TYPEDEF USART_HEAD = {0};//����� ID: 13 - 14 usart3


SERVO_INFO_TYPEDEF SERVO[17] = {0};

uint8_t SERVO_COMM_BUSY = 0;
extern int16_t ang_goal[15]; 
//�ŷ������ʼ��
void User_ServoInit(void)
{               
	//��������Ӧ����
	USART_LEG1.p_usart_n = &huart1;
	USART_LEG1.p_hdma_usart_n_rx = &hdma_usart1_rx;
	USART_LEG2.p_usart_n = &huart1;
	USART_LEG2.p_hdma_usart_n_rx = &hdma_usart1_rx;
	USART_LEG3.p_usart_n = &huart2;
	USART_LEG3.p_hdma_usart_n_rx = &hdma_usart2_rx;
	USART_LEG4.p_usart_n = &huart2;
	USART_LEG4.p_hdma_usart_n_rx = &hdma_usart2_rx;
	USART_RIGHT_LEG.p_usart_n = &huart1;
	USART_RIGHT_LEG.p_hdma_usart_n_rx = &hdma_usart1_rx;
	USART_LEFT_LEG.p_usart_n = &huart2;
	USART_LEFT_LEG.p_hdma_usart_n_rx = &hdma_usart2_rx;
	USART_HEAD.p_usart_n = &huart3;
	USART_HEAD.p_hdma_usart_n_rx = &hdma_usart3_rx;
	
	//�����ŷ�����ĳ�ʼ�Ƕȣ�*10��
	ang_goal[1] = 950;
	ang_goal[2] = 0;
	ang_goal[3] = 0;
	ang_goal[4] = 0;
	ang_goal[5] = 0;
	ang_goal[6] = 0;
	ang_goal[7] = 900;
	ang_goal[8] = 0;
	ang_goal[9] = 0;
	ang_goal[10] = -900;
	ang_goal[11] = 0;
	ang_goal[12] = 0;
	ang_goal[13] = 0;
	ang_goal[14] = 0;
	for(int i=0;i<=14;i++)
		SERVO[i].zero_ang = 0;
	
	
	//���������жϽ���
	__HAL_UART_ENABLE_IT(USART_RIGHT_LEG.p_usart_n, UART_IT_IDLE);         
	HAL_UART_Receive_DMA(USART_RIGHT_LEG.p_usart_n, (uint8_t*)USART_RIGHT_LEG.usart_rx_buf, USART_SERVO_RX_SIZE);
	
	__HAL_UART_ENABLE_IT(USART_LEFT_LEG.p_usart_n, UART_IT_IDLE);         
	HAL_UART_Receive_DMA(USART_LEFT_LEG.p_usart_n, (uint8_t*)USART_LEFT_LEG.usart_rx_buf, USART_SERVO_RX_SIZE);
  
    __HAL_UART_ENABLE_IT(USART_HEAD.p_usart_n, UART_IT_IDLE);         
	HAL_UART_Receive_DMA(USART_HEAD.p_usart_n, (uint8_t*)USART_HEAD.usart_rx_buf, USART_SERVO_RX_SIZE);
}
//���ڿ����жϽ���
void User_ServoLegRIGHT_IRQHandler(void)
{
	if(RESET != __HAL_UART_GET_FLAG(USART_RIGHT_LEG.p_usart_n, UART_FLAG_IDLE))  //���UART�Ŀ����жϱ�־λ�Ƿ���λ
	{
		__HAL_UART_CLEAR_IDLEFLAG(USART_RIGHT_LEG.p_usart_n);   				    //����жϱ�־λ����ֹ�ظ������ж�		
		HAL_UART_DMAStop(USART_RIGHT_LEG.p_usart_n);             				//��ֹ��ǰDMA���䣬ȷ��������������������ݳ��ȣ���׼ȷ��                                         
		USART_RIGHT_LEG.rx_data_len  = USART_SERVO_RX_SIZE - __HAL_DMA_GET_COUNTER(USART_RIGHT_LEG.p_hdma_usart_n_rx);  //����ʵ�ʽ��ճ���
		User_UsartServoDataParas(&USART_RIGHT_LEG);								//��������
		HAL_UART_Receive_DMA(USART_RIGHT_LEG.p_usart_n, (uint8_t*)USART_RIGHT_LEG.usart_rx_buf, USART_SERVO_RX_SIZE);//����DMA����
	}
}
void User_ServoLegLEFT_IRQHandler(void)
{
	if(RESET != __HAL_UART_GET_FLAG(USART_LEFT_LEG.p_usart_n, UART_FLAG_IDLE))  //���UART�Ŀ����жϱ�־λ�Ƿ���λ
	{
		__HAL_UART_CLEAR_IDLEFLAG(USART_LEFT_LEG.p_usart_n);   				    //����жϱ�־λ����ֹ�ظ������ж�		
		HAL_UART_DMAStop(USART_LEFT_LEG.p_usart_n);             				//��ֹ��ǰDMA���䣬ȷ��������������������ݳ��ȣ���׼ȷ��                                         
		USART_LEFT_LEG.rx_data_len  = USART_SERVO_RX_SIZE - __HAL_DMA_GET_COUNTER(USART_LEFT_LEG.p_hdma_usart_n_rx);  //����ʵ�ʽ��ճ���
		User_UsartServoDataParas(&USART_LEFT_LEG);								//��������
		HAL_UART_Receive_DMA(USART_LEFT_LEG.p_usart_n, (uint8_t*)USART_LEFT_LEG.usart_rx_buf, USART_SERVO_RX_SIZE);//����DMA����
	}
}
void User_ServoHead_IRQHandler(void)
{
	if(RESET != __HAL_UART_GET_FLAG(USART_HEAD.p_usart_n, UART_FLAG_IDLE))  //���UART�Ŀ����жϱ�־λ�Ƿ���λ
	{
		__HAL_UART_CLEAR_IDLEFLAG(USART_HEAD.p_usart_n);   				    //����жϱ�־λ����ֹ�ظ������ж�		
		HAL_UART_DMAStop(USART_HEAD.p_usart_n);             				//��ֹ��ǰDMA���䣬ȷ��������������������ݳ��ȣ���׼ȷ��                                         
		USART_HEAD.rx_data_len  = USART_SERVO_RX_SIZE - __HAL_DMA_GET_COUNTER(USART_HEAD.p_hdma_usart_n_rx);  //����ʵ�ʽ��ճ���
		User_UsartServoDataParas(&USART_HEAD);								//��������
		HAL_UART_Receive_DMA(USART_HEAD.p_usart_n, (uint8_t*)USART_HEAD.usart_rx_buf, USART_SERVO_RX_SIZE);//����DMA����
	}
}
//����������ݽ���
void User_UsartServoDataParas(USART_SERVO_TYPEDEF* p_usart_servo_x)
{		
	uint8_t i = 0;
	uint8_t sum = 0;
	
	for(i=0;i<p_usart_servo_x->rx_data_len-1;i++)
	{
		sum += p_usart_servo_x->usart_rx_buf[i];
	}
	if((p_usart_servo_x->usart_rx_buf[0] == 0x05) && (p_usart_servo_x->usart_rx_buf[1] == 0x1c) && (p_usart_servo_x->usart_rx_buf[i] == sum))
	{
		switch(p_usart_servo_x->usart_rx_buf[2])
		{
			case 0x0a://cmd_id:read_angle
			{
				SERVO[p_usart_servo_x->usart_rx_buf[4]].ang_read = (p_usart_servo_x->usart_rx_buf[5]+(((int16_t)p_usart_servo_x->usart_rx_buf[6])<<8)) - SERVO[p_usart_servo_x->usart_rx_buf[4]].zero_ang;
			}break;
			case 0x03://cmd_id:read_data
			{
				switch(p_usart_servo_x->usart_rx_buf[5])
				{
					case 0x01://data_id:volt
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].volt_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);
					}break;
					case 0x02://data_id:current
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].current_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);					
					}break;
					case 0x03://data_id:power
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].power_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);
					}break;
					case 0x04://data_id:temper
					{
						SERVO[p_usart_servo_x->usart_rx_buf[4]].temper_read = p_usart_servo_x->usart_rx_buf[6]+(((int16_t)p_usart_servo_x->usart_rx_buf[7])<<8);					
					}break;
				}
			}break;
		}
	}
}
//����Ƕȶ�ȡ����
void User_UsartReadServoAng(uint8_t servo_id)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		case 4:;case 5:;case 6:p_usart_servo_x = &USART_LEG2;break;
		case 7:;case 8:;case 9:p_usart_servo_x = &USART_LEG3;break;
		case 10:;case 11:;case 12:p_usart_servo_x = &USART_LEG4;break;
		case 13:;case 14:p_usart_servo_x = &USART_HEAD;break;
		default:break;
	}
	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0x0a;// ��ȡ����Ƕȵ�ָ��
	p_usart_servo_x->usart_tx_buf[3] = 0x01;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = servo_id;// ���ݰ����� ���ID

	for(i=0;i<=4;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[5] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,6);
}
//���������Ϣ��1-��ѹ,2-������3-���ʣ�4-�¶ȣ���ȡ����
void User_UsartReadServoData(uint8_t servo_id,uint8_t data_id)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF*p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		case 4:;case 5:;case 6:p_usart_servo_x = &USART_LEG2;break;
		case 7:;case 8:;case 9:p_usart_servo_x = &USART_LEG3;break;
		case 10:;case 11:;case 12:p_usart_servo_x = &USART_LEG4;break;
		case 13:;case 14:p_usart_servo_x = &USART_HEAD;break;
		default:break;
	}
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0x03;// ��ȡ������ݵ�ָ��
	p_usart_servo_x->usart_tx_buf[3] = 0x02;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = servo_id;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[5] = data_id;//1-��ѹ,2-������3-���ʣ�4-�¶�
	for(i=0;i<=5;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[6] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,7);
}
//���õ�������ĽǶȺ�ִ��ʱ��
void User_UsartSetServoAngTime(uint8_t servo_id,int16_t ang,uint16_t ms)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		case 4:;case 5:;case 6:p_usart_servo_x = &USART_LEG2;break;
		case 7:;case 8:;case 9:p_usart_servo_x = &USART_LEG3;break;
		case 10:;case 11:;case 12:p_usart_servo_x = &USART_LEG4;break;
		case 13:;case 14:p_usart_servo_x = &USART_HEAD;break;
		default:break;
	}
	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = servo_id;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[5] = ang&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[6] = ang>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[7] = ms&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[8] = ms>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,12);
}
//��������ģʽ����(0xFF����㲥ģʽ)
void User_SetDamping(uint8_t leg_id,uint16_t Power)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(leg_id)
	{
		case 1:p_usart_servo_x = &USART_RIGHT_LEG;break;
		case 2:p_usart_servo_x = &USART_LEFT_LEG;break;
		case 3:p_usart_servo_x = &USART_HEAD;break;
		default:break;
	}	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0x09;// �������ģʽ
	p_usart_servo_x->usart_tx_buf[3] = 0x03;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = 0xff;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[5] = Power&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[6] = Power>>8;//���Ŀ��Ƕ� ��8λ
	for(i=0;i<=6;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[7] = sum%256;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,8);
}
//����ԭ�㺯�����̼�Ŀǰ��֧�֣�
void User_SetOriginPoint(uint8_t leg_id)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(leg_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		case 4:;case 5:;case 6:p_usart_servo_x = &USART_LEG2;break;
		case 7:;case 8:;case 9:p_usart_servo_x = &USART_LEG3;break;
		case 10:;case 11:;case 12:p_usart_servo_x = &USART_LEG4;break;
		default:break;
	}	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0x17;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[3] = 0x02;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = 0x05;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[5] = 0x00;//���Ŀ��Ƕ� ��8λ
	for(i=0;i<=5;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[6] = sum%256;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,7);
}
//�������Ȼ������ȵĶ�������ǶȺ�ִ��ʱ��
void User_UsartSetLegAngTime(uint8_t leg_id,int16_t ang[6],uint16_t ms[6])
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(leg_id)
	{
		case 1:p_usart_servo_x = &USART_RIGHT_LEG;break;
		case 2:p_usart_servo_x = &USART_LEFT_LEG;break;
		default:break;
	}	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = leg_id*6-5;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[5] = ang[0]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[6] = ang[0]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[7] = ms[0]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[8] = ms[0]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[12+0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[12+1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[12+2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[12+3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[12+4] = leg_id*6-4;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[12+5] = ang[1]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[12+6] = ang[1]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[12+7] = ms[1]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[12+8] = ms[1]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[12+9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[12+10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[12+i];
	}
	p_usart_servo_x->usart_tx_buf[12+11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[24+0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[24+1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[24+2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[24+3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[24+4] = leg_id*6-3;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[24+5] = ang[2]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[24+6] = ang[2]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[24+7] = ms[2]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[24+8] = ms[2]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[24+9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[24+10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[24+i];
	}
	p_usart_servo_x->usart_tx_buf[24+11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[36+0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[36+1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[36+2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[36+3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[36+4] = leg_id*6-2;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[36+5] = ang[3]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[36+6] = ang[3]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[36+7] = ms[3]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[36+8] = ms[3]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[36+9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[36+10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[36+i];
	}
	p_usart_servo_x->usart_tx_buf[36+11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[48+0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[48+1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[48+2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[48+3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[48+4] = leg_id*6-1;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[48+5] = ang[4]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[48+6] = ang[4]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[48+7] = ms[4]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[48+8] = ms[4]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[48+9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[48+10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[48+i];
	}
	p_usart_servo_x->usart_tx_buf[48+11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[60+0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[60+1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[60+2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[60+3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[60+4] = leg_id*6;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[60+5] = ang[5]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[60+6] = ang[5]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[60+7] = ms[5]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[60+8] = ms[5]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[60+9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[60+10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[60+i];
	}
	p_usart_servo_x->usart_tx_buf[60+11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	//HAL_UART_Transmit(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,72, 1000);
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,72);
}
//����ͷ���Ķ�������ǶȺ�ִ��ʱ��
void User_UsartSetHeadAngTime(int16_t ang[2],uint16_t ms[2])
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	p_usart_servo_x = &USART_HEAD;
	
	p_usart_servo_x->usart_tx_buf[0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = 13;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[5] = ang[0]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[6] = ang[0]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[7] = ms[0]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[8] = ms[0]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	p_usart_servo_x->usart_tx_buf[11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	sum = 0;
	p_usart_servo_x->usart_tx_buf[12+0] = 0x12;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[12+1] = 0x4c;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[12+2] = 0x08;// �Ƕȿ���ģʽ
	p_usart_servo_x->usart_tx_buf[12+3] = 0x07;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[12+4] = 14;// ���ݰ����� ���ID
	p_usart_servo_x->usart_tx_buf[12+5] = ang[1]&0xff;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[12+6] = ang[1]>>8;//���Ŀ��Ƕ� ��8λ
	p_usart_servo_x->usart_tx_buf[12+7] = ms[1]&0xff;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[12+8] = ms[1]>>8;//ִ��ʱ���8λ
	p_usart_servo_x->usart_tx_buf[12+9] = 0;//һ����0
	p_usart_servo_x->usart_tx_buf[12+10] = 0;//һ����0
	for(i=0;i<=10;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[12+i];
	}
	p_usart_servo_x->usart_tx_buf[12+11] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������	
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,24);
}
//�����ۺϿ������������˵������Ȳ�
void User_LegAllSetAngTime(void) 
{
	int16_t ang_set[6] = {0}; 
	uint16_t ms_set[6] = {0}; 
	ms_set[0] = 2;
	ms_set[1] = 2;
	ms_set[2] = 2;
	ms_set[3] = 2;
	ms_set[4] = 2;
	ms_set[5] = 2;
	ang_set[0] = ang_goal[1];
	ang_set[1] = ang_goal[2];
	ang_set[2] = ang_goal[3];
	ang_set[3] = ang_goal[4];
	ang_set[4] = ang_goal[5];
    ang_set[5] = ang_goal[6];
	User_UsartSetLegAngTime(1,ang_set,ms_set);//����
	ang_set[0] = ang_goal[7];
	ang_set[1] = ang_goal[8];
	ang_set[2] = ang_goal[9];
	ang_set[3] = ang_goal[10];
	ang_set[4] = ang_goal[11];
	ang_set[5] = ang_goal[12];
	User_UsartSetLegAngTime(2,ang_set,ms_set);//����
	ang_set[0] = ang_goal[13];
	ang_set[1] = ang_goal[14];
	User_UsartSetHeadAngTime(ang_set,ms_set);//ͷ��
}
uint16_t User_AbsAngErr(int16_t ang)
{
	return (ang>=0)?ang:-ang;
}
uint16_t ACT_TIME_DIV = 100;
void User_SeovoActTimeCalcu(void)
{
	uint8_t i = 0;
	for(i=1;i<=12;i++)
	{
		SERVO[i].ms_set = User_AbsAngErr(SERVO[i].ang_set - SERVO[i].ang_read)/ACT_TIME_DIV;
	}
}
uint16_t ACT_ERR = 20;
uint8_t User_IsLegAngArrive(uint8_t leg_id) 
{
	uint8_t sub_id = 0;
	for(sub_id = 1;sub_id <=3;sub_id++)
	{
		if(User_AbsAngErr(SERVO[(leg_id-1)*3+sub_id].ang_read - SERVO[(leg_id-1)*3+sub_id].ang_set) > ACT_ERR)
			return 0;
	}
	return 1;
}
uint8_t User_IsLegTrigFloor(uint8_t leg_id) 
{
	return 0;
}



