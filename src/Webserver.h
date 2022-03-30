/**
* @brief This function returns all the files in a given directory in a json
         formatted string.
*
* @param path String
*
* @return String
*
* @note none
*/
String handleFileList(String path)
{
    File root = FILESYSTEM.open(path);
    path = String();
    int filecount = 0;

    String output = "[";
    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
            if (output != "[") {
                output += ',';
            }

            output += "{\"";
            output += filecount;
            output += "\":\"";

#ifdef ESP_ARDUINO_VERSION_MAJOR
            output += String(file.name());
#else
            output += String(file.name()).substring(7);
#endif

            output += "\"}";
            file = root.openNextFile();
            filecount++;
        }

        file.close();
    }
    output += "]";
    root.close();
    return output;
}

String handleAPISList()
{
    File root = FILESYSTEM.open("/uploads");

    int filecount = 0;

    String output = "[";
    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
            String filepath = String(file.name()).substring(0, 16);
            if (filepath == "/uploads/config_") {
                file = root.openNextFile();
                filecount++;
            }
            else {
                String filename = String(file.name()).substring(9);
                if (output != "[") {
                    output += ',';
                }

                output += "{\"";
                output += filecount;
                output += "\":\"";
                output += String(file.name()).substring(9);
                output += "\"}";
                file = root.openNextFile();
                filecount++;
            }
        }
        file.close();
    }
    output += "]";
    root.close();
    return output;
}

/**
* @brief This function returns information about FreeTouchDeck in a json
         formatted string.
*
* @param none
*
* @return String
*
* @note none
*/
String handleInfo()
{
    float freemem = SPIFFS.totalBytes() - SPIFFS.usedBytes();

    String output = "[";

    output += "{\"";
    output += "Version";
    output += "\":\"";
    output += String(versionnumber);
    output += "\"},";

    output += "{\"";
    output += "Free Space";
    output += "\":\"";
    output += String(freemem / 1000);
    output += " kB\"},";

    output += "{\"";
    output += "BLE Keyboard Version";
    output += "\":\"";
    output += String(BLE_KEYBOARD_VERSION);
    output += "\"},";

    output += "{\"";
    output += "ArduinoJson Version";
    output += "\":\"";
    output += String(ARDUINOJSON_VERSION);
    output += "\"},";

    output += "{\"";
    output += "TFT_eSPI Version";
    output += "\":\"";
    output += String(TFT_ESPI_VERSION);
    output += "\"},";

    output += "{\"";
    output += "ESP-IDF";
    output += "\":\"";
    output += String(esp_get_idf_version());
    output += "\"},";

    output += "{\"";
    output += "WiFi Mode";
    output += "\":\"";
    output += String(wificonfig.wifimode);
    output += "\"},";

#ifdef touchInterruptPin
    output += "{\"";
    output += "Sleep";
    output += "\":\"";
    if (generalconfig.sleepenable) {
        output += String("Enabled. ");
        output += String("Timer: ");
        output += String(generalconfig.sleeptimer);
        output += String(" minutes");
        output += "\"}";
    }
    else {
        output += String("Disabled");
        output += "\"}";
    }
#else
    output += "{\"";
    output += "Sleep";
    output += "\":\"";
    output += String("Disabled");
    output += "\"}";

#endif

    output += "]";

    return output;
}

String errorCode;
String errorText;

/**
 * @brief This function handles error.htm template processing.
 *
 * @param var const String&
 *
 * @return String
 *
 * @note none
 */
String processor(const String &var)
{
    if (var == "ERROR_CODE") {
        return errorCode;
    }
    if (var == "ERROR_TEXT") {
        return errorText;
    }
    return String();
}

/**
 * @brief This function handles JSON file uploads. only menu0.json, menu1.json, menu2.json (etc.), and colors.json are accepted.
 *
 * @param *request AsyncWebServerRequest
 * @param filename String
 * @param index size_t
 * @param *data uint8_t
 * @param len size_t
 * @param final boolean
 *
 * @return none
 *
 * @note none
 */
void handleJSONUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    bool validMenuName = false;
    int endIndex = 0;
    int startIndex = 0;
    String menuIndexText;
    int menuIndexNum = 0;

    if (filename.indexOf("menu") == 0) {
        startIndex = 4;
        endIndex = filename.indexOf(".json");
        if (endIndex > startIndex) {
            menuIndexText = filename.substring(startIndex, endIndex);
            menuIndexNum = menuIndexText.toInt();

            if ((menuIndexNum > 0) || (menuIndexText[0] == '0')) {
                validMenuName = true;
            }
        }
    }

    if (!validMenuName && filename != "general.json" && filename != "wificonfig.json") {
        Serial.printf("[INFO]: JSON has invalid name: %s\n", filename.c_str());
        errorCode = "102";
        errorText = "JSON file has an invalid name. You can only upload JSON files with the following file names:";
        errorText += "<ul><li>menu0.json</li><li>menu1.json</li><li>menu2.json</li><li>menu3.json</li><li>menu4.json</li>";
        errorText += "<li>general.json</li><li>wificonfig.json</li></ul>";
        request->send(FILESYSTEM, "/error.htm", String(), false, processor);
        return;
    }
    if (!index) {
        Serial.printf("[INFO]: JSON Upload Start: %s\n", filename.c_str());
        filename = "/config/" + filename;  // TODO: Does the config directory need to be hardcoded?

        // Open the file on first call and store the file handle in the request object
        request->_tempFile = SPIFFS.open(filename, "w");
    }
    if (len) {
        // Stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final) {
        Serial.printf("[INFO]: JSON Uploaded: %s\n", filename.c_str());
        // Close the file handle as the upload is now done
        request->_tempFile.close();
        request->send(FILESYSTEM, "/upload.htm");
    }
}

/**
 * @brief This function handles API JSON file uploads. These are placed in the uploads folder.
 *
 * @param *request AsyncWebServerRequest
 * @param filename String
 * @param index size_t
 * @param *data uint8_t
 * @param len size_t
 * @param final boolean
 *
 * @return none
 *
 * @note none
 */
void handleAPIUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) {
        Serial.printf("[INFO]: API file Upload Start: %s\n", filename.c_str());
        filename = "/uploads/" + filename;  // TODO: Does the uploads directory need to be hardcoded?

        // Open the file on first call and store the file handle in the request object
        request->_tempFile = SPIFFS.open(filename, "w");
    }
    if (len) {
        // Stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final) {
        Serial.printf("[INFO]: API file Uploaded: %s\n", filename.c_str());
        // Close the file handle as the upload is now done
        request->_tempFile.close();
        request->send(FILESYSTEM, "/upload.htm");
    }
}

/* --------------- Checking for free space on SPIFFS ----------------
Purpose: This checks if the free memory on the SPIFFS is bigger then a set threshold
Input  : none
Output : boolean
Note   : none
*/

bool spaceLeft()
{
    float minmem = 100000.00;  // Always leave 100 kB free pace on SPIFFS
    float freeMemory = SPIFFS.totalBytes() - SPIFFS.usedBytes();
    Serial.printf("[INFO]: Free memory left: %f bytes\n", freeMemory);
    if (freeMemory < minmem) {
        return false;
    }

    return true;
}
/**
* @brief This function handles a file upload used by the Webserver
*
* @param *request
* @param filename String
* @param index size_t
* @param *data uint8_t
* @param len size_t
* @param final boolean
*
* @return none
*
* @note The reason the file is first uploaded and then deleted if there is not enough free space, is that
         if the request is not handled, the ESP32 craches. So we have to accept the upload but
         can delete it.
*/
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) {
        Serial.printf("[INFO]: File Upload Start: %s\n", filename.c_str());
        filename = "/logos/" + filename;
        // Open the file on first call and store the file handle in the request object
        request->_tempFile = SPIFFS.open(filename, "w");
    }
    if (len) {
        // Stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final) {
        Serial.printf("[INFO]: File Uploaded: %s\n", filename.c_str());
        // Close the file handle as the upload is now done
        request->_tempFile.close();

        // If there is not enough space left, we have to delete the recently uploaded file
        if (!spaceLeft()) {
            MSG_WARNLN("[WARNING]: Not enough free space left");
            errorCode = "103";
            errorText = "There is not enough free space left to upload a logo. Please delete unused logos and try again.";
            request->send(FILESYSTEM, "/error.htm", String(), false, processor);

            // Remove the recently uploaded file
            String fileToDelete = "/logos/";
            fileToDelete += filename;
            FILESYSTEM.remove(fileToDelete);
            MSG_WARNLN("[WARNING]: File removed to keep enough free space");
            return;
        }
        else {
            request->send(FILESYSTEM, "/upload.htm");
        }
    }
}

String resultHeader;
String resultText;
String resultFiles = "";

/**
 * @brief This function handles delete.htm template processing.
 *
 * @param  &var const String
 *
 * @return String
 *
 * @note Only need to call this once! This is also where the saving of config files is done.
 */
