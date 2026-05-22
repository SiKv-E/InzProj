#include "logika.h"

int32_t Calculate_Avg_Pressure(BMP180_HandleTypedef *bmp) {
    int64_t sum = 0;
    for(int i = 0; i < 10; i++) {
        sum += BMP180_ReadPressure(bmp);
        HAL_Delay(50);
    }
    return (int32_t)(sum / 10);
}
