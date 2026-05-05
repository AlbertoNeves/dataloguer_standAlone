#include "Buttons.h"
#include <Arduino.h>
#include "drivers/Display.h"

#define BTN_REPEAT_START 800
#define BTN_REPEAT_RATE 250
#define BTN_DEBOUNCE 250
#define BTN_HOLD_TIME 2000

#define BTN_HOME 2
#define BTN_DOWN 4
#define BTN_UP 6
#define BTN_LEFT 8
#define BTN_RIGHT 10

static Button buttons[] =
{
    {BTN_HOME, EVT_ENTER, false, HIGH, 0, 0, false},
    {BTN_DOWN, EVT_DOWN, true, HIGH, 0, 0, false},
    {BTN_UP, EVT_UP, true, HIGH, 0, 0, false},
    {BTN_LEFT, EVT_LEFT, true, HIGH, 0, 0, false},
    {BTN_RIGHT, EVT_RIGHT, true, HIGH, 0, 0, false}
};

static const uint8_t BTN_COUNT = sizeof(buttons) / sizeof(Button);

void Buttons_Init()
{
    for (uint8_t i = 0; i < BTN_COUNT; i++)
    {
        pinMode(buttons[i].pin, INPUT_PULLUP);
        buttons[i].lastState = digitalRead(buttons[i].pin);
    }
}

void Buttons_Update()
{
    uint32_t now = millis();

    for (uint8_t i = 0; i < BTN_COUNT; i++)
    {
        Button &b = buttons[i];
        uint8_t state = digitalRead(b.pin);

        if (b.lastState == HIGH && state == LOW)
        {
            if (now - b.lastRepeat > BTN_DEBOUNCE)
            {
                b.pressTime = now;
                b.lastRepeat = now;
                b.holdSent = false;
                Display_UserActivity();
            }
        }

        if (!b.repeat && state == LOW && !b.holdSent)
        {
            if (now - b.pressTime > BTN_HOLD_TIME)
            {
                b.holdSent = true;
                Event_Push(EVT_HOLD);
                Display_UserActivity();
            }
        }

        if (b.repeat && state == LOW)
        {
            if (now - b.pressTime > BTN_REPEAT_START)
            {
                if (now - b.lastRepeat > BTN_REPEAT_RATE)
                {
                    b.lastRepeat = now;
                    Event_Push(b.evt);
                }
            }
        }

        if (b.lastState == LOW && state == HIGH)
        {
            if (!b.holdSent)
                Event_Push(b.evt);

            b.holdSent = false;
        }

        b.lastState = state;
    }
}
