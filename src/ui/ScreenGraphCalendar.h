#pragma once

#include "core/EventQueue.h"

void ScreenGraphCalendar_Draw();
void ScreenGraphCalendar_Handle(EventType evt);

uint8_t ScreenGraphCalendar_GetDay();
uint8_t ScreenGraphCalendar_GetMonth();
uint16_t ScreenGraphCalendar_GetYear();