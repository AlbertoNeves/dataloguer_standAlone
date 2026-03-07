#pragma once

#include <Arduino.h>

class Sensors
{
public:
    static void init();

    static bool read(float &t, float &h, float &p);

    static int16_t getTemperature_x10();
};
