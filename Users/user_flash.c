#include "user_flash.h"
#include "cmsis_os.h" 
#include "user_servo.h"
#include "user_timer.h"
#include "user_states.h"
#include "string.h"

#define DATA_32                 ((uint32_t)0x12345678)
/* Ҫ�����ڲ�FLASH����ʼ��ַ */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08020000)
/* Ҫ�����ڲ�FLASH�Ľ�����ַ */
#define FLASH_USER_END_ADDR     ((uint32_t)0x0803FFFF)

#define TEST_SECTOR             1
#define TEST_SECTOR_ADDR        ((uint32_t)0x08020000)

HAL_StatusTypeDef status_erase;
HAL_StatusTypeDef status_write;
uint8_t Len = 0;
const uint32_t testData[8] = {
    0x12345611, 0x341212, 0x561212, 0x7845, 0x9A, 0xBC, 0xDE, 0xF0,
};


//����flash
HAL_StatusTypeDef Flash_EraseSector(uint32_t sector, uint32_t bank) {
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t sectorError = 0;

    /* ���� Flash */
    HAL_FLASH_Unlock();

    /* ������д����־ */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);

    /* ���ò���������STM32H7 ʹ�� Sector ������ */
    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInit.Banks = bank;           // ѡ�� Bank1 �� Bank2
    eraseInit.Sector = sector;                // �����ţ�0~7 �� 0~15��ȡ�����ͺţ�
    eraseInit.NbSectors = 1;                  // ���� 1 ������
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3; // ��ѹ��Χ��H7 �������ã�

    /* ִ�в��� */
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&eraseInit, &sectorError);

    /* ���� Flash */
    HAL_FLASH_Lock();
	
	return status;
}


/**
 * @brief д�����ݵ� Flash
 * @param addr: ��ʼ��ַ�����Ѳ�����
 * @param data: ����ָ��
 * @param len: ���ݳ��ȣ��ֽڣ�
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef Flash_Write(uint32_t addr, uint32_t *data, uint32_t len) {
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t *pData = (uint32_t*)data;
    //uint32_t words = 2; // ת��Ϊ 32-bit ��

    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);

    // �� 32-bit Ϊ��λд�루H7 ֧�� 256-bit д�룬�� HAL ������Ϊ 32-bit��
    for (uint32_t i = 0; i < len; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr + i*4, (uint64_t)((uint32_t)pData[i]));
        if (status != HAL_OK) break;
    }
	//status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr, ((uint32_t)pData));
	
    HAL_FLASH_Lock();
    return status;
}

/**
 * @brief ��֤ Flash ����
 * @param addr: ��ʼ��ַ
 * @param data: Ԥ������
 * @param len: ���ݳ��ȣ��ֽڣ�
 * @return HAL_StatusTypeDef
 */
uint8_t flashData[64] = {0}; 
void Flash_Verify(uint32_t addr) {
    memcpy(flashData, (void*)addr, 32);
}

/*
	//ʹ������
	status_erase = Flash_EraseSector(FLASH_SECTOR_1, FLASH_BANK_1);
	Len = sizeof(testData);
	status_write = Flash_Write(TEST_SECTOR_ADDR, (uint32_t*)testData, sizeof(testData));
	Flash_Verify(TEST_SECTOR_ADDR);
*/

/**
 * @brief 
 * @param 
 * @param 
 * @param 
 * @return 
 */

//ʾ��mode��35������ϵ�ж�����д�뺯��
//HAL_StatusTypeDef Flash_Write_User_Action(uint32_t addr, Motion_t motion_t) {
//    HAL_StatusTypeDef status = HAL_OK;
//    int16_t pData[35][14] = {0}; // 35 ��������ÿ������ 14 ������Ƕ�
//    //uint32_t words = 2; // ת��Ϊ 32-bit ��

//    HAL_FLASH_Unlock();
//    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
//	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);

//    // �� 32-bit Ϊ��λд�루H7 ֧�� 256-bit д�룬�� HAL ������Ϊ 32-bit��4bytes = 32bits
//    for (uint32_t i = 0; i < 35; i++) {
//        for(uint32_t j = 0; j < 14; j++) //14*2=28
//        {
//            pData[i][j] = motion_t.motion[0].actions[i].servoAngles[j];//�浥������14������ĽǶ�
//        }
//        for(uint32_t h = 0; h < 7; h++)
//        {
//            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr + (i*14*2 + h*4), *(uint32_t*)&pData[i][h*4]);//д�����ݵ�flash
//            if (status != HAL_OK) break;
//        }
//    }
//    HAL_FLASH_Lock();
//    return status;
//}

