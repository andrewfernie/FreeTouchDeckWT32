#pragma once
#include "FreeTouchDeckWT32.h"
bool startWifiStation();
void startWifiAP();
void startDefaultAP();
void configmode();
bool saveWifiSSID(String ssid);
bool saveWifiPW(String password);
bool saveWifiMode(String wifimode);
bool checkfile(const char* filename, bool showMessage);
bool resetconfig(String file);