#include <Arduino.h>
#include "CalendarUI.h"

static uint8_t selectedDay = 1;
static uint8_t selectedMonth = 1;
static uint16_t selectedYear = 2026;

//------------------------------------------------
static bool isLeap(uint16_t y)
{
    if((y%4==0 && y%100!=0) || (y%400==0))
        return true;
    return false;
}

//------------------------------------------------
static uint8_t daysInMonth(uint16_t y,uint8_t m)
{
    const uint8_t days[]={31,28,31,30,31,30,31,31,30,31,30,31};

    if(m==2)
    {
        if(isLeap(y))
            return 29;
    }

    return days[m-1];
}

//------------------------------------------------
uint8_t Calendar_DayOfWeek(uint16_t y,uint8_t m,uint8_t d)
{
    if(m<3)
    {
        m+=12;
        y--;
    }

    uint16_t k=y%100;
    uint16_t j=y/100;

    uint8_t h=(d+13*(m+1)/5+k+k/4+j/4+5*j)%7;

    return ((h+6)%7);
}

//------------------------------------------------
void Calendar_Set(uint16_t y,uint8_t m)
{
    selectedYear=y;
    selectedMonth=m;
}

//------------------------------------------------
uint8_t Calendar_GetDay()
{
    return selectedDay;
}

//------------------------------------------------
void Calendar_NextDay()
{
    uint8_t maxDay=daysInMonth(selectedYear,selectedMonth);

    if(selectedDay<maxDay)
        selectedDay++;
}

//------------------------------------------------
void Calendar_PrevDay()
{
    if(selectedDay>1)
        selectedDay--;
}