HAL_StatusTypeDef Flash_Write_User_Action(uint32_t addr, Motion_t motion_t) {
    // 1. ����ַ�Ƿ�32�ֽڶ��루�������㣩
    if ((addr % 32) != 0) {
        return HAL_ERROR;
    }

    // 2. �����������ݻ���������32�ֽڶ��룩
    uint32_t flash_buffer[8]; // 32�ֽ� = 8x uint32_t
    HAL_StatusTypeDef status = HAL_OK;

    // 3. ����Flash����������־
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1 | FLASH_FLAG_ALL_ERRORS_BANK2);

    // 4. ��FLASHWORD��32�ֽڣ�д��
    for (uint32_t i = 0; i < 35; i++) {
        // 4.1 ��14��int16_t�Ƕ�ֵ�����flash_buffer��ע���ֽ���
        for (uint32_t j = 0; j < 14; j++) {
            // ��int16_t����uint32_t�ĵ�16λ����16λ��0��
            if (j % 2 == 0) {
                flash_buffer[j / 2] = (uint32_t)motion_t.motion[0].actions[i].servoAngles[j];
            } else {
                flash_buffer[j / 2] |= (uint32_t)motion_t.motion[0].actions[i].servoAngles[j] << 16;
            }
        }

        // 4.2 ���ʣ���flash_buffer��14��int16_t = 28�ֽڣ��貹4�ֽڶ��뵽32�ֽڣ�
        flash_buffer[7] = 0; // ��ѡ�����Ϊ0������Ĭ��ֵ

        // 4.3 д��32�ֽڵ�Flash��FLASHWORDģʽ��
        status = HAL_FLASH_Program(
            FLASH_TYPEPROGRAM_FLASHWORD,
            addr + (i * 32),           // ��ַ��32�ֽڵ���
            (uint64_t)(uint32_t)flash_buffer // ���ݵ�ַ
        );
        if (status != HAL_OK) break;
    }

    // 5. ��������Flash
    HAL_FLASH_Lock();
    return status;
}



/*
Sector7 :0x081e 0000---0x081f ffff  128k
*/
#define ALIGN_256BIT(address) (((address) & ~0x1F))

// ��Flashд�����ݵĺ���
HAL_StatusTypeDef write_to_flash(uint32_t original_address, uint32_t data) {
    HAL_StatusTypeDef status = HAL_OK;
    // ����256λ����ĵ�ַ
    uint32_t aligned_address = ALIGN_256BIT(original_address);
    // ����ƫ����
    uint32_t offset = original_address - aligned_address;

    // ����Flash
    HAL_FLASH_Unlock();

    // ��ȡ�����ַ����ԭ�����ݣ�Ϊ�˲��ƻ������������ݣ�
    uint32_t buffer[8];
    for (int i = 0; i < 8; i++) {
        buffer[i] = *(volatile uint32_t *)(aligned_address + i * 4);
    }

    // ����ƫ�������»������е�����
    int index = offset / 4;
    buffer[index] = data;

    // �����º������д��Flash
    for (int i = 0; i < 8; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, aligned_address + i * 4, buffer[i]);
        if (status != HAL_OK) {
            HAL_FLASH_Lock();
            return status;
        }
    }
    // ����Flash
    HAL_FLASH_Lock();
    return status;
}

//�Ȱ�flash���ݶ�������д�õģ�����flash�Ķ�д,һ������128k
int16_t dataStore[128][512];//�ݶ�128������������ÿ����������1k, 1024/14/2 = 64

void sroreInit(void)
{
	if(*(uint32_t *)(0x081ffffc) != 0xabababab) //��һ��ʹ�ó�ʼ������������Ϊ0
	{
		Flash_EraseSector(FLASH_SECTOR_7, FLASH_BANK_2);//����bank2sector7�����ݣ�128k
        write_to_flash(0x081ffffc, 0xabababab);//д��ǵ�һ��ʹ�ñ�־
        for(int i = 0; i < 128; i++)
        {
            for(int j = 0; j < 512; j++)
            {
                dataStore[i][j] = 0;
            }
        }
	}
}

