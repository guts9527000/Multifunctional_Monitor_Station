
#include "global.h"
#include "display.h"

static void Key_Short(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin) {
    if (GPIOx == GPIOA && GPIO_Pin == GPIO_PIN_1) {
        if (mode == Mode_realtime_display) {
            mode = Mode_linechat;
        }
        else if (mode == Mode_linechat) {
            mode = Mode_realtime_display;
        }
    }
}

static void Key_Long(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin) {
    if (GPIOx == GPIOA && GPIO_Pin == GPIO_PIN_1) {
        mode = Mode_realtime_display;
    }
}

void Key1_Judgement(void) {
    static uint32_t previous = 0;
    static uint8_t debounce = 0;
    static uint8_t long_handled = 0;     // ← 新增

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) {
        if (debounce == 0) {
            previous = HAL_GetTick();
            debounce = 1;
        }
        else if (HAL_GetTick() - previous >= 500) {
            Key_Long(GPIOA, GPIO_PIN_1);
            debounce = 0;
            long_handled = 1;            // ← 标记：长按已处理
        }
    }
    else {
        if (debounce == 1) {
            uint32_t elapsed = HAL_GetTick() - previous;
            if (elapsed >= 20 && elapsed < 500) {
                if (long_handled == 0) { // ← 只有没被长按处理过才触发短按
                    Key_Short(GPIOA, GPIO_PIN_1);
                }
            }
            debounce = 0;
            long_handled = 0;            // ← 复位
        }
    }
}
