
#include "SDService.h"
#include "RTCService.h"
#include <Arduino.h>

SdFat SD;

//------------------------------------------------
bool SDService_FileExists(
    char type,
    uint8_t day,
    uint8_t month,
    uint16_t year)
{
    char filename[16];

    sprintf(filename,"%02d%02d%02d_%c.CSV",
        year % 100,
        month,
        day,
        type);

    return SD.exists(filename);
}
//------------------------------------------------
bool SD_Init()
{
    if (!SD.begin(53))
    {
        Serial.println("SD FAIL");
        return false;
    }

    return true;
}

//------------------------------------------------
bool SD_Check()
{
    return SD.card()->isBusy() == false;
}

//------------------------------------------------
static void buildGraphFileNameFromDate(
    char *name,
    char type,
    uint8_t day,
    uint8_t month,
    uint16_t year)
{
    sprintf(name, "%02d%02d%02d_%c.CSV",
            year % 100,
            month,
            day,
            type);
}
// Gera nome de arquivo baseado na data atual
//------------------------------------------------

/*
static void buildGraphFileName(char *name, char type)
{
    DateTime now = RTC_GetDateTime();

    sprintf(name, "%02d%02d%02d_%c.CSV",
            now.year() % 100,
            now.month(),
            now.day(),
            type);
}
*/

//------------------------------------------------
// Loader genérico de gráfico
//------------------------------------------------
bool SDService_LoadGraph(
    char type,
    uint8_t day,
    uint8_t month,
    uint16_t year,
    int16_t *buffer,
    uint16_t size,
    uint16_t offset,
    uint16_t *samplesLoaded)
{
    char filename[16];

    buildGraphFileNameFromDate(
        filename,
        type,
        day,
        month,
        year);

    FsFile file = SD.open(filename);

    if (!file)
    {
        Serial.println("Graph file not found");
        return false;
    }

    uint16_t sample = 0;
    uint16_t index = 0;

    while (file.available())
    {
        String line = file.readStringUntil('\n');

        int value = line.toInt();

        // ignora linhas inválidas
        if (value == 0 && line.indexOf('0') == -1)
            continue;

        if (sample >= offset)
        {
            buffer[index++] = value;

            if (index >= size)
                break;
        }

        sample++;
    }

    file.close();

    *samplesLoaded = index;

    return index > 0;
}
//----------------------------------
bool SDService_LoadGraphWithMarkers(
    char type,
    uint8_t day,
    uint8_t month,
    uint16_t year,
    int16_t *buffer,
    bool *markers,
    uint16_t size,
    uint16_t offset,
    uint16_t *samplesLoaded,
    uint16_t *totalSamples,
    char *firstLabel,
    char *lastLabel)
{
    char filename[16];

    buildGraphFileNameFromDate(
        filename,
        type,
        day,
        month,
        year);

    FsFile file = SD.open(filename);

    if (!file)
    {
        Serial.println("Graph file not found");
        return false;
    }

    for (uint16_t i = 0; i < size; i++)
        markers[i] = false;

    firstLabel[0] = '\0';
    lastLabel[0] = '\0';

    uint16_t sample = 0;
    uint16_t index = 0;

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        line.trim();

        if (line.startsWith("---"))
        {
            if (sample > 0)
            {
                uint16_t markerSample = sample - 1;

                if (markerSample >= offset)
                {
                    uint16_t markerIndex = markerSample - offset;

                    if (markerIndex < size)
                        markers[markerIndex] = true;
                }
            }

            continue;
        }

        char label[7] = "";
        int separator = line.indexOf(',');
        int value;

        if (separator >= 0)
        {
            line.substring(0, separator).toCharArray(label, sizeof(label));
            value = line.substring(separator + 1).toInt();
        }
        else
        {
            value = line.toInt();
        }

        if (value == 0 && line.indexOf('0') == -1)
            continue;

        if (sample >= offset && index < size)
        {
            buffer[index++] = value;

            if (label[0] != '\0')
            {
                if (firstLabel[0] == '\0')
                    strncpy(firstLabel, label, 7);

                strncpy(lastLabel, label, 7);
            }
        }

        sample++;
    }

    file.close();

    *samplesLoaded = index;
    *totalSamples = sample;

    return index > 0;
}
//----------------------------------
