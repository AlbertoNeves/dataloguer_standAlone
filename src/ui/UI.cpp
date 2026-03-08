#include "Screens.h"
#include "ScreenDisplayAdjust.h"
#include "ScreenLogging.h"
#include <Arduino.h>
#include "UI.h"
#include "Menu.h"
#include "Renderer.h"
#include "ui/RTCAdjust.h"
#include "core/SystemState.h"
#include "core/EventQueue.h"
#include "drivers/Display.h"
#include "Graph.h"
#include "services/SDService.h"
#include "ui/ScreenGraph.h"

void UI_Init()
{
    Renderer_Init();
    Renderer_RequestDraw();
    // DrawBoot();
}
//---------------------------------------
void UI_Update()
{
    Display_BacklightTask();

    EventType evt = Event_Pop();
    if (evt == EVT_NONE)
        return;

    bool handled = false;

    switch (System_GetState())
    {
    case ST_HOME:
        ScreenHome_Handle(evt);
        handled = true;
        break;

    case ST_MENU:
        Menu_Handle(evt);
        handled = true;
        break;

    case ST_DISPLAY_ADJUST:
        handled = DisplayAdjust_Handle(evt);
        break;

    case ST_RTC_CONFIG:
        ScreenRTC_Handle(evt);
        handled = true;
        break;

    case ST_LOGGING:
        HandleLogging(evt);
        handled = true;
        break;

    case ST_ABOUT:
        if (evt == EVT_LEFT || evt == EVT_ENTER)
        {
            System_SetState(ST_MENU);
            Renderer_RequestDraw();
        }
        break;

    case ST_GRAPH:
        ScreenGraph_Handle(evt);
        handled = true;
        break;

    default:
        break;
    }

    // evento global BACK
    if (!handled && evt == EVT_LEFT)
    {
        System_SetState(ST_HOME);
        Renderer_RequestDraw();
        return;
    }

    Renderer_Update();
}