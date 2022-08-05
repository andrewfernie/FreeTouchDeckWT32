#include "DrawHelper.h"
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
void drawlatched(int b, int col, int row)
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
* @param logonumber int
* @param col int
* @param row int
* @param transparent boolean
*
* @return none
*
* @note Logos start at the top left and are 0 indexed. The same goes
         for the colomn and the row.
*/
void drawlogo(int col, int row, bool transparent, bool latch)
{
    //    int16_t x = KEY_X - KEY_X / 2 + 4 + col * (KEY_W + KEY_SPACING_X);
    //    int16_t y = KEY_Y - KEY_Y / 2 + 4 + row * (KEY_H + KEY_SPACING_Y);
    int16_t x = KEY_X + col * (KEY_W + KEY_SPACING_X) - LOGO_SIZE_X_Y / 2;
    int16_t y = KEY_Y + row * (KEY_H + KEY_SPACING_Y) - LOGO_SIZE_X_Y / 2;

    if ((pageNum >= 0) && (pageNum < NUM_PAGES)) {
        if (transparent == true) {
            drawBmpTransparent(menu[pageNum].button[row][col].logo, x, y);
        }
        else {
            drawBmp(menu[pageNum].button[row][col].logo, x, y);
        }
    }
}

/**
* @brief This function draws the buttons that are on every page.
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
                uint16_t buttonBG;
                bool drawTransparent;
                uint16_t imageBGColor;
                if (menu[pageNum].button[row][col].islatched) {
                    imageBGColor = getLatchImageBG(row, col);
                }
                else {
                    imageBGColor = getImageBG(row, col);
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
                tft.setFreeFont(LABEL_FONT);
                key[row][col].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                                         KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
                                         KEY_W, KEY_H, TFT_WHITE, buttonBG, TFT_WHITE,
                                         (char *)"", KEY_TEXTSIZE);
                key[row][col].drawButton();
                // After drawing the button outline we call this to draw a logo.
                if (menu[pageNum].button[row][col].islatched) {
                    drawlogo(col, row, drawTransparent, true);
                }
                else {
                    drawlogo(col, row, drawTransparent, false);
                }
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
        tft.setTextSize(2);
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
