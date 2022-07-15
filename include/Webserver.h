#pragma once

#include "FreeTouchDeckWT32.h"
extern String errorCode;
extern String errorText;
extern String resultHeader;
extern String resultText;
extern String resultFiles;

String handleFileList(String path);
String handleAPISList();
String handleInfo();
String processor(const String &var);
void handleJSONUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void handleAPIUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
bool spaceLeft();
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
String deleteProcessor(const String &var);
void handlerSetup();