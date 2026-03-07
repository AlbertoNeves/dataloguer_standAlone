#include "Logger.h"
#include "RTCService.h"
#include "SDService.h"
#include "core\Config.h"
#include <SdFat.h>
#include <RTClib.h>

#include "Logger.h"
#include <EEPROM.h>

#define LOG_ACTIVE    0xA5
#define LOG_INACTIVE  0x00


static bool loggerRunning = false;

//---------------------------------------------
void Logger_Stop()
{
    loggerRunning = false;

    EEPROM.update(EEPROM_LOG_FLAG, LOG_INACTIVE);
}
//---------------------------------------------
bool Logger_IsRunning()
{
    return loggerRunning;
}
//---------------------------------------------
void Logger_Start()
{
    loggerRunning = true;

    EEPROM.update(EEPROM_LOG_FLAG, LOG_ACTIVE);
}
//------------------------------------------
void Logger_Init()
{
    uint8_t flag = EEPROM.read(EEPROM_LOG_FLAG);

    if(flag == LOG_ACTIVE)
        loggerRunning = true;
    else
        loggerRunning = false;
}
//-------------------------------------------
void sdDateTime(uint16_t* date, uint16_t* time)
{
    DateTime now = RTC_GetDateTime();

    *date = FAT_DATE(now.year(), now.month(), now.day());
    *time = FAT_TIME(now.hour(), now.minute(), now.second());
}
//------------------------------------------------
void SD_EnableTimestamp()
{
    FsDateTime::setCallback(sdDateTime);
}
//------------------------------------------------
void SD_DisableTimestamp()
{
    FsDateTime::clearCallback();
}

//------------------------------------------------
bool Logger::logTemperature(int16_t t)
{
    return logValue(t,'T');
}
//----------------------------------------------
bool Logger::logHumidity(int16_t h)
{
    return logValue(h,'H');
}
//------------------------------------------------
bool Logger::logPressure(int16_t p)
{
    return logValue(p,'P');
}
//---------------------------------------------
void Logger::buildFileName(char *name, char type)
{
    DateTime now = RTC_GetDateTime();

    sprintf(name,"%02d%02d%02d_%c.CSV",
            now.year()%100,
            now.month(),
            now.day(),
            type);
}
//--------------------------------------------------
bool Logger::logValue(int16_t value, char type)
{
    char filename[16];

    buildFileName(filename, type);

    bool exists = SD.exists(filename);

    if(!exists)
        SD_EnableTimestamp();

    FsFile file = SD.open(filename, O_WRITE | O_CREAT | O_APPEND);

    if(!file)
        return false;

    if(!exists)
        SD_DisableTimestamp();

    file.println(value);

    file.close();

    return true;
}
//--------------------------------------
