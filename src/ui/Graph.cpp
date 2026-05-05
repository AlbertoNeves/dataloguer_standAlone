#include "Graph.h"
#include <EEPROM.h>
#include "core/Config.h"

static int16_t *buffer = nullptr;
static uint16_t bufferSize = 0;
static bool *markerBuffer = nullptr;
static uint16_t markerSize = 0;

static int16_t minVal;
static int16_t maxVal;
static bool autoScaleEnabled = true;
static bool scaleInitialized = false;

static const uint8_t originX = 18;
static const uint8_t originY = 53;

static const uint8_t topY = 4;
static const uint8_t width = 120;

static int16_t scaleMargin = 0;
static const int16_t scaleStep = 5;
static uint16_t valorX = 0;
static uint16_t xSamples = 7;
static char xFirstLabel[7] = "";
static char xLastLabel[7] = "";
static uint8_t valueDivisor = 10;
static bool fixedXSpacing = false;

static void Graph_SaveAutoScale()
{
    EEPROM.update(EEPROM_GRAPH_AUTO_SCALE, autoScaleEnabled ? 1 : 0);
}

static void Graph_LoadAutoScale()
{
    uint8_t raw = EEPROM.read(EEPROM_GRAPH_AUTO_SCALE);

    if (raw > 1)
    {
        autoScaleEnabled = true;
        Graph_SaveAutoScale();
        return;
    }

    autoScaleEnabled = (raw == 1);
}

static void Graph_ApplyAutoScaleModel()
{
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
    scaleInitialized = true;
}

static void Graph_EnsureScale()
{
    if (!scaleInitialized && buffer != nullptr && bufferSize > 0)
        Graph_ApplyAutoScaleModel();
}

static int Graph_MapY(int16_t value)
{
    return map(value, minVal, maxVal, originY, topY);
}

static bool Graph_ClipYLine(int &x0, int &y0, int &x1, int &y1)
{
    const int minY = topY;
    const int maxY = originY;

    if ((y0 < minY && y1 < minY) || (y0 > maxY && y1 > maxY))
        return false;

    if (y0 == y1)
        return (y0 >= minY && y0 <= maxY);

    if (y0 < minY)
    {
        x0 += (long)(x1 - x0) * (minY - y0) / (y1 - y0);
        y0 = minY;
    }
    else if (y0 > maxY)
    {
        x0 += (long)(x1 - x0) * (maxY - y0) / (y1 - y0);
        y0 = maxY;
    }

    if (y1 < minY)
    {
        x1 = x0 + (long)(x1 - x0) * (minY - y0) / (y1 - y0);
        y1 = minY;
    }
    else if (y1 > maxY)
    {
        x1 = x0 + (long)(x1 - x0) * (maxY - y0) / (y1 - y0);
        y1 = maxY;
    }

    return true;
}

