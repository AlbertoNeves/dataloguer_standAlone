#include <Arduino.h>
#include "Screens.h"
#include "Menu.h"
#include "ScreenDisplayAdjust.h"
#include "Renderer.h"
#include "RTCAdjust.h"
#include "services/Logger.h"
#include "drivers/Display.h"
#include "core/SystemState.h"

static int cursor = 0;
//-------------------------------------------
void ScreenGraph_Draw()
{
    Display_Print(0, 30, "Graph Screen");
}
//--------------------------------------------
void ScreenAbout_Draw()
{
    Display_Print(0, 30, "IFK-ENV-01");
    Display_Print(0, 45, "INFORMATEK");
}
//---------------------------------------------
void ScreenRTC_Handle(EventType evt)
{
    RTCAdjust_Handle(evt);
}
//-----------------------------------------------
void ScreenRTC_Draw()
{
    RTCAdjust_Draw();
}
//-----------------------------------------------
void ScreenHome_Draw()
{
    Display_Clear();

    Display_Print(0, 10, "IFK-ENV-01");

    if (cursor == 0)
    {
        Display_Print(0, 30, "> Start Log");
        Display_Print(0, 45, "  Menu");
    }
    else
    {
        Display_Print(0, 30, "  Start Log");
        Display_Print(0, 45, "> Menu");
    }
}
//-----------------------------------------------
void ScreenHome_Handle(EventType evt)
{
    switch (evt)
    {
    case EVT_UP:
    case EVT_DOWN:
    {
        int old = cursor;
        cursor ^= 1;

        if (old != cursor)
            Renderer_RequestDraw();
        break;
    }

    case EVT_ENTER:
    {
        if (cursor == 0)
        {
            System_SetState(ST_LOGGING);
        }
        else
        {
            System_SetState(ST_MENU);
            OpenMainMenu();
        }

        Renderer_RequestDraw();
        break;
    }

    default:
        break;
    }
}
//-----------------------------------------------
void StartLog()
{
    Logger_Start();
    System_SetState(ST_LOGGING);
    Renderer_RequestDraw();
}
//-----------------------------------------------
void OpenGraph()
{
    System_SetState(ST_GRAPH);
    Renderer_RequestDraw();
}
//-----------------------------------------------
void OpenDisplayAdjust()
{
    System_SetState(ST_DISPLAY_ADJUST);
    DisplayAdjust_Init();
    Renderer_RequestDraw();
}
//-----------------------------------------------
void OpenRTC()
{
    System_SetState(ST_RTC_CONFIG);
    RTCAdjust_Start(); // ← importante
    Renderer_RequestDraw();
}
//------------------------------------------------
void OpenAbout()
{
    System_SetState(ST_ABOUT);
    Renderer_RequestDraw();
}
//------------------------------------------------
MenuItem mainMenu[] =
    {
        {"Iniciar Log", StartLog},
        {"Grafico", OpenGraph},
        {"Display", OpenDisplayAdjust},
        {"Config RTC", OpenRTC},
        {"Sobre", OpenAbout}};
//--------------------------------------------------------
void OpenMainMenu()
{
    Menu_Set(mainMenu, sizeof(mainMenu) / sizeof(MenuItem));
    Menu_Draw();
}
//-----------------------------------------------------------
void DrawGraph()
{
    Display_Clear();
    Display_Print(0, 30, "Graph Screen");
}
//-----------------------------------------------------
void HandleRTC(EventType evt)
{
    RTCAdjust_Handle(evt);
}
//----------------------------------------------------------
void DrawAbout()
{
    Display_Clear();
    Display_Print(0, 30, "IFK-ENV-01");
    Display_Print(0, 45, "INFORMATEK");
}
//--------------------------------------------------------------