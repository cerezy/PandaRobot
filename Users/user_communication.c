#include "user_communication.h"
#include <string.h>
#include "main.h"

ProtocolHandle ph; // 协议句柄实例
uint8_t actionStop = 0;
WorkStatus stateRobot;
/**
 * @brief 通信模块初始化
 * @note 配置UART和DMA，启用空闲中断
 */
void User_CommunicationInit(void)
{
    ph.huart = &huart4;       // UART句柄
    ph.hdma = &hdma_uart4_rx; // DMA句柄
    ph.buf_idx = 0;           // 初始化缓冲区索引
    __HAL_UART_ENABLE_IT(ph.huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(ph.huart, ph.rx_buf[ph.buf_idx], MAX_DATA_LEN + 9);
}

void Single_Key_Record(uint8_t *key, uint8_t *last_key, uint8_t *key_downside, uint8_t *key_upside)
{
    // 下降沿检测
    if ((*last_key == 1) && (*key == 0))
    {
        *key_downside = 1;
    }
    else
    {
        *key_downside = 0;
    }
    // 上升沿检测
    if ((*last_key == 0) && (*key == 1))
    {
        *key_upside = 1;
    }
    else
    {
        *key_upside = 0;
    }

    *last_key = *key;
}

uint8_t touchTopofHead_Downside, touchTopofHead_Upside, touchTopofHead, Last_touchTopofHead; // 触摸头部
uint8_t touchChin_Downside, touchChin_Upside, touchChin, Last_touchChin;                     // 触摸下巴
uint8_t humanDetection_Downside, humanDetection_Upside, humanDetection, Last_humanDetection; // 触摸下巴
void Key_Downside_Record(void)
{
    touchTopofHead = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
    touchChin = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
    humanDetection = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
    Single_Key_Record(&touchTopofHead, &Last_touchTopofHead, &touchTopofHead_Downside, &touchTopofHead_Upside);
    Single_Key_Record(&touchChin, &Last_touchChin, &touchChin_Downside, &touchChin_Upside);
    Single_Key_Record(&humanDetection, &Last_humanDetection, &humanDetection_Downside, &humanDetection_Upside);
}

/*------------------------ 函数定义 ------------------------*/

/**
 * @brief 计算简单和校验（16位）
 * @param data 待校验数据指针
 * @param len 校验数据长度（字节数）
 * @return 计算得到的16位校验值
 */
uint16_t Calculate_SumCheck(uint8_t *data, uint16_t len)
{
    uint16_t sum = 0;
    while (len--)
    {
        sum += *data++;
    }
    return sum;
}

// ====================== 工作模式相关函数 ======================
/**
 * @brief 获取当前工作模式
 * @return 工作模式枚举值
 */
WorkMode Get_WorkMode(void)
{
	if(ActionNow == IDLE)
    return MODE_IDLE;
	else return MODE_ACTION;
}

/**
 * @brief 获取电源电压
 * @return 当前电压值（单位：伏特）
 */
float Power_GetVoltage(void)
{
    // 临时返回0电压
    return USER_ADC.bat_volt;
}

/**
 * @brief 获取电池电量百分比
 * @return 电量百分比（0-100）
 */
uint8_t Get_BatteryLevel(void)
{
    // 临时返回0%电量
    return USER_ADC.bat_power;
}

/**
 * @brief 检测充电状态
 * @return true:正在充电 false:未充电
 */
bool Is_Charging(void)
{
    // 临时返回未充电状态
    return false;
}

/**
 * @brief 获取系统最高温度
 * @return 最高温度值（单位：摄氏度）
 */
float Get_MaxTemperature(void)
{
    // 临时返回0摄氏度
    return 0.0f;
}

/**
 * @brief 获取当前警告代码
 * @return 警告代码（0表示无警告）
 */
uint16_t Get_Warning(void)
{
    // 临时返回无警告
    return 0;
}

/**
 * @brief 获取最近错误代码
 * @return 错误代码（0表示无错误）
 */
uint16_t Get_LastError(void)
{
    // 临时返回无错误
    return 0;
}

// ====================== 传感器相关函数 ======================
/**
 * @brief 获取触摸传感器值
 * @param touch_type 触摸类型（HEAD/BODY/CHIN）
 * @return 触摸状态（0/1 或 ADC原始值）
 */
uint8_t Get_TouchValue(TouchType touch_type)
{
    // 临时返回无触摸信号
    (void)touch_type; // 消除未使用参数警告
    return 0;
}

/**
 * @brief 获取IMU翻滚角
 * @return 翻滚角度（单位：度，-180~180）
 */
float IMU_GetRoll(void)
{
    // 临时返回0度横滚角
    return 0.0f;
}

/**
 * @brief 获取IMU俯仰角
 * @return 俯仰角度（单位：度，-90~90）
 */
float IMU_GetPitch(void)
{
    // 临时返回0度俯仰角
    return 0.0f;
}

/**
 * @brief 获取IMU偏航角
 * @return 偏航角度（单位：度，0~360）
 */
float IMU_GetYaw(void)
{
    // 临时返回0度偏航角
    return 0.0f;
}

/* 警告/错误代码转换函数示例 */
/**
 * @brief 警告代码转字符串
 * @param code 警告代码
 * @return 可读的警告描述
 */
char *WarningCodeToString(uint16_t code)
{
    switch (code)
    {
    case 0:
        return "none";
    case 1:
        return "low battery";
    case 2:
        return "high temp";
    default:
        return "unknown";
    }
}

/**
 * @brief 错误代码转字符串
 * @param code 错误代码
 * @return 可读的错误描述
 */
char *ErrorCodeToString(uint16_t code)
{
    switch (code)
    {
    case 0:
        return "none";
    case 1:
        return "motor fault";
    case 2:
        return "sensor error";
    default:
        return "unknown";
    }
}


uint8_t tx_buf_[136];
/**
 * @brief 发送传感器数据响应帧
 * @param ph 协议句柄指针
 * @param data 要发送的JSON数据字符串
 * @param len JSON数据长度
 * @note 帧结构：帧头(2B) | 功能码(1B) | 数据长度(2B) | JSON数据(NB) | 校验和(1B) | 帧尾(2B)
 */
void Send_Sensor_Data(ProtocolHandle *ph, const char *data, uint16_t len)
{
    // 校验数据长度合法性
    if (len > MAX_DATA_LEN)
    {
        return;
    }

    // 构造完整数据帧（动态计算帧长度）
//    uint8_t tx_buf[2 + 2 + 5 + len]; // 头帧尾帧4+功能码1+数据长度2+校验1
    uint16_t frame_len = 2+2+5+len;

    // 帧头（2字节）
    tx_buf_[0] = FRAME_HEADER_DOWN >> 8;
    tx_buf_[1] = FRAME_HEADER_DOWN & 0xFF;

    // 功能码（1字节）
    tx_buf_[2] = ph->current_cmd;

    // 数据长度（小端格式，2字节）
    tx_buf_[3] = len & 0xFF;
    tx_buf_[4] = (len >> 8) & 0xFF;

    // 数据内容
    memcpy(&tx_buf_[5], data, len);

    // 计算和校验（功能码+长度+数据）
    uint16_t checksum = Calculate_SumCheck(&tx_buf_[3], len);
    tx_buf_[5 + len] = checksum;

    // 帧尾（2字节）
    tx_buf_[6 + len] = FRAME_FOOTER_DOWN >> 8;
    tx_buf_[7 + len] = FRAME_FOOTER_DOWN & 0xFF;

    // DMA发送
    HAL_UART_Transmit_DMA(ph->huart, tx_buf_, 8 + len);
}

/*------------------------ 舵机参数获取函数 ------------------------*/
/**
 * @brief 获取所有舵机参数
 * @param params 舵机参数结构体数组
 * @note SERVO[1]-SERVO[12] 对应 params[0]-params[11]
 */
void Servo_GetAllParams(SERVO_INFO_TYPEDEF *params)
{
    // SERVO[1]-SERVO[12] 对应 params[0]-params[11]
    for (int i = 0; i < 12; i++)
    {
        params[i] = SERVO[i + 1]; // 数组索引偏移
    }
}

/**
 * @brief 发送标准响应帧
 * @param ph 协议句柄指针
 * @param result 响应结果码（0表示成功，1表示校验失败，2表示开始执行，3表示执行完毕）
 * @note 帧结构：BB BB | func | 01 00 | result | 和校验 | ++ ++
 */
uint8_t _tx_buf_[10] = {0x42, 0x42, 0, 1, 0, 0, 0, 0, 0x2B, 0x2B};
void Send_Response(ProtocolHandle *ph, uint8_t result)
{
//    uint8_t tx_buf[10] = {
//        FRAME_HEADER_DOWN >> 8, FRAME_HEADER_DOWN & 0xFF, // 帧头BB BB
//        ph->current_cmd,                                  // 原样回传功能码
//        1, 0,                                             // 数据长度小端（固定1字节）
//        result,                                           // 结果码
//        0, 0,                                             // 和校验占位
//        FRAME_FOOTER_DOWN >> 8, FRAME_FOOTER_DOWN & 0xFF  // 帧尾++ ++
//    };
	_tx_buf_[2] = ph->current_cmd;
	_tx_buf_[5] = result;
    // 计算校验和（仅对数据内容校验）
    uint16_t checksum = Calculate_SumCheck(&_tx_buf_[5], 1);
    _tx_buf_[6] = checksum & 0xFF; // 校验低字节在前
    _tx_buf_[7] = checksum >> 8;   // 高字节在后

    HAL_UART_Transmit_DMA(ph->huart, _tx_buf_, sizeof(_tx_buf_));
}

void sendStateActive(ProtocolHandle *ph, WorkStatus status)
{
	/* 获取所需状态数据 */
            status.voltage = Power_GetVoltage();       // 电压值（float）
            status.battery_level = Get_BatteryLevel(); // 电量百分比（0~100）
            status.is_charging = Is_Charging();         // 充电状态

        /* 生成精简的JSON格式状态数据 */
        char json_buf[128]; // 适当大小的缓冲区
        snprintf(json_buf, sizeof(json_buf),
                 "{\"mode\":%d,\"voltage\":%.2f,\"battery\":\"%d%%\",\"charging\":%s}",
                 status.mode,
                 status.voltage,        // %.2f 保留两位小数
                 status.battery_level,  // 
                 status.is_charging ? "true" : "false");
        
        ph->cmd_state = CMD_RECEIVED;
        
        /* 发送状态数据 */
        Send_Sensor_Data(ph, json_buf, strlen(json_buf));
}

/**
 * @brief 协议解析主逻辑
 * @param ph 协议句柄指针
 * @note 执行顺序：帧结构校验 -> 和校验 -> 功能码分发
 */
void Parse_Protocol(ProtocolHandle *ph)
{
    ProtocolFrame *frame = (ProtocolFrame *)ph->rx_buf[ph->buf_idx];
    frame->checksum = (frame->data[frame->data_len + 1] << 8) | frame->data[frame->data_len];
    frame->footer = *(uint16_t *)&frame->data[frame->data_len + 2];

    // 清空校验和及帧尾区域
    for (int i = 0; i < 4; i++)
        frame->data[frame->data_len + i] = 0;

    /* 基础结构校验 */
    if (frame->header != FRAME_HEADER_UP || // 验证上位机帧头
        frame->footer != FRAME_FOOTER_UP || // 验证上位机帧尾
        frame->data_len > MAX_DATA_LEN)     // 数据长度合法性检查
    {
        return;
    }

    /* 和校验验证 */
    uint16_t calc_sum = Calculate_SumCheck(frame->data, frame->data_len);
    if (calc_sum != frame->checksum)
    {
        ph->cmd_state = CMD_CHECK_ERROR;  // 设置校验错误状态
        Send_Response(ph, ph->cmd_state); // 响应校验失败
        return;
    }

    TEACHMODE = 0;                 // 清除教学模式标志（如有）
    ph->current_cmd = frame->func; // 记录当前命令功能码

    /* 功能码分发处理 */
    switch (frame->func)
    {
    case 0x01: // 宠物状态控制命令
        if (frame->data_len == 2)
        { // 预期2字节数据
            // 解析状态和程度值（示例）
            uint8_t state = frame->data[0];
            uint8_t level = frame->data[1];

            // 更新状态机
            ph->cmd_state = CMD_RECEIVED;
            Send_Response(ph, ph->cmd_state); // 立即响应接收成功
        }
        break;

    case 0x02: // 动作库控制
    {
        // 首次响应（接收成功）
        ph->cmd_state = CMD_RECEIVED;
        Send_Response(ph, ph->cmd_state);

        // 解析动作编号（范围0-255）
        uint8_t action_id = frame->data[0];
		
		if(action_id == 255) 
		{
			actionStop = 1;
		}
//        // 验证动作编号有效性（示例：有效范围0-121）
//        else if (action_id > 150)
//        {
//            ph->cmd_state = CMD_CHECK_ERROR;
//            Send_Response(ph, ph->cmd_state);
//            break;
//        }

		else
		{
			// 更新动作控制参数
			actionStop = 0;
			ph->cmd_state = CMD_RECEIVED;
			Send_Response(ph, ph->cmd_state); // 再次响应
			ActionNow = action_id;            // 设置当前动作
		}
        break;
    }

    case 0x03: // 关节控制命令
        if (frame->data_len == 24)
        { // 24字节对应12个关节
            // 解析关节角度（小端格式）
            uint16_t angles[12];
            for (int i = 0; i < 12; i++)
            {
                angles[i] = frame->data[i * 2] | (frame->data[i * 2 + 1] << 8);
            }
            ph->cmd_state = CMD_RECEIVED;     // 设置命令接收状态
            Send_Response(ph, ph->cmd_state); // 响应接收成功
        }
        break;

    /*------ 0x04: 传感器查询 ------*/
    case 0x04:
    {
        // 传感器数据
        SensorData sensor = {
            .head_touch = Get_TouchValue(TOUCH_HEAD),
            .body_touch = Get_TouchValue(TOUCH_BODY),
            .chin_touch = Get_TouchValue(TOUCH_CHIN),
            .roll = IMU_GetRoll(),
            .pitch = IMU_GetPitch(),
            .yaw = IMU_GetYaw()};

        // 转换为JSON字符串
        char json_buf[128];
        snprintf(json_buf, sizeof(json_buf),
                 "{\"touch\":[%u,%u,%u],\"pose\":[%.1f,%.1f,%.1f]}",
                 sensor.head_touch, sensor.body_touch, sensor.chin_touch,
                 sensor.roll, sensor.pitch, sensor.yaw);
        ph->cmd_state = CMD_RECEIVED;     // 设置命令接收状态
        Send_Response(ph, ph->cmd_state); // 响应接收成功
        // 发送响应帧
        Send_Sensor_Data(ph, json_buf, strlen(json_buf));
        break;
    }

    /*------ 0x05: 工作状态查询 ------*/
	case 0x05:
    {
        /* 获取所需状态数据 */
        WorkStatus status = {
            .mode = Get_WorkMode(),              // 工作模式
            .voltage = Power_GetVoltage(),       // 电压值（float）
            .battery_level = Get_BatteryLevel(), // 电量百分比（0~100）
            .is_charging = Is_Charging()         // 充电状态
        };

        /* 生成精简的JSON格式状态数据 */
        char json_buf[128]; // 适当大小的缓冲区
        snprintf(json_buf, sizeof(json_buf),
                 "{\"mode\":%d,\"voltage\":%.2f,\"battery\":\"%d%%\",\"charging\":%s}",
                 status.mode,
                 status.voltage,        // %.2f 保留两位小数
                 status.battery_level,  // 
                 status.is_charging ? "true" : "false");
        
        ph->cmd_state = CMD_RECEIVED;
        
        /* 发送状态数据 */
        Send_Sensor_Data(ph, json_buf, strlen(json_buf));
        break;
    }
	

    default:                     // 未知功能码
        Send_Response(ph, 0x01); // 响应校验失败
        break;
    }
}

/**
 * @brief 发送舵机参数信息
 * @note 主动上报所有舵机参数，JSON数组格式
 */
void sendServoParameters(void)
{
    SERVO_INFO_TYPEDEF params[12];
    Servo_GetAllParams(params); // 获取舵机参数

    // 构造JSON数组
    char json_buf[128];
    char *ptr = json_buf;

    // 起始标记
    ptr += sprintf(ptr, "[");

    // 遍历所有舵机
    for (int i = 0; i < 12; i++)
    {
        /* 参数说明：
           i+1        - 舵机物理编号（SERVO[1]对应舵机1）
           pos_read   - 舵机位置（int16_t）
           speed_read - 舵机速度（uint16_t）
           temper_read- 舵机温度（uint16_t） */
        ptr += sprintf(ptr,
                       "{\"id\":%d,\"pos\":%d,\"speed\":%u,\"temp\":%u}%c",
                       i + 1,                 // 舵机物理编号从1开始
                       params[i].pos_read,    // 位置参数
                       params[i].speed_read,  // 速度参数
                       params[i].temper_read, // 温度参数
                       (i == 11) ? ']' : ','  // 数组闭合标记
        );
    }
    ph.current_cmd = 0x06; // 设置当前命令功能码
    // 触发主动上报（带长度校验）
    Send_Sensor_Data(&ph, json_buf, ptr - json_buf);
}

/**
 * @brief 空闲中断处理函数
 * @note 由硬件中断触发，完成DMA接收管理
 */
void User_Communication_IRQHandler(void)
{
    if (__HAL_UART_GET_FLAG(ph.huart, UART_FLAG_IDLE))
    {
        // 清除空闲中断标志
        __HAL_UART_CLEAR_IDLEFLAG(ph.huart);

        // 停止当前DMA传输
        HAL_UART_DMAStop(ph.huart);

        // 计算实际接收长度
        uint16_t len = MAX_DATA_LEN + 9 - __HAL_DMA_GET_COUNTER(ph.hdma);

        // 切换缓冲区并复制数据
        memcpy(ph.rx_buf[ph.buf_idx ^ 1], ph.rx_buf[ph.buf_idx], len);
        ph.buf_idx ^= 1; // 切换缓冲索引

        // 提交协议解析任务
        Parse_Protocol(&ph);

        // 重启DMA接收（使用当前活动缓冲区）
        HAL_UART_Receive_DMA(ph.huart, ph.rx_buf[ph.buf_idx], MAX_DATA_LEN + 9);
    }
}

/**
 * @brief 命令状态机处理（主循环调用）
 * @param ph 协议句柄指针
 * @note 处理多阶段响应（接收->执行->完成）
 */
void Process_Cmd_State(ProtocolHandle *ph)
{
    static uint32_t tick = 0; // 计时基准

    switch (ph->cmd_state)
    {
    case CMD_RECEIVED: // 已接收待执行
        if (HAL_GetTick() - tick > 100)
        {
            Send_Response(ph, 0x02); // 发送开始执行响应
            ph->cmd_state = CMD_EXECUTING;
            tick = HAL_GetTick(); // 重置计时
        }
        break;

    case CMD_EXECUTING: // 执行中
        if (HAL_GetTick() - tick > 1000)
        {
            Send_Response(ph, 0x03); // 发送执行完成响应
            ph->cmd_state = CMD_COMPLETED;
        }
        break;

    case CMD_COMPLETED: // 执行完成
        // 可在此添加状态清理逻辑
        break;

    default:
        break;
    }
}