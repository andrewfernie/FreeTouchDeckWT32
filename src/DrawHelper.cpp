#include "DrawHelper.h"

#ifdef READ_EXTERNAL_BATTERY
extern float readExternalBattery();
extern float externalBatteryVoltage;
#endif

/**
 * @brief This function draws the a "latched" dot. it uses the logonumber, colomn and row to
 *        determine where.
 *
 * @param b int
 * @param col int
 * @param row int
 *
 * @return none
 *
 * @note none
 */
void drawlatched(int b, uint8_t col, uint8_t row)
{
    int offset;
    if (SCREEN_WIDTH < 480) {
        offset = 2;
    }
    else {
        offset = 12;
    }
    int16_t x = KEY_X - KEY_X / 2 + 3 + col * (KEY_W + KEY_SPACING_X);
    int16_t y = KEY_Y - KEY_Y / 2 + 3 + row * (KEY_H + KEY_SPACING_Y);

    tft.fillRoundRect(x - offset, y - offset, 18, 18, 4, generalconfig.latchedColour);
}

/**
* @brief This function draws the logos according to the page
         we are currently on. The pagenumber is a global variable
         and doesn't need to be passed.
*
* @param col int
* @param row int
* @param transparent boolean
* @param latch boolean
*
* @return none
*
* @note row and col start at the top left and are 0 indexed. 
*/
void drawlogo(uint8_t col, uint8_t row, bool transparent, bool latch)
{
    //    int16_t x = KEY_X - KEY_X / 2 + 4 + col * (KEY_W + KEY_SPACING_X);
    //    int16_t y = KEY_Y - KEY_Y / 2 + 4 + row * (KEY_H + KEY_SPACING_Y);
    int16_t x = KEY_X + col * (KEY_W + KEY_SPACING_X) - LOGO_SIZE_X_Y / 2;
    int16_t y = KEY_Y + row * (KEY_H + KEY_SPACING_Y) - LOGO_SIZE_X_Y / 2;

    if ((pageNum >= 0) && (pageNum < NUM_PAGES)) {
        if (latch) {
            if (transparent == true) {
                drawBmpTransparent(menu[pageNum].button[row][col].latchlogo, x, y);
            }
            else {
                drawBmp(menu[pageNum].button[row][col].latchlogo, x, y);
            }
        }
        else {
            if (transparent == true) {
                drawBmpTransparent(menu[pageNum].button[row][col].logo, x, y);
            }
            else {
                drawBmp(menu[pageNum].button[row][col].logo, x, y);
            }
        }
    }
}

/**
* @brief This function checks if the button is active or inactive.
         Inactive means that the logo is "blank.bmp" and that there are no actions
         defined for the button.
*
* @param page int
* @param col int
* @param row int
*
* @return boolean true if active, false if inactive
*
* @note row and col start at the top left and are 0 indexed.
*/
bool isActiveButton(uint8_t page, uint8_t row, uint8_t col)
{
    bool activeButton = false;
    char logoPathAndName[64];

    strcpy(logoPathAndName, logopath);
    strcat(logoPathAndName, "blank.bmp");

    if (strcmp(menu[pageNum].button[row][col].logo, logoPathAndName) != 0) {
        activeButton = true;
    }
    else {
        if ((menu[pageNum].button[row][col].actions[0].action != ActionEnum::Action_NoAction) ||
            (menu[pageNum].button[row][col].actions[1].action != ActionEnum::Action_NoAction) ||
            (menu[pageNum].button[row][col].actions[2].action != ActionEnum::Action_NoAction))
            activeButton = true;
    }

    return activeButton;
}

/**
* @brief This function draws a button, both the logo and the surround box. 
*
* @param page int
* @param col int
* @param row int
*
* @return none
*
* @note row and col start at the top left and are 0 indexed.
*/
void drawButtonRowCol(uint8_t page, uint8_t row, uint8_t col)
{
    bool drawTransparent;
    uint16_t imageBGColor;
    uint16_t buttonBG = TFT_BLACK;
    uint16_t outlineColor = TFT_BLACK;
    bool activeButton = false;

    buttonBG = TFT_BLACK;
    outlineColor = TFT_BLACK;
    activeButton = false;

    activeButton = isActiveButton(pageNum, row, col);

    if (activeButton) {
        if (menu[pageNum].button[row][col].islatched) {
            imageBGColor = getLatchImageBG(pageNum, row, col);
        }
        else {
            imageBGColor = getImageBG(pageNum, row, col);
        }

        if (imageBGColor > 0) {
            buttonBG = imageBGColor;
            drawTransparent = false;
        }
        else {
            if (menu[pageNum].button[row][col].actions[0].action == Action_ChangePage) {
                buttonBG = generalconfig.menuButtonColour;
                drawTransparent = true;
            }
            else {
                buttonBG = generalconfig.functionButtonColour;
                drawTransparent = true;
            }
        }
        outlineColor = TFT_WHITE;
    }
    tft.setFreeFont(LABEL_FONT);
    key[row][col].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                             KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
                             KEY_W, KEY_H, outlineColor, buttonBG, outlineColor,
                             (char *)"", KEY_TEXTSIZE);
    key[row][col].drawButton();
    // After drawing the button outline we call this to draw a logo.
    if (activeButton) {
        if (menu[pageNum].button[row][col].islatched) {
            drawlogo(col, row, drawTransparent, true);
        }
        else {
            drawlogo(col, row, drawTransparent, false);
        }
    }
}

