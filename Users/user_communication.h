#ifndef _user_communication_h
#define _user_communication_h

#include "user_includes.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*------------------------ 协议宏定义 ------------------------*/
/* 帧结构相关 */
#define FRAME_HEADER_UP 0x4141   // 上位机帧头"AA"的十六进制表示
#define FRAME_FOOTER_UP 0x3D3D   // 上位机帧尾"=="的十六进制表示
#define FRAME_HEADER_DOWN 0x4242 // 下位机帧头"BB"的十六进制表示
#define FRAME_FOOTER_DOWN 0x2B2B // 下位机帧尾"++"的十六进制表示
#define MAX_DATA_LEN 128         // 协议允许的最大数据长度
#define CHECKSUM_INIT 0xFFFF     // CRC16校验的初始值

/* 命令响应状态定义 */
typedef enum
{
    CMD_RECEIVED,    // 命令已接收但未开始执行
    CMD_CHECK_ERROR, // 校验错误
    CMD_EXECUTING,   // 命令正在执行中
    CMD_COMPLETED    // 命令执行完成
} CmdState;

/*------------------------ 数据结构定义 ------------------------*/
/**
 * @brief 协议帧完整结构（用于内存映射）
 * @note 使用packed属性避免内存对齐带来的结构空隙
 */
typedef struct __attribute__((packed))
{
    uint16_t header;            // 帧头（2字节）
    uint8_t func;               // 功能码（1字节）
    uint16_t data_len;          // 数据长度（小端格式，2字节）
    uint8_t data[MAX_DATA_LEN]; // 数据负载
    uint16_t checksum;          // CRC16校验值（小端格式，2字节）
    uint16_t footer;            // 帧尾（2字节）
} ProtocolFrame;

typedef struct
{
    UART_HandleTypeDef *huart;           // 绑定的UART硬件实例指针
    DMA_HandleTypeDef *hdma;             // 绑定的DMA硬件实例指针
    uint8_t rx_buf[2][MAX_DATA_LEN + 10]; // 双缓冲接收阵列，+8容纳完整帧头尾
    volatile uint8_t buf_idx;            // 当前活动缓冲区索引（0或1）
    CmdState cmd_state;                  // 命令执行状态机
    uint8_t current_cmd;                 // 当前处理的命令功能码
} ProtocolHandle;

// 触摸传感器类型枚举
typedef enum
{
    TOUCH_HEAD = 0, // 头部触摸
    TOUCH_BODY,     // 身体触摸
    TOUCH_CHIN      // 下巴触摸
} TouchType;

typedef struct
{
    uint8_t head_touch; // 头部触摸状态（0/1 或 ADC 值）
    uint8_t body_touch; // 身体触摸状态
    uint8_t chin_touch; // 下巴触摸状态
    float roll;         // 翻滚角（单位：度）
    float pitch;        // 俯仰角（单位：度）
    float yaw;          // 偏航角（单位：度）
} SensorData;

typedef enum
{
	MODE_NORMAL = 0,   // 正常工作模式
    MODE_IDLE , // 空闲模式
    MODE_ACTION, // 做动作模式
} WorkMode;

typedef struct
{
    WorkMode mode;         // 工作模式
    float voltage;         // 当前电压(V)
    uint8_t battery_level; // 电量百分比(0-100)
    bool is_charging;      // 充电状态
    float max_temp;        // 最高温度(℃)
    uint16_t warning_code; // 警告代码
    uint16_t error_code;   // 错误代码
    uint32_t uptime;       // 运行时间(s)
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
