#ifndef DISPLAY_H
#define DISPLAY_H

#include "i2c_lcd.h"
#include <stdint.h>

void Display_ShowPressure(I2C_LCD_HandleTypeDef *lcd, int32_t pressure);

void Display_ShowStatus(I2C_LCD_HandleTypeDef *lcd, char *message);

#endif
