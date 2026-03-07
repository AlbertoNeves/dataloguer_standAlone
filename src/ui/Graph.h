#ifndef GRAPH_H
#define GRAPH_H

#include <Arduino.h>
#include <U8g2lib.h>

#define GRAPH_POINTS 120

void Graph_SetData(int16_t *data, uint16_t size);

void Graph_CalcScale();

void Graph_DrawAxis(U8G2 &u8g2);
void Graph_DrawData(U8G2 &u8g2);

#endif