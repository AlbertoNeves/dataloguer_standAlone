#include <Arduino.h>
#include "ScreenGraphVar.h"
#include "drivers/Display.h"
#include "Renderer.h"
#include "core/SystemState.h"

static const char* vars[] =
{
    "Temperatura",
    "Umidade",
    "Pressao",
    "Bateria"
};

static const char varId[] =
{
    'T',
    'H',
    'P',
    'B'
};

static uint8_t indexVar = 0;

//------------------------------------------------
char ScreenGraphVar_Get()
{
    return varId[indexVar];
}

//------------------------------------------------
void ScreenGraphVar_Draw()
{
    Display_Print(30,9,"VARIAVEL");
    Display_DrawHLine(0,11,128);

    int startY = 22;

    for(int i=0;i<4;i++)
    {
        int y = startY + (i*10);

        if(i == indexVar)
        {
            Display_DrawFrame(10, y-8, 108, 10);
        }

        Display_Print(14, y, vars[i]);
    }
}

//------------------------------------------------
void ScreenGraphVar_Handle(EventType evt)
{
    switch(evt)
    {
        case EVT_NONE:
            break;

        case EVT_DOWN:
            if(indexVar < 3)
                indexVar++;
            Renderer_RequestDraw();
            break;

        case EVT_UP:
            if(indexVar > 0)
                indexVar--;
            Renderer_RequestDraw();
            break;

        case EVT_ENTER:
            System_SetState(ST_GRAPH);
            Renderer_RequestDraw();
            break;

        case EVT_LEFT:
            System_SetState(ST_GRAPH_CALENDAR);
            Renderer_RequestDraw();
            break;

        case EVT_BACK:
            // sugestão padrão:
            System_SetState(ST_MENU);
            Renderer_RequestDraw();
            break;

        case EVT_RIGHT:
            // não usado nesta tela
            break;

        case EVT_HOLD:
            // opcional: reset rápido, debug, etc.
            break;
    }
}