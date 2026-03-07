#include "drivers/Display.h"
#include "core/SystemState.h"
#include "core/EventQueue.h"
#include "ScreenLogging.h"
#include "Renderer.h"

void ScreenLogging_Draw()
{
    Display_Print(0,20,"Logging...");
    Display_Print(0,35,"Temp: --.- C");
    Display_Print(0,47,"Hum : --.- %");
}

void HandleLogging(EventType evt)
{
    if(evt == EVT_ENTER)   // exemplo: ENTER para sair
    {
        System_SetState(ST_HOME);
        Renderer_RequestDraw();
    }
}