# FreeTouchDeckWT32
This is a port of Dustin Watt's FreeTouchDeck project (https://github.com/DustinWatts/FreeTouchDeck.git) to the to the WT32-SC01 integrated ESP32, LCD, and capacitive touch screen board. http://www.wireless-tag.com/portfolio/wt32-sc01/

Other than adapting to the board, the main change is to allow more buttons per page  - the screen area is just a bit smaller than the size of twelve StreamDeck buttons which seems reasonable. The code is currently set to three rows of four buttons, but can be changed to three rows of five buttons (or two rows of four, etc.) by modifying FreeTouchDeckWT32.h: "#define BUTTON_ROWS 3" and "#define BUTTON_COLS 5".  In principle it should support larger arrays, but not tested, and the configurator page is set to a maximum of 3x5. 

![Sample 3x4 Button Array](images/Screen3x4Home.jpg)

The configurator displays an array of three rows of five buttons, with a drop down list to select the menu page to be modified. If you are using fewer than 3x5, simply ignore the extra rows and columns. On the to-do list is to generate the HTML dynamically to correspond with the number of rows and columns.

To aid the configurability to adapt to different mnenu layout, the code now has an array of  Menus (pages).  Each Menu has a 2D array of Buttons, each Button has an array of 3 Actions, etc.  The code ends up addressing a specific button with something like "menu[pageNum].button[row][row].islatched = true;" Rather than a dedicated home page and a dedicated return to home in the bottom right of each page each button on each page can be modified, and in principle any page could call any other. There is no protection to force a button that provides a way out of a menu, so make sure that you define one.

Other changes: 
1. There are now 10 pages. This includes Menu0 (home).
2. Updated to latest platformio Arduino platform (2.x)
3.Changed from SPIFFS to LittleFS. 

Build Notes:
1. You may see messages like
        In file included from include/FreeTouchDeckWT32.h:14,
                 from include/SaveConfig.h:3,
                 from src/SaveConfig.cpp:1:
        .pio/libdeps/esp-wrover-kit/TFT_eSPI/TFT_eSPI.h:909:8: warning: #warning >>>>------>> TOUCH_CS pin not defined, TFT_eSPI touch functions will not be available! [-Wcpp]
        #warning >>>>------>> TOUCH_CS pin not defined, TFT_eSPI touch functions will not be available!
    
    You can get rid of these by removing the semi-colon in the front of the following line in platformio.ini, but this will also stop any debug messages going to the serial port. I just ignore the messages.

        ; define TOUCH_CS


2. You may  see some error messages like 
        "[vfs_api.cpp:104] open(): /littlefs/list does not exist". 
This is a known issue with ESPAsyncWebsServer related to LittleFS supporting folders while SPIFFS does not. See https://github.com/lorol/