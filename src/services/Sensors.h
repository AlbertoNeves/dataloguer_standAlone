#pragma once

#include <Arduino.h>

void Sensors_init();
bool Read_t_p_h(float &t, float &h, float &p);
void Sensors_Warmup();

// bateria
float Sensors_GetBatteryADC();
uint8_t Sensors_GetBatteryIcon();
//------------------------------------------------
typedef enum
{
    TEMP_SENSOR_BME280 = 0,
    TEMP_SENSOR_DS18B20
} TempSensorType;

extern TempSensorType tempSensorSelected;
//--------------------------------------------