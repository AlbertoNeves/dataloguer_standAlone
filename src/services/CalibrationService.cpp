#include "CalibrationService.h"
#include "ConfigService.h"

// ======================================================
// INIT (opcional - mantém padrão do teu projeto)
// ======================================================
void Calibration_Init()
{
    // Nada a fazer por enquanto
    // Config já é carregado via Config_Load()
}

// ======================================================
// GETTERS
// ======================================================
int16_t Calibration_GetTempOffset()
{
    return Config_Get()->tempOffset;
}

int16_t Calibration_GetHumOffset()
{
    return Config_Get()->humOffset;
}

int16_t Calibration_GetPressOffset()
{
    return Config_Get()->pressOffset;
}

// ======================================================
// SETTERS (com proteção de escrita)
// ======================================================
void Calibration_SetTempOffset(int16_t v)
{
    DeviceConfig* cfg = Config_Get();

    if (cfg->tempOffset != v)
    {
        cfg->tempOffset = v;
        Config_Save();
    }
}

void Calibration_SetHumOffset(int16_t v)
{
    DeviceConfig* cfg = Config_Get();

    if (cfg->humOffset != v)
    {
        cfg->humOffset = v;
        Config_Save();
    }
}

void Calibration_SetPressOffset(int16_t v)
{
    DeviceConfig* cfg = Config_Get();

    if (cfg->pressOffset != v)
    {
        cfg->pressOffset = v;
        Config_Save();
    }
}