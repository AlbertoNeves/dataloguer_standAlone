#pragma once

#include <Arduino.h>
#include "core/EventQueue.h"

void ScreenCalibration_Start(uint8_t type);
bool ScreenCalibration_Handle(EventType evt);
void ScreenCalibration_Draw();