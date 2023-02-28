#pragma once

// Basic macros for debug and info messages to the serial port
#define LOG_MSG_BASIC 1
#define LOG_MSG_LEVEL 2  // 1=ERROR, 2=ERROR+WARN, 3=ERROR+WARN+INFO
#define LOG_MSG_DEBUG 1
#define LOG_MSG_TOUCH_DEBUG 0  // messages to console each time a touch is detected
#define USE_DEBUG_PINS 0

#include "std_defs.h"
 
// NOTE: The hardware configuration for the display (pins, display type, etc.) is defined in the platformio.ini file. 
// This avoids the need to modify files in the TFT_eSPI library, which would be overwritten when the library is updated.


#include <BleKeyboard.h>  // BleKeyboard is used to communicate over BLE
#include <FS.h>           // Filesystem support header
#include <Preferences.h>  // Used to store states before sleep/reboot
#include <TFT_eSPI.h>     // The TFT_eSPI library
#include <pgmspace.h>     // PROGMEM support header

// Define the storage to be used. 
#define FILESYSTEM_LITTLEFS

#ifdef FILESYSTEM_LITTLEFS
#define FILESYSTEM LittleFS
#include <LittleFS.h>  // https://github.com/lorol/LITTLEFS
#else
#define FILESYSTEM SPIFFS
#include <SPIFFS.h>  // Filesystem support header
#endif

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

#include "DrawHelper.h"
#include "Action.h"
#include "ConfigHelper.h"
#include "ConfigLoad.h"
#include "ScreenHelper.h"
#include "Touch.h"
#include "UserActions.h"
#include "Webserver.h"


#define MIN_TO_MS 60 * 1000

const long loop_period = 20;  // 20ms loop period

extern const char* versionnumber;

// ------- Uncomment the define below if you want to use a piezo buzzer and specify the pin where the speaker is connected -------
//#define speakerPin 26

// ------- Enable to read and report battery voltage.  -------
#define READ_EXTERNAL_BATTERY
#ifdef READ_EXTERNAL_BATTERY
#define EXTERNAL_BATTERY_PIN 33
#define EXTERNAL_BATTERY_ADC_SCALE (3.3/4095.0)            // Default attenuation results in 2.6V for full scale on ADC
#define EXTERNAL_BATTERY_SCALE (EXTERNAL_BATTERY_ADC_SCALE * 2.13)  // include any input voltage dividers here
#endif


// ------- Uncomment the define below if you want to preload the logos to PSRAM at start (adds about 10s to startup) -------
//#define PRELOAD_LOGOS

    // Logo Size
#define LOGO_SIZE_X_Y 75

// Button layout and number of pages with buttons
#define BUTTON_ROWS 3
#define BUTTON_COLS 4
#define BUTTONS_PER_PAGE (BUTTON_ROWS * BUTTON_COLS)
#define NUM_PAGES 10  // Includes Menu0 which is the home page

// Gaps between buttons
// First - strictly define the gap between each button
#define KEY_SPACING_X 12   
#define KEY_SPACING_Y 12

// ------- Status bar enable and configuration -------
#define ENABLE_TOP_STATUS_BAR
//#define ENABLE_BOTTOM_STATUS_BAR

#ifdef ENABLE_TOP_STATUS_BAR
#define TOP_STATUS_BAR_HEIGHT 20
#define TOP_STATUS_BAR_SHOW_BT
//#define TOP_STATUS_BAR_SHOW_MENU
#define TOP_STATUS_BAR_SHOW_V
#else
#define TOP_STATUS_BAR_HEIGHT 0
#endif

#ifdef ENABLE_BOTTOM_STATUS_BAR
#define BOTTOM_STATUS_BAR_HEIGHT 20
#else
#define BOTTOM_STATUS_BAR_HEIGHT 0
#endif

// Define or calculate a margin around the edge of the display (default is half of the key spacing value)
// Separate values for top and bottom allow for a status bar at the top or bottom of the screen
#define KEY_MARGIN_X (KEY_SPACING_X / 2)
#define KEY_MARGIN_Y_TOP (TOP_STATUS_BAR_HEIGHT + KEY_SPACING_Y / 2)
#define KEY_MARGIN_Y_BOTTOM (BOTTOM_STATUS_BAR_HEIGHT + KEY_SPACING_Y / 2)

