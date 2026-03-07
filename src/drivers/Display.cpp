#include <Arduino.h>
#include "Display.h"
#include <U8g2lib.h>
#include "core/Config.h"

static bool backlightOn = true;
static uint32_t lastActivity = 0;

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
//================================================
void Display_Backlight(bool on)
{
    digitalWrite(PIN_BACKLIGHT, on ? HIGH : LOW);
}
//================================================
void Display_UserActivity()
{
    lastActivity = millis();

    if (!backlightOn)
    {
        Display_Backlight(true);
        backlightOn = true;
    }
}
//===============================================
void Display_Init()
{
    pinMode(PIN_BACKLIGHT, OUTPUT);
    digitalWrite(PIN_BACKLIGHT, HIGH);

    pinMode(PIN_BACKLIGHT, OUTPUT);
    analogWrite(PIN_BACKLIGHT, 180); // brilho padrão

    pinMode(PIN_CONTRAST, OUTPUT);
    analogWrite(PIN_CONTRAST, 110);

    u8g2.begin();
    u8g2.setFont(u8g2_font_6x12_tf);

    // inicia temporizador de atividade
    lastActivity = millis();
}
//===============================================
void Display_DrawHLine(int x, int y, int w)
{
    u8g2.drawHLine(x, y, w);
}
//===============================================
void Display_DrawFrame(int x, int y, int w, int h)
{
    u8g2.drawFrame(x, y, w, h);
}
//===============================================
void Display_DrawBox(int x, int y, int w, int h)
{
    u8g2.drawBox(x, y, w, h);
}
//===============================================
void Display_Clear()
{
    u8g2.clearBuffer();
}
//===============================================
void Display_Update()
{
    u8g2.sendBuffer();
}
//===============================================
void Display_BacklightTask()
{
    if (backlightOn)
    {
        if (millis() - lastActivity > BACKLIGHT_TIMEOUT)
        {
            Display_Backlight(false);
            backlightOn = false;
        }
    }
}
//===============================================
void Display_Print(int x, int y, const char *txt)
{
    u8g2.drawStr(x, y, txt);
}
//===============================================
void Display_ClearArea(int x, int y, int w, int h)
{
    u8g2.setDrawColor(0);
    u8g2.drawBox(x, y, w, h);
    u8g2.setDrawColor(1);
}