#include <Arduino.h>
#pragma once


class Timer
{
public:
    Timer(uint32_t interval = 1000)
    {
        _interval = interval;
        _last = 0;
    }

    bool elapsed()
    {
        uint32_t now = millis();

        if (now - _last >= _interval)
        {
            _last = now;
            return true;
        }
        return false;
    }

private:
    uint32_t _last;
    uint32_t _interval;
};