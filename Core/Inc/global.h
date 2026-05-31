//
// Created by 24547 on 2026/5/28.
//

#ifndef MULTIFUNCTIONAL_MONITOR_STATION_GLOBAL_H
#define MULTIFUNCTIONAL_MONITOR_STATION_GLOBAL_H

#include "main.h"
#include "oled.h"
#include "hyjk8.h"
#include "DHT11.h"
#include "tim.h"
#include "light.h"
#include "display.h"
#include "flash.h"
#include "mode.h"
#include "key.h"

#include <stdio.h>

extern OLED_TypeDef oled;
extern OLED_InitTypeDef OLED_InitStruct;
extern uint8_t DHT11Rcv[4];
extern int light_intensity;
extern uint16_t year;
extern uint8_t month;
extern uint8_t day;
extern uint8_t hour;
extern uint8_t minute;
extern uint8_t second;
extern uint32_t last_time;
extern char time_str[16];
extern char date_str[16];

void HAL_Delay_us(uint32_t us);
void Clock_Timing(void);

#endif //MULTIFUNCTIONAL_MONITOR_STATION_GLOBAL_H
