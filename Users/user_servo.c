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

int16_t goal_pos[15] = {0};//FEETECH POS GOAL
int16_t goal_speed[15] = {0};//FEETECH SPEED GOAL
int16_t goal_ms[15] = {0};//FEETECH MS GOAL
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
	USART_LEFT_LEG.p_usart_n = &huart1;
	USART_LEFT_LEG.p_hdma_usart_n_rx = &hdma_usart1_rx;
	USART_RIGHT_LEG.p_usart_n = &huart2;
	USART_RIGHT_LEG.p_hdma_usart_n_rx = &hdma_usart2_rx;
	USART_HEAD.p_usart_n = &huart3;
	USART_HEAD.p_hdma_usart_n_rx = &hdma_usart3_rx;
	
	//�����ŷ�����ĳ�ʼ��Ϣ���ٶȡ�λ�á�ʱ�䣩
	for(int i=0;i<=12;i++)
	{
		goal_speed[i] = 2000;
		goal_pos[i] = 0;
		goal_ms[i] = 0;
	}
	 
	
		goal_pos[1] = 0;
		goal_pos[2] = 0;
		goal_pos[3] = 0;
		goal_pos[4] = 0;
		goal_pos[5] = 0;
		
		goal_pos[6] = 0;
		goal_pos[7] = 0;
		goal_pos[8] = 0;
		goal_pos[9] = 0;
		goal_pos[10] = 0;
		
		goal_pos[11] = 0;
		goal_pos[12] = 0;
	
	for(int i=0;i<=14;i++)
		SERVO[i].zero_ang = 0;
	
	
	//���������жϽ���
	__HAL_UART_ENABLE_IT(USART_LEFT_LEG.p_usart_n, UART_IT_IDLE);         
	HAL_UART_Receive_DMA(USART_LEFT_LEG.p_usart_n, (uint8_t*)USART_LEFT_LEG.usart_rx_buf, USART_SERVO_RX_SIZE);
	
	__HAL_UART_ENABLE_IT(USART_RIGHT_LEG.p_usart_n, UART_IT_IDLE);         
	HAL_UART_Receive_DMA(USART_RIGHT_LEG.p_usart_n, (uint8_t*)USART_RIGHT_LEG.usart_rx_buf, USART_SERVO_RX_SIZE);
  
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
		__HAL_UART_CLEAR_IDLEFLAG(USART_HEAD.p_usart_n);   				    //����жϱ�־λ
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
	
	for(i=2;i<p_usart_servo_x->rx_data_len-1;i++)
	{
		sum += p_usart_servo_x->usart_rx_buf[i];
		sum %= 256;
	}
	sum %= 256;
	sum = ~sum;
	if((p_usart_servo_x->usart_rx_buf[0] == 0xFF) && (p_usart_servo_x->usart_rx_buf[1] == 0xFF) && (p_usart_servo_x->usart_rx_buf[4] == 0x00) && (p_usart_servo_x->usart_rx_buf[i] == sum))
	{
		SERVO[p_usart_servo_x->usart_rx_buf[2]].pos_read = (p_usart_servo_x->usart_rx_buf[5]+(((int16_t)p_usart_servo_x->usart_rx_buf[6])<<8)) - SERVO[p_usart_servo_x->usart_rx_buf[2]].zero_ang - 2048;
		SERVO[p_usart_servo_x->usart_rx_buf[2]].speed_read = p_usart_servo_x->usart_rx_buf[7]+(((int16_t)p_usart_servo_x->usart_rx_buf[8])<<8);					
		SERVO[p_usart_servo_x->usart_rx_buf[2]].power_read = p_usart_servo_x->usart_rx_buf[9]+(((int16_t)p_usart_servo_x->usart_rx_buf[10])<<8);
		SERVO[p_usart_servo_x->usart_rx_buf[2]].volt_read = p_usart_servo_x->usart_rx_buf[11];
		SERVO[p_usart_servo_x->usart_rx_buf[2]].temper_read = p_usart_servo_x->usart_rx_buf[12];					
	}
}

//����Ƕ�д�뺯����WRITE DATA��
void FEETECH_UsartSetServoPos(uint8_t servo_id,int16_t pos,uint16_t ms,int16_t speed)
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
	
	p_usart_servo_x->usart_tx_buf[0] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = servo_id;// ���ID��
	p_usart_servo_x->usart_tx_buf[3] = 0x09;// ���ݰ���Ч���ݳ���
	p_usart_servo_x->usart_tx_buf[4] = 0x03;// дָ��
	p_usart_servo_x->usart_tx_buf[5] = 0x2A;// ���Ʊ���Ŀ��λ�õ��׵�ַ
	p_usart_servo_x->usart_tx_buf[6] = pos&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[7] = pos>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[8] = ms&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[9] = ms>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[10] = speed&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[11] = speed>>8;//���Ŀ���ٶ� ��8λ

	sum=0;
	for(i=2;i<=11;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	sum %= 256;
	sum = ~sum;
	p_usart_servo_x->usart_tx_buf[12] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,13);
}

