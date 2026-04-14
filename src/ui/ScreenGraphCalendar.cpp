#include <Arduino.h>
#include <stdio.h>

#include "ScreenGraphCalendar.h"
#include "drivers/Display.h"
#include "Renderer.h"
#include "core/SystemState.h"
#include "services/SDService.h"

static uint8_t month = 3;
static uint16_t year = 2026;

static uint8_t cursorDay = 1;

//---------------------------------------------
// Dias do mês (com ano bissexto)
static uint8_t getDaysInMonth(uint8_t m, uint16_t y)
{
    switch (m)
    {
    case 1:
        return 31;
    case 2:
        if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
            return 29;
        return 28;
    case 3:
        return 31;
    case 4:
        return 30;
    case 5:
        return 31;
    case 6:
        return 30;
    case 7:
        return 31;
    case 8:
        return 31;
    case 9:
        return 30;
    case 10:
        return 31;
    case 11:
        return 30;
    case 12:
        return 31;
    }
    return 30;
}

//---------------------------------------------
static void nextMonth()
{
    month++;

    if (month > 12)
    {
        month = 1;
        year++;
    }
}

//---------------------------------------------
static void prevMonth()
{
    if (month == 1)
    {
        month = 12;
        year--;
    }
    else
    {
        month--;
    }
}

//---------------------------------------------
void ScreenGraphCalendar_Draw()
{
    // nomes dos meses
    const char *months[] =
        {
            "JAN", "FEV", "MAR", "ABR", "MAI", "JUN",
            "JUL", "AGO", "SET", "OUT", "NOV", "DEZ"};

    // cabeçalho
    char header[16];
    sprintf(header, "%s %d", months[month - 1], year);

    Display_Print(30, 9, header);

    // linha separadora
    Display_DrawHLine(0, 11, 128);

    int startX = 5;
    int startY = 22;

    int x = startX;
    int y = startY;

    uint8_t maxDays = getDaysInMonth(month, year);

    for (uint8_t d = 1; d <= maxDays; d++)
    {
        char txt[3];
        sprintf(txt, "%02d", d);

        bool hasData = false;
        /*
                if (SDService_FileExists('T', d, month, year) ||
                    SDService_FileExists('H', d, month, year) ||
                    SDService_FileExists('P', d, month, year) ||
                    SDService_FileExists('B', d, month, year))
                {
                    hasData = true;
                }
        */

        if (hasData)
        {
            Display_DrawBox(x - 2, y - 8, 14, 10);
            Display_GetU8G2().setDrawColor(0);
        }

        if (d == cursorDay)
        {
            Display_DrawFrame(x - 2, y - 9, 14, 10);
        }

        Display_Print(x, y, txt);
        /*
        if (hasData)
                {
                    Display_GetU8G2().setDrawColor(1);
                }
        */

        x += 18;

        if (d % 7 == 0)
        {
            x = startX;
            y += 9;
        }
    }
}

//---------------------------------------------
void ScreenGraphCalendar_Handle(EventType evt)
{
    switch (evt)
    {

    case EVT_RIGHT:

        if (cursorDay < getDaysInMonth(month, year))
        {
            cursorDay++;
        }
        else
        {
            nextMonth();
            cursorDay = 1;
        }

        Renderer_RequestDraw();
        break;

    case EVT_LEFT:

        if (cursorDay > 1)
        {
            cursorDay--;
        }
        else
        {
            prevMonth();
            cursorDay = getDaysInMonth(month, year);
        }

        Renderer_RequestDraw();
        break;

    case EVT_DOWN:

        if (cursorDay + 7 <= getDaysInMonth(month, year))
        {
            cursorDay += 7;
        }
        else
        {
            nextMonth();
            cursorDay = (cursorDay + 7) % getDaysInMonth(month, year);
            if (cursorDay == 0)
                cursorDay = 1;
        }

        Renderer_RequestDraw();
        break;

    case EVT_UP:

        if (cursorDay > 7)
        {
            cursorDay -= 7;
        }
        else
        {
            prevMonth();
            uint8_t maxDay = getDaysInMonth(month, year);
            cursorDay = maxDay - (7 - cursorDay);
        }

        Renderer_RequestDraw();
        break;

    case EVT_ENTER:

        System_SetState(ST_GRAPH_VARIABLE);
        Renderer_RequestDraw();
        break;

    case EVT_BACK:

        System_SetState(ST_MENU);
        Renderer_RequestDraw();
        break;

    default:
        break;
    }
}

//---------------------------------------------
uint8_t ScreenGraphCalendar_GetDay()
{
    return cursorDay;
}

uint8_t ScreenGraphCalendar_GetMonth()
{
    return month;
}

uint16_t ScreenGraphCalendar_GetYear()
{
    return year;
}