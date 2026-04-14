#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "core/EventQueue.h"

typedef struct
{
    uint8_t pin;          // pino do botão
    EventType evt;        // evento gerado
    bool repeat;          // permite auto-repeat

    uint8_t lastState;    // estado anterior
    uint32_t pressTime;   // tempo que pressionou
    uint32_t lastRepeat;  // ultimo repeat

} Button;

void Buttons_Init();
void Buttons_Update();

#endif