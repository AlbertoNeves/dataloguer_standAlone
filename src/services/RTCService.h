#pragma once
#include <Arduino.h>
#include <RTClib.h>

void RTC_Init();
void RTC_Update();

void RTC_GetTime(char* buffer);   // "HH:MM:SS"

DateTime RTC_GetDateTime();

void RTC_SetDateTime(const DateTime& dt);

