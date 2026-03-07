#include <Arduino.h>
#include "Menu.h"
#include "Renderer.h"
#include "drivers/Display.h"

#define MAX_VISIBLE 5
#define LINE_HEIGHT 12

static MenuItem *menu;
static uint8_t itemCount;

static int cursorIndex = 0;
static int firstVisible = 0;

void Menu_Set(MenuItem *items, uint8_t count)
{
    menu = items;
    itemCount = count;
    cursorIndex = 0;
    firstVisible = 0;
}

static void adjustScroll()
{
    if(cursorIndex < firstVisible)
        firstVisible = cursorIndex;

    if(cursorIndex >= firstVisible + MAX_VISIBLE)
        firstVisible = cursorIndex - MAX_VISIBLE + 1;
}

void Menu_Draw()
{
    Display_Clear();

    for(int i=0;i<MAX_VISIBLE;i++)
    {
        int idx = firstVisible + i;
        if(idx >= itemCount) break;

        int y = 14 + (i * LINE_HEIGHT);

        if(idx == cursorIndex)
            Display_Print(0,y,">");
        else
            Display_Print(0,y," ");

        Display_Print(10,y,menu[idx].label);
    }

    Display_Update();
}

void Menu_Handle(EventType evt)
{
    switch(evt)
    {
        case EVT_DOWN:
            if(cursorIndex < itemCount-1)
                cursorIndex++;
            adjustScroll();
            Menu_Draw();
            break;

        case EVT_UP:
            if(cursorIndex > 0)
                cursorIndex--;
            adjustScroll();
            Menu_Draw();
            break;

        case EVT_ENTER:
            if(menu[cursorIndex].action)
                menu[cursorIndex].action();
				Renderer_RequestDraw();
            break;

        default:
            break;
    }
}