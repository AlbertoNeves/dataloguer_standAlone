#include <Wire.h>
#include <RTClib.h>

static RTC_DS1307 rtc;
static char timeBuffer[9] = "00:00:00";
static uint32_t lastUpdate = 0;
//------------------------------------------------
void RTC_SetDateTime(const DateTime& dt)
{
    rtc.adjust(dt);
}
//------------------------------------------------
void RTC_Init()
{
    Wire.begin();

    if(!rtc.begin())
    {
        strcpy(timeBuffer, "RTC ERR");
        return;
    }

    if(!rtc.isrunning())
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}
//------------------------------------------------
void RTC_Update()
{
    uint32_t now = millis();

    if(now - lastUpdate >= 1000)
    {
        lastUpdate = now;

        DateTime t = rtc.now();
        sprintf(timeBuffer,"%02d:%02d:%02d",t.hour(),t.minute(),t.second());
    }
}
//------------------------------------------------
void RTC_GetTime(char* buffer)
{
    strcpy(buffer, timeBuffer);
}

DateTime RTC_GetDateTime()
{
    return rtc.now();
}