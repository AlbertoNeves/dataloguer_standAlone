#include "CalibrationService.h"
#include <EEPROM.h>
#include "core\Config.h"



struct CalibData
{
    int16_t tempOffset;
    int16_t humOffset;
    int16_t pressOffset;
};

static CalibData calib;

void Calibration_Load()
{
    EEPROM.get(EEPROM_ADDR, calib);

    if(calib.tempOffset > 500 || calib.tempOffset < -500)
    {
        calib.tempOffset = 0;
        calib.humOffset = 0;
        calib.pressOffset = 0;
    }
}

void Calibration_Save()
{
    EEPROM.put(EEPROM_ADDR, calib);
}

int16_t Calibration_GetTempOffset() { return calib.tempOffset; }
int16_t Calibration_GetHumOffset() { return calib.humOffset; }
int16_t Calibration_GetPressOffset() { return calib.pressOffset; }

void Calibration_SetTempOffset(int16_t v)
{
    calib.tempOffset = v;
    Calibration_Save();
}

void Calibration_SetHumOffset(int16_t v)
{
    calib.humOffset = v;
    Calibration_Save();
}

void Calibration_SetPressOffset(int16_t v)
{
    calib.pressOffset = v;
    Calibration_Save();
}