#include "drivers/Display.h"
#include "core/SystemState.h"
#include "core/EventQueue.h"
#include "ScreenLogging.h"
#include "Renderer.h"
#include "services\Sensors.h"

void ScreenLogging_Draw()
{
    char vbatt[8];
    char temp[8];
    char umi[8];
    char pres[8];

    float v = Sensors_GetBatteryADC();
    dtostrf(v, 4, 2, vbatt);

    float t;
    float h;
    float p;

    // leitura do BME280
    if (Read_t_p_h(t, h, p))
    {
        dtostrf(t, 6, 2, temp);
        dtostrf(h, 6, 2, umi);
        dtostrf(p, 7, 1, pres);
    }
    else
    {
        strcpy(temp, "--");
        strcpy(umi, "--");
        strcpy(pres, "--");
    }

    Display_Print(0, 35, "Temp:");
    Display_Print(35, 35, temp);
    Display_Print(74, 35, "C");

    Display_Print(0, 47, "Hum :");
    Display_Print(35, 47, umi);
    Display_Print(74, 47, "%");

    Display_Print(0, 59, "V Bat:");
    Display_Print(46, 59, vbatt);
    Display_Print(74, 59, "V");
}

void HandleLogging(EventType evt)
{
    if (evt == EVT_ENTER) // exemplo: ENTER para sair
    {
        System_SetState(ST_HOME);
        Renderer_RequestDraw();
    }
}