
#include "Graph.h"

static int16_t *buffer = nullptr;
static uint16_t bufferSize = 0;

static int16_t minVal;
static int16_t maxVal;

static const uint8_t originX = 18;
static const uint8_t originY = 53;

static const uint8_t topY = 4;
static const uint8_t width = 120;

static int16_t scaleMargin = 0;
static uint8_t valorX = 0;

//---------------------------------------------
void Graph_SetStartX(uint8_t v)
{
    valorX = v;
}
//---------------------------------------------
void Graph_ZoomIn()
{
    scaleMargin += 5;
}
//---------------------------------------------
void Graph_ZoomOut()
{
    if (scaleMargin > 0)
        scaleMargin -= 5;
}
//---------------------------------------------
void Graph_SetData(int16_t *data, uint16_t size)
{
    buffer = data;
    bufferSize = size;
}
//---------------------------------------------
void Graph_CalcScale()
{
    if (buffer == nullptr || bufferSize == 0)
        return;

    minVal = buffer[0];
    maxVal = buffer[0];

    for (uint16_t i = 1; i < bufferSize; i++)
    {
        if (buffer[i] < minVal)
            minVal = buffer[i];
        if (buffer[i] > maxVal)
            maxVal = buffer[i];
    }

    if (maxVal == minVal)
        maxVal++;

    minVal -= scaleMargin;
    maxVal += scaleMargin;
}
//---------------------------------------------
void Graph_DrawAxis(U8G2 &u8g2)
{
    // u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.setFont(u8g2_font_tom_thumb_4x6_mn);


    // eixo principal
    u8g2.drawLine(originX, originY, originX, topY);
    u8g2.drawLine(originX, originY, originX + width, originY);

    // -----------------------
    // EIXO Y (4 divisões)
    // -----------------------
    uint8_t marks = 4;

    // -----------------------
    // EIXO Y (Variavel)
    // -----------------------
    for (uint8_t i = 0; i <= marks; i++)
    {
        uint8_t y = originY - ((originY - topY) * i / marks);

        // tick
        u8g2.drawLine(originX - 2, y, originX, y);

        // valor
        int16_t val = map(i, 0, marks, minVal, maxVal);

        char txt[8];
        dtostrf(val / 10.0, 2, 1, txt);
        int w = u8g2.getStrWidth(txt);
        u8g2.drawStr(originX - 2 - w, y + 3, txt);

        // 8g2.drawStr(0, y + 3, txt);
    }

    // -----------------------
    // EIXO X (horas)
    // -----------------------

    uint8_t marksX = 6;

    for (uint8_t i = 0; i <= marksX; i++)
    {
        uint8_t x = originX + (width * i / marksX);

        // tick
        u8g2.drawLine(x, originY, x, originY + 2);

        char txt[4];

        sprintf(txt, "%d", valorX + i);

        // u8g2.drawStr(x - 4, originY + 10, txt);
        int w = u8g2.getStrWidth(txt);
        u8g2.drawStr(1+x - (w / 2), originY + 10, txt);
    }
}
//---------------------------------------------
void Graph_DrawData(U8G2 &u8g2)
{
    if (buffer == nullptr || bufferSize < 2)
        return;

    float step = (float)width / (bufferSize - 1);

    int prevX = originX;
    int prevY = map(buffer[0], minVal, maxVal, originY, topY);

    for (uint16_t i = 1; i < bufferSize; i++)
    {
        int x = originX + (int)(i * step);

        int y = map(
            buffer[i],
            minVal,
            maxVal,
            originY,
            topY);

        u8g2.drawLine(prevX, prevY, x, y);

        prevX = x;
        prevY = y;
    }
}
