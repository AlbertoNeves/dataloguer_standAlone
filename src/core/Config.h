#pragma once

#include <Arduino.h>

// EEPROM MAP
// ==========================
#define EEPROM_ADDR_CONFIG 0

struct DeviceConfig
{
    int16_t tempOffset;
    int16_t humOffset;
    int16_t pressOffset;

    uint8_t brightness;
    uint8_t contrast;

    uint8_t logFlag;
    uint32_t logInterval;

    uint8_t version;
};
// ==========================


// ==========================
// LIMITES DISPLAY
// ==========================
// LIMITES BRILHO
#define BRIGHT_MIN 20
#define BRIGHT_MAX 255

// LIMITES CONTRASTE
#define CONTR_MIN 5
#define CONTR_MAX 100
// ==========================

// LOGGING
// ==========================
#define LOG_INTERVAL_TEMP 180000UL // 3 minutos

// SCREEN SAVER 
#define BACKLIGHT_TIMEOUT 20000UL   // 20 segundos

// Ajuste Offset temperatura 
// ==========================
extern int16_t g_tempOffset;   // offset em décimos de grau
