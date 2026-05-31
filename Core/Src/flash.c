#include "global.h"

/* 写指针：当前该写第几条了（0~127） */
static uint16_t write_index = 0;
/* 一共写了几条（上电扫描时确定，用于读取历史） */
uint16_t record_count = 0;

/* 擦除整页 */
static void Flash_ErasePage(void)
{
    FLASH_EraseInitTypeDef erase = {0};
    uint32_t error_addr;

    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.PageAddress = FLASH_PAGE_A_ADDR;
    erase.NbPages = 1;

    HAL_FLASH_Unlock();
    __disable_irq();
    HAL_FLASHEx_Erase(&erase, &error_addr);
    __enable_irq();
    HAL_FLASH_Lock();
}

/*
 * 上电初始化：扫描 Flash，找到上次写到了哪里
 * 如果页满了（128条），就擦掉从头开始
 */
void Flash_Init(void)
{
    write_index = 0;

    for (uint16_t i = 0; i < MAX_RECORDS; i++)
    {
        uint32_t addr = FLASH_PAGE_A_ADDR + i * sizeof(Flash_record);
        uint16_t val = *(__IO uint16_t *)addr;
        if (val == 0xFFFF)  // Flash 空值，表示还没写到这里
        {
            write_index = i;
            break;
        }
    }

    record_count = write_index;

    /* 如果页已经写满了，擦掉重新开始 */
    if (write_index >= MAX_RECORDS)
    {
        Flash_ErasePage();
        write_index = 0;
        record_count = 0;
    }
}

/*
 * 写一条温湿度记录（每 10 秒调一次）
 */
void Flash_Write(void)
{
    Flash_record r;
    r.temperature = (uint16_t)(int8_t)DHT11Rcv[2];
    r.humidity    = (uint16_t)DHT11Rcv[0];

    uint32_t addr = FLASH_PAGE_A_ADDR + write_index * sizeof(Flash_record);

    HAL_FLASH_Unlock();
    __disable_irq();

    /* 先写温度（2 字节） */
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, r.temperature);
    /* 再写湿度（2 字节） */
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + 2, r.humidity);

    __enable_irq();
    HAL_FLASH_Lock();

    write_index++;
    record_count++;

    /* 写满了 → 擦掉重来 */
    if (write_index >= MAX_RECORDS)
    {
        Flash_ErasePage();
        write_index = 0;
        record_count = 0;
    }
}
