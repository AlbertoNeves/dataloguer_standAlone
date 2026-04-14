#include "RTCAdjust.h"
#include "Renderer.h"
#include "services/RTCService.h"
#include "drivers/Display.h"
#include "core/SystemState.h"
#include "ui/Layout.h"

enum Field
{
    FIELD_YEAR,
    FIELD_MONTH,
    FIELD_DAY,
    FIELD_HOUR,
    FIELD_MINUTE,
    FIELD_CONFIRM,
    FIELD_CANCEL
};

#define MAX_VISIBLE 5
#define LINE_HEIGHT 10

static bool active = false;
static Field field = FIELD_YEAR;

static int firstVisible = 0;

static int year;
static int month;
static int day;
static int hour;
static int minute;

static void adjustScroll();

//---------------------------------------------------
bool RTCAdjust_IsActive()
{
    return active;
}
//---------------------------------------------------
void RTCAdjust_Start()
{
    DateTime now = RTC_GetDateTime();
    firstVisible = 0;
    year = now.year();
    month = now.month();
    day = now.day();
    hour = now.hour();
    minute = now.minute();

    field = FIELD_YEAR;
    active = true;

    Renderer_RequestDraw();
}
//--------------------------------------------------
static void applyRTC()
{
    DateTime dt(year, month, day, hour, minute, 0);
    RTC_SetDateTime(dt);
}
//--------------------------------------------------
static void nextField()
{
    field = (Field)((field + 1) % 7);
    adjustScroll();
}
//--------------------------------------------------
static void prevField()
{
    field = (Field)((field + 6) % 7);
    adjustScroll();
}
//---------------------------------------------------
static void increment()
{
    switch (field)
    {
    case FIELD_YEAR:
        year++;
        if (year > 2099)
            year = 2000;
        break;
    case FIELD_MONTH:
        month++;
        if (month > 12)
            month = 1;
        break;
    case FIELD_DAY:
        day++;
        if (day > 31)
            day = 1;
        break;
    case FIELD_HOUR:
        hour++;
        if (hour > 23)
            hour = 0;
        break;
    case FIELD_MINUTE:
        minute++;
        if (minute > 59)
            minute = 0;
        break;
    default:
        break;
    }
}
//---------------------------------------------------
static void decrement()
{
    switch (field)
    {
    case FIELD_YEAR:
        year--;
        if (year < 2000)
            year = 2099;
        break;
    case FIELD_MONTH:
        month--;
        if (month < 1)
            month = 12;
        break;
    case FIELD_DAY:
        day--;
        if (day < 1)
            day = 31;
        break;
    case FIELD_HOUR:
        hour--;
        if (hour < 0)
            hour = 23;
        break;
    case FIELD_MINUTE:
        minute--;
        if (minute < 0)
            minute = 59;
        break;
    default:
        break;
    }
}
//---------------------------------------------------
void RTCAdjust_Handle(EventType evt)
{
    if (!active)
        return;

    switch (evt)
    {
    case EVT_UP: // campo anterior
        prevField();
        Renderer_RequestDraw();
        break;

    case EVT_DOWN: // próximo campo
        nextField();
        Renderer_RequestDraw();
        break;

    case EVT_RIGHT: // aumentar valor
        increment();
        Renderer_RequestDraw();
        break;

    case EVT_LEFT: // diminuir valor
        decrement();
        Renderer_RequestDraw();
        break;

    case EVT_ENTER:

        if (field == FIELD_CONFIRM)
        {
            applyRTC();
            active = false;
            System_SetState(ST_MENU);
        }

        else if (field == FIELD_CANCEL)
        {
            active = false;
            System_SetState(ST_MENU);
        }

        Renderer_RequestDraw();
        break;

    default:
        break;
    }
}
//--------------------------------------------------
static void adjustScroll()
{
    if (field < firstVisible)
        firstVisible = field;

    if (field >= firstVisible + MAX_VISIBLE)
        firstVisible = field - MAX_VISIBLE + 1;
}
//---------------------------------------------------
void RTCAdjust_Draw()
{
    const char *labels[] =
        {
            "ANO",
            "MES",
            "DIA",
            "HORA",
            "MIN",
            "CONFIRMAR",
            "CANCELAR"};

    for (int i = 0; i < MAX_VISIBLE; i++)
    {
        int idx = firstVisible + i;
        if (idx > FIELD_CANCEL)
            break;

        int y = CONTENT_Y + (i * LINE_HEIGHT);

        char line[24];

        if (idx <= FIELD_MINUTE)
        {
            int value;

            switch (idx)
            {
            case FIELD_YEAR:
                value = year;
                break;
            case FIELD_MONTH:
                value = month;
                break;
            case FIELD_DAY:
                value = day;
                break;
            case FIELD_HOUR:
                value = hour;
                break;
            case FIELD_MINUTE:
                value = minute;
                break;
            }

            sprintf(line, "%c %s: %02d",
                    idx == field ? '>' : ' ',
                    labels[idx],
                    value);

            Display_Print(0, y, line);
        }
        else
        {
            sprintf(line, "%c %s",
                    idx == field ? '>' : ' ',
                    labels[idx]);

            Display_Print(0, y, line);
        }
    }
}