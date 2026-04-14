#include "ScreenCalibration.h"
#include "drivers/Display.h"
#include "services/CalibrationService.h"
#include "core/SystemState.h"
#include "Renderer.h"

static uint8_t calibType;
static int16_t value;

#define TEMP_OFFSET_LIMIT 100  // ±10.0 C
#define HUM_OFFSET_LIMIT 100   // ±10.0 %
#define PRESS_OFFSET_LIMIT 800 // ±80 hPa
//------------------------------------------------------
static int16_t GetLimit()
{
    if (calibType == 0)
        return TEMP_OFFSET_LIMIT;
    if (calibType == 1)
        return HUM_OFFSET_LIMIT;
    if (calibType == 2)
        return PRESS_OFFSET_LIMIT;

    return 0;
}
//------------------------------------------------------
void ScreenCalibration_Start(uint8_t type)
{
    calibType = type;

    if (type == 0)
        value = Calibration_GetTempOffset();
    if (type == 1)
        value = Calibration_GetHumOffset();
    if (type == 2)
        value = Calibration_GetPressOffset();

    Renderer_RequestDraw();
}
//------------------------------------------------------
void ScreenCalibration_Draw()
{
    char buf[10];

    float v = value / 10.0f;

    dtostrf(v, 5, 1, buf);

    if (calibType == 0)
        Display_Print(6, 10, "Offset Temperatura");

    if (calibType == 1)
        Display_Print(6, 10, "Offset Umidade");

    if (calibType == 2)
        Display_Print(6, 10, "Offset Pressao");

    Display_Print(46, 40, buf);
    Display_SetFontDefault();
}
//------------------------------------------------------
bool ScreenCalibration_Handle(EventType evt)
{
    switch (evt)
    {
    case EVT_UP:
    {
        int16_t limit = GetLimit();
        if (value < limit)
            value++;

        Renderer_RequestDraw();
        break;
    }

    case EVT_DOWN:
    {
        int16_t limit = GetLimit();
        if (value > -limit)
            value--;

        Renderer_RequestDraw();
        break;
    }

    case EVT_ENTER:
    {
        if (calibType == 0)
            Calibration_SetTempOffset(value);
        if (calibType == 1)
            Calibration_SetHumOffset(value);
        if (calibType == 2)
            Calibration_SetPressOffset(value);

        System_SetState(ST_MENU);
        Renderer_RequestDraw();
        break;
    }

    case EVT_LEFT:
    {
        System_SetState(ST_MENU);
        Renderer_RequestDraw();
        break;
    }
    default:
        break;
    }

    return true;
}
//------------------------------------------------------