//����Ƕ�ͬ��д�뺯����SYNC WRITE������HEAD
void FEETECH_HEADSYNCWRITE(int16_t pos[5],int16_t ms[5],int16_t speed[5])
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	p_usart_servo_x = &USART_HEAD;
	
	p_usart_servo_x->usart_tx_buf[0] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0xFE;// ���ID��
	p_usart_servo_x->usart_tx_buf[3] = 0x12;// ���ݰ���Ч���ݳ���
	p_usart_servo_x->usart_tx_buf[4] = 0x83;// ͬ��дָ��
	p_usart_servo_x->usart_tx_buf[5] = 0x2A;// ���Ʊ���Ŀ��λ�õ��׵�ַ
	p_usart_servo_x->usart_tx_buf[6] = 0x06;// д������ĳ���
	
	p_usart_servo_x->usart_tx_buf[7] = 11;//���ID
	p_usart_servo_x->usart_tx_buf[8] = pos[0]&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[9] = pos[0]>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[10] = ms[0]&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[11] = ms[0]>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[12] = speed[0]&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[13] = speed[0]>>8;//���Ŀ���ٶ� ��8λ
	
	p_usart_servo_x->usart_tx_buf[14] = 12;//���ID
	p_usart_servo_x->usart_tx_buf[15] = pos[1]&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[16] = pos[1]>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[17] = ms[1]&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[18] = ms[1]>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[19] = speed[1]&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[20] = speed[1]>>8;//���Ŀ���ٶ� ��8λ
	
	sum=0;
	for(i=2;i<=20;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	sum %= 256;
	sum = ~sum;
	p_usart_servo_x->usart_tx_buf[21] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,22);
	//HAL_UART_Transmit(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,13,100);
}

//����Ƕ�ͬ��д�뺯����SYNC WRITE������LEG
void FEETECH_LEGSYNCWRITE(uint8_t leg_id,int16_t pos[5],int16_t ms[5],int16_t speed[5])
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(leg_id)
	{
		case 1:p_usart_servo_x = &USART_LEFT_LEG;break;
		case 2:p_usart_servo_x = &USART_RIGHT_LEG;break;
		default:break;
	}
	
	p_usart_servo_x->usart_tx_buf[0] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0xFE;// ���ID��
	p_usart_servo_x->usart_tx_buf[3] = 0x27;// ���ݰ���Ч���ݳ���
	p_usart_servo_x->usart_tx_buf[4] = 0x83;// ͬ��дָ��
	p_usart_servo_x->usart_tx_buf[5] = 0x2A;// ���Ʊ���Ŀ��λ�õ��׵�ַ
	p_usart_servo_x->usart_tx_buf[6] = 0x06;// д������ĳ���
	
	p_usart_servo_x->usart_tx_buf[7] = leg_id*5 - 4;//���ID
	p_usart_servo_x->usart_tx_buf[8] = pos[0]&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[9] = pos[0]>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[10] = ms[0]&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[11] = ms[0]>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[12] = speed[0]&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[13] = speed[0]>>8;//���Ŀ���ٶ� ��8λ
	
	p_usart_servo_x->usart_tx_buf[14] = leg_id*5 - 3;//���ID
	p_usart_servo_x->usart_tx_buf[15] = pos[1]&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[16] = pos[1]>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[17] = ms[1]&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[18] = ms[1]>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[19] = speed[1]&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[20] = speed[1]>>8;//���Ŀ���ٶ� ��8λ
	
	p_usart_servo_x->usart_tx_buf[21] = leg_id*5 - 2;//���ID
	p_usart_servo_x->usart_tx_buf[22] = pos[2]&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[23] = pos[2]>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[24] = ms[2]&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[25] = ms[2]>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[26] = speed[2]&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[27] = speed[2]>>8;//���Ŀ���ٶ� ��8λ
	
	p_usart_servo_x->usart_tx_buf[28] = leg_id*5 - 1;//���ID
	p_usart_servo_x->usart_tx_buf[29] = pos[3]&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[30] = pos[3]>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[31] = ms[3]&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[32] = ms[3]>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[33] = speed[3]&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[34] = speed[3]>>8;//���Ŀ���ٶ� ��8λ
	
	p_usart_servo_x->usart_tx_buf[35] = leg_id*5;//���ID
	p_usart_servo_x->usart_tx_buf[36] = pos[4]&0xff;//���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[37] = pos[4]>>8;// ���Ŀ��λ�� ��8λ
	p_usart_servo_x->usart_tx_buf[38] = ms[4]&0xff;//���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[39] = ms[4]>>8;// ���Ŀ��ʱ�� ��8λ
	p_usart_servo_x->usart_tx_buf[40] = speed[4]&0xff;//���Ŀ���ٶ� ��8λ
	p_usart_servo_x->usart_tx_buf[41] = speed[4]>>8;//���Ŀ���ٶ� ��8λ

	sum=0;
	for(i=2;i<=41;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	sum %= 256;
	sum = ~sum;
	p_usart_servo_x->usart_tx_buf[42] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,43);
	//HAL_UART_Transmit(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,13,100);
}
//ָ������Ƕȶ�ȡ����
void FEETECH_ReadServoPos(uint8_t servo_id)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:;case 2:;case 3:p_usart_servo_x = &USART_LEG1;break;
		case 4:;case 5:p_usart_servo_x = &USART_LEG2;break;
		case 6:;case 7:;case 8:p_usart_servo_x = &USART_LEG3;break;
		case 9:;case 10:p_usart_servo_x = &USART_LEG4;break;
		case 11:;case 12:p_usart_servo_x = &USART_HEAD;break;
		default:break;
	}
	
	p_usart_servo_x->usart_tx_buf[0] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = servo_id;// ���ID
	p_usart_servo_x->usart_tx_buf[3] = 0x04;// ���ݰ����ݵ��ֽڳ���
	p_usart_servo_x->usart_tx_buf[4] = 0x02;// ���ݰ����� 
	p_usart_servo_x->usart_tx_buf[5] = 0x38;// ���ݰ���ַ 
	p_usart_servo_x->usart_tx_buf[6] = 0x08;// ���ݰ����� 

	sum=0;
	for(i=2;i<=6;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	sum %= 256;
	sum = ~sum;
	p_usart_servo_x->usart_tx_buf[7] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������ȡ����
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,8);
	
}
//����Ƕ�ͬ����ȡ����
void FEETECH_LEGSYNCRead(uint8_t servo_id)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	USART_SERVO_TYPEDEF* p_usart_servo_x;
	
	switch(servo_id)
	{
		case 1:p_usart_servo_x = &USART_LEFT_LEG;break;
		case 2:p_usart_servo_x = &USART_RIGHT_LEG;break;
		default:break;
	}
	
	p_usart_servo_x->usart_tx_buf[0] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[1] = 0xFF;// ֡ͷ
	p_usart_servo_x->usart_tx_buf[2] = 0xFE;// �㲥
	p_usart_servo_x->usart_tx_buf[3] = 0x06;// ���ݰ�����
	p_usart_servo_x->usart_tx_buf[4] = 0x82;// ���ݰ�ָ��
	p_usart_servo_x->usart_tx_buf[5] = 0x38;// ���ݰ���ַ 
	p_usart_servo_x->usart_tx_buf[6] = 0x08;// ���ݰ����� 
	p_usart_servo_x->usart_tx_buf[7] = 0x01;// ���ݰ����� 
	p_usart_servo_x->usart_tx_buf[8] = 0x02;// ���ݰ����� 

	sum=0;
	for(i=2;i<=8;i++)
	{
		sum += p_usart_servo_x->usart_tx_buf[i];
	}
	sum %= 256;
	sum = ~sum;
	p_usart_servo_x->usart_tx_buf[9] = sum;// ���ݰ�У��ͣ���0��n-1���ֽ�������ͣ�Ȼ���256ȡ������ȡ����
	
	HAL_UART_Transmit_DMA(p_usart_servo_x->p_usart_n,p_usart_servo_x->usart_tx_buf,10);
}


