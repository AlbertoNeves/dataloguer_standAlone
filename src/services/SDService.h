#pragma once
#include <Arduino.h>
#include <SdFat.h>

extern SdFat SD;

bool SD_Init();
bool SD_Check();


bool SDService_LoadGraph(
    char type,
    uint8_t day,
    uint8_t month,
    uint16_t year,
    int16_t *buffer,
    uint16_t size,
    uint16_t offset,
    uint16_t *samplesLoaded);

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
    char *lastLabel);

bool SDService_FileExists(
    char type,
    uint8_t day,
    uint8_t month,
    uint16_t year);
