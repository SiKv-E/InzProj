#ifndef Logika
#define Logika
#include "bmp180.h"

typedef struct {
    int32_t last_avg_pressure;
    uint8_t is_error;
} SensorData_t;

int32_t Calculate_Avg_Pressure(BMP180_HandleTypedef *bmp);
#endif
