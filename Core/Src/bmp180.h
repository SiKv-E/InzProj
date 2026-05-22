#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stm32h7xx_hal.h"

#define BMP180_ADDR (0x77 << 1)

typedef struct
{
    I2C_HandleTypeDef *i2c;

    int16_t AC1, AC2, AC3;
    uint16_t AC4, AC5, AC6;
    int16_t B1, B2, MB, MC, MD;

} BMP180_HandleTypedef;

HAL_StatusTypeDef BMP180_Init(BMP180_HandleTypedef *bmp, I2C_HandleTypeDef *i2c);

float BMP180_ReadTemperature(BMP180_HandleTypedef *bmp);
int32_t BMP180_ReadPressure(BMP180_HandleTypedef *bmp);

#endif