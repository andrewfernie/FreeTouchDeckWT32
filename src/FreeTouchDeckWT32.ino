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
// Basic macros for debug and info messages to the serial port
#define LOG_MSG_BASIC 1
#define LOG_MSG_LEVEL 3
#define LOG_MSG_DEBUG 1
#include "std_defs.h"

// ------- Uncomment the next line if you use capacitive touch -------
// (THE ESP32 TOUCHDOWN USES THIS!)
#define USECAPTOUCH

// ------- Uncomment and populate the following if your cap touch uses custom i2c pins -------
//#define CUSTOM_TOUCH_SDA 26
//#define CUSTOM_TOUCH_SCL 27

// PAY ATTENTION! Even if resistive touch is not used, the TOUCH pin has to be defined!
// It can be a random unused pin.
// TODO: Find a way around this!

// ------- Uncomment the define below if you want to use SLEEP and wake up on touch -------
// The pin where the IRQ from the touch screen is connected uses ESP-style GPIO_NUM_* instead of just pinnumber
#define touchInterruptPin GPIO_NUM_39

// ------- Uncomment the define below if you want to use a piezo buzzer and specify the pin where the speaker is connected -------
//#define speakerPin 26

// ------- NimBLE definition, use only if the NimBLE library is installed
// and if you are using the original ESP32-BLE-Keyboard library by T-VK -------
//#define USE_NIMBLE

const char *versionnumber = "WT32-0.9.17";

/* Version 0.9.16.
 *
 * Added UserActions. In the UserAction.h file there are a few functions you can define and
 * select through the configurator. The functions have to written before compiling. These functions
 * are then hardcoded. Look at UserActions.h for some examples.
 *
 * Added some missing characters.
 */

/* TODO NEXT VERSION
 *
 * - get image height/width and use it in bmp drawing.
 */

#include <BleKeyboard.h>  // BleKeyboard is used to communicate over BLE
#include <FS.h>           // Filesystem support header
#include <Preferences.h>  // Used to store states before sleep/reboot
#include <SPIFFS.h>       // Filesystem support header
#include <TFT_eSPI.h>     // The TFT_eSPI library
#include <pgmspace.h>     // PROGMEM support header

#if defined(USE_NIMBLE)

#include "NimBLEBeacon.h"  // Additional BLE functionaity using NimBLE
#include "NimBLEDevice.h"  // Additional BLE functionaity using NimBLE
#include "NimBLEUtils.h"   // Additional BLE functionaity using NimBLE

#else

#include "BLEBeacon.h"  // Additional BLE functionaity
#include "BLEDevice.h"  // Additional BLE functionaity
#include "BLEUtils.h"   // Additional BLE functionaity

#endif  // USE_NIMBLE

#include <ArduinoJson.h>        // Using ArduinoJson to read and write config files
#include <AsyncTCP.h>           //Async Webserver support header
#include <ESPAsyncWebServer.h>  //Async Webserver support header
#include <ESPmDNS.h>            // DNS functionality
#include <WiFi.h>               // Wifi support

#include "esp_bt_device.h"  // Additional BLE functionaity
#include "esp_bt_main.h"    // Additional BLE functionaity
#include "esp_sleep.h"      // Additional BLE functionaity

// Set the width and height of your screen here:
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

// Button layout and number of paghes with buttons
#define BUTTON_ROWS 2
#define BUTTON_COLS 3
#define BUTTONS_PER_PAGE (BUTTON_ROWS * BUTTON_COLS)
#define NUM_PAGES 7

// Keypad start position, centre of the first button
#define KEY_X SCREEN_WIDTH / (BUTTON_COLS * 2)
#define KEY_Y SCREEN_HEIGHT / (BUTTON_ROWS * 2)

// Gaps between buttons
#define KEY_SPACING_X SCREEN_WIDTH / (BUTTON_COLS * 8)
#define KEY_SPACING_Y SCREEN_HEIGHT / (BUTTON_ROWS * 8)

// Width and height of a button
#define KEY_W (SCREEN_WIDTH / 3) - KEY_SPACING_X
#define KEY_H (SCREEN_WIDTH / 3) - KEY_SPACING_Y

// Font size multiplier
#define KEY_TEXTSIZE 1

// Touch panel definition
#ifdef USECAPTOUCH
#include <Wire.h>

