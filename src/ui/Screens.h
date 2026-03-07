#pragma once
#include <Arduino.h>
#include "core/EventQueue.h"


// =============================
// HOME
// =============================
void ScreenHome_Draw();
void ScreenHome_Handle(EventType evt);


// =============================
// MENU
// =============================
void OpenMainMenu();


// =============================
// LOGGING
// =============================
void ScreenLogging_Draw();


// =============================
// GRAPH
// =============================
void ScreenGraph_Draw();


// =============================
// RTC CONFIG
// =============================
void ScreenRTC_Draw();
void ScreenRTC_Handle(EventType evt);


// =============================
// ABOUT
// =============================
void ScreenAbout_Draw();


// =============================
// CALLBACKS (ações do menu)
// =============================
void StartLog();
void OpenGraph();
void OpenDisplayAdjust();
void OpenRTC();
void OpenAbout();