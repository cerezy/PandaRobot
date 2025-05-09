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
    0x12345611, 0x341212, 0x561212, 0x7845, 0x9A, 0xBC, 0xDE, 0xF0,
};


//擦除flash
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
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);

    // 以 32-bit 为单位写入（H7 支持 256-bit 写入，但 HAL 库限制为 32-bit）
    for (uint32_t i = 0; i < len; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr + i*4, (uint64_t)((uint32_t)pData[i]));
        if (status != HAL_OK) break;
    }
	//status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr, ((uint32_t)pData));
	
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

/**
 * @brief 
 * @param 
 * @param 
 * @param 
 * @return 
 */

//示教mode下35总数的系列动作的写入函数
//HAL_StatusTypeDef Flash_Write_User_Action(uint32_t addr, Motion_t motion_t) {
//    HAL_StatusTypeDef status = HAL_OK;
//    int16_t pData[35][14] = {0}; // 35 个动作，每个动作 14 个舵机角度
//    //uint32_t words = 2; // 转换为 32-bit 字

//    HAL_FLASH_Unlock();
//    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
//	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);

//    // 以 32-bit 为单位写入（H7 支持 256-bit 写入，但 HAL 库限制为 32-bit）4bytes = 32bits
//    for (uint32_t i = 0; i < 35; i++) {
//        for(uint32_t j = 0; j < 14; j++) //14*2=28
//        {
//            pData[i][j] = motion_t.motion[0].actions[i].servoAngles[j];//存单个动作14个舵机的角度
//        }
//        for(uint32_t h = 0; h < 7; h++)
//        {
//            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, addr + (i*14*2 + h*4), *(uint32_t*)&pData[i][h*4]);//写入数据到flash
//            if (status != HAL_OK) break;
//        }
//    }
//    HAL_FLASH_Lock();
//    return status;
//}

HAL_StatusTypeDef Flash_Write_User_Action(uint32_t addr, Motion_t motion_t) {
    // 1. 检查地址是否32字节对齐（必须满足）
    if ((addr % 32) != 0) {
        return HAL_ERROR;
    }

    // 2. 定义对齐的数据缓冲区（按32字节对齐）
    uint32_t flash_buffer[8]; // 32字节 = 8x uint32_t
    HAL_StatusTypeDef status = HAL_OK;

    // 3. 解锁Flash并清除错误标志
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1 | FLASH_FLAG_ALL_ERRORS_BANK2);

    // 4. 按FLASHWORD（32字节）写入
    for (uint32_t i = 0; i < 35; i++) {
        // 4.1 将14个int16_t角度值打包到flash_buffer（注意字节序）
        for (uint32_t j = 0; j < 14; j++) {
            // 将int16_t存入uint32_t的低16位（高16位填0）
            if (j % 2 == 0) {
                flash_buffer[j / 2] = (uint32_t)motion_t.motion[0].actions[i].servoAngles[j];
            } else {
                flash_buffer[j / 2] |= (uint32_t)motion_t.motion[0].actions[i].servoAngles[j] << 16;
            }
        }

        // 4.2 填充剩余的flash_buffer（14个int16_t = 28字节，需补4字节对齐到32字节）
        flash_buffer[7] = 0; // 可选：填充为0或其他默认值

        // 4.3 写入32字节到Flash（FLASHWORD模式）
        status = HAL_FLASH_Program(
            FLASH_TYPEPROGRAM_FLASHWORD,
            addr + (i * 32),           // 地址按32字节递增
            (uint64_t)(uint32_t)flash_buffer // 数据地址
        );
        if (status != HAL_OK) break;
    }

    // 5. 重新上锁Flash
    HAL_FLASH_Lock();
    return status;
}



/*
Sector7 :0x081e 0000---0x081f ffff  128k
*/
#define ALIGN_256BIT(address) (((address) & ~0x1F))

// 向Flash写入数据的函数
HAL_StatusTypeDef write_to_flash(uint32_t original_address, uint32_t data) {
    HAL_StatusTypeDef status = HAL_OK;
    // 计算256位对齐的地址
    uint32_t aligned_address = ALIGN_256BIT(original_address);
    // 计算偏移量
    uint32_t offset = original_address - aligned_address;

    // 解锁Flash
    HAL_FLASH_Unlock();

    // 读取对齐地址处的原有数据（为了不破坏其他部分数据）
    uint32_t buffer[8];
    for (int i = 0; i < 8; i++) {
        buffer[i] = *(volatile uint32_t *)(aligned_address + i * 4);
    }

    // 根据偏移量更新缓冲区中的数据
    int index = offset / 4;
    buffer[index] = data;

    // 将更新后的数据写回Flash
    for (int i = 0; i < 8; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, aligned_address + i * 4, buffer[i]);
        if (status != HAL_OK) {
            HAL_FLASH_Lock();
            return status;
        }
    }
    // 锁定Flash
    HAL_FLASH_Lock();
    return status;
}

//先把flash数据读出来再写用的，方便flash的读写,一个扇区128k
int16_t dataStore[128][512];//暂定128个单个动作，每个动作分配1k, 1024/14/2 = 64

void sroreInit(void)
{
	if(*(uint32_t *)(0x081ffffc) != 0xabababab) //第一次使用初始化七扇区内容为0
	{
		Flash_EraseSector(FLASH_SECTOR_7, FLASH_BANK_2);//擦除bank2sector7的数据，128k
        write_to_flash(0x081ffffc, 0xabababab);//写入非第一次使用标志
        for(int i = 0; i < 128; i++)
        {
            for(int j = 0; j < 512; j++)
            {
                dataStore[i][j] = 0;
            }
        }
	}
}

