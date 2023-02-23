#pragma once
#include "FreeTouchDeckWT32.h"
void drawlatched(int b, uint8_t col, uint8_t row);
void drawlogo(uint8_t col, uint8_t row, bool transparent, bool latch);
void drawButtonRowCol(uint8_t page, uint8_t row, uint8_t col);
bool isActiveButton(uint8_t page, uint8_t row, uint8_t col);
void drawKeypad();
void drawErrorMessage(String message);
void printDeviceAddress();
void printinfo();
void drawTopStatusBar(bool force_redraw);
void drawBottomStatusBar(bool force_redraw);
bool CopyFile(String FileOriginal, String FileCopy);
uint32_t usedPSRAM();
