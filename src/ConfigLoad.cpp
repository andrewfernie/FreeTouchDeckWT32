#include "ConfigLoad.h"
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
    MSG_INFOLN("[INFO] Entering loadMainConfig");
    if (!FILESYSTEM.exists("/config/wificonfig.json")) {
        MSG_WARNLN("[WARNING]: Config file not found!");
        return false;
    }
    File configfile = FILESYSTEM.open("/config/wificonfig.json");

    DynamicJsonDocument doc(384);

    DeserializationError error = deserializeJson(doc, configfile);

    if (error == DeserializationError::Ok) {
        MSG_INFOLN("[INFO] wificonfig.json deserialized loaded OK");
    }
    else {
        MSG_WARN1("[WARNING]: wificonfig.json deserialization error: ", error.c_str());
    }

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
        MSG_ERROR("[ERROR] deserializeJson() error");
        MSG_ERRORLN(error.c_str());
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
* @note Options for values are: colors, menu1, menu2, menu3
         menu4, and menu5
*/
bool loadConfig(String value)
{
    char fileNameType[20];
    int fileNameMenuNumber = 0;
    int numConverted = 0;
    numConverted = sscanf(value.c_str(), "%4s%d", fileNameType, &fileNameMenuNumber);
    MSG_INFO2("[INFO] load_config parameter", value.c_str(), numConverted);

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

        char colorbuf[64];
        snprintf(colorbuf, sizeof(colorbuf), "[INFO] menuButtonColour (RGB565) = 0x%06X", generalconfig.menuButtonColour);
        MSG_INFOLN(colorbuf);

        char functionbuttoncolorchar[64];
        strcpy(functionbuttoncolorchar, functionbuttoncolor);
        unsigned long rgb888functionbuttoncolor = convertHTMLtoRGB888(functionbuttoncolorchar);
        generalconfig.functionButtonColour = convertRGB888ToRGB565(rgb888functionbuttoncolor);

        snprintf(colorbuf, sizeof(colorbuf), "[INFO] functionButtonColour (RGB565) = 0x%06X", generalconfig.functionButtonColour);
        MSG_INFOLN(colorbuf);

        char latchcolorchar[64];
        strcpy(latchcolorchar, latchcolor);
        unsigned long rgb888latchcolor = convertHTMLtoRGB888(latchcolorchar);
        generalconfig.latchedColour = convertRGB888ToRGB565(rgb888latchcolor);

        snprintf(colorbuf, sizeof(colorbuf), "[INFO] latchedColour (RGB565) = 0x%06X", generalconfig.latchedColour);
        MSG_INFOLN(colorbuf);

        char backgroundcolorchar[64];
        strcpy(backgroundcolorchar, bgcolor);
        unsigned long rgb888backgroundcolor = convertHTMLtoRGB888(backgroundcolorchar);
        generalconfig.backgroundColour = convertRGB888ToRGB565(rgb888backgroundcolor);

        snprintf(colorbuf, sizeof(colorbuf), "[INFO] backgroundColour (RGB565) = 0x%06X", generalconfig.backgroundColour);
        MSG_INFOLN(colorbuf);

        // Loading general settings

        bool sleepenable = doc["sleepenable"] | false;
        if (sleepenable) {
            generalconfig.sleepenable = true;
        }
        else {
            generalconfig.sleepenable = false;
        }

        uint16_t sleeptimer = doc["sleeptimer"] | 60;
        generalconfig.sleeptimer = sleeptimer;

        bool usbcommsenable = doc["usbcommsenable"] | false;
        if (usbcommsenable) {
            generalconfig.usbcommsenable = true;
        }
        else {
            generalconfig.usbcommsenable = false;
        }

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
            MSG_ERROR1("[ERROR] deserializeJson() error", error.c_str());
            return false;
        }
        return true;
    }

    // --------------------- Loading menu ----------------------
    else if ((numConverted == 2) && (strncmp("menu", value.c_str(), 4) == 0) && (fileNameMenuNumber >= 0) && (fileNameMenuNumber < NUM_PAGES)) {
        char configFileName[30];
        char menuFileNameRoot[10];

        snprintf(configFileName, sizeof(configFileName), "/config/menu%d.json", fileNameMenuNumber);

        MSG_INFO1("[INFO] load_config opening file ", configFileName);
        File configfile = FILESYSTEM.open(configFileName, "r");

        DynamicJsonDocument doc(5000);

        DeserializationError error = deserializeJson(doc, configfile);

        snprintf(menuFileNameRoot, sizeof(menuFileNameRoot), "menu%d", fileNameMenuNumber);
        const char *menuName = doc["menuname"] | menuFileNameRoot;
        MSG_INFO1("[INFO] load_config menuname is ", menuName);
        strncpy(menu[fileNameMenuNumber].name, menuName, sizeof(menu[fileNameMenuNumber].name));

        for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
            for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                char objectName[10];

                snprintf(objectName, sizeof(objectName), "button%d%d", row + 1, col + 1);

                const char *logo = doc[objectName]["logo"] | "question.bmp";

                strcpy(templogopath, logopath);
                strcat(templogopath, logo);
                strcpy(menu[fileNameMenuNumber].button[row][col].logo, templogopath);
                MSG_INFO2("[INFO] load_config loading logo", objectName, templogopath);

                const char *latchlogo = doc[objectName]["latchlogo"] | "question.bmp";

                menu[fileNameMenuNumber].button[row][col].latch = doc[objectName]["latch"] | false;

                strcpy(templogopath, logopath);
                strcat(templogopath, latchlogo);
                strcpy(menu[fileNameMenuNumber].button[row][col].latchlogo, templogopath);
                MSG_INFO2("[INFO] load_config loading latchlogo", objectName, templogopath);

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

                //
                // --------------------- Special Cases ----------------------
                //
                // Sleep enable button
                if (menu[fileNameMenuNumber].button[row][col].actions[0].action == Action_SpecialFn &&
                    menu[fileNameMenuNumber].button[row][col].actions[0].value == 4) {
                    menu[fileNameMenuNumber].button[row][col].islatched = generalconfig.sleepenable;
                }
                // USB comms enable button
                if (menu[fileNameMenuNumber].button[row][col].actions[0].action == Action_SpecialFn &&
                    menu[fileNameMenuNumber].button[row][col].actions[0].value == 8) {
                    menu[fileNameMenuNumber].button[row][col].islatched = generalconfig.usbcommsenable;
                }
            }
        }

        configfile.close();

        if (error) {
            MSG_ERROR1("[ERROR] deserializeJson() error", error.c_str());
            MSG_ERROR2("[ERROR] Will initialize ", configFileName, " to default.json");

            if (CopyFile("/config/default.json", configFileName)) {
                MSG_INFO2("[INFO] Successful initialization of ", configFileName, " to default.json");
            }
            else {
                MSG_ERROR1("[ERROR] Failed to initialize ", configFileName);
                checkfile(configFileName, true);  // This will force a message to the TFT screen

                while (1)
                    yield();  // Stop!

                return false;
            }
        }
        return true;
    }
    else {
        MSG_ERROR1("[ERROR] Invalid call to loadConfig(). Argument was ", value.c_str());
        return false;
    }
}
