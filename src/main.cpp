#include <Arduino.h>
#include <SdFat.h>

// CORE
#include "core/SystemState.h"
#include "core/EventQueue.h"

// DRIVERS
#include "drivers/Buttons.h"
#include "drivers/Display.h"

// SERVICES
#include "services/RTCService.h"
#include "services/LogEngine.h"
#include "services/Sensors.h"
#include "services/SDService.h"
#include "services/SerialService.h"
#include "services/CalibrationService.h"

// UI
#include "ui/UI.h"
#include "ui/Renderer.h"
#include "ui/ScreenCalibration.h"
#include "ui/Graph.h"

void setup()
{
    Serial.begin(115200);
    Calibration_Load();
    SerialService_Init();
    Display_Init();
    Buttons_Init();
    RTC_Init();
    SD_Init();
    Sensors_init();
    Sensors_Warmup();
    Graph_Init();

    for (uint8_t i = 0; i < 3; i++)
        ScreenCalibration_Start(i);

    Log_Init();
    UI_Init();
    Renderer_Init();
    System_SetState(ST_HOME);
    Renderer_RequestDraw();
}

void loop()
{
    Buttons_Update();
    UI_Update();
    Log_Update();
    Renderer_Update();
    SerialService_Update();
}
