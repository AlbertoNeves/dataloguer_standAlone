#pragma once
#include <Arduino.h>

void Calibration_Load();
void Calibration_Save();

int16_t Calibration_GetTempOffset();
int16_t Calibration_GetHumOffset();
int16_t Calibration_GetPressOffset();

void Calibration_SetTempOffset(int16_t v);
void Calibration_SetHumOffset(int16_t v);
void Calibration_SetPressOffset(int16_t v);