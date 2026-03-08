#include "ScreenGraph.h"
#include "drivers/Display.h"
#include "services/SDService.h"
#include "Graph.h"
#include "ui/Graph.h"
#include "Renderer.h"
#include "core/SystemState.h"

static int16_t graphBuffer[120];
static uint16_t offset = 0;
//----------------------------------------------------
void ScreenGraph_Draw()
{
    Display_Clear();

    u8g2.setDrawColor(1);

    bool ok = SDService_LoadGraphTemperature(graphBuffer, 120, offset);

    if (!ok)
    {
        Display_Print(0, 30, "NO DATA");
        Display_Update();
        return;
    }

    Graph_SetData(graphBuffer, 120);

    Graph_SetStartX(offset / 20);

    Graph_CalcScale();

    Graph_DrawAxis(u8g2);
    Graph_DrawData(u8g2);

    Display_Update();

    u8g2.setFont(u8g2_font_6x12_tf);
}
//==================================================
void ScreenGraph_Handle(EventType evt)
{

    switch (evt)
    {
    case EVT_RIGHT:
        if (offset < 120)
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