#include "global.h"
#include "light.h"
#include "adc.h"

void Obtain_Light_Intensity(void) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint32_t adc_data = HAL_ADC_GetValue(&hadc1);
    //计算电压
    float voltage = adc_data * (3.3f - 0.0f) / 4095.0f;
    //计算光强度
    light_intensity = (3.3f - voltage) / 3.3f * 100.0f;

}