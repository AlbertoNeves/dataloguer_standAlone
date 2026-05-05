#include <Arduino.h>
#include <SdFat.h>

#include "services/SDService.h"
#include "LogEngine.h"
#include "Sensors.h"
#include "Logger.h"
#include "RTCService.h"
#include "../core/SystemState.h"
#include "../core/Config.h"

static uint32_t lastTempLog = 0;
static uint32_t lastBatteryLog = 0;
static uint32_t lastSDCheck = 0;
static bool statusBat = true;
static bool lastStatusBat = true;
static bool batteryStatusInitialized = false;

static uint16_t GetDailySampleIndex()
{
    DateTime now = RTC_GetDateTime();
    uint32_t seconds = ((uint32_t)now.hour() * 3600UL) + ((uint32_t)now.minute() * 60UL) + now.second();
    uint16_t sample = seconds / (LOG_INTERVAL_TEMP / 1000UL);

    return sample > 480 ? 480 : sample;
}

static void AlignNextTempLog()
{
    DateTime nowRtc = RTC_GetDateTime();
    uint32_t seconds = ((uint32_t)nowRtc.hour() * 3600UL) + ((uint32_t)nowRtc.minute() * 60UL) + nowRtc.second();
    uint32_t elapsedSinceSlot = (seconds % (LOG_INTERVAL_TEMP / 1000UL)) * 1000UL;

    lastTempLog = millis() - elapsedSinceSlot;
}

static void PrepareDailySensorLogs()
{
    float t, h, p;

    if (!Read_t_p_h(t, h, p))
        return;

    Logger::prepareDailySensorFiles(
        (int16_t)(t * 10),
        (int16_t)(h * 10),
        (int16_t)p,
        GetDailySampleIndex());
}

static void UpdateBatteryStatus()
{
    statusBat = (digitalRead(BATTERY_LOG_ROTATE_PIN) == HIGH);

    if (!batteryStatusInitialized)
    {
        lastStatusBat = statusBat;
        batteryStatusInitialized = true;
        return;
    }

    if (statusBat != lastStatusBat)
    {
        if (statusBat == LOW)
            Logger::rotateBatteryFile();
        else
            Logger::logBatterySeparator();

        lastStatusBat = statusBat;
    }
}

void Log_Init()
{
    pinMode(BATTERY_LOG_ROTATE_PIN, INPUT_PULLUP);
    Logger_Init();
    AlignNextTempLog();
    lastBatteryLog = millis();
    lastSDCheck = millis();

    if (Log_IsActive())
        PrepareDailySensorLogs();
}

void Log_Update()
{
    uint32_t now = millis();

    if (now - lastSDCheck > 10000)
    {
        lastSDCheck = now;

        if (!SD_Check())
        {
            Serial.println("SD CARD ERROR");
            System_SetState(ST_ERROR);
            return;
        }
    }

    if (!Log_IsActive())
        return;

    UpdateBatteryStatus();

    if (now - lastTempLog >= LOG_INTERVAL_TEMP)
    {
        lastTempLog = now;

        float t, h, p;

        if (Read_t_p_h(t, h, p))
        {
            Logger::logTemperature((int16_t)(t * 10));
            Logger::logHumidity((int16_t)(h * 10));
            Logger::logPressure((int16_t)(p));
        }
    }

    if (now - lastBatteryLog >= LOG_INTERVAL_BATTERY)
    {
        lastBatteryLog = now;
        Logger::logBattery((uint16_t)(Sensors_GetBatteryADC() * 100));
    }
}

bool Log_IsActive()
{
    return Logger_IsRunning();
}

void Log_Start()
{
    Logger_Start();
    AlignNextTempLog();
    lastBatteryLog = millis();
    batteryStatusInitialized = false;
    PrepareDailySensorLogs();
}

void Log_Stop()
{
    Logger_Stop();
}