// Width and height of a button
// Second - define button width based on spacing and number of buttons (use what's left)
#define KEY_W ((SCREEN_WIDTH - (2 * KEY_MARGIN_X)) - ( BUTTON_COLS - 1 ) * ( KEY_SPACING_X + 1 ) ) / BUTTON_COLS
#define KEY_H ((SCREEN_HEIGHT - (KEY_MARGIN_Y_TOP + KEY_MARGIN_Y_BOTTOM)) - ( BUTTON_ROWS - 1 ) * ( KEY_SPACING_Y + 1 ) ) / BUTTON_ROWS

// Keypad start position, centre of the first button
#define KEY_X KEY_MARGIN_X + (KEY_W / 2)
#define KEY_Y KEY_MARGIN_Y_TOP + (KEY_H / 2)

// Font size multiplier
#define KEY_TEXTSIZE 1

// Display of wifi password when wifi is enabled
//#define WIFI_PASSWORD_DISPLAY

// Touch panel definition
#ifdef USECAPTOUCH
    // ------- Uncomment and populate the following if your cap touch uses custom i2c pins -------
#define CUSTOM_TOUCH_SDA PIN_SDA
#define CUSTOM_TOUCH_SCL PIN_SCL

    // With a capacitive touch screen the choice is between the FT6336U library and the FT6236 library from Dustin Watts
    // The FT6336U library supports multi-touch, which is not used yet in this project, but might be in the future.
    // At present, either will work fine.
#define USE_FT6336U_LIB
#include <Wire.h>
#ifdef USE_FT6336U_LIB
#include "FT6336U.h"
    extern FT6336U ts;
#else
#include "FT6236.h"
    extern FT6236 ts;
#endif
#else
    #include "Touch.h"
#endif

extern BleKeyboard bleKeyboard;

extern AsyncWebServer webserver;

extern TFT_eSPI tft;

extern Preferences savedStates;

extern bool psramAvailable;

const uint8_t DEBUG_PIN_1 = 25;
const uint8_t DEBUG_PIN_2 = 26;
const uint8_t DEBUG_PIN_3 = 27;
const uint8_t DEBUG_PIN_4 = 5;

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
extern int pageNum;
extern int callingPageNum;

// Initial LED brightness
extern int ledBrightness;

// path to the directory the logo are in ! including leading AND trailing / !
extern char logopath[64];

// templogopath is used to hold the complete path of an image. It is empty for now.
extern char templogopath[64];


extern long loop_count;

// Struct Action: actions and value
struct Actions {
    uint8_t action;
    uint8_t value;
    char symbol[64];
};

// Each button has an action struct in it
struct Button {
    Actions actions[3];
    bool latch;
    bool islatched;
    char logo[32];
    char latchlogo[32];
    uint16_t imageBGColour;
    uint16_t latchImageBGColour;
    uint8_t imageBGColourValid;
    uint8_t latchImageBGColourValid;
    uint16_t* pImage;
    uint16_t* pLatchImage;
};

// Each menu has an array of buttons
struct Menu {
    char name[32];
    Button button[BUTTON_ROWS][BUTTON_COLS];
};

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
    bool usbcommsenable;
    bool beep;
    uint8_t modifier1;
    uint8_t modifier2;
    uint8_t modifier3;
    uint16_t helperdelay;
    uint16_t startup_menu;
};

struct Wificonfig {
    char ssid[64];
    char password[64];
    char wifimode[9];
    char hostname[64];
    uint8_t attempts;
    uint16_t attemptdelay;
};

// Create instances of the structs
extern Wificonfig wificonfig;

extern Config generalconfig;

// And finally, we have an array of menus
extern Menu menu[NUM_PAGES];

extern Generallogos generallogo;

// struct Logos screen[NUM_PAGES];

extern unsigned long previousMillis;
extern unsigned long Interval;
extern bool displayinginfo;
extern char jsonFileFail[32];

// Invoke the TFT_eSPI button class and create all the button objects
extern TFT_eSPI_Button key[BUTTON_ROWS][BUTTON_COLS];

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