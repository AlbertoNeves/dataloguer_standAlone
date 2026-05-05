#pragma once
#include <stdint.h>

#define CONFIG_VERSION 1

typedef struct
{
    int16_t tempOffset;
    int16_t humOffset;
    int16_t pressOffset;

    uint8_t brightness;
    uint8_t contrast;

    uint8_t logFlag;
    uint32_t logInterval;

    uint8_t version;

} DeviceConfig;

void Config_Load();
void Config_Save();
DeviceConfig* Config_Get();
