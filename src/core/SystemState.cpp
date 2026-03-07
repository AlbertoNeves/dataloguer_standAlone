#include <Arduino.h>
#include "SystemState.h"

static SystemState currentState = ST_BOOT;

void System_SetState(SystemState s)
{
    currentState = s;
}

SystemState System_GetState()
{
    return currentState;
}