String deleteProcessor(const String &var)
{
    if (var == "RESULT") {
        return resultHeader;
    }
    if (var == "TEXT") {
        return resultText;
    }
    if (var == "FILES") {
        return resultFiles;
    }
    return String();
}

/**
 * @brief This function adds all the handlers we need to the webserver.
 *
 * @param none
 *
 * @return none
 *
 * @note Only need to call this once! This is also where the saving of config files is done.
 */
void handlerSetup()
{
    webserver.serveStatic("/", FILESYSTEM, "/").setDefaultFile("index.htm");

    //----------- index.htm handler -----------------

    webserver.on("/index.htm", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(FILESYSTEM, "/index.htm");
    });

    //----------- saveconfig handler -----------------

    webserver.on("/saveconfig", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("save", true)) {
            AsyncWebParameter *p = request->getParam("save", true);
            String savemode = p->value().c_str();

            if (savemode == "general") {
                // --- Saving general config
                MSG_INFOLN("[INFO]: Saving General Config");

                FILESYSTEM.remove("/config/general.json");
                File file = FILESYSTEM.open("/config/general.json", "w");
                if (!file) {
                    MSG_WARNLN("[WARNING]: Failed to create file");
                    return;
                }

                DynamicJsonDocument doc(400);

                JsonObject general = doc.to<JsonObject>();

                AsyncWebParameter *menubuttoncolor = request->getParam("menubuttoncolor", true);
                general["menubuttoncolor"] = menubuttoncolor->value().c_str();
                AsyncWebParameter *functionbuttoncolor = request->getParam("functionbuttoncolor", true);
                general["functionbuttoncolor"] = functionbuttoncolor->value().c_str();
                AsyncWebParameter *latchcolor = request->getParam("latchcolor", true);
                general["latchcolor"] = latchcolor->value().c_str();
                AsyncWebParameter *background = request->getParam("background", true);
                general["background"] = background->value().c_str();

                AsyncWebParameter *sleepenable = request->getParam("sleepenable", true);
                String sleepEnable = sleepenable->value().c_str();

                if (sleepEnable == "true") {
                    general["sleepenable"] = true;
                }
                else {
                    general["sleepenable"] = false;
                }

                AsyncWebParameter *beep = request->getParam("beep", true);
                String Beep = beep->value().c_str();

                if (Beep == "true") {
                    general["beep"] = true;
                }
                else {
                    general["beep"] = false;
                }

                // Sleep timer
                AsyncWebParameter *sleeptimer = request->getParam("sleeptimer", true);

                String sleepTimer = sleeptimer->value().c_str();
                general["sleeptimer"] = sleepTimer.toInt();

                // Modifiers

                AsyncWebParameter *modifier1 = request->getParam("modifier1", true);
                String Modifier1 = modifier1->value().c_str();
                general["modifier1"] = Modifier1.toInt();

                AsyncWebParameter *modifier2 = request->getParam("modifier2", true);
                String Modifier2 = modifier2->value().c_str();
                general["modifier2"] = Modifier2.toInt();

                AsyncWebParameter *modifier3 = request->getParam("modifier3", true);
                String Modifier3 = modifier3->value().c_str();
                general["modifier3"] = Modifier3.toInt();

                AsyncWebParameter *helperdelay = request->getParam("helperdelay", true);
                String Helperdelay = helperdelay->value().c_str();
                general["helperdelay"] = Helperdelay.toInt();

                if (serializeJsonPretty(doc, file) == 0) {
                    MSG_WARNLN("[WARNING]: Failed to write to file");
                }
                file.close();
            }
            else if (savemode == "wifi") {
                // --- Saving wifi config
                MSG_INFOLN("[INFO]: Saving Wifi Config");

                FILESYSTEM.remove("/config/wificonfig.json");
                File file = FILESYSTEM.open("/config/wificonfig.json", "w");
                if (!file) {
                    MSG_WARNLN("[WARNING]: Failed to create file");
                    return;
                }

                DynamicJsonDocument doc(384);

                JsonObject wifi = doc.to<JsonObject>();

                AsyncWebParameter *menubuttoncolor = request->getParam("ssid", true);
                wifi["ssid"] = menubuttoncolor->value().c_str();
                AsyncWebParameter *functionbuttoncolor = request->getParam("password", true);
                wifi["password"] = functionbuttoncolor->value().c_str();
                AsyncWebParameter *latchcolor = request->getParam("wifimode", true);
                wifi["wifimode"] = latchcolor->value().c_str();
                AsyncWebParameter *background = request->getParam("wifihostname", true);
                wifi["wifihostname"] = background->value().c_str();

                AsyncWebParameter *attempts = request->getParam("attempts", true);
                String Attempts = attempts->value().c_str();
                wifi["attempts"] = Attempts.toInt();

                AsyncWebParameter *attemptdelay = request->getParam("attemptdelay", true);
                String Attemptdelay = attemptdelay->value().c_str();
                wifi["attemptdelay"] = Attemptdelay.toInt();

                if (serializeJsonPretty(doc, file) == 0) {
                    MSG_WARNLN("[WARNING]: Failed to write to file");
                }
                file.close();
            }
            else if (strncmp("menu", savemode.c_str(), 4) == 0) {
                // --- Save menu
                char tmp[5];
                String fileName = String("/config/menu");
                int menuNumber;
                File file;

                int16_t numScanned = sscanf(savemode.c_str(), "%4s%d", tmp, &menuNumber);

                if ((numScanned == 2) && (menuNumber >= 0) && (menuNumber < NUM_PAGES)) {
                    MSG_INFO1("[INFO]: Saving Menu ", menuNumber);
                    fileName += menuNumber;
                    fileName += ".json";
                    FILESYSTEM.remove(fileName.c_str());
                    file = FILESYSTEM.open(fileName.c_str(), "w");
                    if (!file) {
                        MSG_WARN("[WARNING]: Failed to create ");
                        MSG_WARNLN(fileName);
                        return;
                    }
                }
                else {
                    MSG_WARN1("[WARNING]: Invalid menu number ", menuNumber);
                    return;
                }

                DynamicJsonDocument doc(1200);

                JsonObject menu = doc.to<JsonObject>();

                for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
                    for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                        AsyncWebParameter *menuXlogo0 = request->getParam("menuXlogo0", true);
                        menu["logo0"] = menuXlogo0->value().c_str();
                        AsyncWebParameter *menuXlogo1 = request->getParam("menuXlogo1", true);
                        menu["logo1"] = menuXlogo1->value().c_str();
                        AsyncWebParameter *menuXlogo2 = request->getParam("menuXlogo2", true);
                        menu["logo2"] = menuXlogo2->value().c_str();
                        AsyncWebParameter *menuXlogo3 = request->getParam("menuXlogo3", true);
                        menu["logo3"] = menuXlogo3->value().c_str();
                        AsyncWebParameter *menuXlogo4 = request->getParam("menuXlogo4", true);
                        menu["logo4"] = menuXlogo4->value().c_str();

                        JsonObject button0 = doc.createNestedObject("button0");

                        if (request->hasParam("menuXbutton0latch", true)) {
                            button0["latch"] = true;
                        }
                        else {
                            button0["latch"] = false;
                        }

                        AsyncWebParameter *menuXlatchlogo0 = request->getParam("menuXlatchlogo0", true);
                        Serial.println(menuXlatchlogo0->value().c_str());
                        if (strcmp(menuXlatchlogo0->value().c_str(), "---") == 0) {
                            button0["latchlogo"] = "";
                        }
                        else {
                            button0["latchlogo"] = menuXlatchlogo0->value().c_str();
                        }

                        JsonArray button0_actionarray = button0.createNestedArray("actionarray");
                        AsyncWebParameter *menuXbutton0action0 = request->getParam("menuXbutton0action0", true);
                        button0_actionarray.add(menuXbutton0action0->value().c_str());
                        AsyncWebParameter *menuXbutton0action1 = request->getParam("menuXbutton0action1", true);
                        button0_actionarray.add(menuXbutton0action1->value().c_str());
                        AsyncWebParameter *menuXbutton0action2 = request->getParam("menuXbutton0action2", true);
                        button0_actionarray.add(menuXbutton0action2->value().c_str());

                        JsonArray button0_valuearray = button0.createNestedArray("valuearray");
                        AsyncWebParameter *menuXbutton0value0 = request->getParam("menuXbutton0value0", true);
                        button0_valuearray.add(menuXbutton0value0->value().c_str());
                        AsyncWebParameter *menuXbutton0value1 = request->getParam("menuXbutton0value1", true);
                        button0_valuearray.add(menuXbutton0value1->value().c_str());
                        AsyncWebParameter *menuXbutton0value2 = request->getParam("menuXbutton0value2", true);
                        button0_valuearray.add(menuXbutton0value2->value().c_str());
                    }
                }
                if (serializeJsonPretty(doc, file) == 0) {
                    MSG_WARNLN("[WARNING]: Failed to write to file");
                }
                file.close();
            }
            request->send(FILESYSTEM, "/saveconfig.htm");
        }
    });

    //----------- File list handler -----------------

    webserver.on("/list", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("dir")) {
            AsyncWebParameter *p = request->getParam("dir");
            request->send(200, "application/json", handleFileList(p->value().c_str()));
        }
    });

    webserver.on("/apislist", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", handleAPISList());
    });

    webserver.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", handleInfo());
    });

    //----------- 404 handler -----------------

    webserver.onNotFound([](AsyncWebServerRequest *request) {
        Serial.printf("NOT_FOUND: ");
        if (request->method() == HTTP_GET)
            Serial.printf("GET");
        else if (request->method() == HTTP_POST)
            Serial.printf("POST");
        else if (request->method() == HTTP_DELETE)
            Serial.printf("DELETE");
        else if (request->method() == HTTP_PUT)
            Serial.printf("PUT");
        else if (request->method() == HTTP_PATCH)
            Serial.printf("PATCH");
        else if (request->method() == HTTP_HEAD)
            Serial.printf("HEAD");
        else if (request->method() == HTTP_OPTIONS)
            Serial.printf("OPTIONS");
        else
            Serial.printf("UNKNOWN");
        Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

        if (request->contentLength()) {
            Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
            Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
        }

        int headers = request->headers();
        int i;
        for (i = 0; i < headers; i++) {
            AsyncWebHeader *h = request->getHeader(i);
            Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
        }

        int params = request->params();
        for (i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->isFile()) {
                Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
            }
            else if (p->isPost()) {
                Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
            else {
                Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }

        request->send(404);
    });

    webserver.onFileUpload([](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!index)
            Serial.printf("UploadStart: %s\n", filename.c_str());
        Serial.printf("%s", (const char *)data);
        if (final)
            Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
    });

    webserver.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        if (!index)
            Serial.printf("BodyStart: %u\n", total);
        Serial.printf("%s", (const char *)data);
        if (index + len == total)
            Serial.printf("BodyEnd: %u\n", total);
    });

    webserver.on(
        "/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
            request->send(FILESYSTEM, "/upload.htm");
        },
        handleUpload);

    webserver.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
        // First send some text to the browser otherwise an ugly browser error shows up
        request->send(200, "text/plain", "FreeTouchDeck is restarting...");
        // Then restart the ESP
        MSG_WARNLN("[WARNING]: Restarting");
        ESP.restart();
    });

    // ----------------------------- Error Handle ---------------------------------

    webserver.on("/error", HTTP_GET, [](AsyncWebServerRequest *request) {
        errorCode = "123";
        errorText = "Nothing went wrong, all is good. This is just a test";
        request->send(FILESYSTEM, "/error.htm", String(), false, processor);
    });

    // ----------------------------- Editor Handle ---------------------------------

    webserver.on("/editor", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        int i;
        int filecount = 0;
        for (i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            Serial.printf("[INFO]: Deleting file: %s\n", p->value().c_str());
            String filename = "/logos/";
            filename += p->value().c_str();
            if (SPIFFS.exists(filename)) {
                SPIFFS.remove(filename);
            }

            resultFiles += p->value().c_str();
            resultFiles += "<br>";
            filecount++;
        }
        if (filecount > 0) {
            resultHeader = "Succes!";
        }
        else {
            resultHeader = "Fail!";
        }
        resultText = String(filecount);
        request->send(FILESYSTEM, "/editor.htm", String(), false, deleteProcessor);
        resultFiles = "";
    });

    // ----------------------------- JSON Download Handle ---------------------------------

    webserver.on("/download", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebParameter *p = request->getParam("file");
        String filerequest = p->value().c_str();
        Serial.printf("[INFO]: Requested file: %s\n", filerequest.c_str());

        String downloadfile = "/config/" + filerequest;
        Serial.printf("[INFO]: Full path: %s\n", downloadfile.c_str());

        if (FILESYSTEM.exists(downloadfile)) {
            Serial.printf("[INFO]: Download file %s\n", downloadfile.c_str());
            request->send(FILESYSTEM, downloadfile, String(), true);
        }
        else {
            Serial.printf("[INFO]: Download file %s doesn't exits!\n", downloadfile.c_str());
        }
    });

    // ----------------------------- JSON Upload Handle ---------------------------------

    webserver.on(
        "/uploadJSON", HTTP_POST, [](AsyncWebServerRequest *request) {}, handleJSONUpload);
}
