#include "user_communication.h"
#include <string.h>
#include "main.h"

ProtocolHandle ph; // Э����ʵ��
uint8_t actionStop = 0;
WorkStatus stateRobot;
/**
 * @brief ͨ��ģ���ʼ��
 * @note ����UART��DMA�����ÿ����ж�
 */
void User_CommunicationInit(void)
{
    ph.huart = &huart4;       // UART���
    ph.hdma = &hdma_uart4_rx; // DMA���
    ph.buf_idx = 0;           // ��ʼ������������
    __HAL_UART_ENABLE_IT(ph.huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(ph.huart, ph.rx_buf[ph.buf_idx], MAX_DATA_LEN + 9);
}

void Single_Key_Record(uint8_t *key, uint8_t *last_key, uint8_t *key_downside, uint8_t *key_upside)
{
    // �½��ؼ��
    if ((*last_key == 1) && (*key == 0))
    {
        *key_downside = 1;
    }
    else
    {
        *key_downside = 0;
    }
    // �����ؼ��
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

uint8_t touchTopofHead_Downside, touchTopofHead_Upside, touchTopofHead, Last_touchTopofHead; // ����ͷ��
uint8_t touchChin_Downside, touchChin_Upside, touchChin, Last_touchChin;                     // �����°�
uint8_t humanDetection_Downside, humanDetection_Upside, humanDetection, Last_humanDetection; // �����°�
void Key_Downside_Record(void)
{
    touchTopofHead = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
    touchChin = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
    humanDetection = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
    Single_Key_Record(&touchTopofHead, &Last_touchTopofHead, &touchTopofHead_Downside, &touchTopofHead_Upside);
    Single_Key_Record(&touchChin, &Last_touchChin, &touchChin_Downside, &touchChin_Upside);
    Single_Key_Record(&humanDetection, &Last_humanDetection, &humanDetection_Downside, &humanDetection_Upside);
}

/*------------------------ �������� ------------------------*/

/**
 * @brief ����򵥺�У�飨16λ��
 * @param data ��У������ָ��
 * @param len У�����ݳ��ȣ��ֽ�����
 * @return ����õ���16λУ��ֵ
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

// ====================== ����ģʽ��غ��� ======================
/**
 * @brief ��ȡ��ǰ����ģʽ
 * @return ����ģʽö��ֵ
 */
WorkMode Get_WorkMode(void)
{
	if(ActionNow == IDLE)
    return MODE_IDLE;
	else return MODE_ACTION;
}

/**
 * @brief ��ȡ��Դ��ѹ
 * @return ��ǰ��ѹֵ����λ�����أ�
 */
float Power_GetVoltage(void)
{
    // ��ʱ����0��ѹ
    return USER_ADC.bat_volt;
}

/**
 * @brief ��ȡ��ص����ٷֱ�
 * @return �����ٷֱȣ�0-100��
 */
uint8_t Get_BatteryLevel(void)
{
    // ��ʱ����0%����
    return USER_ADC.bat_power;
}

/**
 * @brief �����״̬
 * @return true:���ڳ�� false:δ���
 */
bool Is_Charging(void)
{
    // ��ʱ����δ���״̬
    return false;
}

/**
 * @brief ��ȡϵͳ����¶�
 * @return ����¶�ֵ����λ�����϶ȣ�
 */
float Get_MaxTemperature(void)
{
    // ��ʱ����0���϶�
    return 0.0f;
}

/**
 * @brief ��ȡ��ǰ�������
 * @return ������루0��ʾ�޾��棩
 */
uint16_t Get_Warning(void)
{
    // ��ʱ�����޾���
    return 0;
}

/**
 * @brief ��ȡ����������
 * @return ������루0��ʾ�޴���
 */
uint16_t Get_LastError(void)
{
    // ��ʱ�����޴���
    return 0;
}

// ====================== ��������غ��� ======================
/**
 * @brief ��ȡ����������ֵ
 * @param touch_type �������ͣ�HEAD/BODY/CHIN��
 * @return ����״̬��0/1 �� ADCԭʼֵ��
 */
uint8_t Get_TouchValue(TouchType touch_type)
{
    // ��ʱ�����޴����ź�
    (void)touch_type; // ����δʹ�ò�������
    return 0;
}

/**
 * @brief ��ȡIMU������
 * @return �����Ƕȣ���λ���ȣ�-180~180��
 */
float IMU_GetRoll(void)
{
    // ��ʱ����0�Ⱥ����
    return 0.0f;
}

/**
 * @brief ��ȡIMU������
 * @return �����Ƕȣ���λ���ȣ�-90~90��
 */
float IMU_GetPitch(void)
{
    // ��ʱ����0�ȸ�����
    return 0.0f;
}

/**
 * @brief ��ȡIMUƫ����
 * @return ƫ���Ƕȣ���λ���ȣ�0~360��
 */
float IMU_GetYaw(void)
{
    // ��ʱ����0��ƫ����
    return 0.0f;
}

/* ����/�������ת������ʾ�� */
/**
 * @brief �������ת�ַ���
 * @param code �������
 * @return �ɶ��ľ�������
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
 * @brief �������ת�ַ���
 * @param code �������
 * @return �ɶ��Ĵ�������
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
 * @brief ���ʹ�����������Ӧ֡
 * @param ph Э����ָ��
 * @param data Ҫ���͵�JSON�����ַ���
 * @param len JSON���ݳ���
 * @note ֡�ṹ��֡ͷ(2B) | ������(1B) | ���ݳ���(2B) | JSON����(NB) | У���(1B) | ֡β(2B)
 */
void Send_Sensor_Data(ProtocolHandle *ph, const char *data, uint16_t len)
{
    // У�����ݳ��ȺϷ���
    if (len > MAX_DATA_LEN)
    {
        return;
    }

    // ������������֡����̬����֡���ȣ�
//    uint8_t tx_buf[2 + 2 + 5 + len]; // ͷ֡β֡4+������1+���ݳ���2+У��1
    uint16_t frame_len = 2+2+5+len;

    // ֡ͷ��2�ֽڣ�
    tx_buf_[0] = FRAME_HEADER_DOWN >> 8;
    tx_buf_[1] = FRAME_HEADER_DOWN & 0xFF;

    // �����루1�ֽڣ�
    tx_buf_[2] = ph->current_cmd;

    // ���ݳ��ȣ�С�˸�ʽ��2�ֽڣ�
    tx_buf_[3] = len & 0xFF;
    tx_buf_[4] = (len >> 8) & 0xFF;

    // ��������
    memcpy(&tx_buf_[5], data, len);

    // �����У�飨������+����+���ݣ�
    uint16_t checksum = Calculate_SumCheck(&tx_buf_[3], len);
    tx_buf_[5 + len] = checksum;

    // ֡β��2�ֽڣ�
    tx_buf_[6 + len] = FRAME_FOOTER_DOWN >> 8;
    tx_buf_[7 + len] = FRAME_FOOTER_DOWN & 0xFF;

    // DMA����
    HAL_UART_Transmit_DMA(ph->huart, tx_buf_, 8 + len);
}

/*------------------------ ���������ȡ���� ------------------------*/
/**
 * @brief ��ȡ���ж������
 * @param params ��������ṹ������
 * @note SERVO[1]-SERVO[12] ��Ӧ params[0]-params[11]
 */
void Servo_GetAllParams(SERVO_INFO_TYPEDEF *params)
{
    // SERVO[1]-SERVO[12] ��Ӧ params[0]-params[11]
    for (int i = 0; i < 12; i++)
    {
        params[i] = SERVO[i + 1]; // ��������ƫ��
    }
}

/**
 * @brief ���ͱ�׼��Ӧ֡
 * @param ph Э����ָ��
 * @param result ��Ӧ����루0��ʾ�ɹ���1��ʾУ��ʧ�ܣ�2��ʾ��ʼִ�У�3��ʾִ����ϣ�
 * @note ֡�ṹ��BB BB | func | 01 00 | result | ��У�� | ++ ++
 */
uint8_t _tx_buf_[10] = {0x42, 0x42, 0, 1, 0, 0, 0, 0, 0x2B, 0x2B};
void Send_Response(ProtocolHandle *ph, uint8_t result)
{
//    uint8_t tx_buf[10] = {
//        FRAME_HEADER_DOWN >> 8, FRAME_HEADER_DOWN & 0xFF, // ֡ͷBB BB
//        ph->current_cmd,                                  // ԭ���ش�������
//        1, 0,                                             // ���ݳ���С�ˣ��̶�1�ֽڣ�
//        result,                                           // �����
//        0, 0,                                             // ��У��ռλ
//        FRAME_FOOTER_DOWN >> 8, FRAME_FOOTER_DOWN & 0xFF  // ֡β++ ++
//    };
	_tx_buf_[2] = ph->current_cmd;
	_tx_buf_[5] = result;
    // ����У��ͣ�������������У�飩
    uint16_t checksum = Calculate_SumCheck(&_tx_buf_[5], 1);
    _tx_buf_[6] = checksum & 0xFF; // У����ֽ���ǰ
    _tx_buf_[7] = checksum >> 8;   // ���ֽ��ں�

    HAL_UART_Transmit_DMA(ph->huart, _tx_buf_, sizeof(_tx_buf_));
}

void sendStateActive(ProtocolHandle *ph, WorkStatus status)
{
	/* ��ȡ����״̬���� */
            status.voltage = Power_GetVoltage();       // ��ѹֵ��float��
            status.battery_level = Get_BatteryLevel(); // �����ٷֱȣ�0~100��
            status.is_charging = Is_Charging();         // ���״̬

        /* ���ɾ����JSON��ʽ״̬���� */
        char json_buf[128]; // �ʵ���С�Ļ�����
        snprintf(json_buf, sizeof(json_buf),
                 "{\"mode\":%d,\"voltage\":%.2f,\"battery\":\"%d%%\",\"charging\":%s}",
                 status.mode,
                 status.voltage,        // %.2f ������λС��
                 status.battery_level,  // 
                 status.is_charging ? "true" : "false");
        
        ph->cmd_state = CMD_RECEIVED;
        
        /* ����״̬���� */
        Send_Sensor_Data(ph, json_buf, strlen(json_buf));
}

/**
 * @brief Э��������߼�
 * @param ph Э����ָ��
 * @note ִ��˳��֡�ṹУ�� -> ��У�� -> ������ַ�
 */
void Parse_Protocol(ProtocolHandle *ph)
{
    ProtocolFrame *frame = (ProtocolFrame *)ph->rx_buf[ph->buf_idx];
    frame->checksum = (frame->data[frame->data_len + 1] << 8) | frame->data[frame->data_len];
    frame->footer = *(uint16_t *)&frame->data[frame->data_len + 2];

    // ���У��ͼ�֡β����
    for (int i = 0; i < 4; i++)
        frame->data[frame->data_len + i] = 0;

    /* �����ṹУ�� */
    if (frame->header != FRAME_HEADER_UP || // ��֤��λ��֡ͷ
        frame->footer != FRAME_FOOTER_UP || // ��֤��λ��֡β
        frame->data_len > MAX_DATA_LEN)     // ���ݳ��ȺϷ��Լ��
    {
        return;
    }

    /* ��У����֤ */
    uint16_t calc_sum = Calculate_SumCheck(frame->data, frame->data_len);
    if (calc_sum != frame->checksum)
    {
        ph->cmd_state = CMD_CHECK_ERROR;  // ����У�����״̬
        Send_Response(ph, ph->cmd_state); // ��ӦУ��ʧ��
        return;
    }

    TEACHMODE = 0;                 // �����ѧģʽ��־�����У�
    ph->current_cmd = frame->func; // ��¼��ǰ�������

    /* ������ַ����� */
    switch (frame->func)
    {
    case 0x01: // ����״̬��������
        if (frame->data_len == 2)
        { // Ԥ��2�ֽ�����
            // ����״̬�ͳ̶�ֵ��ʾ����
            uint8_t state = frame->data[0];
            uint8_t level = frame->data[1];

            // ����״̬��
            ph->cmd_state = CMD_RECEIVED;
            Send_Response(ph, ph->cmd_state); // ������Ӧ���ճɹ�
        }
        break;

    case 0x02: // ���������
    {
        // �״���Ӧ�����ճɹ���
        ph->cmd_state = CMD_RECEIVED;
        Send_Response(ph, ph->cmd_state);

        // ����������ţ���Χ0-255��
        uint8_t action_id = frame->data[0];
		
		if(action_id == 255) 
		{
			actionStop = 1;
		}
//        // ��֤���������Ч�ԣ�ʾ������Ч��Χ0-121��
//        else if (action_id > 150)
//        {
//            ph->cmd_state = CMD_CHECK_ERROR;
//            Send_Response(ph, ph->cmd_state);
//            break;
//        }

		else
		{
			// ���¶������Ʋ���
			actionStop = 0;
			ph->cmd_state = CMD_RECEIVED;
			Send_Response(ph, ph->cmd_state); // �ٴ���Ӧ
			ActionNow = action_id;            // ���õ�ǰ����
		}
        break;
    }

    case 0x03: // �ؽڿ�������
        if (frame->data_len == 24)
        { // 24�ֽڶ�Ӧ12���ؽ�
            // �����ؽڽǶȣ�С�˸�ʽ��
            uint16_t angles[12];
            for (int i = 0; i < 12; i++)
            {
                angles[i] = frame->data[i * 2] | (frame->data[i * 2 + 1] << 8);
            }
            ph->cmd_state = CMD_RECEIVED;     // �����������״̬
            Send_Response(ph, ph->cmd_state); // ��Ӧ���ճɹ�
        }
        break;

    /*------ 0x04: ��������ѯ ------*/
    case 0x04:
    {
        // ����������
        SensorData sensor = {
            .head_touch = Get_TouchValue(TOUCH_HEAD),
            .body_touch = Get_TouchValue(TOUCH_BODY),
            .chin_touch = Get_TouchValue(TOUCH_CHIN),
            .roll = IMU_GetRoll(),
            .pitch = IMU_GetPitch(),
            .yaw = IMU_GetYaw()};

        // ת��ΪJSON�ַ���
        char json_buf[128];
        snprintf(json_buf, sizeof(json_buf),
                 "{\"touch\":[%u,%u,%u],\"pose\":[%.1f,%.1f,%.1f]}",
                 sensor.head_touch, sensor.body_touch, sensor.chin_touch,
                 sensor.roll, sensor.pitch, sensor.yaw);
        ph->cmd_state = CMD_RECEIVED;     // �����������״̬
        Send_Response(ph, ph->cmd_state); // ��Ӧ���ճɹ�
        // ������Ӧ֡
        Send_Sensor_Data(ph, json_buf, strlen(json_buf));
        break;
    }

    /*------ 0x05: ����״̬��ѯ ------*/
	case 0x05:
    {
        /* ��ȡ����״̬���� */
        WorkStatus status = {
            .mode = Get_WorkMode(),              // ����ģʽ
            .voltage = Power_GetVoltage(),       // ��ѹֵ��float��
            .battery_level = Get_BatteryLevel(), // �����ٷֱȣ�0~100��
            .is_charging = Is_Charging()         // ���״̬
        };

        /* ���ɾ����JSON��ʽ״̬���� */
        char json_buf[128]; // �ʵ���С�Ļ�����
        snprintf(json_buf, sizeof(json_buf),
                 "{\"mode\":%d,\"voltage\":%.2f,\"battery\":\"%d%%\",\"charging\":%s}",
                 status.mode,
                 status.voltage,        // %.2f ������λС��
                 status.battery_level,  // 
                 status.is_charging ? "true" : "false");
        
        ph->cmd_state = CMD_RECEIVED;
        
        /* ����״̬���� */
        Send_Sensor_Data(ph, json_buf, strlen(json_buf));
        break;
    }
	

    default:                     // δ֪������
        Send_Response(ph, 0x01); // ��ӦУ��ʧ��
        break;
    }
}

/**
 * @brief ���Ͷ��������Ϣ
 * @note �����ϱ����ж��������JSON�����ʽ
 */
void sendServoParameters(void)
{
    SERVO_INFO_TYPEDEF params[12];
    Servo_GetAllParams(params); // ��ȡ�������

    // ����JSON����
    char json_buf[128];
    char *ptr = json_buf;

    // ��ʼ���
    ptr += sprintf(ptr, "[");

    // �������ж��
    for (int i = 0; i < 12; i++)
    {
        /* ����˵����
           i+1        - ��������ţ�SERVO[1]��Ӧ���1��
           pos_read   - ���λ�ã�int16_t��
           speed_read - ����ٶȣ�uint16_t��
           temper_read- ����¶ȣ�uint16_t�� */
        ptr += sprintf(ptr,
                       "{\"id\":%d,\"pos\":%d,\"speed\":%u,\"temp\":%u}%c",
                       i + 1,                 // ��������Ŵ�1��ʼ
                       params[i].pos_read,    // λ�ò���
                       params[i].speed_read,  // �ٶȲ���
                       params[i].temper_read, // �¶Ȳ���
                       (i == 11) ? ']' : ','  // ����պϱ��
        );
    }
    ph.current_cmd = 0x06; // ���õ�ǰ�������
    // ���������ϱ���������У�飩
    Send_Sensor_Data(&ph, json_buf, ptr - json_buf);
}

/**
 * @brief �����жϴ�����
 * @note ��Ӳ���жϴ��������DMA���չ���
 */
void User_Communication_IRQHandler(void)
{
    if (__HAL_UART_GET_FLAG(ph.huart, UART_FLAG_IDLE))
    {
        // ��������жϱ�־
        __HAL_UART_CLEAR_IDLEFLAG(ph.huart);

        // ֹͣ��ǰDMA����
        HAL_UART_DMAStop(ph.huart);

        // ����ʵ�ʽ��ճ���
        uint16_t len = MAX_DATA_LEN + 9 - __HAL_DMA_GET_COUNTER(ph.hdma);

        // �л�����������������
        memcpy(ph.rx_buf[ph.buf_idx ^ 1], ph.rx_buf[ph.buf_idx], len);
        ph.buf_idx ^= 1; // �л���������

        // �ύЭ���������
        Parse_Protocol(&ph);

        // ����DMA���գ�ʹ�õ�ǰ���������
        HAL_UART_Receive_DMA(ph.huart, ph.rx_buf[ph.buf_idx], MAX_DATA_LEN + 9);
    }
}

/**
 * @brief ����״̬��������ѭ�����ã�
 * @param ph Э����ָ��
 * @note �����׶���Ӧ������->ִ��->��ɣ�
 */
void Process_Cmd_State(ProtocolHandle *ph)
{
    static uint32_t tick = 0; // ��ʱ��׼

    switch (ph->cmd_state)
    {
    case CMD_RECEIVED: // �ѽ��մ�ִ��
        if (HAL_GetTick() - tick > 100)
        {
            Send_Response(ph, 0x02); // ���Ϳ�ʼִ����Ӧ
            ph->cmd_state = CMD_EXECUTING;
            tick = HAL_GetTick(); // ���ü�ʱ
        }
        break;

    case CMD_EXECUTING: // ִ����
        if (HAL_GetTick() - tick > 1000)
        {
            Send_Response(ph, 0x03); // ����ִ�������Ӧ
            ph->cmd_state = CMD_COMPLETED;
        }
        break;

    case CMD_COMPLETED: // ִ�����
        // ���ڴ����״̬�����߼�
        break;

    default:
        break;
    }
}