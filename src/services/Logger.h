#pragma once
#include <Arduino.h>
bool Logger_IsRunning();
void Logger_Start();
void Logger_Stop();
bool Logger_IsRunning();

class Logger
{
public:

    static bool logTemperature(int16_t t);
    static bool logHumidity(int16_t h);
    static bool logPressure(int16_t p);

private:

    static bool logValue(int16_t value, char type);

    static void buildFileName(char *name, char type);
};