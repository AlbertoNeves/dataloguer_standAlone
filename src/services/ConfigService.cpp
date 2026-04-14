#include "ConfigService.h"
#include <EEPROM.h>
#include <string.h>

// ======================================================
// EEPROM BASE ADDRESS
// ======================================================
#define EEPROM_ADDR_CONFIG 0

// ======================================================
// STATIC CONFIG INSTANCE
// ======================================================
static DeviceConfig cfg;

// ======================================================
// DEFAULT CONFIG
// ======================================================
static void Config_SetDefaults()
{
    memset(&cfg, 0, sizeof(DeviceConfig));

    cfg.tempOffset  = 0;
    cfg.humOffset   = 0;
    cfg.pressOffset = 0;

    cfg.brightness = 128;
    cfg.contrast   = 128;

    cfg.logFlag = 0;
    cfg.logInterval = 60;

    cfg.version = CONFIG_VERSION;
}

// ======================================================
// VALIDATION (anti-lixo EEPROM)
// ======================================================
static bool Config_IsValid()
{
    if (cfg.version != CONFIG_VERSION) return false;

    if (cfg.tempOffset  < -500 || cfg.tempOffset  > 500) return false;
    if (cfg.humOffset   < -500 || cfg.humOffset   > 500) return false;
    if (cfg.pressOffset < -500 || cfg.pressOffset > 500) return false;

    if (cfg.brightness > 255) return false;
    if (cfg.contrast   > 255) return false;

    if (cfg.logInterval == 0 || cfg.logInterval > 86400) return false;

    return true;
}

// ======================================================
// LOAD
// ======================================================
void Config_Load()
{
    EEPROM.get(EEPROM_ADDR_CONFIG, cfg);

    if (!Config_IsValid())
    {
        Config_SetDefaults();
        EEPROM.put(EEPROM_ADDR_CONFIG, cfg);
    }
}

// ======================================================
// SAVE (com proteção de desgaste)
// ======================================================
void Config_Save()
{
    static DeviceConfig last;

    if (memcmp(&last, &cfg, sizeof(DeviceConfig)) != 0)
    {
        EEPROM.put(EEPROM_ADDR_CONFIG, cfg);
        last = cfg;
    }
}

// ======================================================
// ACCESS
// ======================================================
DeviceConfig* Config_Get()
{
    return &cfg;
}