/**
 * @brief This function opens wificonfig.json and fills the wificonfig
 *        struct accordingly.
 *
 * @param none
 *
 * @return True when succeeded. False otherwise.
 *
 * @note This is also where the sleep configuration lives.
 */
bool loadMainConfig()
{
    if (!FILESYSTEM.exists("/config/wificonfig.json")) {
        MSG_WARNLN("[WARNING]: Config file not found!");
        return false;
    }
    File configfile = FILESYSTEM.open("/config/wificonfig.json");

    DynamicJsonDocument doc(256);

    DeserializationError error = deserializeJson(doc, configfile);

    strlcpy(wificonfig.ssid, doc["ssid"] | "FAILED", sizeof(wificonfig.ssid));
    strlcpy(wificonfig.password, doc["password"] | "FAILED", sizeof(wificonfig.password));
    strlcpy(wificonfig.wifimode, doc["wifimode"] | "FAILED", sizeof(wificonfig.wifimode));
    strlcpy(wificonfig.hostname, doc["wifihostname"] | "freetouchdeck", sizeof(wificonfig.hostname));

    uint8_t attempts = doc["attempts"] | 10;
    wificonfig.attempts = attempts;

    uint16_t attemptdelay = doc["attemptdelay"] | 500;
    wificonfig.attemptdelay = attemptdelay;

    configfile.close();

    if (error) {
        MSG_ERRORLN("[ERROR]: deserializeJson() error");
        Serial.println(error.c_str());
        return false;
    }

    return true;
}

