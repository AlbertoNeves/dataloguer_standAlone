#pragma once
#include <Arduino.h>
#include <U8g2lib.h>

void Display_Backlight(bool on);
void Display_UserActivity();
void Display_BacklightTask();

void Display_Init();
void Display_Clear();
void Display_Update();

void Display_Print(int x, int y, const char *txt);

void Display_DrawFrame(int x,int y,int w,int h);

void Display_DrawBox(int x,int y,int w,int h);

void Display_DrawHLine(int x, int y, int w);

void Display_ClearArea(int x, int y, int w, int h);

extern U8G2_ST7920_128X64_F_SW_SPI u8g2;