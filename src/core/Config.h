#pragma once

#include <Arduino.h>

// EEPROM MAP
// ==========================
// Display
#define EEPROM_BRIGHTNESS 10 //(1 byte)
#define EEPROM_CONTRAST 11   //(1 byte)

// Logger
#define EEPROM_LOG_FLAG 20     // 1 byte
#define EEPROM_LOG_INTERVAL 24 // uint32_t

// System
#define EEPROM_BOOT_COUNTER 40   // uint16_t
#define EEPROM_CONFIG_VERSION 50 // 1 byte
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
