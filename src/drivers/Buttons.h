#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "core/EventQueue.h"

typedef struct
{
    uint8_t pin;
    EventType evt;
    bool repeat;

    uint8_t lastState;
    uint32_t pressTime;
    uint32_t lastRepeat;
    bool holdSent;

} Button;

void Buttons_Init();
void Buttons_Update();

#endif
