#include <Arduino.h>
#include "services/RTCService.h"
#include "drivers/Display.h"
#include "core/SystemState.h"
#include "services/Logger.h"
#include "StatusBar.h"
#include "Layout.h"

void StatusBar_Init()
{
    // futuramente podemos iniciar RTC aqui
}

void StatusBar_Draw()
{
    Display_DrawHLine(0, SEPARATOR_Y, 128);
    RTC_Update();

    char buf[9];
    RTC_GetTime(buf);

    // limpa área do relógio
    Display_ClearArea(2, 0, 60, 10);
    Display_Print(2, 8, buf);

    // indicador SD fixo
    Display_Print(70, 8, "SD");

    // LOG piscando fixo na posição
    static bool blink = false;
    static uint32_t blinkTimer = 0;

    if (millis() - blinkTimer > 500)
    {
        blinkTimer = millis();
        blink = !blink;
    }

    if (Logger_IsRunning())
    {
        if (blink)
            Display_Print(95, 8, "LOG");
        else
            Display_Print(95, 8, "   "); // limpa mantendo largura
    }
    else
    {
        Display_Print(95, 8, "   ");
    }
}