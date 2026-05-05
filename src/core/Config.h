#pragma once

#include <Arduino.h>

// EEPROM MAP
// ==========================
// Offset temperatura
#define EEPROM_ADDR 0       //(1 byte)


// Display
#define EEPROM_BRIGHTNESS 10 //(1 byte)
#define EEPROM_CONTRAST 11   //(1 byte)
#define EEPROM_GRAPH_AUTO_SCALE 12 // 1 byte

// Logger
#define EEPROM_LOG_FLAG 20     // 1 byte
#define EEPROM_LOG_INTERVAL 24 // uint32_t
#define EEPROM_BATTERY_LOG_FILE 60 // 16 bytes

// Entrada que força a criação do próximo arquivo de log da bateria.
// Usar contato para GND; a entrada é configurada com INPUT_PULLUP.
#define BATTERY_LOG_ROTATE_PIN 22

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
#define LOG_INTERVAL_BATTERY 600000UL // 10 minutos

// SCREEN SAVER 
#define BACKLIGHT_TIMEOUT 20000UL   // 20 segundos

// Ajuste Offset temperatura 
// ==========================
extern int16_t g_tempOffset;   // offset em décimos de grau
