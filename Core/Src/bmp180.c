#include "bmp180.h"

static int32_t B5;

static void BMP180_ReadCalibration(BMP180_HandleTypedef *bmp)
{
    uint8_t data[22];

    HAL_I2C_Mem_Read(bmp->i2c, BMP180_ADDR, 0xAA, 1, data, 22, HAL_MAX_DELAY);

    bmp->AC1 = (data[0] << 8) | data[1];
    bmp->AC2 = (data[2] << 8) | data[3];
    bmp->AC3 = (data[4] << 8) | data[5];
    bmp->AC4 = (data[6] << 8) | data[7];
    bmp->AC5 = (data[8] << 8) | data[9];
    bmp->AC6 = (data[10] << 8) | data[11];
    bmp->B1  = (data[12] << 8) | data[13];
    bmp->B2  = (data[14] << 8) | data[15];
    bmp->MB  = (data[16] << 8) | data[17];
    bmp->MC  = (data[18] << 8) | data[19];
    bmp->MD  = (data[20] << 8) | data[21];
}

HAL_StatusTypeDef BMP180_Init(BMP180_HandleTypedef *bmp, I2C_HandleTypeDef *i2c)
{
    bmp->i2c = i2c;

    uint8_t id;

    if(HAL_I2C_Mem_Read(i2c, BMP180_ADDR, 0xD0, 1, &id, 1, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    BMP180_ReadCalibration(bmp);

    return HAL_OK;
}

float BMP180_ReadTemperature(BMP180_HandleTypedef *bmp)
{
    uint8_t cmd = 0x2E;
    uint8_t data[2];

    HAL_I2C_Mem_Write(bmp->i2c, BMP180_ADDR, 0xF4, 1, &cmd, 1, HAL_MAX_DELAY);

    HAL_Delay(5);

    HAL_I2C_Mem_Read(bmp->i2c, BMP180_ADDR, 0xF6, 1, data, 2, HAL_MAX_DELAY);

    int32_t UT = (data[0] << 8) | data[1];

    int32_t X1 = ((UT - bmp->AC6) * bmp->AC5) >> 15;
    int32_t X2 = (bmp->MC << 11) / (X1 + bmp->MD);

    B5 = X1 + X2;

    float temp = ((B5 + 8) >> 4) / 10.0;

    return temp;
}

int32_t BMP180_ReadPressure(BMP180_HandleTypedef *bmp)
{
    BMP180_ReadTemperature(bmp);

    uint8_t cmd = 0x34;
    uint8_t data[3];

    HAL_I2C_Mem_Write(bmp->i2c, BMP180_ADDR, 0xF4, 1, &cmd, 1, HAL_MAX_DELAY);

    HAL_Delay(8);

    HAL_I2C_Mem_Read(bmp->i2c, BMP180_ADDR, 0xF6, 1, data, 3, HAL_MAX_DELAY);

    int32_t UP = (((int32_t)data[0] << 16) | ((int32_t)data[1] << 8) | data[2]) >> 8;

    int32_t B6 = B5 - 4000;

    int32_t X1 = (bmp->B2 * ((B6 * B6) >> 12)) >> 11;
    int32_t X2 = (bmp->AC2 * B6) >> 11;
    int32_t X3 = X1 + X2;

    int32_t B3 = ((((int32_t)bmp->AC1 * 4 + X3)) + 2) / 4;

    X1 = (bmp->AC3 * B6) >> 13;
    X2 = (bmp->B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;

    uint32_t B4 = (bmp->AC4 * (uint32_t)(X3 + 32768)) >> 15;
    uint32_t B7 = ((uint32_t)UP - B3) * 50000;

    int32_t p;

    if(B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    p = p + ((X1 + X2 + 3791) >> 4);

    return p;
}