#include "FT62XXTouchScreen.h"
FT62XXTouchScreen ts = FT62XXTouchScreen(SCREEN_HEIGHT, PIN_SDA, PIN_SCL);
#else
#include "Touch.h"
#endif

BleKeyboard bleKeyboard("FreeTouchDeck", "Made by me");

AsyncWebServer webserver(80);

TFT_eSPI tft = TFT_eSPI();

Preferences savedStates;

// Define the storage to be used. For now just SPIFFS.
#define FILESYSTEM SPIFFS

// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The FILESYSTEM file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

// Text Button Label Font
#define LABEL_FONT &FreeSansBold12pt7b

// placeholder for the pagenumber we are on (0 indicates home)
int pageNum = 0;
int callingPageNum = 0;

// Initial LED brightness
int ledBrightness = 255;

// path to the directory the logo are in ! including leading AND trailing / !
char logopath[64] = "/logos/";

// templogopath is used to hold the complete path of an image. It is empty for now.
char templogopath[64] = "";

// Struct Action: actions and value
struct Actions {
    uint8_t action;
    uint8_t value;
    char symbol[64];
};

// Each button has an action struct in it
struct Button {
    struct Actions actions[3];
    bool latch;
    bool islatched;
    char logo[32];
    char latchlogo[32];
};

// Each menu has an array of buttons
struct Menu {
    struct Button button[BUTTON_ROWS][BUTTON_COLS];
};

// And finally, we have an array of menus
struct Menu menu[NUM_PAGES];

// Struct to hold the general logos.
struct Generallogos {
    char homebutton[64];
    char configurator[64];
};

// Struct to hold the general config like colours.
struct Config {
    uint16_t menuButtonColour;
    uint16_t functionButtonColour;
    uint16_t backgroundColour;
    uint16_t latchedColour;
    bool sleepenable;
    uint16_t sleeptimer;
    bool beep;
    uint8_t modifier1;
    uint8_t modifier2;
    uint8_t modifier3;
    uint16_t helperdelay;
};

struct Wificonfig {
    char ssid[64];
    char password[64];
    char wifimode[9];
    char hostname[64];
    uint8_t attempts;
    uint16_t attemptdelay;
};

// Array to hold all the latching statuses
// bool islatched[NUM_PAGES][BUTTONS_PER_PAGE] = {0};

// Create instances of the structs
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

// Checking for BLE Keyboard version
#ifndef BLE_KEYBOARD_VERSION
#warning Old BLE Keyboard version detected. Please update.
#define BLE_KEYBOARD_VERSION "Outdated"
#endif

// Special pages
#define WEB_REQUEST_PAGE (NUM_PAGES + 1)
#define SPECIAL_PAGE_INFO (NUM_PAGES + 2)
#define SPECIAL_3_PAGE (NUM_PAGES + 3)
#define SPECIAL_4_PAGE (NUM_PAGES + 4)

uint8_t sleepIsLatched = false;

//--------- Internal references ------------
// (this needs to be below all structs etc..)
enum ActionEnum {
    Action_NoAction = 0,
    Action_Delay = 1,
    Action_TabArrow = 2,
    Action_Media = 3,
    Action_Char = 4,
    Action_Option = 5,
    Action_FnKey = 6,
    Action_Number = 7,
    Action_SpecialChar = 8,
    Action_Combos = 9,
    Action_Helpers = 10,
    Action_SpecialFn = 11,
    Action_Numpad = 12,
    Action_CustomFn = 13,
    Action_ChangePage = 14
};

#include "ScreenHelper.h"
#include "ConfigLoad.h"
#include "DrawHelper.h"
#include "ConfigHelper.h"
#include "UserActions.h"
#include "Action.h"
#include "Webserver.h"
#include "Touch.h"

//-------------------------------- SETUP --------------------------------------------------------------

