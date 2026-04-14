#include <Arduino.h>
#include <Arduino.h>
#include "EventQueue.h"

#define QUEUE_SIZE 10

static EventType queue[QUEUE_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

void Event_Push(EventType e)
{
    uint8_t next = (head + 1) % QUEUE_SIZE;

    if (next != tail)
    {
        queue[head] = e;
        head = next;
    }
}

EventType Event_Pop()
{
    if (tail == head)
        return EVT_NONE;

    EventType e = queue[tail];
    tail = (tail + 1) % QUEUE_SIZE;
    return e;
}

