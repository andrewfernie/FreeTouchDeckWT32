#include "Action.h"
#include "SaveConfig.h"
#include "BleKeyboard.h"

/**
* @brief This function takes an int as an "action" and "value". It uses
         a switch statement to determine which type of action to do.
         e.g. write, print, press. If an action requires a char, you
         can pass the pointer to that char through the parameter "symbol"
*
* @param action int
* @param value int
* @param symbol char *
*
* @return none
*
* @note Case 11 is used for special functions, none bleKeyboard related.
*/

void bleKeyboardAction(int action, int value, char *symbol)
{
    MSG_DEBUG3("[INFO] BLE Keyboard action received:", action, value, symbol);
    switch (action) {
        case Action_NoAction:
            // No Action
            break;
        case Action_Delay:  // Delay
            delay(value);
            break;
        case Action_TabArrow:  // Send TAB ARROW etc
            switch (value) {
                case 1:
                    bleKeyboard.write(KEY_UP_ARROW);
                    break;
                case 2:
                    bleKeyboard.write(KEY_DOWN_ARROW);
                    break;
                case 3:
                    bleKeyboard.write(KEY_LEFT_ARROW);
                    break;
                case 4:
                    bleKeyboard.write(KEY_RIGHT_ARROW);
                    break;
                case 5:
                    bleKeyboard.write(KEY_BACKSPACE);
                    break;
                case 6:
                    bleKeyboard.write(KEY_TAB);
                    break;
                case 7:
                    bleKeyboard.write(KEY_RETURN);
                    break;
                case 8:
                    bleKeyboard.write(KEY_PAGE_UP);
                    break;
                case 9:
                    bleKeyboard.write(KEY_PAGE_DOWN);
                    break;
                case 10:
                    bleKeyboard.write(KEY_DELETE);
                    break;
                case 11:
                    bleKeyboard.write(KEY_PRTSC);
                    break;
                case 12:
                    bleKeyboard.write(KEY_ESC);
                    break;
                case 13:
                    bleKeyboard.write(KEY_HOME);
                    break;
                case 14:
                    bleKeyboard.write(KEY_END);
                    break;

                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_Media:  // Send Media Key
            switch (value) {
                case 1:
                    bleKeyboard.write(KEY_MEDIA_MUTE);
                    break;
                case 2:
                    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
                    break;
                case 3:
                    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
                    break;
                case 4:
                    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
                    break;
                case 5:
                    bleKeyboard.write(KEY_MEDIA_STOP);
                    break;
                case 6:
                    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
                    break;
                case 7:
                    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
                    break;
                case 8:
                    bleKeyboard.write(KEY_MEDIA_WWW_HOME);
                    break;
                case 9:
                    bleKeyboard.write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
                    break;
                case 10:
                    bleKeyboard.write(KEY_MEDIA_CALCULATOR);
                    break;
                case 11:
                    bleKeyboard.write(KEY_MEDIA_WWW_BOOKMARKS);
                    break;
                case 12:
                    bleKeyboard.write(KEY_MEDIA_WWW_SEARCH);
                    break;
                case 13:
                    bleKeyboard.write(KEY_MEDIA_WWW_STOP);
                    break;
                case 14:
                    bleKeyboard.write(KEY_MEDIA_WWW_BACK);
                    break;
                case 15:
                    bleKeyboard.write(KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION);
                    break;
                case 16:
                    bleKeyboard.write(KEY_MEDIA_EMAIL_READER);
                    break;
                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_Char:  // Send Character
            bleKeyboard.print(symbol);
            break;
        case Action_Option:  // Option Keys
            switch (value) {
                case 1:
                    bleKeyboard.press(KEY_LEFT_CTRL);
                    break;
                case 2:
                    bleKeyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 3:
                    bleKeyboard.press(KEY_LEFT_ALT);
                    break;
                case 4:
                    bleKeyboard.press(KEY_LEFT_GUI);
                    break;
                case 5:
                    bleKeyboard.press(KEY_RIGHT_CTRL);
                    break;
                case 6:
                    bleKeyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 7:
                    bleKeyboard.press(KEY_RIGHT_ALT);
                    break;
                case 8:
                    bleKeyboard.press(KEY_RIGHT_GUI);
                    break;
                case 9:
                    bleKeyboard.releaseAll();
                    break;
                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_FnKey:  // Function Keys
            switch (value) {
                case 1:
                    bleKeyboard.press(KEY_F1);
                    break;
                case 2:
                    bleKeyboard.press(KEY_F2);
                    break;
                case 3:
                    bleKeyboard.press(KEY_F3);
                    break;
                case 4:
                    bleKeyboard.press(KEY_F4);
                    break;
                case 5:
                    bleKeyboard.press(KEY_F5);
                    break;
                case 6:
                    bleKeyboard.press(KEY_F6);
                    break;
                case 7:
                    bleKeyboard.press(KEY_F7);
                    break;
                case 8:
                    bleKeyboard.press(KEY_F8);
                    break;
                case 9:
                    bleKeyboard.press(KEY_F9);
                    break;
                case 10:
                    bleKeyboard.press(KEY_F10);
                    break;
                case 11:
                    bleKeyboard.press(KEY_F11);
                    break;
                case 12:
                    bleKeyboard.press(KEY_F12);
                    break;
                case 13:
                    bleKeyboard.press(KEY_F13);
                    break;
                case 14:
                    bleKeyboard.press(KEY_F14);
                    break;
                case 15:
                    bleKeyboard.press(KEY_F15);
                    break;
                case 16:
                    bleKeyboard.press(KEY_F16);
                    break;
                case 17:
                    bleKeyboard.press(KEY_F17);
                    break;
                case 18:
                    bleKeyboard.press(KEY_F18);
                    break;
                case 19:
                    bleKeyboard.press(KEY_F19);
                    break;
                case 20:
                    bleKeyboard.press(KEY_F20);
                    break;
                case 21:
                    bleKeyboard.press(KEY_F21);
                    break;
                case 22:
                    bleKeyboard.press(KEY_F22);
                    break;
                case 23:
                    bleKeyboard.press(KEY_F23);
                    break;
                case 24:
                    bleKeyboard.press(KEY_F24);
                    break;
                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_Number:  // Send Number
            bleKeyboard.print(value);
            break;
        case Action_SpecialChar:  // Send Special Character
            bleKeyboard.print(symbol);
            break;
        case Action_Combos:  // Combos
            switch (value) {
                case 1:
                    bleKeyboard.press(KEY_LEFT_CTRL);
                    bleKeyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 2:
                    bleKeyboard.press(KEY_LEFT_ALT);
                    bleKeyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 3:
                    bleKeyboard.press(KEY_LEFT_GUI);
                    bleKeyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 4:
                    bleKeyboard.press(KEY_LEFT_CTRL);
                    bleKeyboard.press(KEY_LEFT_GUI);
                    break;
                case 5:
                    bleKeyboard.press(KEY_LEFT_ALT);
                    bleKeyboard.press(KEY_LEFT_GUI);
                    break;
                case 6:
                    bleKeyboard.press(KEY_LEFT_CTRL);
                    bleKeyboard.press(KEY_LEFT_ALT);
                    break;
                case 7:
                    bleKeyboard.press(KEY_LEFT_CTRL);
                    bleKeyboard.press(KEY_LEFT_ALT);
                    bleKeyboard.press(KEY_LEFT_GUI);
                    break;
                case 8:
                    bleKeyboard.press(KEY_RIGHT_CTRL);
                    bleKeyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 9:
                    bleKeyboard.press(KEY_RIGHT_ALT);
                    bleKeyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 10:
                    bleKeyboard.press(KEY_RIGHT_GUI);
                    bleKeyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 11:
                    bleKeyboard.press(KEY_RIGHT_CTRL);
                    bleKeyboard.press(KEY_RIGHT_GUI);
                    break;
                case 12:
                    bleKeyboard.press(KEY_RIGHT_ALT);
                    bleKeyboard.press(KEY_RIGHT_GUI);
                    break;
                case 13:
                    bleKeyboard.press(KEY_RIGHT_CTRL);
                    bleKeyboard.press(KEY_RIGHT_ALT);
                    break;
                case 14:
                    bleKeyboard.press(KEY_RIGHT_CTRL);
                    bleKeyboard.press(KEY_RIGHT_ALT);
                    bleKeyboard.press(KEY_RIGHT_GUI);
                    break;
            }
            break;
        case Action_Helpers:  // Helpers
            switch (value) {
                case 1:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F1);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 2:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F2);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 3:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F3);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 4:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F4);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 5:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F5);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 6:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F6);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 7:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F7);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 8:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F8);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 9:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F9);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 10:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F10);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 11:
                    if (generalconfig.modifier1 != 0) {
                        bleKeyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        bleKeyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        bleKeyboard.press(generalconfig.modifier3);
                    }
                    bleKeyboard.press(KEY_F11);
                    bleKeyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
            }
            break;
        case Action_SpecialFn:  // Special functions
            switch (value) {
                case 1:  // Enter config mode
                    
                    configmode();

                    break;
                case 2:  // Display Brightness Down
                    if (ledBrightness > 25) {
                        ledBrightness = ledBrightness - 25;
                        ledcWrite(0, ledBrightness);
                        savedStates.putInt("ledBrightness", ledBrightness);
                        MSG_INFOLN("[INFO] Brightness down.");
                    }
                    break;
                case 3:  // Display Brightness Up
                    if (ledBrightness < 230) {
                        ledBrightness = ledBrightness + 25;
                        ledcWrite(0, ledBrightness);
                        savedStates.putInt("ledBrightness", ledBrightness);
                        MSG_INFOLN("[INFO] Brightness up.");
                    }
                    break;
                case 4:  // Sleep Enabled
                    if (generalconfig.sleepenable) {
                        generalconfig.sleepenable = false;
                        MSG_INFOLN("[INFO] Sleep disabled.");
                    }
                    else {
                        generalconfig.sleepenable = true;
                        Interval = generalconfig.sleeptimer * MIN_TO_MS;
                        MSG_INFOLN("[INFO] Sleep enabled.");
                        MSG_INFO("[INFO] Timer set to: ");
                        MSG_INFOLN(generalconfig.sleeptimer);
                    }
                    break;
                case 5:
                    callingPageNum = pageNum;
                    pageNum = SPECIAL_PAGE_INFO;
                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "Info");
                        Serial.println(usbData);
                    }

                    break;
                case 6:
                    callingPageNum = pageNum;
                    pageNum = 0;
                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "Home");
                        Serial.println(usbData);
                    }
                    drawKeypad();
                    break;
                case 7:
                    saveCurrentConfigGeneral();
                    break;
                case 8:  // USB Comms Enable/Disable
                    if (generalconfig.usbcommsenable) {
                        generalconfig.usbcommsenable = false;
                        MSG_INFOLN("[INFO] USB Comms Disabled.");
                    }
                    else {
                        generalconfig.usbcommsenable = true;
                        MSG_INFOLN("[INFO] USB Comms Enabled.");
                    }
                    break;
            }
            break;
        case Action_Numpad:  // Numpad
            switch (value) {
                case 0:
                    bleKeyboard.write(KEY_NUM_0);
                    break;
                case 1:
                    bleKeyboard.write(KEY_NUM_1);
                    break;
                case 2:
                    bleKeyboard.write(KEY_NUM_2);
                    break;
                case 3:
                    bleKeyboard.write(KEY_NUM_3);
                    break;
                case 4:
                    bleKeyboard.write(KEY_NUM_4);
                    break;
                case 5:
                    bleKeyboard.write(KEY_NUM_5);
                    break;
                case 6:
                    bleKeyboard.write(KEY_NUM_6);
                    break;
                case 7:
                    bleKeyboard.write(KEY_NUM_7);
                    break;
                case 8:
                    bleKeyboard.write(KEY_NUM_8);
                    break;
                case 9:
                    bleKeyboard.write(KEY_NUM_9);
                    break;
                case 10:
                    bleKeyboard.write(KEY_NUM_SLASH);
                    break;
                case 11:
                    bleKeyboard.write(KEY_NUM_ASTERIX);
                    break;
                case 12:
                    bleKeyboard.write(KEY_NUM_MINUS);
                    break;
                case 13:
                    bleKeyboard.write(KEY_NUM_PLUS);
                    break;
                case 14:
                    bleKeyboard.write(KEY_NUM_ENTER);
                    break;
                case 15:
                    bleKeyboard.write(KEY_NUM_PERIOD);
                    break;
            }
            break;
        case Action_CustomFn:  // Custom functions
            switch (value) {
                case 1:
                    userAction1();
                    break;
                case 2:
                    userAction2();
                    break;
                case 3:
                    userAction3();
                    break;
                case 4:
                    userAction4();
                    break;
                case 5:
                    userAction5();
                    break;
                case 6:
                    userAction6();
                    break;
                case 7:
                    userAction7();
                    break;
            }
            break;
        case Action_ChangePage:  // Custom functions
            if ((value >= 0) && (value < NUM_PAGES)) {
                callingPageNum = pageNum;
                pageNum = value;
                if (generalconfig.usbcommsenable) {
                    char usbData[40];
                    snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, menu[pageNum].name);
                    Serial.println(usbData);
                }
                drawKeypad();
            }
            break;
        default:
            // If nothing matches do nothing
            break;
    }
}
