#include <Arduino.h>
#include "Sensors.h"
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "services/CalibrationService.h"
#include "drivers/DS18B20.h"

#define BME_POWER 12
#define DS18B20_PIN 15

//TempSensorType tempSensorSelected = TEMP_SENSOR_DS18B20;
TempSensorType tempSensorSelected = TEMP_SENSOR_BME280;

// bateria
#define BATTERY_PIN A0
static uint16_t adcFiltrado = 0;
static uint16_t tensaoBateria = 0;
// sensor
Adafruit_BME280 bme;
static float tFiltrado = 0;
static float hFiltrado = 0;
static float pFiltrado = 0;
static bool filtroInicializado = false;

//----------------------------------------
void Sensors_init()
{
    pinMode(BATTERY_PIN, INPUT);
    pinMode(BME_POWER, OUTPUT);
    // desliga sensor inicialmente (PNP)
    // 09-03-26--manter ligado
    // digitalWrite(BME_POWER, HIGH);

    DS18B20_Init(DS18B20_PIN);

    Wire.begin();
    digitalWrite(BME_POWER, LOW);
    delay(20);
    bme.begin(0x76);
    bme.setSampling(
        Adafruit_BME280::MODE_NORMAL,
        Adafruit_BME280::SAMPLING_X2,  // temperatura
        Adafruit_BME280::SAMPLING_X16, // pressão
        Adafruit_BME280::SAMPLING_X1,  // umidade
        Adafruit_BME280::FILTER_X16,   // filtro interno
        Adafruit_BME280::STANDBY_MS_500);
}
//--------------------------------------------------
bool Read_t_p_h(float &t, float &h, float &p)

{
    float tr;
    float hr = bme.readHumidity();
    float pr = bme.readPressure() / 100.0f;

    switch (tempSensorSelected)
    {
    case TEMP_SENSOR_BME280:
        tr = bme.readTemperature();
        break;

    case TEMP_SENSOR_DS18B20:
        if (DS18B20_Read(tr))
            break;
    }

    // inicializa filtro na primeira leitura
    if (!filtroInicializado)
    {
        tFiltrado = tr;
        hFiltrado = hr;
        pFiltrado = pr;
        filtroInicializado = true;
    }

    // filtro exponencial
    // tFiltrado += (tr - tFiltrado) * 0.015625;
    // hFiltrado += (hr - hFiltrado) * 0.015625;
    // pFiltrado += (pr - pFiltrado) * 0.015625;

    /* Ganho do filtro (tempo de resposta)

    O coeficiente controla a suavização.

    Fator	Efeito
    0.5	       (1/2)    resposta rápida
    0.25	   (1/4)    média
    0.125      (1/8)	suave
    0.0625	   (1/16)   muito suave
    0.03125    (1/32)
    0,015625   (1/64)

    */

    // t = tFiltrado;
    // h = hFiltrado;
    // p = pFiltrado;

    t = tr;
    h = hr; // esses 3 sao par fazer leitura sem utilizar filtro digital
    p = pr;

    t += Calibration_GetTempOffset() / 10.0f;
    h += Calibration_GetHumOffset() / 10.0f;
    p += Calibration_GetPressOffset() / 10.0f;

    return true;
}
//----------------------------------------------
float Sensors_GetBatteryADC()
{
    uint16_t leitura = analogRead(BATTERY_PIN);

    // filtro exponencial inteiro 1/8
    adcFiltrado = adcFiltrado - (adcFiltrado >> 3) + (leitura >> 3);

    // histerese
    if (adcFiltrado > tensaoBateria + 4)
        tensaoBateria = adcFiltrado;

    else if (adcFiltrado + 4 < tensaoBateria)
        tensaoBateria = adcFiltrado;

    // return tensaoBateria;
    return tensaoBateria * (5.0 / 1023.0);
}
//----------------------------------------------
uint8_t Sensors_GetBatteryIcon()
{
    if (tensaoBateria >= 859)
        return 53;
    if (tensaoBateria >= 828)
        return 52;
    if (tensaoBateria >= 798)
        return 51;
    if (tensaoBateria >= 777)
        return 50;
    if (tensaoBateria >= 757)
        return 49;
    if (tensaoBateria >= 737)
        return 48;

    return 47;
}
//----------------------------------------------
void Sensors_Warmup()
{
    float t, h, p;
    for (uint8_t i = 0; i < 30; i++)
    {
        Sensors_GetBatteryADC();
        Read_t_p_h(t, h, p);
        delay(20);
    }
}
//----------------------------------------------
