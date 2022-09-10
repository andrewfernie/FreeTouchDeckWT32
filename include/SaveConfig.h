#pragma once

#include "FreeTouchDeckWT32.h"

int saveConfigGeneral(AsyncWebServerRequest *request);
int saveConfigWiFi(AsyncWebServerRequest *request);
int saveConfigMenu(AsyncWebServerRequest *request, String savemode);
int saveCurrentConfigGeneral();