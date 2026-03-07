#include <Arduino.h>
#pragma once

enum SystemState
{
    ST_BOOT,
    ST_HOME,
    ST_MENU,

    ST_LOGGING,
    ST_GRAPH,

    ST_DISPLAY_ADJUST,
    ST_RTC_CONFIG,
    ST_ABOUT,

    ST_ERROR
};

void System_SetState(SystemState s);
SystemState System_GetState();