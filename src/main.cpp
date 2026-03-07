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

// UI
#include "ui/UI.h"
#include "ui/Renderer.h"
//---------------------------------------------------
void setup()
{
    Serial.begin(115200);
SerialService_Init();
    Display_Init();
    Buttons_Init();
    RTC_Init();
    SD_Init();  
    Sensors::init();
    Log_Init();
    UI_Init();
    Renderer_Init();
    System_SetState(ST_HOME);
    Renderer_RequestDraw();
}

void loop()
{
    // 1. Ler hardware (gera eventos)
    Buttons_Update();

  // 2. Processar UI e estados
    UI_Update();

    Log_Update();

    // 4. Renderer decide se redesenha
    Renderer_Update();

    // 5. funcao serial
    SerialService_Update();
}