#include <Arduino.h>
#include "Renderer.h"
#include "Screens.h"
#include "Menu.h"
#include "StatusBar.h"
#include "ScreenLogging.h"
#include "ScreenDisplayAdjust.h"
#include "ui/Layout.h"
#include "ui/RTCAdjust.h"

#include "drivers/Display.h"
#include "core/SystemState.h"

static bool dirty = true;
//-------------------------------------------------
void Renderer_Init()
{
    dirty = true;
}
//-------------------------------------------------
void Renderer_RequestDraw()
{
    dirty = true;
}
//-------------------------------------------------
void Renderer_DrawTitle(const char *title)
{
    Display_Print(2, TITLE_Y, "AJUSTE RTC");
    Display_DrawHLine(0, SEPARATOR_Y, 128);
}
//----------------------------------------------------
void Renderer_Update()
{
    static uint32_t lastAutoRefresh = 0;

    uint32_t now = millis();

    // força refresh a cada 1000ms para atualizar relógio
    if (now - lastAutoRefresh >= 1000)
    {
        lastAutoRefresh = now;
        dirty = true;
    }

    if (!dirty)
        return;

    dirty = false;

    Display_Clear();

    if (System_GetState() != ST_RTC_CONFIG)
    {
        StatusBar_Draw();
    }
    else
    {
        Renderer_DrawTitle("AJUSTE RTC");
    }

    switch (System_GetState())
    {
    case ST_HOME:
        ScreenHome_Draw();
        break;
    case ST_MENU:
        Menu_Draw();
        break;
    case ST_LOGGING:
        ScreenLogging_Draw();
        break;
    case ST_GRAPH:
        ScreenGraph_Draw();
        break;
    case ST_DISPLAY_ADJUST:
        DisplayAdjust_Draw();
        break;
    case ST_RTC_CONFIG:
        ScreenRTC_Draw();
        break;
    case ST_ABOUT:
        ScreenAbout_Draw();
        break;
    default:
        break;
    }

    Display_Update();
}
//-------------------------------------------------