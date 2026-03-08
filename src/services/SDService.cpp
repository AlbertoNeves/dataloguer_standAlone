#include "SDService.h"
#include "RTCService.h"
#include <Arduino.h>

SdFat SD;

//------------------------------------------------
// Inicialização do SD
//------------------------------------------------
bool SD_Init()
{
    if (!SD.begin(53))
    {
        Serial.println("SD FAIL");
        return false;
    }

    Serial.println("SD OK");
    return true;
}

//------------------------------------------------
// Teste simples do SD
//------------------------------------------------
bool SD_Check()
{
    return SD.card()->isBusy() == false;
}

//------------------------------------------------
// Monta nome do arquivo do dia
//------------------------------------------------
static void buildGraphFileName(char *name, char type)
{
    DateTime now = RTC_GetDateTime();

    sprintf(name, "%02d%02d%02d_%c.CSV",
            now.year() % 100,
            now.month(),
            now.day(),
            type);
}

//------------------------------------------------
// Carrega dados para o gráfico
//------------------------------------------------
bool SDService_LoadGraphTemperature(
    int16_t *buffer,
    uint16_t size,
    uint16_t offset)
{
    char filename[16];

    buildGraphFileName(filename, 'T');

    FsFile file = SD.open(filename);

    if (!file)
    {
        Serial.println("Graph file not found");
        return false;
    }

    uint16_t sample = 0;
    uint16_t index  = 0;

    while (file.available())
    {
        int value = file.parseInt();

        if (sample >= offset)
        {
            buffer[index++] = value;

            if (index >= size)
                break;
        }

        sample++;
    }

    file.close();

    if (index == 0)
        return false;

    // preenche restante do buffer
    while (index < size)
    {
        buffer[index] = buffer[index - 1];
        index++;
    }

    return true;
}