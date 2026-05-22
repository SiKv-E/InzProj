#include "display.h"
#include <stdio.h>

void Display_ShowPressure(I2C_LCD_HandleTypeDef *lcd1, int32_t pressure) {
    char buffer[20];
    lcd_clear(lcd1);
    if(pressure < 90000 || pressure > 110000) {
        lcd_puts(lcd1, "Out of range");
    } else {
        sprintf(buffer, "Avg:%ld Pa", pressure);
        lcd_puts(lcd1, buffer);
    }
}
