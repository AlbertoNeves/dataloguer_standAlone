#include <Arduino.h>
#include "Screens.h"
#include "Menu.h"
#include "ScreenDisplayAdjust.h"
#include "Renderer.h"
#include "RTCAdjust.h"
#include "services/LogEngine.h"
#include "drivers/Display.h"
#include "core/SystemState.h"
#include "ui/Graph.h"
#include "services/SDService.h"
#include "ScreenCalibration.h"

static int cursor = 0;
//-------------------------------
void ScreenAbout_Draw()
{
    Display_PrintCentered(25, "DataLogger");
    Display_PrintCentered(38, "26 Abril 2026");
    Display_PrintCentered(50, "alberto.alves.neves");
    Display_PrintCentered(60, "@gmail.com");
}
//-------------------------------
void ScreenRTC_Handle(EventType evt)
{
    RTCAdjust_Handle(evt);
}
//-------------------------------
void ScreenRTC_Draw()
{
    RTCAdjust_Draw();
}
//-------------------------------
void ScreenHome_Draw()
{
    Display_Clear();
     Display_PrintCentered(10, "DataLogger");

    if (cursor == 0)
    {
        Display_Print(0, 30, "> Variaveis");
        Display_Print(0, 45, "  Menu");
    }
    else
    {
        Display_Print(0, 30, "  Variaveis");
        Display_Print(0, 45, "> Menu");
    }
}
//-------------------------------
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
        if (cursor == 0)
        {
            Log_Start();
            System_SetState(ST_LOGGING);
        }
        else
        {
            System_SetState(ST_MENU);
            OpenMainMenu();
        }

        Renderer_RequestDraw();
        break;

    default:
        break;
    }
}
//-------------------------------
void StartLog()
{
    Log_Start();
    System_SetState(ST_LOGGING);
    Renderer_RequestDraw();
}
//-------------------------------
void Menu_TempOffset()
{
    System_SetState(ST_CALIBRATION);
    ScreenCalibration_Start(0);
}
//-------------------------------
void Menu_HumOffset()
{
    System_SetState(ST_CALIBRATION);
    ScreenCalibration_Start(1);
}
//-------------------------------
void Menu_PressOffset()
{
    System_SetState(ST_CALIBRATION);
    ScreenCalibration_Start(2);
}
//-------------------------------
void OpenGraph()
{
    System_SetState(ST_GRAPH_CALENDAR);
    Renderer_RequestDraw();
}
//-------------------------------
void OpenDisplayAdjust()
{
    System_SetState(ST_DISPLAY_ADJUST);
    DisplayAdjust_Init();
    Renderer_RequestDraw();
}
//-------------------------------
void OpenRTC()
{
    System_SetState(ST_RTC_CONFIG);
    RTCAdjust_Start();
    Renderer_RequestDraw();
}
//-------------------------------
void OpenAbout()
{
    System_SetState(ST_ABOUT);
    Renderer_RequestDraw();
}
//-------------------------------
MenuItem calibrationMenu[] = {
    {"Temp", Menu_TempOffset},
    {"Umidade", Menu_HumOffset},
    {"Pressao", Menu_PressOffset},
    {"Sair", OpenMainMenu}
};
//-------------------------------
void StartCalibrationMenu()
{
    Menu_Set(calibrationMenu, sizeof(calibrationMenu) / sizeof(MenuItem));
    Menu_Draw();
}
//-------------------------------
MenuItem mainMenu[] = {
    {"Variaveis", StartLog},
    {"Calibracao", StartCalibrationMenu},
    {"Grafico", OpenGraph},
    {"Display", OpenDisplayAdjust},
    {"Config RTC", OpenRTC},
    {"Sobre", OpenAbout}
};
//-------------------------------
void OpenMainMenu()
{
    Menu_Set(mainMenu, sizeof(mainMenu) / sizeof(MenuItem));
    Menu_Draw();
}
