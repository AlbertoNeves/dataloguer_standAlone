#pragma once

#include <Arduino.h>
#include <U8g2lib.h>

#define GRAPH_POINTS 120

void Graph_Init();
void Graph_SetData(int16_t *data, uint16_t size);
void Graph_SetMarkers(bool *data, uint16_t size);
void Graph_SetValueDivisor(uint8_t divisor);
void Graph_SetAutoScale(bool enabled);
bool Graph_IsAutoScale();

void Graph_CalcScale();

void Graph_DrawAxis(U8G2 &u8g2);

void Graph_DrawData(U8G2 &u8g2);

void Graph_ZoomIn();

void Graph_ZoomOut();

void Graph_SetStartX(uint8_t v);
void Graph_SetXAxis(uint16_t start, uint16_t samples);
void Graph_SetFixedXSpacing(bool enabled);
void Graph_SetXAxisLabels(const char *first, const char *last);
