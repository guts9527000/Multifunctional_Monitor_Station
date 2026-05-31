#include "global.h"

//启动函数
void DHT11_Start(void) {
    //拉高进入空闲状态
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_Delay(2);  //稍微延时确保稳定

    //拉低开始发送信号
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_Delay(25); //确保18ms以上

    //再次拉高
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_Delay_us(30);   //持续24~40us
}

uint8_t DHT11_Rec_Byte(void) {
    uint8_t i;
    uint8_t dat = 0;

    for (i = 0; i < 8; i++) {
        //等待50us低电平
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET);
        HAL_Delay_us(30);  //延时26~28us

        dat <<= 1;
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET) {
            dat |= 1;
        }
        //等待拉低判断下一位数据
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET);
    }
    return dat;
}

void DHT11_ReadData(void) {
    uint8_t R_H,R_L,T_H,T_L,RT_CHECK;

    //1.发送开始信号
    DHT11_Start();

    //DHT11相应后会拉低电平80us
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) {
        //2.等待80us的低高电平结束
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET);
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET);

        //3.接受40位数据
        R_H = DHT11_Rec_Byte();  //十进制2位
        R_L = DHT11_Rec_Byte();  //0
        T_H = DHT11_Rec_Byte();  //十进制2位
        T_L = DHT11_Rec_Byte();  //十进制1位
        RT_CHECK = DHT11_Rec_Byte();
        //等待结束信号结束，不小于50us
        HAL_Delay_us(60);
        if ((R_H + R_L + T_H + T_L) == RT_CHECK) {
            DHT11Rcv[0] = R_H;
            DHT11Rcv[1] = R_L;
            DHT11Rcv[2] = T_H;
            DHT11Rcv[3] = T_L;
        }
    }
}


