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
    0x12345611, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
};


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

    // �� 32-bit Ϊ��λд�루H7 ֧�� 256-bit д�룬�� HAL ������Ϊ 32-bit��
    /*for (uint32_t i = 0; i < words; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr + i*4, (uint64_t)((uint32_t)pData[i]));
        if (status != HAL_OK) break;
    }*/
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr, ((uint32_t)pData));
	
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



