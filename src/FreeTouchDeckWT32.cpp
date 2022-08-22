/*
  Author: Dustin Watts
  Date: 27-08-2020

  My thanks goes out to Brian Lough, Colin Hickey, and the people on my Discord server
  for helping me a lot with the code and troubleshooting! https://discord.gg/RE3XevS

  FreeTouchDeck is based on the FreeDeck idea by Koriwi. It uses the TFT_eSPI library
  by Bodmer for the display and touch functionality and it uses an ESP32-BLE-Keyboard fork
  with a few modifications. For saving and loading configuration it uses ArduinoJson V6.

  FreeTouchDeck uses some libraries from other sources. These must be installed for
  FreeTouchDeck to compile and run.

  These are those libraries:

      !----------------------------- Library Dependencies --------------------------- !
      - Adafruit-GFX-Library (tested with version 1.10.4), available through Library Manager
      - TFT_eSPI (tested with version 2.3.70), available through Library Manager
      - ESP32-BLE-Keyboard (latest version) download from: https://github.com/T-vK/ESP32-BLE-Keyboard
      - ESPAsyncWebserver (latest version) download from: https://github.com/me-no-dev/ESPAsyncWebServer
      - AsyncTCP (latest version) download from: https://github.com/me-no-dev/AsyncTCP
      - ArduinoJson (tested with version 6.17.3), available through Library Manager

      --- If you use Capacitive touch (ESP32 TouchDown) ---
      - Dustin Watts FT6236 Library (version 1.0.2), https://github.com/DustinWatts/FT6236

  The FILESYSTEM (SPI FLASH filing system) is used to hold touch screen calibration data.
  It has to be runs at least once when using resistive touch. After that you can set
  REPEAT_CAL to false (default).

  !-- Make sure you have setup your TFT display and ESP setup correctly in TFT_eSPI/user_setup.h --!

        Select the right screen driver and the board (ESP32 is the only one tested) you are
        using. Also make sure TOUCH_CS is defined correctly. TFT_BL is also be needed!

        You can find examples of User_Setup.h in the "user_setup.h Examples" folder.

*/
#include <Arduino.h>

#include "FreeTouchDeckWT32.h"

const char *versionnumber = "WT32-0.1.5-AF";

/*
 * Version 0.1.5-AF  - In work
 *    Added USB Comms functionality (like FreeTeamsDeck)
 *
 * Version 0.1.4-AF  - A.Fernie 2022-08-19
 *    Added ability to locally save general config. Allows
 *    SleepEnable status to be preserved without going through web page. Later on allow 
 *    additional params like sleeptimer, colors, etc. 
 *
 * Version 0.1.3-AF  - A.Fernie 2022-08-08
 *  Added monitoring of the voltage of an external battery
 *
 * Version 0.1.2-AF  - A.Fernie 2022-08-04
 * 1. Load config.json file (wasn't being read, so always used defaults)
 * 2. Fix logic for button background color so that it is based on the action for that button being
 *    a menu change or not
 * 3. Fix VSCode.bmp background color (was not completely back)
 * 4. Added some "[INFO]" messages for colors and sleepenable/sleeptimer. Note that yuou need to enable
 *    [INFO] messages by setting "#define LOG_MSG_LEVEL 3  // 1=ERROR, 2=ERROR+WARN, 3=ERROR+WARN+INFO"
 *    in FreeTouchDeckWT32.h to see them
 * 5. No colons after [INFO], [WARN], [ERROR]
 *
 *
 * Version 0.1.1-AF  - A.Fernie 2022-07-18
 * 1. Store sample 3x5 menus
 * 2. Switch to 3x4 (more readable).
 *
 * Version 0.1.0-AF  - A.Fernie 2022-7-15
 *
 * Started from 0.9.16 and added/modified:
 * 1. Now iterates through arrays of buttons rather than using repeated code for each button in
 *    most cases. Still two sections in index.htm that use repeated code.
 * 2. Support up to a 3x5 array of buttons (limited by index.htm)
 * 3. Home page is no longer a special page that just links to five other pages, and the other pages
 *    are no longer simply five action buttons plus a link back to the home page. All buttons on
 *    each page can be actions or links. Just make sure you always have a way back.
 * 4. There are now 10 pages. This includes Menu0 (home).
 * 5. Updated to latest platformio Arduino platform (2.x)
 * 6. Changed from SPIFFS to LittleFS. Note that if you check the serial monitor output you will see
 *    some error messages like "[vfs_api.cpp:104] open(): /littlefs/list does not exist". This is a
 *    known issue with ESPAsyncWebsServer related to LittleFS supporting folders while SPIFFS does not.
 *    see https://github.com/lorol/LITTLEFS/issues/2 for more on the topic.
 * 7. Currently only working on WT32-SC01 card http://www.wireless-tag.com/portfolio/wt32-sc01/
 *
 * Version 0.9.16.
 *
 * Added UserActions. In the UserAction.h file there are a few functions you can define and
 * select through the configurator. The functions have to written before compiling. These functions
 * are then hardcoded. Look at UserActions.h for some examples.
 *
 * Added some missing characters.
 */