//---------------------------------------------
void Graph_Init()
{
    Graph_LoadAutoScale();
}
//---------------------------------------------
void Graph_SetStartX(uint8_t v)
{
    Graph_SetXAxisLabels(nullptr, nullptr);
    Graph_SetFixedXSpacing(false);
    Graph_SetXAxis(v, 7);
}
//---------------------------------------------
void Graph_SetXAxis(uint16_t start, uint16_t samples)
{
    valorX = start;
    xSamples = samples < 2 ? 2 : samples;
}
//---------------------------------------------
void Graph_SetFixedXSpacing(bool enabled)
{
    fixedXSpacing = enabled;
}
//---------------------------------------------
void Graph_SetXAxisLabels(const char *first, const char *last)
{
    if (first == nullptr || last == nullptr)
    {
        xFirstLabel[0] = '\0';
        xLastLabel[0] = '\0';
        return;
    }

    strncpy(xFirstLabel, first, sizeof(xFirstLabel));
    strncpy(xLastLabel, last, sizeof(xLastLabel));
    xFirstLabel[sizeof(xFirstLabel) - 1] = '\0';
    xLastLabel[sizeof(xLastLabel) - 1] = '\0';
}
//---------------------------------------------
void Graph_SetAutoScale(bool enabled)
{
    autoScaleEnabled = enabled;

    if (!autoScaleEnabled)
        Graph_EnsureScale();

    Graph_SaveAutoScale();
}
//---------------------------------------------
bool Graph_IsAutoScale()
{
    return autoScaleEnabled;
}
//---------------------------------------------
void Graph_ZoomIn()
{
    Graph_EnsureScale();

    if (autoScaleEnabled)
    {
        if (scaleMargin >= scaleStep)
            scaleMargin -= scaleStep;
        return;
    }

    int16_t span = maxVal - minVal;

    if (span > (scaleStep * 2 + 1))
    {
        minVal += scaleStep;
        maxVal -= scaleStep;
    }
}
//---------------------------------------------
void Graph_ZoomOut()
{
    Graph_EnsureScale();

    if (autoScaleEnabled)
    {
        scaleMargin += scaleStep;
        return;
    }

    minVal -= scaleStep;
    maxVal += scaleStep;
}
//---------------------------------------------
void Graph_SetData(int16_t *data, uint16_t size)
{
    buffer = data;
    bufferSize = size;

    if (buffer == nullptr || bufferSize == 0)
        scaleInitialized = false;
}
//---------------------------------------------
void Graph_SetMarkers(bool *data, uint16_t size)
{
    markerBuffer = data;
    markerSize = size;
}
//---------------------------------------------
void Graph_SetValueDivisor(uint8_t divisor)
{
    valueDivisor = divisor == 0 ? 10 : divisor;
}
//---------------------------------------------
void Graph_CalcScale()
{
    if (buffer == nullptr || bufferSize == 0)
        return;

    if (autoScaleEnabled || !scaleInitialized)
        Graph_ApplyAutoScaleModel();
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
        dtostrf((float)val / valueDivisor, 2, valueDivisor == 100 ? 2 : 1, txt);
        int w = u8g2.getStrWidth(txt);
        u8g2.drawStr(originX - 2 - w, y + 3, txt);

        // 8g2.drawStr(0, y + 3, txt);
    }

    // -----------------------
    // EIXO X (horas)
    // -----------------------

    if (xFirstLabel[0] != '\0' && xLastLabel[0] != '\0')
    {
        u8g2.drawLine(originX, originY, originX, originY + 2);

        int lastX = 127;

        if (fixedXSpacing && bufferSize > 1 && xSamples > 1)
            lastX = originX + ((uint32_t)width * (bufferSize - 1) / (xSamples - 1));

        if (lastX > 127)
            lastX = 127;

        u8g2.drawLine(lastX, originY, lastX, originY + 2);

        u8g2.drawStr(originX, originY + 10, xFirstLabel);

        int w = u8g2.getStrWidth(xLastLabel);
        int labelX = lastX - w;

        if (labelX < originX)
            labelX = originX;
        if (labelX > 127 - w)
            labelX = 127 - w;

        u8g2.drawStr(labelX, originY + 10, xLastLabel);
        return;
    }

    uint8_t marksX = 6;

    for (uint8_t i = 0; i <= marksX; i++)
    {
        uint8_t x = originX + (width * i / marksX);

        // tick
        u8g2.drawLine(x, originY, x, originY + 2);

        char txt[7];

        uint16_t label = valorX + ((xSamples - 1) * i / marksX);
        sprintf(txt, "%u", label);

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

    float step = (float)width / ((fixedXSpacing ? xSamples : bufferSize) - 1);

    int prevX = originX;
    int prevY = Graph_MapY(buffer[0]);

    for (uint16_t i = 1; i < bufferSize; i++)
    {
        int x = originX + (int)(i * step);

        int y = Graph_MapY(buffer[i]);
        int drawPrevX = prevX;
        int drawPrevY = prevY;
        int drawX = x;
        int drawY = y;

        if (Graph_ClipYLine(drawPrevX, drawPrevY, drawX, drawY))
            u8g2.drawLine(drawPrevX, drawPrevY, drawX, drawY);

        prevX = x;
        prevY = y;
    }

    if (markerBuffer == nullptr || markerSize == 0)
        return;

    u8g2.setFont(u8g2_font_4x6_tr);

    for (uint16_t i = 0; i < bufferSize && i < markerSize; i++)
    {
        if (!markerBuffer[i])
            continue;

        int x = originX + (int)(i * step);
        int y = constrain(Graph_MapY(buffer[i]), topY, originY);

        u8g2.drawVLine(x, y + 1, 3);
        u8g2.drawStr(x - 2, originY + 10, "D");
    }
}
