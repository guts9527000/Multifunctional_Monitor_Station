#include "global.h"

void HAL_Delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000UL);
    while ((DWT->CYCCNT - start) < ticks);
}

void Clock_Timing(void) {
    if (HAL_GetTick() - last_time >= 1000) {
        second++;
        if (second >= 60) {
            second = 0;
            minute++;
            if (minute >= 60) {
                minute = 0;
                hour++;
            }
        }
        if (hour >= 24) {
            hour = 0;
            day++;
            switch (month) {
                case 1:case 3:case 5:case 7:case 8:case 10:case 12:
                    if (day >= 31) {
                        day = 1;
                        month++;
                    }
                    break;
                case 2:
                    if (year % 4 == 0 && year % 100 != 0) {
                        if (day >= 29) {
                            day = 1;
                            month++;
                        }
                        break;
                    }
                    else {
                        if (day >= 28) {
                            day = 1;
                            month++;
                        }
                        break;
                    }
                    break;
                case 4:case 6:case 9:case 11:
                    if (day >= 30) {
                        day = 1;
                        month++;
                    }
                    break;
                default:break;
            }
        }
        last_time = HAL_GetTick();
    }
}