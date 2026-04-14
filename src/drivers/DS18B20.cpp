#include "DS18B20.h"
#include <OneWire.h>
#include <DallasTemperature.h>

static OneWire* oneWire = nullptr;
static DallasTemperature* sensors = nullptr;
//-----------------------------------------------------
void DS18B20_Init(uint8_t pin)
{
    oneWire = new OneWire(pin);
    sensors = new DallasTemperature(oneWire);
    sensors->begin();
}
//-----------------------------------------------------
bool DS18B20_Read(float &temp)
{
    if (!sensors) return false;

    sensors->requestTemperatures();
    float t = sensors->getTempCByIndex(0);

    if (t == DEVICE_DISCONNECTED_C) return false;

    temp = t;
    return true;
}