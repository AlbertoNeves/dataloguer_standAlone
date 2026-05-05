#include "ScreenGraph.h"
#include "drivers/Display.h"
#include "services/SDService.h"
#include "Graph.h"
#include "Renderer.h"
#include "core/SystemState.h"
#include "ScreenGraphVar.h"
#include "ScreenGraphCalendar.h"

static int16_t graphBuffer[GRAPH_POINTS];
static bool graphMarkers[GRAPH_POINTS];
static uint16_t offset = 0;
static uint16_t lastTotalSamples = 0;
static char graphFirstLabel[7];
static char graphLastLabel[7];

static void DrawScaleMode()
{
    U8G2 &u8g2 = Display_GetU8G2();
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.drawStr(2, 63, Graph_IsAutoScale() ? "AUT" : "MAN");
}

void ScreenGraph_Draw()
{
    uint16_t samples = 0;
    uint16_t totalSamples = 0;

    char var = ScreenGraphVar_Get();
    uint8_t day = ScreenGraphCalendar_GetDay();
    uint8_t month = ScreenGraphCalendar_GetMonth();
    uint16_t year = ScreenGraphCalendar_GetYear();

    bool loaded = false;

    if (var == 'B')
    {
        graphFirstLabel[0] = '\0';
        graphLastLabel[0] = '\0';

        loaded = SDService_LoadGraphWithMarkers(
            var,
            day,
            month,
            year,
            graphBuffer,
            graphMarkers,
            GRAPH_POINTS,
            offset,
            &samples,
            &totalSamples,
            graphFirstLabel,
            graphLastLabel);

        if (totalSamples <= GRAPH_POINTS && offset != 0)
        {
            offset = 0;
            graphFirstLabel[0] = '\0';
            graphLastLabel[0] = '\0';

            loaded = SDService_LoadGraphWithMarkers(
                var,
                day,
                month,
                year,
                graphBuffer,
                graphMarkers,
                GRAPH_POINTS,
                offset,
                &samples,
                &totalSamples,
                graphFirstLabel,
                graphLastLabel);
        }

        Graph_SetMarkers(graphMarkers, samples);
        Graph_SetValueDivisor(100);
    }
    else
    {
        loaded = SDService_LoadGraph(
            var,
            day,
            month,
            year,
            graphBuffer,
            GRAPH_POINTS,
            offset,
            &samples);

        Graph_SetMarkers(nullptr, 0);
        Graph_SetValueDivisor(10);
    }

    if (loaded)
    {
        lastTotalSamples = var == 'B' ? totalSamples : 0;

        Graph_SetData(graphBuffer, samples);
        Graph_CalcScale();
    }
    else
    {
        lastTotalSamples = 0;
        Graph_SetMarkers(nullptr, 0);
        Graph_SetXAxisLabels(nullptr, nullptr);
        Graph_SetData(nullptr, 0);
    }

    if (var == 'B')
    {
        Graph_SetFixedXSpacing(true);
        Graph_SetXAxis(offset, GRAPH_POINTS);
        Graph_SetXAxisLabels(graphFirstLabel, graphLastLabel);
    }
    else
    {
        Graph_SetStartX(offset / 20);
    }
    DrawScaleMode();
    Graph_DrawAxis(Display_GetU8G2());
    Graph_DrawData(Display_GetU8G2());
}

void ScreenGraph_Handle(EventType evt)
{
    switch (evt)
    {
    case EVT_RIGHT:
        if (ScreenGraphVar_Get() == 'B')
        {
            if (offset + 20 < lastTotalSamples)
                offset += 20;
        }
        else if (offset < 360)
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

    case EVT_HOLD:
        Graph_SetAutoScale(!Graph_IsAutoScale());
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
