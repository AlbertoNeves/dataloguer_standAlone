#include <Arduino.h>
#include <SdFat.h>

#include "services/SDService.h"
#include "LogEngine.h"
#include "Sensors.h"
#include "Logger.h"
#include "../core/SystemState.h"

static uint32_t lastTempLog = 0;
static uint32_t lastSDCheck = 0;

//------------------------------------
void Log_Init()
{
    lastTempLog = millis();
    lastSDCheck = millis();
}

//------------------------------------
void Log_Update()
{
    uint32_t now = millis();

    // --- checar SD a cada 10s ---
    if(now - lastSDCheck > 10000)
    {
        lastSDCheck = now;

        if(!SD_Check())
        {
            Serial.println("SD CARD ERROR");
            System_SetState(ST_ERROR);
            return;
        }
    }

    // --- log sensores a cada 3 minutos ---
    if(now - lastTempLog >= 180000UL)
    {
        lastTempLog = now;

        float t,h,p;

        if(Sensors::read(t,h,p))
        {
            Logger::logTemperature((int16_t)(t*10));
            Logger::logHumidity((int16_t)(h*10));
            Logger::logPressure((int16_t)(p));
        }
    }
}

//------------------------------------
bool Log_IsActive()
{
    return true;
}

//------------------------------------
void Log_Start()
{
}

//------------------------------------
void Log_Stop()
{
}