void setup()
{
    // Use serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println("");

    MSG_INFOLN("[INFO]: Loading saved brightness state");
    savedStates.begin("ftd", false);

    ledBrightness = savedStates.getInt("ledBrightness", 255);

#ifdef USECAPTOUCH
#ifdef CUSTOM_TOUCH_SDA
    if (!ts.begin(40, CUSTOM_TOUCH_SDA, CUSTOM_TOUCH_SCL))
#else
    if (!ts.begin())
#endif
    {
        MSG_WARNLN("[WARNING]: Unable to start the capacitive touchscreen.");
    }
    else {
        MSG_INFOLN("[INFO]: Capacitive touch started!");
    }
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
        MSG_ERRORLN("[ERROR]: SPIFFS initialisation failed!");
        drawErrorMessage("Failed to init SPIFFS! Did you upload the data folder?");
        while (1)
            yield();  // We stop here
    }
    MSG_INFOLN("[INFO]: SPIFFS initialised.");

    // Check for free space

    MSG_INFO("[INFO]: Free Space: ");
    Serial.println(SPIFFS.totalBytes() - SPIFFS.usedBytes());

    //------------------ Load Wifi Config ----------------------------------------------

    MSG_INFOLN("[INFO]: Loading Wifi Config");
    if (!loadMainConfig()) {
        MSG_WARNLN("[WARNING]: Failed to load WiFi Credentials!");
    }
    else {
        MSG_INFOLN("[INFO]: WiFi Credentials Loaded");
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
        drawBmp("/logos/freetouchdeck_logo.bmp", 0, 0);
        tft.setCursor(1, 3);
        tft.setTextFont(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.printf("Loading version %s\n", versionnumber);
        Serial.printf("[INFO]: Loading version %s\n", versionnumber);
    }

// Calibrate the touch screen and retrieve the scaling factors
#ifndef USECAPTOUCH
    touch_calibrate();
#endif

    // Let's first check if all the files we need exist
    if (!checkfile("/config/general.json")) {
        MSG_ERRORLN("[ERROR]: /config/general.json not found!");
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

        if (!checkfile(filename)) {
            MSG_ERROR("[ERROR]: ");
            Serial.print(filename);
            Serial.println(" not found!");
            while (1)
                yield();  // Stop!
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
            Serial.println(".json seems to be corrupted!");
            Serial.print("[WARNING]: To reset to default type 'reset ");
            Serial.print(menuName);
            Serial.println("'.");
            strcpy(jsonFileFail, menuName);
            pageNum = SPECIAL_4_PAGE;
        }
    }

    MSG_INFOLN("[INFO]: All configs loaded");

    // Setup the Font used for plain text
    tft.setFreeFont(LABEL_FONT);

    //------------------BLE Initialization ------------------------------------------------------------------------

    MSG_INFOLN("[INFO]: Starting BLE");
    bleKeyboard.begin();

    // ---------------- Printing version numbers -----------------------------------------------
    MSG_INFO("[INFO]: BLE Keyboard version: ");
    Serial.println(BLE_KEYBOARD_VERSION);
    MSG_INFO("[INFO]: ArduinoJson version: ");
    Serial.println(ARDUINOJSON_VERSION);
    MSG_INFO("[INFO]: TFT_eSPI version: ");
    Serial.println(TFT_ESPI_VERSION);

    // ---------------- Start the first keypad -------------

    // Draw background
    tft.fillScreen(generalconfig.backgroundColour);

    // Draw keypad
    MSG_INFOLN("[INFO]: Drawing keypad");
    drawKeypad();

#ifdef touchInterruptPin
    if (generalconfig.sleepenable) {
        pinMode(touchInterruptPin, INPUT_PULLUP);
        Interval = generalconfig.sleeptimer * 60000;
        MSG_INFOLN("[INFO]: Sleep enabled.");
        MSG_INFO("[INFO]: Sleep timer = ");
        Serial.print(generalconfig.sleeptimer);
        Serial.println(" minutes");
        sleepIsLatched = 1;        
    }
#endif
}

//--------------------- LOOP ---------------------------------------------------------------------

