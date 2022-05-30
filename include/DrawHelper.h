#pragma once
#include "FreeTouchDeckWT32.h"
void drawlatched(int b, int col, int row);
void drawlogo(int col, int row, bool transparent, bool latch);
void drawKeypad();
void drawErrorMessage(String message);
void printDeviceAddress();
void printinfo();
