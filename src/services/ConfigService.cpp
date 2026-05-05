#include "ConfigService.h"
#include <EEPROM.h>
#include <string.h>

#define EEPROM_ADDR_CONFIG 0

static DeviceConfig cfg;

static void Config_SetDefaults()
{
    memset(&cfg, 0, sizeof(DeviceConfig));
    cfg.brightness = 128;
    cfg.contrast   = 128;
    cfg.logInterval = 60;
    cfg.version = CONFIG_VERSION;
}

static bool Config_IsValid()
{
    if (cfg.version != CONFIG_VERSION) return false;
    if (cfg.tempOffset < -500 || cfg.tempOffset > 500) return false;
    if (cfg.humOffset < -500 || cfg.humOffset > 500) return false;
    if (cfg.pressOffset < -500 || cfg.pressOffset > 500) return false;
    if (cfg.logInterval == 0 || cfg.logInterval > 86400) return false;
    return true;
}

void Config_Load()
{
    EEPROM.get(EEPROM_ADDR_CONFIG, cfg);

    if (!Config_IsValid())
    {
        Config_SetDefaults();
        EEPROM.put(EEPROM_ADDR_CONFIG, cfg);
    }
}

void Config_Save()
{
    static DeviceConfig last;

    if (memcmp(&last, &cfg, sizeof(DeviceConfig)) != 0)
    {
        EEPROM.put(EEPROM_ADDR_CONFIG, cfg);
        last = cfg;
    }
}

DeviceConfig* Config_Get()
{
    return &cfg;
}
