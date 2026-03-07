#include "Graph.h"

static int16_t *graphBuffer = NULL;
static uint16_t graphSize = 0;

static int16_t escala_Y_min = 0;
static int16_t escala_Y_max = 0;

static uint8_t originX = 12;
static uint8_t originY = 53;
static uint8_t topoGrafico = 13;

void Graph_SetData(int16_t *data, uint16_t size)
{
    graphBuffer = data;
    graphSize = size;
}

void Graph_CalcScale()
{
    if(graphBuffer == NULL || graphSize == 0)
        return;

    int16_t minVal = graphBuffer[0];
    int16_t maxVal = graphBuffer[0];

    for(uint16_t i=1;i<graphSize;i++)
    {
        if(graphBuffer[i] < minVal) minVal = graphBuffer[i];
        if(graphBuffer[i] > maxVal) maxVal = graphBuffer[i];
    }

    escala_Y_min = minVal;
    escala_Y_max = maxVal;

    if(escala_Y_max == escala_Y_min)
        escala_Y_max++;
}

void Graph_DrawAxis(U8G2 &u8g2)
{
    uint8_t larguraGrafico = 120;

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setDrawColor(1);

    u8g2.drawLine(originX, originY, originX, topoGrafico);
    u8g2.drawLine(originX, originY, originX + larguraGrafico, originY);

    uint8_t qtde = 5;

    for(uint8_t i=0;i<=qtde;i++)
    {
        uint8_t y = originY - ((originY - topoGrafico) * i / qtde);

        u8g2.drawLine(originX-2,y,originX,y);

        int16_t valor = map(i,0,qtde,escala_Y_min,escala_Y_max);

        u8g2.setCursor(0,y+3);
        u8g2.print(valor/10.0);
    }
}

void Graph_DrawData(U8G2 &u8g2)
{
    if(graphBuffer == NULL)
        return;

    for(uint16_t i=0;i<graphSize;i++)
    {
        int16_t val = graphBuffer[i];

        uint8_t pixelY = map(
            val,
            escala_Y_min,
            escala_Y_max,
            originY,
            topoGrafico
        );

        uint8_t pixelX = originX + i;

        u8g2.drawPixel(pixelX,pixelY);
    }
}