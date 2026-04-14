#include "ScreenGraph.h"
#include "drivers/Display.h"
#include "services/SDService.h"
#include "Graph.h"
#include "Renderer.h"
#include "core/SystemState.h"
#include "ScreenGraphVar.h"
#include "ScreenGraphCalendar.h"

static int16_t graphBuffer[GRAPH_POINTS];
static uint16_t offset = 0;

//----------------------------------------------------
void ScreenGraph_Draw()
{
    uint16_t samples = 0;

    char var = ScreenGraphVar_Get();

    uint8_t day = ScreenGraphCalendar_GetDay();
    uint8_t month = ScreenGraphCalendar_GetMonth();
    uint16_t year = ScreenGraphCalendar_GetYear();

    if (SDService_LoadGraph(
            var,
            day,
            month,
            year,
            graphBuffer,
            GRAPH_POINTS,
            offset,
            &samples))
    {
        Graph_SetData(graphBuffer, samples);
        Graph_CalcScale();
    }
    else
    {
        Graph_SetData(nullptr, 0);
    }
    Graph_SetStartX(offset / 20);
    Graph_DrawAxis(Display_GetU8G2());
    Graph_DrawData(Display_GetU8G2());
}
//==================================================
void ScreenGraph_Handle(EventType evt)
{
    switch (evt)
    {
    case EVT_RIGHT:

        if (offset < 360)
            offset += 20;

        Renderer_RequestDraw();
        break;

    case EVT_LEFT:

        if (offset >= 20)
            offset -= 20;

        Renderer_RequestDraw();
        break;

    case EVT_ENTER:

        System_SetState(ST_MENU);
        Renderer_RequestDraw();
        break;

    case EVT_UP:

        Graph_ZoomIn();
        Renderer_RequestDraw();
        break;

    case EVT_DOWN:

        Graph_ZoomOut();
        Renderer_RequestDraw();
        break;

    default:
        break;
    }
}