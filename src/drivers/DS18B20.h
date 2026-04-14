#ifndef DS18B20_H
#define DS18B20_H

#include <Arduino.h>

void DS18B20_Init(uint8_t pin);
bool DS18B20_Read(float &temp);

#endif