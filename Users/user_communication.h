#ifndef _user_communication_h
#define _user_communication_h

#include "user_includes.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*------------------------ Э��궨�� ------------------------*/
/* ֡�ṹ��� */
#define FRAME_HEADER_UP 0x4141   // ��λ��֡ͷ"AA"��ʮ�����Ʊ�ʾ
#define FRAME_FOOTER_UP 0x3D3D   // ��λ��֡β"=="��ʮ�����Ʊ�ʾ
#define FRAME_HEADER_DOWN 0x4242 // ��λ��֡ͷ"BB"��ʮ�����Ʊ�ʾ
#define FRAME_FOOTER_DOWN 0x2B2B // ��λ��֡β"++"��ʮ�����Ʊ�ʾ
#define MAX_DATA_LEN 128         // Э�������������ݳ���
#define CHECKSUM_INIT 0xFFFF     // CRC16У��ĳ�ʼֵ

/* ������Ӧ״̬���� */
typedef enum
{
    CMD_RECEIVED,    // �����ѽ��յ�δ��ʼִ��
    CMD_CHECK_ERROR, // У�����
    CMD_EXECUTING,   // ��������ִ����
    CMD_COMPLETED    // ����ִ�����
} CmdState;

/*------------------------ ���ݽṹ���� ------------------------*/
/**
 * @brief Э��֡�����ṹ�������ڴ�ӳ�䣩
 * @note ʹ��packed���Ա����ڴ��������Ľṹ��϶
 */
typedef struct __attribute__((packed))
{
    uint16_t header;            // ֡ͷ��2�ֽڣ�
    uint8_t func;               // �����루1�ֽڣ�
    uint16_t data_len;          // ���ݳ��ȣ�С�˸�ʽ��2�ֽڣ�
    uint8_t data[MAX_DATA_LEN]; // ���ݸ���
    uint16_t checksum;          // CRC16У��ֵ��С�˸�ʽ��2�ֽڣ�
    uint16_t footer;            // ֡β��2�ֽڣ�
} ProtocolFrame;

typedef struct
{
    UART_HandleTypeDef *huart;           // �󶨵�UARTӲ��ʵ��ָ��
    DMA_HandleTypeDef *hdma;             // �󶨵�DMAӲ��ʵ��ָ��
    uint8_t rx_buf[2][MAX_DATA_LEN + 10]; // ˫����������У�+8��������֡ͷβ
    volatile uint8_t buf_idx;            // ��ǰ�������������0��1��
    CmdState cmd_state;                  // ����ִ��״̬��
    uint8_t current_cmd;                 // ��ǰ������������
} ProtocolHandle;

// ��������������ö��
typedef enum
{
    TOUCH_HEAD = 0, // ͷ������
    TOUCH_BODY,     // ���崥��
    TOUCH_CHIN      // �°ʹ���
} TouchType;

typedef struct
{
    uint8_t head_touch; // ͷ������״̬��0/1 �� ADC ֵ��
    uint8_t body_touch; // ���崥��״̬
    uint8_t chin_touch; // �°ʹ���״̬
    float roll;         // �����ǣ���λ���ȣ�
    float pitch;        // �����ǣ���λ���ȣ�
    float yaw;          // ƫ���ǣ���λ���ȣ�
} SensorData;

typedef enum
{
	MODE_NORMAL = 0,   // ��������ģʽ
    MODE_IDLE , // ����ģʽ
    MODE_ACTION, // ������ģʽ
} WorkMode;

typedef struct
{
    WorkMode mode;         // ����ģʽ
    float voltage;         // ��ǰ��ѹ(V)
    uint8_t battery_level; // �����ٷֱ�(0-100)
    bool is_charging;      // ���״̬
    float max_temp;        // ����¶�(��)
    uint16_t warning_code; // �������
    uint16_t error_code;   // �������
    uint32_t uptime;       // ����ʱ��(s)
} WorkStatus;

extern ACTION_STATE ActionNow;
extern ProtocolHandle ph;
extern WorkStatus stateRobot;
extern uint8_t actionStop;

void sendStateActive(ProtocolHandle *ph, WorkStatus status);
void Key_Downside_Record(void);
void User_CommunicationInit(void);
void User_Communication_IRQHandler(void);
void Send_Response(ProtocolHandle *ph, uint8_t result);
#endif
