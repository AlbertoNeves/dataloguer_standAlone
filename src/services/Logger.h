#pragma once
#include <Arduino.h>

bool Logger_IsRunning();
void Logger_Start();
void Logger_Stop();
void Logger_Init();

class Logger
{
public:
    static bool logTemperature(int16_t t);
    static bool logHumidity(int16_t h);
    static bool logPressure(int16_t p);
    static bool logBattery(uint16_t centivolts);
    static bool logBatterySeparator();
    static bool rotateBatteryFile();
    static bool prepareDailySensorFiles(int16_t t, int16_t h, int16_t p, uint16_t samplesToFill);

private:
    static bool logValue(int16_t value, char type);
    static void buildFileName(char *name, char type);
    static bool prepareDailySensorFile(int16_t value, char type, uint16_t samplesToFill);
};
