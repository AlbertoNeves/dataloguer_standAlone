#include <Arduino.h>
#include "Sensors.h"
#include <Wire.h>
#include <Adafruit_BME280.h>

#define BME_POWER 12

Adafruit_BME280 bme;

void Sensors::init()
{
    pinMode(BME_POWER, OUTPUT);

    // desliga sensor inicialmente (PNP)
    digitalWrite(BME_POWER, HIGH);

    Wire.begin();
}

bool Sensors::read(float &t, float &h, float &p)
{
    // liga sensor
    digitalWrite(BME_POWER, LOW);
    delay(10);

    if (!bme.begin(0x76))
    {
        digitalWrite(BME_POWER, HIGH);
        return false;
    }

    t = bme.readTemperature();
    h = bme.readHumidity();
    p = bme.readPressure() / 100.0f;

    // desliga sensor
    digitalWrite(BME_POWER, HIGH);

    return true;
}

int16_t Sensors::getTemperature_x10()
{
    float t, h, p;

    if (!read(t, h, p))
        return -32768;

    return (int16_t)(t * 10.0f);
}