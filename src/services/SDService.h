#pragma once
#include <Arduino.h>
#include <SdFat.h>

extern SdFat SD;

bool SD_Init();
bool SD_Check();

bool SDService_LoadGraphTemperature(
    int16_t *buffer,
    uint16_t size,
    uint16_t offset
);