void loop(void)
{
#ifdef USECAPTOUCH
    TouchPoint touchPos;
#endif

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
                Serial.printf("[INFO]: Saved new SSID: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO]: New configuration loaded");
            }
        }
        else if (command == "setpassword") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setpassword ", value.c_str());
            if (saveWifiPW(value)) {
                Serial.printf("[INFO]: Saved new Password: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO]: New configuration loaded");
            }
        }
        else if (command == "setwifimode") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                Serial.printf("[INFO]: Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO]: New configuration loaded");
            }
        }
        else if (command == "setwifimodesta") {
            String value = "WIFI_STA";
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                Serial.printf("[INFO]: Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO]: New configuration loaded");
            }
        }
        else if (command == "setwifimodeap") {
            String value = "WIFI_AP";
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                Serial.printf("[INFO]: Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO]: New configuration loaded");
            }
        }
        else if (command == "restart") {
            MSG_WARNLN("[WARNING]: Restarting");
            ESP.restart();
        }
        else if (command == "reset") {
            String file = Serial.readString();
            Serial.printf("[INFO]: Resetting %s.json now\n", file.c_str());
            resetconfig(file);
        }
        else {
            for (size_t i = 1; i < NUM_PAGES; i++) {
                char menuName[16];
                sprintf(menuName, "menu%d", i);

                if (command == menuName && pageNum != i && pageNum != WEB_REQUEST_PAGE) {
                    pageNum = i;
                    drawKeypad();
                    Serial.print("Auto Switched to ");
                    Serial.println(menuName);
                }
            }
        }
    }

    if (pageNum == WEB_REQUEST_PAGE) {
        // If the pageNum is set to NUM_PAGES+1, do not draw anything on screen or check for touch
        // and start handeling incomming web requests.
    }
    else if (pageNum == SPECIAL_PAGE_INFO) {
        if (!displayinginfo) {
            printinfo();
        }

        //    uint16_t t_x = 0, t_y = 0;

        // At the beginning of a new loop, make sure we do not use last loop's touch.
        boolean pressed = false;

#ifdef USECAPTOUCH

        touchPos = ts.read();

        if (touchPos.touched) {
            // Flip things around so it matches our screen rotation
            //        p.x = map(p.x, 0, 320, 320, 0);
            //       t_x = touchPos.xPos;
            //        t_y = touchPos.yPos;

            pressed = true;
        }

#else
        uint16_t t_x = 0, t_y = 0;
        pressed = tft.getTouch(&t_x, &t_y);

#endif

        if (pressed) {
            displayinginfo = false;
            pageNum = callingPageNum;
            tft.fillScreen(generalconfig.backgroundColour);
            drawKeypad();
        }
    }
    else if (pageNum == SPECIAL_3_PAGE) {
        // We were unable to connect to WiFi. Waiting for touch to get back to the settings menu.
        //    uint16_t t_x = 0, t_y = 0;

        // At the beginning of a new loop, make sure we do not use last loop's touch.
        boolean pressed = false;

#ifdef USECAPTOUCH
        touchPos = ts.read();

        if (touchPos.touched) {
            // Flip things around so it matches our screen rotation
            //         p.x = map(p.x, 0, 320, 320, 0);
            //        t_x = touchPos.xPos;
            //        t_y = touchPos.yPos;

            pressed = true;
        }

#else
        uint16_t t_x = 0, t_y = 0;
        pressed = tft.getTouch(&t_x, &t_y);

#endif

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
        // uint16_t t_x = 0, t_y = 0;

        // At the beginning of a new loop, make sure we do not use last loop's touch.
        boolean pressed = false;

#ifdef USECAPTOUCH
        touchPos = ts.read();

        if (touchPos.touched) {
            // Flip things around so it matches our screen rotation
            //         p.x = map(p.x, 0, 320, 320, 0);
            //      t_x = touchPos.xPos;
            //     t_y = touchPos.yPos;

            pressed = true;
        }

#else
        uint16_t t_x = 0, t_y = 0;
        pressed = tft.getTouch(&t_x, &t_y);

#endif

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

#ifdef touchInterruptPin
        if (generalconfig.sleepenable) {
            if (millis() > previousMillis + Interval) {
                // The timer has ended and we are going to sleep  .
                tft.fillScreen(TFT_BLACK);
                MSG_INFOLN("[INFO]: Going to sleep.");
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
                MSG_INFOLN("[INFO]: Saving latched states");

                esp_sleep_enable_ext0_wakeup(touchInterruptPin, 0);
                esp_deep_sleep_start();
            }
        }
#endif

        // Touch coordinates are stored here
        uint16_t t_x = 0, t_y = 0;

        // At the beginning of a new loop, make sure we do not use last loop's touch.
        boolean pressed = false;

#ifdef USECAPTOUCH
        touchPos = ts.read();

        if (touchPos.touched) {
            // Flip things around so it matches our screen rotation
            //         p.x = map(p.x, 0, 320, 320, 0);
            t_x = touchPos.xPos;
            t_y = touchPos.yPos;

            pressed = true;
        }
#else

        pressed = tft.getTouch(&t_x, &t_y);

#endif

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
                        if (pageNum == 0) {
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

                    //---------------------------------------- Button press handeling --------------------------------------------------

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
                        }
                        else  // Back home
                        {
                            pageNum = 0;
                            drawKeypad();
                        }
                    }
                }

                delay(10);  // UI debouncing
            }
        }
    }
}
