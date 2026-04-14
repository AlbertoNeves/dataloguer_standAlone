#pragma once
#include <Arduino.h>
#include "core/EventQueue.h"

void RTCAdjust_Start();
void RTCAdjust_Handle(EventType evt);
void RTCAdjust_Draw();

bool RTCAdjust_IsActive();