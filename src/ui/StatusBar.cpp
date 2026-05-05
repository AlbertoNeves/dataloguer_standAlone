#include <Arduino.h>
#include "services/RTCService.h"
#include "drivers/Display.h"
#include "services/LogEngine.h"
#include "StatusBar.h"
#include "Layout.h"

void StatusBar_Draw()
{
    Display_DrawHLine(0, SEPARATOR_Y, 128);
    RTC_Update();

    char buf[9];
    RTC_GetTime(buf);

    Display_ClearArea(2, 0, 60, 10);
    Display_Print(2, 8, buf);
    Display_Print(70, 8, "SD");

    static bool blink = false;
    static uint32_t blinkTimer = 0;

    if (millis() - blinkTimer > 500)
    {
        blinkTimer = millis();
        blink = !blink;
    }

    if (Log_IsActive() && blink)
        Display_Print(95, 8, "LOG");
    else
        Display_Print(95, 8, "   ");
}
