#include <Arduino.h>
#include <stdio.h>

#include "ScreenGraphCalendar.h"
#include "drivers/Display.h"
#include "Renderer.h"
#include "core/SystemState.h"
#include "services/SDService.h"
#include "services/RTCService.h"

static uint8_t month = 3;
static uint16_t year = 2026;
static uint8_t cursorDay = 1;


static bool initialized = false;

//--------------------------------------------

void ScreenGraphCalendar_Start()
{
    if (!initialized)
    {
        DateTime now = RTC_GetDateTime();

        cursorDay   = now.day();
        month = now.month();
        year  = now.year();

        initialized = true;
    }
}

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
// RTClib: 0 = domingo, 1 = segunda, ... 6 = sabado.
static uint8_t getFirstWeekday(uint8_t m, uint16_t y)
{
    DateTime firstDay(y, m, 1, 0, 0, 0);
    return firstDay.dayOfTheWeek();
}

//---------------------------------------------
static void getCalendarCell(uint8_t d, uint8_t m, uint16_t y, uint8_t *col, uint8_t *row)
{
    uint8_t index = getFirstWeekday(m, y) + d - 1;

    *col = index % 7;
    *row = index / 7;
}

//---------------------------------------------
static uint8_t findDayInColumn(uint8_t targetCol, uint8_t m, uint16_t y, bool last)
{
    uint8_t firstWeekday = getFirstWeekday(m, y);
    uint8_t maxDay = getDaysInMonth(m, y);
    int index;

    if (last)
    {
        index = ((firstWeekday + maxDay - 1) / 7) * 7 + targetCol;

        if (index > firstWeekday + maxDay - 1)
            index -= 7;
    }
    else
    {
        index = targetCol;

        if (index < firstWeekday)
            index += 7;
    }

    int day = index - firstWeekday + 1;

    if (day < 1)
        return 1;

    if (day > maxDay)
        return maxDay;

    return (uint8_t)day;
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
    ScreenGraphCalendar_Start();

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

    const char *weekDays[] =
        {
            "D", "S", "T", "Q", "Q", "S", "S"};

    const int startX = 5;
    const int weekY = 20;
    const int startY = 28;
    const int colW = 18;
    const int rowH = 7;

    U8G2 &u8g2 = Display_GetU8G2();
    u8g2.setFont(u8g2_font_tom_thumb_4x6_tr);

    for (uint8_t c = 0; c < 7; c++)
    {
        Display_Print(startX + c * colW + 4, weekY, weekDays[c]);
    }

    uint8_t maxDays = getDaysInMonth(month, year);

    for (uint8_t d = 1; d <= maxDays; d++)
    {
        char txt[3];
        sprintf(txt, "%02d", d);

        uint8_t col;
        uint8_t row;
        getCalendarCell(d, month, year, &col, &row);

        int x = startX + col * colW;
        int y = startY + row * rowH;

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
            Display_DrawBox(x - 1, y - 5, 12, 7);
            Display_GetU8G2().setDrawColor(0);
        }

        if (d == cursorDay)
        {
            Display_DrawFrame(x - 1, y - 6, 12, 8);
        }

        Display_Print(x, y, txt);

        if (hasData)
        {
            Display_GetU8G2().setDrawColor(1);
        }

    }

    Display_SetFontDefault();
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

    {
        uint8_t col;
        uint8_t row;
        getCalendarCell(cursorDay, month, year, &col, &row);

        if (cursorDay + 7 <= getDaysInMonth(month, year))
        {
            cursorDay += 7;
        }
        else
        {
            nextMonth();
            cursorDay = findDayInColumn(col, month, year, false);
        }

        Renderer_RequestDraw();
        break;
    }

    case EVT_UP:

    {
        uint8_t col;
        uint8_t row;
        getCalendarCell(cursorDay, month, year, &col, &row);

        if (row > 0)
        {
            cursorDay -= 7;
        }
        else
        {
            prevMonth();
            cursorDay = findDayInColumn(col, month, year, true);
        }

        Renderer_RequestDraw();
        break;
    }

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
