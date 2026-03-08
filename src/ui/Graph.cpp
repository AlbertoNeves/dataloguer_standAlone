#include "Graph.h"

static int16_t *buffer = nullptr;
static uint16_t bufferSize = 0;

static int16_t minVal;
static int16_t maxVal;

static const uint8_t originX = 18;
static const uint8_t originY = 53;

static const uint8_t topY = 4; //topo do eixo Y.
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

    // aplica margem da escala
    minVal -= scaleMargin;
    maxVal += scaleMargin;
}
//---------------------------------------------
void Graph_DrawAxis(U8G2 &u8g2)
{
    u8g2.setFont(u8g2_font_5x7_tr);

    u8g2.drawLine(originX, originY, originX, topY);
    u8g2.drawLine(originX, originY, originX + width, originY);

    // Eixo Y
    uint8_t marks = 4;

    for (uint8_t i = 0; i <= marks; i++)
    {
        uint8_t y = originY - ((originY - topY) * i / marks);

        u8g2.drawLine(originX - 2, y, originX, y);

        int16_t val = map(i, 0, marks, minVal, maxVal);

        char txt[6];
        dtostrf(val / 10.0, 2, 0, txt);
        u8g2.drawStr(0, y + 3, txt);
        /*
        2️⃣ dtostrf(val / 10.0, 4, 0, txt);
        Essa função converte float → string.
        Nome da função:
        d to str f
        double to string (float)
        Formato:
        dtostrf(valor, largura, casas_decimais, buffer);
        No seu caso:
        dtostrf(val / 10.0, 4, 0, txt);
        parâmetro	significado
        val / 10.0	valor real da temperatura
        4	largura mínima do texto
        0	casas decimais
        txt	onde o texto será guardado
        */
    }

    // Eixo X
    for (uint8_t i = 0; i <= 6; i++)
    {
        uint8_t x = originX + (width * i / 6);

        u8g2.drawLine(x, originY, x, originY + 2);

        char txt[4];
        sprintf(txt, "%d", valorX + i);

        u8g2.drawStr(x - 4, originY + 10, txt);
    }
}
//---------------------------------------------
void Graph_DrawData(U8G2 &u8g2)
{
    if (buffer == nullptr)
        return;

    int prevX = originX;
    int prevY = originY;

    for (uint16_t i = 0; i < bufferSize; i++)
    {
        int16_t v = buffer[i];

        uint8_t y = map(
            v,
            minVal,
            maxVal,
            originY,
            topY);

        uint8_t x = originX + i;

        if (i > 0)
            u8g2.drawLine(prevX, prevY, x, y);

        prevX = x;
        prevY = y;
    }
}
//---------------------------------------------