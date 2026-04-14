#pragma once
#include <stdint.h>

// Inicialização (opcional, se quiser manter padrão do projeto)
void Calibration_Init();

// GET
int16_t Calibration_GetTempOffset();
int16_t Calibration_GetHumOffset();
int16_t Calibration_GetPressOffset();

// SET
void Calibration_SetTempOffset(int16_t v);
void Calibration_SetHumOffset(int16_t v);
void Calibration_SetPressOffset(int16_t v);