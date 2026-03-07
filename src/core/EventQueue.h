#include <Arduino.h>
#pragma once

enum EventType
{
    EVT_NONE,
    EVT_UP,
    EVT_DOWN,
    EVT_ENTER,
    EVT_BACK,
    EVT_LEFT,
    EVT_RIGHT,
    EVT_HOLD
};

void Event_Push(EventType e);
EventType Event_Pop();