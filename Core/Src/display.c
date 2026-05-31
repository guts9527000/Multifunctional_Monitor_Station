#include "global.h"

void Display_Light_Intensity(void) {
    char test[50] = "0";
    sprintf(test, "光强: %d %%", light_intensity);

    OLED_SetPen(&oled,PEN_COLOR_WHITE,1);
    OLED_SetBrush(&oled,PEN_COLOR_TRANSPARENT);
    OLED_SetFont(&oled, &hyjk8);
    OLED_SetCursor(&oled,10,30);
    OLED_DrawString(&oled, test);

}

void Display_Temperature_Humidity(void) {
    char message1[50] = "0";
    char message2[50] = "0";
    sprintf(message1, "湿度 = %d%%\r\n", (int)DHT11Rcv[0]);
    sprintf(message2, "温度 = %d °C\r\n", (int)DHT11Rcv[2]);

    OLED_SetPen(&oled,PEN_COLOR_WHITE,1);
    OLED_SetBrush(&oled,PEN_COLOR_TRANSPARENT);
    OLED_SetFont(&oled, &hyjk8);

    OLED_SetCursor(&oled, 10, 10);
    OLED_DrawString(&oled, message1);

    OLED_SetCursor(&oled, 10, 20);
    OLED_DrawString(&oled, message2);

}

void Display_Time(void) {
    sprintf(time_str, "%02d：%02d：%02d", hour, minute, second);
    sprintf(date_str, "%04d-%02d-%02d", year, month, day);

    OLED_SetPen(&oled,PEN_COLOR_WHITE,1);
    OLED_SetBrush(&oled,PEN_COLOR_TRANSPARENT);
    OLED_SetFont(&oled, &hyjk8);
    OLED_SetCursor(&oled,10,40);
    OLED_DrawString(&oled, time_str);
    OLED_SetCursor(&oled,10,50);
    OLED_DrawString(&oled, date_str);

}

//把温度映射到Y轴
uint16_t temp_to_y(uint16_t temp) {
    if (temp > 40) temp = 40; //限幅
    return 54 - (uint16_t)(temp * 54 / 40);
}

void Display_LineChat(void) {
    OLED_Clear(&oled);
    OLED_SetPen(&oled,PEN_COLOR_WHITE,1);
    OLED_SetBrush(&oled,PEN_COLOR_TRANSPARENT);
    /* 画Y轴 (竖线) */
    OLED_SetCursor(&oled, 20, 0);
    OLED_LineTo(&oled, 20, 54);
    /* 画X轴 (横线) */
    OLED_SetCursor(&oled, 20, 54);
    OLED_LineTo(&oled, 127, 54);
    /* Y轴标两个值 */
    OLED_SetCursor(&oled, 0, 8);
    OLED_DrawString(&oled, "40");   // 顶部
    OLED_SetCursor(&oled, 0, 54);
    OLED_DrawString(&oled, "0");    // 底部

    uint16_t temp[128];
    uint16_t valid_count = 0;
    for (int i = 0; i < 128; i++) {
        uint16_t t = *(__IO uint16_t*)(FLASH_PAGE_A_ADDR + i * 4);
        if (t != 0xFFFF)  //flash的空值是0xFF
        {
            temp[valid_count] = t;
            valid_count++;
        }
    }

    uint16_t show[5];
    uint8_t show_count = 0;
    if (valid_count >= 5) {
        for (int i = 0; i < 5; i++) {
            show[i] = temp[valid_count - 5 + i];
        }
        show_count = 5;
    } else if (valid_count >= 2) {
        for (int i = 0; i < valid_count; i++) {
            show[i] = temp[i];
        }
        show_count = valid_count;
    }

    //至少两个点画线
    if (show_count >= 2) {
        uint16_t step = 107 / (show_count - 1);
        OLED_SetCursor(&oled, 20, temp_to_y(show[0]));
        for (uint16_t i = 1; i < show_count; i++) {
            OLED_LineTo(&oled, 20 + i * step, temp_to_y(show[i]));
        }
    }
    OLED_SendBuffer(&oled);
}