#ifdef ENABLE_TOUCH_SCREEN
#ifdef USECAPTOUCH
FT6336U ts = FT6336U(PIN_SDA, PIN_SCL);
#endif
#endif

BleKeyboard bleKeyboard("FreeTouchDeck", "Made by me");

AsyncWebServer webserver(80);

TFT_eSPI tft = TFT_eSPI(SCREEN_HEIGHT, SCREEN_WIDTH);  // Apparently the display is a portrait mode, but we want landscape

Preferences savedStates;

// placeholder for the pagenumber we are on (0 indicates home)
int pageNum = 0;
int callingPageNum = 0;

// Initial LED brightness
int ledBrightness = 255;

// path to the directory the logo are in ! including leading AND trailing / !
char logopath[64] = "/logos/";

// templogopath is used to hold the complete path of an image. It is empty for now.
char templogopath[64] = "";

// Create instances of the structs
Menu menu[NUM_PAGES];

Wificonfig wificonfig;

Config generalconfig;

Generallogos generallogo;

// struct Logos screen[NUM_PAGES];

unsigned long previousMillis = 0;
unsigned long Interval = 0;
bool displayinginfo;
char jsonFileFail[32] = "";

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[BUTTON_ROWS][BUTTON_COLS];

#ifdef READ_EXTERNAL_BATTERY
float readExternalBattery();
float externalBatteryVoltage = 0.0;
long lastADCRead = 0;
#endif

    //-------------------------------- SETUP --------------------------------------------------------------

    void
    setup()
{
    // Use serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    MSG_INFOLN("");

    MSG_INFOLN("[INFO] Loading saved brightness state");
    savedStates.begin("ftd", false);

    ledBrightness = savedStates.getInt("ledBrightness", 255);
    MSG_INFOLN("[INFO] Brightness has been set");

#ifdef ENABLE_TOUCH_SCREEN
#ifdef USECAPTOUCH
    if (!ts.begin()) {
        MSG_WARNLN("[WARNING]: Unable to start the capacitive touchscreen.");
    }
    else {
        MSG_INFOLN("[INFO] Capacitive touch started!");
    }
#endif
#endif

    // Setup PWM channel and attach pin 32
    ledcSetup(0, 5000, 8);
#ifdef TFT_BL
    ledcAttachPin(TFT_BL, 0);
#else
    ledcAttachPin(32, 0);
#endif
    ledcWrite(0, ledBrightness);  // Start @ initial Brightness

    // --------------- Init Display -------------------------

    // Initialise the TFT screen
    tft.init();

    // Set the rotation before we calibrate
    tft.setRotation(1);

    // Clear the screen
    tft.fillScreen(TFT_BLACK);

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    // -------------- Start filesystem ----------------------

    if (!FILESYSTEM.begin()) {
        MSG_ERRORLN("[ERROR] LittleFS initialisation failed!");
        drawErrorMessage("Failed to init LittleFS! Did you upload the data folder?");
        while (1)
            yield();  // We stop here
    }
    MSG_INFOLN("[INFO] LittleFS initialised.");

    // Check for free space

    MSG_INFO("[INFO] Free Space: ");
    MSG_INFOLN(FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes());

    //------------------ Load Wifi Config ----------------------------------------------

    MSG_INFOLN("[INFO] Loading Wifi Config");
    if (!loadMainConfig()) {
        MSG_WARNLN("[WARNING]: Failed to load WiFi Credentials!");
    }
    else {
        MSG_INFOLN("[INFO] WiFi Credentials Loaded");
    }

    // ----------------- Load webserver ---------------------

    handlerSetup();

    // ------------------- Splash screen ------------------

    // If we are woken up we do not need the splash screen
    if (wakeup_reason > 0) {
        // But we do draw something to indicate we are waking up
        tft.setTextFont(2);
        tft.println(" Waking up...");
    }
    else {
        // Draw a splash screen
        if (checkfile("/logos/freetouchdeck_logo.bmp", false)) {
            drawBmp("/logos/freetouchdeck_logo.bmp", 0, 0);
        }
        else {
            MSG_INFOLN("[INFO] No \"/logos/freetouchdeck_logo.bmp\" file found for splash screen.");
        }
        tft.setCursor(1, 3);
        tft.setTextFont(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.printf("Loading version %s\n", versionnumber);
        MSG_INFO1F("[INFO] Loading version %s\n", versionnumber);
    }

// Calibrate the touch screen and retrieve the scaling factors
#ifndef USECAPTOUCH
    touch_calibrate();
#endif

#ifdef READ_EXTERNAL_BATTERY
// External battery read voltage setup

    pinMode(EXTERNAL_BATTERY_PIN, INPUT);
    analogSetPinAttenuation(EXTERNAL_BATTERY_PIN, ADC_11db);
    adcAttachPin(EXTERNAL_BATTERY_PIN);
#endif

    // Let's first check if all the files we need exist
    if (!checkfile("/config/general.json", true)) {
        MSG_ERRORLN("[ERROR] /config/general.json not found!");
        while (1)
            yield();  // Stop!
    }

    for (size_t i = 0; i < NUM_PAGES; i++) {
        char filename[32];
        char filenumber[4];
        strcpy(filename, "/config/menu");
        sprintf(filenumber, "%d", i);
        strcat(filename, filenumber);
        strcat(filename, ".json");

        if (!checkfile(filename, false)) {
            MSG_WARN2("[WARNING]: ", filename, " not found. Initializing to default.json");
            if (CopyFile("/config/default.json", filename)) {
                MSG_INFO2("[WARN] Successful initialization of ", filename, " to default.json");
            }
            else {
                MSG_ERROR1("[ERROR] Failed to initialize ", filename);
                checkfile(filename, true);  // This will force a message to the TFT screen

                while (1)
                    yield();  // Stop!
            }
        }
    }

    // Setup PWM channel for Piezo speaker

#ifdef speakerPin
    ledcSetup(2, 500, 8);

    if (generalconfig.beep) {
        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 600);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);

        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 800);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);

        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 1200);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);
    }

