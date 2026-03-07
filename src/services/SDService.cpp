#include "SDService.h"
#include <Arduino.h>

SdFat SD;

bool SD_Init()
{
    if(!SD.begin(53))
    {
        Serial.println("SD FAIL");
        return false;
    }

    Serial.println("SD OK");
    return true;
}

bool SD_Check()
{
    FsFile test = SD.open("sdtest.tmp", O_WRITE | O_CREAT);

    if(!test)
        return false;

    test.close();
    SD.remove("sdtest.tmp");

    return true;
}