#include <Arduino.h>
#pragma once
#include "core/EventQueue.h"

typedef void (*MenuCallback)();

struct MenuItem
{
    const char* label;
    MenuCallback action;
};

void Menu_Set(MenuItem *items, uint8_t count);
void Menu_Draw();
void Menu_Handle(EventType evt);