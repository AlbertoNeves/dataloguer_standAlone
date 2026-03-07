#include <Arduino.h>
#include <EEPROM.h>
#include "drivers/Display.h"
#include "core/SystemState.h"
#include "core/EventQueue.h"
#include "ui/Renderer.h"
#include "core/Config.h"

#define PIN_BACKLIGHT 11
#define PIN_CONTRAST 3

static uint8_t brightness;
static uint8_t contrast;

static uint8_t field = 0; // 0=brightness 1=contrast


//bool DisplayAdjust_Handle(EventType evt);

void DrawBar(int y, uint8_t value, uint8_t minV, uint8_t maxV)
{
    const int x = 12;
    const int wMax = 104;

    value = constrain(value, minV, maxV);

    int w = map(value, minV, maxV, 0, wMax);
    w = constrain(w, 0, wMax);

    Display_DrawFrame(x, y, wMax, 8);

    if(w > 4)   // só desenha se tiver espaço útil
        Display_DrawBox(x+2, y+2, w-4, 4);
}

void DisplayAdjust_Init()
{
    brightness = EEPROM.read(EEPROM_BRIGHTNESS);
    contrast   = EEPROM.read(EEPROM_CONTRAST);

    brightness = constrain(brightness, BRIGHT_MIN, BRIGHT_MAX);
    contrast   = constrain(contrast,   CONTR_MIN,   CONTR_MAX);

    analogWrite(PIN_BACKLIGHT, brightness);
    analogWrite(PIN_CONTRAST,  contrast);
}

void DisplayAdjust_Draw()
{
	
	char buf[5];

sprintf(buf,"%3u",brightness);
Display_Print(110,31,buf);

sprintf(buf,"%3u",contrast);
Display_Print(110,50,buf);

    Display_Print(0,20,"Display Adjust");

    // Brightness
    if(field == 0)
        Display_Print(0,31,"> Brightness");
    else
        Display_Print(0,31,"  Brightness");

    DrawBar(34, brightness, BRIGHT_MIN, BRIGHT_MAX);

    // Contrast
    if(field == 1)
        Display_Print(0,50,"> Contrast");
    else
        Display_Print(0,50,"  Contrast");

    DrawBar(52, contrast, CONTR_MIN, CONTR_MAX);
}
bool DisplayAdjust_Handle(EventType evt)
{
    switch(evt)
    {
    case EVT_UP:
    if(field > 0)
        field--;
    Renderer_RequestDraw();
    return true;

case EVT_DOWN:
    if(field < 1)
        field++;
    Renderer_RequestDraw();
    return true;

        case EVT_RIGHT:
            if(field == 0 && brightness < BRIGHT_MAX) brightness++;
            if(field == 1 && contrast   < CONTR_MAX) contrast++;
			analogWrite(PIN_BACKLIGHT, brightness);
			analogWrite(PIN_CONTRAST, contrast);
            Renderer_RequestDraw();
            return true;

        case EVT_LEFT:
            if(field == 0 && brightness > BRIGHT_MIN) brightness--;
            if(field == 1 && contrast   > CONTR_MIN) contrast--;
			analogWrite(PIN_BACKLIGHT, brightness);
			analogWrite(PIN_CONTRAST, contrast);
            Renderer_RequestDraw();
            return true;

        case EVT_ENTER:
            EEPROM.update(EEPROM_BRIGHTNESS, brightness);
            EEPROM.update(EEPROM_CONTRAST, contrast);

            System_SetState(ST_HOME);
            Renderer_RequestDraw();
            return true;

        default:
            return false;
    }
}