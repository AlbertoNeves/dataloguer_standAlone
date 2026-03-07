#include "Buttons.h"
#include <Arduino.h>
#include "drivers/Display.h"
/*
    Driver de botões orientado a tabela
    -----------------------------------
    - suporta múltiplos botões
    - repeat opcional
    - baseado em millis()
    - integração com EventQueue
*/

#define BTN_REPEAT_START 600 // ms até iniciar repeat
#define BTN_REPEAT_RATE 250  // ms entre repeats
#define BTN_DEBOUNCE 100     // ms mínimo entre cliques

// Definição dos pinos
#define BTN_HOME 2
#define BTN_DOWN 4
#define BTN_UP 6
#define BTN_LEFT 8
#define BTN_RIGHT 10

/*-----------------------------
Adicionar botão novo

Basta incluir na tabela:

{12, EVT_MENU, false, HIGH, 0, 0}
+++++++++++++++++++++++++++++++++++++
Estrutura do botão

No  Buttons.h  definido:

typedef struct
{
    uint8_t pin;
    EventType evt;
    bool repeat;

    uint8_t lastState;
    uint32_t pressTime;
    uint32_t lastRepeat;

} Button;

Portanto cada botão tem 6 campos.
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ao adicionar a linha abaixo

{12, EVT_MENU, false, HIGH, 0, 0}

Significa:

valor	campo	significado
12	pin	botão conectado no pino digital 12
EVT_MENU	evt	evento enviado para EventQueue
false	repeat	não permite auto-repeat
HIGH	lastState	estado inicial (pullup → solto)
0	pressTime	tempo do clique (inicializado)
0	lastRepeat	controle do repeat
-----------------------------*/

// Tabela de configuração dos botões
static Button buttons[] =
    {
        {BTN_HOME, EVT_ENTER, false, HIGH, 0, 0},
        {BTN_DOWN, EVT_DOWN, true, HIGH, 0, 0},
        {BTN_UP, EVT_UP, true, HIGH, 0, 0},
        {BTN_LEFT, EVT_LEFT, true, HIGH, 0, 0},
        {BTN_RIGHT, EVT_RIGHT, true, HIGH, 0, 0}};

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

        // detecta clique
        if (b.lastState == HIGH && state == LOW)
        {
            if (now - b.lastRepeat > BTN_DEBOUNCE)
            {
                b.pressTime = now;
                b.lastRepeat = now;

                Event_Push(b.evt);
                Display_UserActivity();
            }
        }
        // auto-repeat
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

        b.lastState = state;
    }
}