/**
* @brief This function draws the set buttons that are on each page.
         Pagenumber is global and doesn't need to be passed.
*
* @param none
*
* @return none
*
* @note none
*/
void drawKeypad()
{
    if ((pageNum >= 0) && (pageNum < NUM_PAGES)) {
        // Draw the button outlines and fill them with colours
        for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
            for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                drawButtonRowCol(pageNum, row, col);
            }
        }
    }

    else if (pageNum == SPECIAL_4_PAGE) {
        // Pagenum max + 4 means that a JSON config failed to load completely.
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextFont(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

        tft.printf("  %s failed to load and might be corrupted.\n", jsonFileFail);
        tft.println("  You can reset that specific file to default by opening the serial monitor");
        tft.printf("  and typing \"reset %s\"\n", jsonFileFail);
        tft.println("  If you don't do this, the configurator will fail to load.");
    }
}

/* ------------- Print an error message the TFT screen  ----------------
Purpose: This function prints an message to the TFT screen on a black
         background.
Input  : String message
Output : none
Note   : none
*/

void drawErrorMessage(String message)
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 20);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println(message);
}

/**
* @brief This function gets the Bluetooth device address and prints it to the serial monitor
         and the TFT screen in a 6 byte format, seperating each byte by ":".
*
* @param none
*
* @return none
*
* @note e.g. 00:11:22:33:22:EE
*/
void printDeviceAddress()
{
    const uint8_t *point = esp_bt_dev_get_address();

    for (int i = 0; i < 6; i++) {
        char str[3];

        sprintf(str, "%02X", (int)point[i]);
        // Serial.print(str);
        tft.print(str);

        if (i < 5) {
            // Serial.print(":");
            tft.print(":");
        }
    }
}

/**
* @brief This function prints some information about the current version
         and setup of FreetouchDeck to the TFT screen.
*
* @param none
*
* @return none
*
* @note none
*/
void printinfo()
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(1, 3);
    tft.setTextFont(2);
    if (SCREEN_WIDTH < 480) {
        tft.setTextSize(1);
    }
    else {
        tft.setTextSize(1);
    }
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.printf("Version: %s\n", versionnumber);

#ifdef TOUCH_INTERRUPT_PIN
    if (generalconfig.sleepenable) {
        tft.println("Sleep: Enabled");
        tft.printf("Sleep timer: %u minutes\n", generalconfig.sleeptimer);
    }
    else {
        tft.println("Sleep: Disabled");
    }
#else
    tft.println("Sleep: Disabled");
#endif

    if (generalconfig.usbcommsenable) {
        tft.println("USB Comms: Enabled");
    }
    else {
        tft.println("USB Comms: Disabled");
    }

#ifdef speakerPin
    if (generalconfig.beep) {
        tft.println("Speaker: Enabled");
    }
    else {
        tft.println("Speaker: Disabled");
    }
#else
    tft.println("Speaker: Disabled");
#endif

#ifdef READ_EXTERNAL_BATTERY
    tft.printf("Battery voltage: %f V\n", externalBatteryVoltage);
#endif

    tft.print("Free Storage: ");
    float freemem = FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes();
    tft.print(freemem / 1000);
    tft.println(" kB");
    tft.print("BLE Keyboard version: ");
    tft.println(BLE_KEYBOARD_VERSION);
    tft.print("ArduinoJson version: ");
    tft.println(ARDUINOJSON_VERSION);
    tft.print("TFT_eSPI version: ");
    tft.println(TFT_ESPI_VERSION);
    tft.print("ESP-IDF: ");
    tft.println(esp_get_idf_version());
    tft.println(esp_get_idf_version());
    tft.print("Free heap: ");
    float freeram = esp_get_free_heap_size();
    tft.print(freeram / 1000);
    tft.print("kB,");
    freeram = esp_get_minimum_free_heap_size();
    tft.print(freeram / 1000);
    tft.print("kB");
    displayinginfo = true;
}