//�����ۺϿ������������˵������Ȳ�
void User_AllSetAngTime(void) 
{
	int16_t tmp_pos[5] = {0};
	int16_t tmp_ms[5] = {0};
	int16_t tmp_speed[5] = {0};
	
	//����ʹ��
	for(int x=0;x<5;x++)
	{
		tmp_ms[x] = goal_ms[x];
		tmp_speed[x] = goal_speed[x];
	}
	
	tmp_pos[0] = 2048 + goal_pos[1];
	tmp_pos[1] = 2048 + goal_pos[2];
	tmp_pos[2] = 2048 + goal_pos[3];
	tmp_pos[3] = 2048 + goal_pos[4];
	tmp_pos[4] = 2048 + goal_pos[5];
	FEETECH_LEGSYNCWRITE(1,tmp_pos,tmp_ms,tmp_speed);//����
	tmp_pos[0] = 2048 + goal_pos[6];
	tmp_pos[1] = 2048 + goal_pos[7];
	tmp_pos[2] = 2048 + goal_pos[8];
	tmp_pos[3] = 2048 + goal_pos[9];
	tmp_pos[4] = 2048 + goal_pos[10];
	FEETECH_LEGSYNCWRITE(2,tmp_pos,tmp_ms,tmp_speed);//����
	tmp_pos[0] = 2048 + goal_pos[11];
	tmp_pos[1] = 2048 + goal_pos[12];
	FEETECH_HEADSYNCWRITE(tmp_pos,tmp_ms,tmp_speed);//ͷ��
}

/*�ñ�����������ÿ���������Ķ���Ƕ�*/
void User_BezierCurve(int stepping, ServoActionSeries* Action_analyze)
{
    //�����Ͽ��Ƶ����(�ֶ�ֵ)
    float count_t = 1.0f /(stepping - 1);
    for(int i = 0; i <= stepping - 1; i++)
    {
        float cur_t = count_t * (float)i;
        float one_minus_t = 1.0f - cur_t;
        int j = 0;
        for(j = 1; j <= 12; j++)
        {
            Action_analyze->actions[i].servoAngles[j] =  (int)(one_minus_t * Action_analyze->startservoAngles[j] + cur_t * Action_analyze->endservoAngles[j]);
        }
    }
}

