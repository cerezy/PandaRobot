#include "user_flash.h"
#include "cmsis_os.h" 
#include "user_servo.h"
#include "user_timer.h"
#include "user_states.h"
#include "string.h"

#define DATA_32                 ((uint32_t)0x12345678)
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08020000)
/* 要擦除内部FLASH的结束地址 */
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

    /* 解锁 Flash */
    HAL_FLASH_Unlock();

    /* 清除所有错误标志 */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);

    /* 配置擦除参数（STM32H7 使用 Sector 擦除） */
    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInit.Banks = bank;           // 选择 Bank1 或 Bank2
    eraseInit.Sector = sector;                // 扇区号（0~7 或 0~15，取决于型号）
    eraseInit.NbSectors = 1;                  // 擦除 1 个扇区
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3; // 电压范围（H7 必须设置）

    /* 执行擦除 */
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&eraseInit, &sectorError);

    /* 锁定 Flash */
    HAL_FLASH_Lock();
	
	return status;
}


/**
 * @brief 写入数据到 Flash
 * @param addr: 起始地址（需已擦除）
 * @param data: 数据指针
 * @param len: 数据长度（字节）
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef Flash_Write(uint32_t addr, uint32_t *data, uint32_t len) {
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t *pData = (uint32_t*)data;
    //uint32_t words = 2; // 转换为 32-bit 字

    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);

    // 以 32-bit 为单位写入（H7 支持 256-bit 写入，但 HAL 库限制为 32-bit）
    /*for (uint32_t i = 0; i < words; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr + i*4, (uint64_t)((uint32_t)pData[i]));
        if (status != HAL_OK) break;
    }*/
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr, ((uint32_t)pData));
	
    HAL_FLASH_Lock();
    return status;
}

/**
 * @brief 验证 Flash 数据
 * @param addr: 起始地址
 * @param data: 预期数据
 * @param len: 数据长度（字节）
 * @return HAL_StatusTypeDef
 */
uint8_t flashData[64] = {0}; 
void Flash_Verify(uint32_t addr) {
    memcpy(flashData, (void*)addr, 32);
}

/*
	//使用样例
	status_erase = Flash_EraseSector(FLASH_SECTOR_1, FLASH_BANK_1);
	Len = sizeof(testData);
	status_write = Flash_Write(TEST_SECTOR_ADDR, (uint32_t*)testData, sizeof(testData));
	Flash_Verify(TEST_SECTOR_ADDR);
*/



