#include <Arduino.h>
#pragma once

#include "core/EventQueue.h"
#include "core/Config.h"


void DisplayAdjust_Init();
void DisplayAdjust_Draw();
bool DisplayAdjust_Handle(EventType evt);