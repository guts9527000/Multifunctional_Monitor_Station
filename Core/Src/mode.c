#include "global.h"

Mode_name mode = Mode_realtime_display;

void Mode_realtime_disp(void) {
    Key1_Judgement();
    DHT11_ReadData();
    static uint32_t last = 0;
    if (HAL_GetTick() - last >= 10000) {
        last = HAL_GetTick();
        Flash_Write();
    }
    OLED_Clear(&oled);
    Display_Temperature_Humidity();
    Clock_Timing();
    Display_Time();
    Obtain_Light_Intensity();
    Display_Light_Intensity();
    OLED_SendBuffer(&oled);
}

void Mode_line_chart(void) {
    Key1_Judgement();
    static uint8_t first = 1;
    if (first) {
        first = 0;
        Flash_Init();
    }
    DHT11_ReadData();
    static uint32_t last = 0;
    if (HAL_GetTick() - last >= 10000) {
        last = HAL_GetTick();
        Flash_Write();
    }
    Display_LineChat();
}

Mode Mode_table[] = {
    {Mode_realtime_display, Mode_realtime_disp},
    {Mode_linechat, Mode_line_chart}
};

const uint16_t Mode_Count = sizeof(Mode_table) / sizeof(Mode);

void Mode_Process(void) {
    for (uint16_t i = 0; i < Mode_Count; i++) {
        if (mode == Mode_table[i].mode) {
            Mode_table[i].handler();
        }
    }
}