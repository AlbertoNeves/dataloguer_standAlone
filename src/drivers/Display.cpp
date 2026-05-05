#include <Arduino.h>
#include "Display.h"
#include <U8g2lib.h>
#include <EEPROM.h>
#include "core/Config.h"

static bool backlightOn = true;
static uint32_t lastActivity = 0;
static uint8_t displayBrightness = 180;
static uint8_t displayContrast = 100;

/*
   ST7920 SPI (software)
   E  -> 9
   RW -> 7
   RS -> 5
*/

U8G2_ST7920_128X64_F_SW_SPI u8g2(
    U8G2_R2,
    9, // E (clock)
    7, // RW (data)
    5, // RS (CS)
    13 // RESET
);

#define PIN_BACKLIGHT 11
#define PIN_CONTRAST 3
#define DEFAULT_BRIGHTNESS 180
#define DEFAULT_CONTRAST 100

static uint8_t sanitizeBrightness(uint8_t value)
{
    if (value < BRIGHT_MIN || value > BRIGHT_MAX)
        return DEFAULT_BRIGHTNESS;

    return value;
}

static uint8_t sanitizeContrast(uint8_t value)
{
    if (value < CONTR_MIN || value > CONTR_MAX)
        return DEFAULT_CONTRAST;

    return value;
}

static void applyDisplayLevels()
{
    analogWrite(PIN_BACKLIGHT, backlightOn ? displayBrightness : 0);
    analogWrite(PIN_CONTRAST, displayContrast);
}

void Display_Backlight(bool on)
{
    backlightOn = on;
    applyDisplayLevels();
}

void Display_UserActivity()
{
    lastActivity = millis();

    if (!backlightOn)
        Display_Backlight(true);
}

void Display_LoadSettings()
{
    uint8_t rawBrightness = EEPROM.read(EEPROM_BRIGHTNESS);
    uint8_t rawContrast = EEPROM.read(EEPROM_CONTRAST);

    displayBrightness = sanitizeBrightness(rawBrightness);
    displayContrast = sanitizeContrast(rawContrast);

    if (displayBrightness != rawBrightness)
        EEPROM.update(EEPROM_BRIGHTNESS, displayBrightness);

    if (displayContrast != rawContrast)
        EEPROM.update(EEPROM_CONTRAST, displayContrast);

    applyDisplayLevels();
}

void Display_SetLevels(uint8_t brightness, uint8_t contrast)
{
    displayBrightness = constrain(brightness, BRIGHT_MIN, BRIGHT_MAX);
    displayContrast = constrain(contrast, CONTR_MIN, CONTR_MAX);

    applyDisplayLevels();
}

void Display_Init()
{
    pinMode(PIN_BACKLIGHT, OUTPUT);
    pinMode(PIN_CONTRAST, OUTPUT);

    Display_LoadSettings();

    u8g2.begin();
    Display_SetFontDefault();
    lastActivity = millis();
}

void Display_DrawHLine(int x, int y, int w)
{
    u8g2.drawHLine(x, y, w);
}

void Display_DrawFrame(int x, int y, int w, int h)
{
    u8g2.drawFrame(x, y, w, h);
}

void Display_DrawBox(int x, int y, int w, int h)
{
    u8g2.drawBox(x, y, w, h);
}

void Display_Clear()
{
    u8g2.clearBuffer();
}

void Display_Update()
{
    u8g2.sendBuffer();
}

void Display_BacklightTask()
{
    if (backlightOn && millis() - lastActivity > BACKLIGHT_TIMEOUT)
        Display_Backlight(false);
}

void Display_Print(int x, int y, const char *txt)
{
    u8g2.drawStr(x, y, txt);
}

void Display_ClearArea(int x, int y, int w, int h)
{
    u8g2.setDrawColor(0);
    u8g2.drawBox(x, y, w, h);
    u8g2.setDrawColor(1);
}

U8G2 &Display_GetU8G2()
{
    return u8g2;
}

void Display_SetFontDefault()
{
    u8g2.setFont(u8g2_font_6x12_tf);
}

void Display_PrintCentered(int y, const char *txt)
{
    int w = u8g2.getStrWidth(txt);
    int x = (128 - w) / 2;

    u8g2.drawStr(x, y, txt);
}

void Display_SetColor(uint8_t c)
{
    u8g2.setDrawColor(c);
}
