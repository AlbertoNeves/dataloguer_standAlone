#include "Logger.h"
#include "RTCService.h"
#include "SDService.h"
#include "core\Config.h"
#include <SdFat.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <string.h>

#define LOG_ACTIVE 0xA5
#define LOG_INACTIVE 0x00
#define BATTERY_LOG_FILENAME_LEN 16

static bool loggerRunning = false;
static bool openBatteryFile(FsFile &file, oflag_t flags);
static char batteryFileName[BATTERY_LOG_FILENAME_LEN] = "";

static void buildDatedBatteryFileName(char *filename, int8_t sequence)
{
    DateTime now = RTC_GetDateTime();

    if (sequence < 0)
    {
        sprintf(filename, "%02d%02d%02d_B.CSV",
                now.year() % 100,
                now.month(),
                now.day());
    }
    else
    {
        sprintf(filename, "%02d%02d%02d%dB.CSV",
                now.year() % 100,
                now.month(),
                now.day(),
                sequence);
    }
}

static bool isBatteryFileNameValid(const char *filename)
{
    size_t len = strnlen(filename, BATTERY_LOG_FILENAME_LEN);

    if (len < 10 || len >= BATTERY_LOG_FILENAME_LEN)
        return false;

    if (strcmp(filename + len - 4, ".CSV") != 0)
        return false;

    return filename[len - 5] == 'B';
}

static void saveBatteryFileName()
{
    for (uint8_t i = 0; i < BATTERY_LOG_FILENAME_LEN; i++)
        EEPROM.update(EEPROM_BATTERY_LOG_FILE + i, batteryFileName[i]);
}

static void loadBatteryFileName()
{
    for (uint8_t i = 0; i < BATTERY_LOG_FILENAME_LEN; i++)
        batteryFileName[i] = EEPROM.read(EEPROM_BATTERY_LOG_FILE + i);

    batteryFileName[BATTERY_LOG_FILENAME_LEN - 1] = '\0';

    if (!isBatteryFileNameValid(batteryFileName))
    {
        buildDatedBatteryFileName(batteryFileName, -1);
        saveBatteryFileName();
    }
}

static void buildBatteryFileName(char *filename)
{
    if (!isBatteryFileNameValid(batteryFileName))
        loadBatteryFileName();

    strncpy(filename, batteryFileName, BATTERY_LOG_FILENAME_LEN);
    filename[BATTERY_LOG_FILENAME_LEN - 1] = '\0';
}

void Logger_Stop()
{
    loggerRunning = false;
    EEPROM.update(EEPROM_LOG_FLAG, LOG_INACTIVE);
}

bool Logger_IsRunning()
{
    return loggerRunning;
}

void Logger_Start()
{
    loggerRunning = true;
    EEPROM.update(EEPROM_LOG_FLAG, LOG_ACTIVE);
}

void Logger_Init()
{
    loggerRunning = (EEPROM.read(EEPROM_LOG_FLAG) == LOG_ACTIVE);
    loadBatteryFileName();
}

static void sdDateTime(uint16_t *date, uint16_t *time)
{
    DateTime now = RTC_GetDateTime();

    *date = FAT_DATE(now.year(), now.month(), now.day());
    *time = FAT_TIME(now.hour(), now.minute(), now.second());
}

void SD_EnableTimestamp()
{
    FsDateTime::setCallback(sdDateTime);
}

void SD_DisableTimestamp()
{
    FsDateTime::clearCallback();
}

bool Logger::logTemperature(int16_t t)
{
    return logValue(t, 'T');
}

bool Logger::logHumidity(int16_t h)
{
    return logValue(h, 'H');
}

bool Logger::logPressure(int16_t p)
{
    return logValue(p, 'P');
}

bool Logger::prepareDailySensorFiles(int16_t t, int16_t h, int16_t p, uint16_t samplesToFill)
{
    bool ok = true;

    ok = prepareDailySensorFile(t, 'T', samplesToFill) && ok;
    ok = prepareDailySensorFile(h, 'H', samplesToFill) && ok;
    ok = prepareDailySensorFile(p, 'P', samplesToFill) && ok;

    return ok;
}

bool Logger::logBattery(uint16_t centivolts)
{
    FsFile file;
    DateTime now = RTC_GetDateTime();

    if (!openBatteryFile(file, O_WRITE | O_CREAT | O_APPEND))
        return false;

    char line[16];
    sprintf(line, "%02d%02d%02d,%u",
            now.day(),
            now.hour(),
            now.minute(),
            centivolts);

    file.println(line);
    file.close();
    return true;
}

bool Logger::logBatterySeparator()
{
    FsFile file;

    if (!openBatteryFile(file, O_WRITE | O_CREAT | O_APPEND))
        return false;

    file.println("-------------");
    file.close();
    return true;
}

bool Logger::rotateBatteryFile()
{
    char filename[16];
    bool foundName = false;

    buildDatedBatteryFileName(filename, -1);

    if (!SD.exists(filename) && strcmp(filename, batteryFileName) != 0)
    {
        foundName = true;
    }
    else
    {
        for (int8_t sequence = 0; sequence <= 9; sequence++)
        {
            buildDatedBatteryFileName(filename, sequence);

            if (!SD.exists(filename) && strcmp(filename, batteryFileName) != 0)
            {
                foundName = true;
                break;
            }
        }
    }

    if (!foundName)
        return false;

    strncpy(batteryFileName, filename, BATTERY_LOG_FILENAME_LEN);
    batteryFileName[BATTERY_LOG_FILENAME_LEN - 1] = '\0';
    saveBatteryFileName();

    FsFile file;

    if (!openBatteryFile(file, O_WRITE | O_CREAT))
        return false;

    file.close();
    return true;
}

void Logger::buildFileName(char *name, char type)
{
    DateTime now = RTC_GetDateTime();

    sprintf(name, "%02d%02d%02d_%c.CSV",
            now.year() % 100,
            now.month(),
            now.day(),
            type);
}

bool Logger::logValue(int16_t value, char type)
{
    char filename[16];

    buildFileName(filename, type);

    bool exists = SD.exists(filename);

    if (!exists)
        SD_EnableTimestamp();

    FsFile file = SD.open(filename, O_WRITE | O_CREAT | O_APPEND);

    if (!file)
        return false;

    if (!exists)
        SD_DisableTimestamp();

    file.println(value);
    file.close();
    return true;
}

bool Logger::prepareDailySensorFile(int16_t value, char type, uint16_t samplesToFill)
{
    char filename[16];

    buildFileName(filename, type);

    if (SD.exists(filename))
        return true;

    SD_EnableTimestamp();
    FsFile file = SD.open(filename, O_WRITE | O_CREAT);

    if (!file)
    {
        SD_DisableTimestamp();
        return false;
    }

    SD_DisableTimestamp();

    for (uint16_t i = 0; i < samplesToFill; i++)
        file.println(value);

    file.close();
    return true;
}

static bool openBatteryFile(FsFile &file, oflag_t flags)
{
    char filename[16];

    buildBatteryFileName(filename);

    bool exists = SD.exists(filename);

    if (!exists)
        SD_EnableTimestamp();

    file = SD.open(filename, flags);

    if (!file)
        return false;

    if (!exists)
        SD_DisableTimestamp();

    return true;
}
