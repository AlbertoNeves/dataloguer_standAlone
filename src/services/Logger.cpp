#include "Logger.h"
#include "ConfigService.h"

// ======================================================
// START
// ======================================================
void Logger_Start()
{
    DeviceConfig* cfg = Config_Get();

    if (cfg->logFlag != 1)
    {
        cfg->logFlag = 1;
        Config_Save();
    }
}

// ======================================================
// STOP
// ======================================================
void Logger_Stop()
{
    DeviceConfig* cfg = Config_Get();

    if (cfg->logFlag != 0)
    {
        cfg->logFlag = 0;
        Config_Save();
    }
}

// ======================================================
// INIT
// ======================================================
void Logger_Init()
{
    DeviceConfig* cfg = Config_Get();

    // Aqui você decide o comportamento
    // Exemplo: iniciar automaticamente se estava ativo

    if (cfg->logFlag == 1)
    {
        // Logger_StartHardware(); // se tiver
    }
}