/**
* @brief This function loads the menu configuration.
*
* @param String the config to be loaded
*
* @return none
*
* @note Options for values are: colors, homescreen, menu1, menu2, menu3
         menu4, and menu5
*/
bool loadConfig(String value)
{
    char fileNameType[20];
    int fileNameMenuNumber = 0;
    int numConverted = 0;
    numConverted = sscanf(value.c_str(), "%4s%d", fileNameType, &fileNameMenuNumber);
    MSG_DEBUG2("[INFO] load_config parameter", value.c_str(),numConverted);

    if (value == "general") {
        File configfile = FILESYSTEM.open("/config/general.json", "r");

        DynamicJsonDocument doc(384);

        DeserializationError error = deserializeJson(doc, configfile);

        // Parsing colors
        const char *menubuttoncolor = doc["menubuttoncolor"] | "#009bf4";          // Get the colour for the menu and back home buttons.
        const char *functionbuttoncolor = doc["functionbuttoncolor"] | "#00efcb";  // Get the colour for the function buttons.
        const char *latchcolor = doc["latchcolor"] | "#fe0149";                    // Get the colour to use when latching.
        const char *bgcolor = doc["background"] | "#000000";                       // Get the colour for the background.

        char menubuttoncolorchar[64];
        strcpy(menubuttoncolorchar, menubuttoncolor);
        unsigned long rgb888menubuttoncolor = convertHTMLtoRGB888(menubuttoncolorchar);
        generalconfig.menuButtonColour = convertRGB888ToRGB565(rgb888menubuttoncolor);

        char functionbuttoncolorchar[64];
        strcpy(functionbuttoncolorchar, functionbuttoncolor);
        unsigned long rgb888functionbuttoncolor = convertHTMLtoRGB888(functionbuttoncolorchar);
        generalconfig.functionButtonColour = convertRGB888ToRGB565(rgb888functionbuttoncolor);

        char latchcolorchar[64];
        strcpy(latchcolorchar, latchcolor);
        unsigned long rgb888latchcolor = convertHTMLtoRGB888(latchcolorchar);
        generalconfig.latchedColour = convertRGB888ToRGB565(rgb888latchcolor);

        char backgroundcolorchar[64];
        strcpy(backgroundcolorchar, bgcolor);
        unsigned long rgb888backgroundcolor = convertHTMLtoRGB888(backgroundcolorchar);
        generalconfig.backgroundColour = convertRGB888ToRGB565(rgb888backgroundcolor);

        // Loading general settings

        bool sleepenable = doc["sleepenable"] | false;
        if (sleepenable) {
            generalconfig.sleepenable = true;
            sleepIsLatched = 1;
        }
        else {
            generalconfig.sleepenable = false;
        }

        // uint16_t sleeptimer = doc["sleeptimer"];
        uint16_t sleeptimer = doc["sleeptimer"] | 60;
        generalconfig.sleeptimer = sleeptimer;

        bool beep = doc["beep"] | false;
        generalconfig.beep = beep;

        uint8_t modifier1 = doc["modifier1"] | 0;
        generalconfig.modifier1 = modifier1;

        uint8_t modifier2 = doc["modifier2"] | 0;
        generalconfig.modifier2 = modifier2;

        uint8_t modifier3 = doc["modifier3"] | 0;
        generalconfig.modifier3 = modifier3;

        uint16_t helperdelay = doc["helperdelay"] | 250;
        generalconfig.helperdelay = helperdelay;

        configfile.close();

        if (error) {
            MSG_ERRORLN("[ERROR]: deserializeJson() error");
            Serial.println(error.c_str());
            return false;
        }
        return true;
    }

    // --------------------- Loading menu ----------------------
    else if ((numConverted == 2) && (strncmp("menu", value.c_str(), 4)==0) && (fileNameMenuNumber >= 0) && (fileNameMenuNumber < NUM_PAGES)) {
        char configFileName[30];

        snprintf(configFileName, sizeof(configFileName), "/config/menu%d.json", fileNameMenuNumber);

        MSG_DEBUG1("[INFO] load_config opening file ", configFileName);
        File configfile = FILESYSTEM.open(configFileName, "r");

        DynamicJsonDocument doc(1500);

        DeserializationError error = deserializeJson(doc, configfile);

        for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
            for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                char objectName[10];

                snprintf(objectName, sizeof(objectName), "logo%d%d", row, col);
                const char *logoxx = doc[objectName] | "question.bmp";

                strcpy(templogopath, logopath);
                strcat(templogopath, logoxx);
                strcpy(menu[fileNameMenuNumber].button[row][col].logo, templogopath);
                MSG_DEBUG1("[INFO] load_config loading logo", templogopath);
            }
        }

        for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
            for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                {
                    char objectName[10];

                    snprintf(objectName, sizeof(objectName), "button%d%d", row, col);
                    const char *latchlogo = doc[objectName]["latchlogo"] | "question.bmp";

                    menu[fileNameMenuNumber].button[row][col].latch = doc[objectName]["latch"] | false;

                    strcpy(templogopath, logopath);
                    strcat(templogopath, latchlogo);
                    strcpy(menu[fileNameMenuNumber].button[row][col].latchlogo, templogopath);
                    MSG_DEBUG1("[INFO] load_config loading latchlogo", templogopath);

                    JsonArray button_actionarray = doc[objectName]["actionarray"];

                    int button_actionarray_0 = button_actionarray[0];
                    int button_actionarray_1 = button_actionarray[1];
                    int button_actionarray_2 = button_actionarray[2];

                    JsonArray button_valuearray = doc[objectName]["valuearray"];

                    if (button_actionarray_0 == Action_Char || button_actionarray_0 == Action_SpecialChar) {
                        const char *button_symbolarray_0 = button_valuearray[0];
                        strcpy(menu[fileNameMenuNumber].button[row][col].actions[0].symbol, button_symbolarray_0);
                    }
                    else {
                        int button_valuearray_0 = button_valuearray[0];
                        menu[fileNameMenuNumber].button[row][col].actions[0].value = button_valuearray_0;
                    }

                    if (button_actionarray_1 == Action_Char || button_actionarray_1 == Action_SpecialChar) {
                        const char *button_symbolarray_1 = button_valuearray[1];
                        strcpy(menu[fileNameMenuNumber].button[row][col].actions[1].symbol, button_symbolarray_1);
                    }
                    else {
                        int button_valuearray_1 = button_valuearray[1];
                        menu[fileNameMenuNumber].button[row][col].actions[1].value = button_valuearray_1;
                    }

                    if (button_actionarray_2 == Action_Char || button_actionarray_2 == Action_SpecialChar) {
                        const char *button_symbolarray_2 = button_valuearray[2];
                        strcpy(menu[fileNameMenuNumber].button[row][col].actions[2].symbol, button_symbolarray_2);
                    }
                    else {
                        int button_valuearray_2 = button_valuearray[2];
                        menu[fileNameMenuNumber].button[row][col].actions[2].value = button_valuearray_2;
                    }

                    menu[fileNameMenuNumber].button[row][col].actions[0].action = button_actionarray_0;
                    menu[fileNameMenuNumber].button[row][col].actions[1].action = button_actionarray_1;
                    menu[fileNameMenuNumber].button[row][col].actions[2].action = button_actionarray_2;
                }
            }
        }

        configfile.close();

        if (error) {
            MSG_ERRORLN("[ERROR]: deserializeJson() error");
            Serial.println(error.c_str());
            return false;
        }
        return true;
    }
    else {
        return false;
    }
}