#endif

    // Now load the config files, starting with general.json
    if (!loadConfig("general")) {
        Serial.print("[WARNING]: general.json failed to load!");
        Serial.print("[WARNING]: To reset to default type \'reset general\'");
        pageNum = SPECIAL_4_PAGE;
    }
    else {
#ifdef TOUCH_INTERRUPT_PIN
        Interval = generalconfig.sleeptimer * MIN_TO_MS;
        if (generalconfig.sleepenable) {
            pinMode(TOUCH_INTERRUPT_PIN, INPUT_PULLUP);
            MSG_INFOLN("[INFO] Sleep enabled.");
            MSG_INFO("[INFO] Sleep timer = ");
            MSG_INFO(generalconfig.sleeptimer);
            MSG_INFOLN(" minutes");
        }
        else {
            MSG_INFOLN("[INFO] Sleep not enabled.");
            MSG_INFO("[INFO] Sleep timer = ");
            MSG_INFO(generalconfig.sleeptimer);
            MSG_INFOLN(" minutes");
        }
#endif
    }

    // Next, load the menu pages
    for (size_t i = 0; i < NUM_PAGES; i++) {
        char menuName[16];
        char menuNumber[4];
        char menuNumberZeroIndex[4];
        strcpy(menuName, "menu");
        sprintf(menuNumber, "%d", i);
        strcat(menuName, menuNumber);

        if (!loadConfig(menuName)) {
            Serial.print("[WARNING]: menu");
            sprintf(menuNumberZeroIndex, "%d", i);
            Serial.print(menuNumberZeroIndex);
            MSG_INFOLN(".json seems to be corrupted!");
            Serial.print("[WARNING]: To reset to default type 'reset ");
            Serial.print(menuName);
            MSG_INFOLN("'.");
            strcpy(jsonFileFail, menuName);
            pageNum = SPECIAL_4_PAGE;
        }
    }

    MSG_INFOLN("[INFO] All configs loaded");

    // Setup the Font used for plain text
    tft.setFreeFont(LABEL_FONT);

    //------------------BLE Initialization ------------------------------------------------------------------------

    MSG_INFOLN("[INFO] Starting BLE");
    bleKeyboard.begin();

    // ---------------- Printing version numbers -----------------------------------------------
    MSG_INFO("[INFO] BLE Keyboard version: ");
    MSG_INFOLN(BLE_KEYBOARD_VERSION);
    MSG_INFO("[INFO] ArduinoJson version: ");
    MSG_INFOLN(ARDUINOJSON_VERSION);
    MSG_INFO("[INFO] TFT_eSPI version: ");
    MSG_INFOLN(TFT_ESPI_VERSION);

    // ---------------- Start the first keypad -------------

    // Draw background
    tft.fillScreen(generalconfig.backgroundColour);

    // Draw keypad
    MSG_INFOLN("[INFO] Drawing keypad");
    drawKeypad();
}

