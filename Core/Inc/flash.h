//
// Created by 24547 on 2026/5/29.
//

#ifndef MULTIFUNCTIONAL_MONITOR_STATION_FLASH_H
#define MULTIFUNCTIONAL_MONITOR_STATION_FLASH_H

#include "stm32f1xx_hal.h"

#define FLASH_PAGE_A_ADDR 0x0800F000  //第60页
#define FLASH_PAGE_B_ADDR 0x0800F400  //第61页
#define FLASH_SIZE 1024 //1Kb
#define MAX_RECORDS 128

void Flash_Init(void);
void Flash_Write(void);
extern uint16_t record_count;   /* 当前一共存了多少条 */

#pragma pack(1)
typedef struct {
    uint16_t temperature;
    uint16_t humidity;
} Flash_record;
#pragma pack()

#endif //MULTIFUNCTIONAL_MONITOR_STATION_FLASH_H