//--------------------- LOOP ---------------------------------------------------------------------

void loop(void)
{
#ifdef ENABLE_TOUCH_SCREEN
#ifdef USECAPTOUCH
    FT6336U_TouchPointType touchPos;
#endif
#endif

    bool pressed = false;
    uint16_t t_x = 0, t_y = 0;

    // Check if there is data available on the serial input that needs to be handled.

    if (Serial.available()) {
        String command = Serial.readStringUntil(' ');

        if (command == "cal") {
            MSG_INFOLN("[INFO] received command cal");
            FILESYSTEM.remove(CALIBRATION_FILE);
            ESP.restart();
        }
        else if (command == "setssid") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setssid ", value.c_str());
            if (saveWifiSSID(value)) {
                MSG_INFO1F("[INFO] Saved new SSID: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setpassword") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setpassword ", value.c_str());
            if (saveWifiPW(value)) {
                MSG_INFO1F("[INFO] Saved new Password: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setwifimode") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                MSG_INFO1F("[INFO] Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setwifimodesta") {
            String value = "WIFI_STA";
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                MSG_INFO1F("[INFO] Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setwifimodeap") {
            String value = "WIFI_AP";
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                MSG_INFO1F("[INFO] Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "restart") {
            MSG_WARNLN("[WARNING]: Restarting");
            ESP.restart();
        }
        else if (command == "reset") {
            String file = Serial.readString();
            MSG_INFO1F("[INFO] Resetting %s.json now\n", file.c_str());
            resetconfig(file);
        }
        else {
            for (size_t i = 0; i < NUM_PAGES; i++) {
                char menuName[16];
                sprintf(menuName, "menu%d", i);

                if (command == menuName && pageNum != i && pageNum != WEB_REQUEST_PAGE) {
                    pageNum = i;
                    drawKeypad();
                    MSG_INFO("Auto Switched to ");
                    MSG_INFOLN(menuName);
                }
            }
        }
    }

    touchPos = ts.scan();

    if (touchPos.tp[0].tapped) {
        // MSG_DEBUG(" Tap Detected: ");
        // MSG_DEBUGLN(touchPos.touch_count);
        // MSG_DEBUG(" pageNum: ");
        // MSG_DEBUGLN(pageNum);
        // MSG_DEBUG(" x,y: ");
        // MSG_DEBUG(touchPos.tp[0].x);
        // MSG_DEBUG(" : ");
        // MSG_DEBUGLN(touchPos.tp[0].y);

        // Flip things around so it matches our screen rotation
        //         p.x = map(p.x, 0, 320, 320, 0);

        t_x = touchPos.tp[0].y;
        t_y = 320 - touchPos.tp[0].x;
        pressed = true;
    }
    else {
        pressed = false;
    }

    if (pageNum == WEB_REQUEST_PAGE) {
        // If the pageNum is set to NUM_PAGES+1, do not draw anything on screen or check for touch
        // and start handling incomming web requests.
    }
    else if (pageNum == SPECIAL_PAGE_INFO) {
        if (!displayinginfo) {
            printinfo();
        }

        if (pressed) {
            displayinginfo = false;
            pageNum = callingPageNum;
            tft.fillScreen(generalconfig.backgroundColour);
            drawKeypad();
        }
    }
    else if (pageNum == SPECIAL_3_PAGE) {
        // We were unable to connect to WiFi. Waiting for touch to get back to the settings menu.

        if (pressed) {
            // Return to Settings page
            displayinginfo = false;
            pageNum = callingPageNum;
            tft.fillScreen(generalconfig.backgroundColour);
            drawKeypad();
        }
    }
    else if (pageNum == SPECIAL_4_PAGE) {
        // A JSON file failed to load. We are drawing an error message. And waiting for a touch.

        if (pressed) {
            // Load home screen
            displayinginfo = false;
            pageNum = callingPageNum;
            tft.fillScreen(generalconfig.backgroundColour);
            drawKeypad();
        }
    }
    else {
        // Check if sleep is enabled and if our timer has ended.

#ifdef TOUCH_INTERRUPT_PIN
        if (generalconfig.sleepenable) {
            if (millis() > previousMillis + Interval) {
                // The timer has ended and we are going to sleep  .
                tft.fillScreen(TFT_BLACK);
                MSG_INFOLN("[INFO] Going to sleep.");
#ifdef speakerPin
                if (generalconfig.beep) {
                    ledcAttachPin(speakerPin, 2);
                    ledcWriteTone(2, 1200);
                    delay(150);
                    ledcDetachPin(speakerPin);
                    ledcWrite(2, 0);

                    ledcAttachPin(speakerPin, 2);
                    ledcWriteTone(2, 800);
                    delay(150);
                    ledcDetachPin(speakerPin);
                    ledcWrite(2, 0);

                    ledcAttachPin(speakerPin, 2);
                    ledcWriteTone(2, 600);
                    delay(150);
                    ledcDetachPin(speakerPin);
                    ledcWrite(2, 0);
                }
#endif
                MSG_INFOLN("[INFO] Saving latched states");

                esp_sleep_enable_ext0_wakeup(TOUCH_INTERRUPT_PIN, 0);
                esp_deep_sleep_start();
            }
        }
#endif

        // // Touch coordinates are stored here

        // Check if the X and Y coordinates of the touch are within one of our buttons
        for (uint8_t i = 0; i < BUTTON_ROWS; i++) {
            for (uint8_t j = 0; j < BUTTON_COLS; j++) {
                if (pressed && key[i][j].contains(t_x, t_y)) {
                    key[i][j].press(true);  // tell the button it is pressed

                    // After receiving a valid touch reset the sleep timer
                    previousMillis = millis();
                }
                else {
                    key[i][j].press(false);  // tell the button it is NOT pressed
                }
            }
        }
        // Check if any key has changed state
        for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
            for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                if (key[row][col].justReleased()) {
                    // Draw normal button space (non inverted)

                    uint16_t buttonBG;
                    bool drawTransparent;

                    uint16_t imageBGColor;
                    if (menu[pageNum].button[row][col].islatched) {
                        // TODO                if (menu[pageNum].button[row][col].islatched && b < (BUTTONS_PER_PAGE)) {
                        //                     if (islatched[pageNum][b] && b < (BUTTONS_PER_PAGE)) {
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

                if (key[row][col].justPressed()) {
// Beep
#ifdef speakerPin
                    if (generalconfig.beep) {
                        ledcAttachPin(speakerPin, 2);
                        ledcWriteTone(2, 600);
                        delay(50);
                        ledcDetachPin(speakerPin);
                        ledcWrite(2, 0);
                    }
#endif

                    tft.setFreeFont(LABEL_FONT);
                    key[row][col].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                                             KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
                                             KEY_W, KEY_H, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                                             (char *)"", KEY_TEXTSIZE);
                    key[row][col].drawButton();

                    //---------------------------------------- Button press handling --------------------------------------------------

                    if ((pageNum >= 0) && (pageNum < NUM_PAGES)) {
                        if (row < BUTTON_ROWS && col < BUTTON_COLS) {
                            bleKeyboardAction(menu[pageNum].button[row][col].actions[0].action, menu[pageNum].button[row][col].actions[0].value, menu[pageNum].button[row][col].actions[0].symbol);
                            bleKeyboardAction(menu[pageNum].button[row][col].actions[1].action, menu[pageNum].button[row][col].actions[1].value, menu[pageNum].button[row][col].actions[1].symbol);
                            bleKeyboardAction(menu[pageNum].button[row][col].actions[2].action, menu[pageNum].button[row][col].actions[2].value, menu[pageNum].button[row][col].actions[2].symbol);
                            bleKeyboard.releaseAll();
                            if (menu[pageNum].button[row][col].latch) {
                                if (menu[pageNum].button[row][col].islatched) {
                                    menu[pageNum].button[row][col].islatched = false;
                                }
                                else {
                                    menu[pageNum].button[row][col].islatched = true;
                                }
                            }

                            if (generalconfig.usbcommsenable){
                                Serial.print("{");
                                Serial.print(menu[pageNum].name);
                                Serial.print(",");

                                //find last "/" in logo
                                char *lastSlash = strrchr(menu[pageNum].button[row][col].logo, '/');

                                // if lastSlash not equal to null, get a pointer to the rest of the character array
                                if (lastSlash != NULL) {
                                    lastSlash++;
                                }
                                else {
                                    lastSlash = menu[pageNum].button[row][col].logo;
                                }

                                Serial.print(lastSlash);
                                Serial.println("}");
                            }
                        }
                        else  // Back home
                        {
                            pageNum = 0;
                            drawKeypad();
                        }
                    }

                    MSG_INFO1("Battery voltage:", externalBatteryVoltage);
                }

                delay(10);  // UI debouncing
            }
        }
    }
#ifdef READ_EXTERNAL_BATTERY
    if((millis() - lastADCRead) > 100){
        float newVoltage = readExternalBattery();

        externalBatteryVoltage = externalBatteryVoltage + 0.1 * (newVoltage - externalBatteryVoltage);
        lastADCRead = millis();
    }
#endif
}

#ifdef READ_EXTERNAL_BATTERY
float readExternalBattery()
{
    uint16_t voltage = analogRead(EXTERNAL_BATTERY_PIN);
    float scaledVoltage = (float)voltage  * EXTERNAL_BATTERY_SCALE;
    return scaledVoltage;
}
#endif
