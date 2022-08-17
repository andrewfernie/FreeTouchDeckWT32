#include "ScreenHelper.h"
/**
* @brief This function reads chuncks of 2 bytes of data from a
         file and returns the data.
*
* @param &f
*
* @return uint16_t
*
* @note litte-endian
*/
uint16_t read16(fs::File &f)
{
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read();  // LSB
    ((uint8_t *)&result)[1] = f.read();  // MSB
    return result;
}

/**
* @brief This function reads chuncks of 4 bytes of data from a
         file and returns the data.
*
* @param &f
*
* @return uint32_t
*
* @note litte-endian
*/
uint32_t read32(fs::File &f)
{
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read();  // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read();  // MSB
    return result;
}

/**
* @brief This functions accepts a HTML including the # colour code
         (eg. #FF00FF)  and returns it in RGB888 format.
*
* @param *html char (including #)
*
* @return unsigned long
*
* @note none
*/
unsigned long convertHTMLtoRGB888(char *html)
{
    char *hex = html + 1;  // remove the #
    unsigned long rgb = strtoul(hex, NULL, 16);
    return rgb;
}

/**
* @brief This functions accepts a RGB888 format color and returns an HTML including the # colour code
         (eg. #FF00FF).
*
* @param unsigned long
*
* @return *html char (including #)
*
* @note none
*/
char *convertRGB888toHTML(unsigned long rgb)
{
    char hex[10];
    sprintf(hex, "#%6.6X", rgb & 0x00ffffff);
    return hex;
}

/**
 * @brief This function converts RGB888 to RGB565.
 *
 * @param rgb unsigned long
 *
 * @return unsigned int
 *
 * @note none
 */
unsigned int convertRGB888ToRGB565(unsigned long rgb)
{
    return (((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5) | ((rgb & 0xf8) >> 3));
}

/**
 * @brief This function converts RGB565 to RGB888.
 *
 * @param rgb unsigned int
 *
 * @return unsigned long
 *
 * @note none
 */
unsigned long convertRGB565ToRGB888(unsigned int rgb)
{
    return (((rgb & 0xf800) << 8) | ((rgb & 0x7e0) << 5) | ((rgb & 0x1f) << 3));
}

/**
 * @brief This function converts HTML color code to RGB565.
 *
 * @param *html char (including #)
 *
 * @return unsigned int
 *
 * @note none
 */
unsigned int convertHTMLtoRGB565(char *html){
    unsigned long rgb888 = convertHTMLtoRGB888(html);
    return convertRGB888ToRGB565(rgb888);
}

/**
 * @brief This function converts RGB565 to HTML color code.
 *
 * @param unsigned int
 *
 * @return *html char (including #)
 *
 * @note none
 */
char *convertRGB565toHTML(unsigned int rgb){
    unsigned long rgb888 = convertRGB565ToRGB888(rgb);
    return convertRGB888toHTML(rgb888);
}

/**
* @brief This function draws a transparent BMP on the TFT screen according
         to the given x and y coordinates.
*
* @param  *filename
* @param x int16_t
* @param y int16_t
*
* @return none
*
* @note A completely black pixel is transparent e.g. (0x0000) not drawn.
*/
void drawBmpTransparent(const char *filename, int16_t x, int16_t y)
{
    if ((x >= tft.width()) || (y >= tft.height()))
        return;

    fs::File bmpFS;

    bmpFS = FILESYSTEM.open(filename, "r");

    if (!bmpFS) {
        MSG_WARNLN("[WARNING]: Bitmap not found: ");
        MSG_WARNLN(filename);
        filename = "/logos/question.bmp";
        bmpFS = FILESYSTEM.open(filename, "r");
    }

    uint32_t seekOffset;
    uint16_t w, h, row;
    uint8_t r, g, b;

    if (read16(bmpFS) == 0x4D42) {
        read32(bmpFS);
        read32(bmpFS);
        seekOffset = read32(bmpFS);
        read32(bmpFS);
        w = read32(bmpFS);
        h = read32(bmpFS);

        if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0)) {
            y += h - 1;

            bool oldSwapBytes = tft.getSwapBytes();
            tft.setSwapBytes(true);
            bmpFS.seek(seekOffset);

            uint16_t padding = (4 - ((w * 3) & 3)) & 3;
            uint8_t lineBuffer[w * 3 + padding];

            for (row = 0; row < h; row++) {
                bmpFS.read(lineBuffer, sizeof(lineBuffer));
                uint8_t *bptr = lineBuffer;
                uint16_t *tptr = (uint16_t *)lineBuffer;
                // Convert 24 to 16 bit colours
                for (uint16_t col = 0; col < w; col++) {
                    b = *bptr++;
                    g = *bptr++;
                    r = *bptr++;
                    *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
                }

                // Push the pixel row to screen, pushImage will crop the line if needed
                // y is decremented as the BMP image is drawn bottom up
                tft.pushImage(x, y--, w, 1, (uint16_t *)lineBuffer, TFT_BLACK);
            }
            tft.setSwapBytes(oldSwapBytes);
        }
        else
            MSG_ERRORLN("ERROR: BMP format not recognized.");
    }
    bmpFS.close();
}

/**
* @brief This function draws a BMP on the TFT screen according
         to the given x and y coordinates.
*
* @param  *filename
* @param x int16_t
* @param y int16_t
*
* @return none
*
* @note In contradiction to drawBmpTransparent() this does draw black pixels.
*/
void drawBmp(const char *filename, int16_t x, int16_t y)
{
    if ((x >= tft.width()) || (y >= tft.height()))
        return;

    fs::File bmpFS;

    bmpFS = FILESYSTEM.open(filename, "r");

    if (!bmpFS) {
        MSG_ERROR("ERROR: File not found:");
        MSG_ERRORLN(filename);
        return;
    }

    uint32_t seekOffset;
    uint16_t w, h, row;
    uint8_t r, g, b;

    if (read16(bmpFS) == 0x4D42) {
        read32(bmpFS);
        read32(bmpFS);
        seekOffset = read32(bmpFS);
        read32(bmpFS);
        w = read32(bmpFS);
        h = read32(bmpFS);

        if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0)) {
            y += h - 1;

            bool oldSwapBytes = tft.getSwapBytes();
            tft.setSwapBytes(true);
            bmpFS.seek(seekOffset);

            uint16_t padding = (4 - ((w * 3) & 3)) & 3;
            uint8_t lineBuffer[w * 3 + padding];

            for (row = 0; row < h; row++) {
                bmpFS.read(lineBuffer, sizeof(lineBuffer));
                uint8_t *bptr = lineBuffer;
                uint16_t *tptr = (uint16_t *)lineBuffer;
                // Convert 24 to 16 bit colours
                for (uint16_t col = 0; col < w; col++) {
                    b = *bptr++;
                    g = *bptr++;
                    r = *bptr++;
                    *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
                }

                // Push the pixel row to screen, pushImage will crop the line if needed
                // y is decremented as the BMP image is drawn bottom up
                tft.pushImage(x, y--, w, 1, (uint16_t *)lineBuffer);
            }
            tft.setSwapBytes(oldSwapBytes);
        }
        else
            MSG_ERRORLN("[ERROR] BMP format not recognized.");
    }
    bmpFS.close();
}

/**
* @brief This function reads a number of bytes from the given
         file at the given position.
*
* @param *p_file File
* @param position int
* @param nBytes byte
*
* @return int32_t
*
* @note none
*/
int32_t readNbytesInt(File *p_file, int position, byte nBytes)
{
    if (nBytes > 4)
        return 0;

    p_file->seek(position);

    int32_t weight = 1;
    int32_t result = 0;
    for (; nBytes; nBytes--) {
        result += weight * p_file->read();
        weight <<= 8;
    }
    return result;
}

/**
* @brief This function reads the RGB565 colour of the first pixel for a
         given the logo number. The pagenumber is global.
*
* @param *filename const char
*
* @return uint16_t
*
* @note Uses readNbytesInt
*/
uint16_t getBMPColor(const char *filename)
{
    // Open File
    File bmpImage;
    bmpImage = FILESYSTEM.open(filename, FILE_READ);

    int32_t dataStartingOffset = readNbytesInt(&bmpImage, 0x0A, 4);
    int16_t pixelsize = readNbytesInt(&bmpImage, 0x1C, 2);

    if (pixelsize != 24) {
        MSG_ERRORLN("[ERROR] getBMPColor: Image is not 24 bpp");
        return 0x0000;
    }

    bmpImage.seek(dataStartingOffset);  // skip bitmap header

    byte R, G, B;

    B = bmpImage.read();
    G = bmpImage.read();
    R = bmpImage.read();

    bmpImage.close();

    return tft.color565(R, G, B);
}

/**
* @brief This function returns the RGB565 colour of the first pixel for a
         given the logo number. The pagenumber is global.
*
* @param logonumber int
*
* @return uint16_t
*
* @note Uses getBMPColor to read the actual image data.
*/
uint16_t getImageBG(int row, int col)
{
    if ((pageNum >= 0) && (pageNum < NUM_PAGES)){
        if ((row >= 0) && (row < BUTTON_ROWS) && (col >= 0) && (col < BUTTON_COLS)) {
            return getBMPColor(menu[pageNum].button[row][col].logo);
        }
        else {
            MSG_ERRORLN("[ERROR] getImageBG: Invalid logo index");
            return 0x0000;
        }
    }
    else {
        MSG_ERRORLN("[ERROR] getImageBG: Invalid pageNum");
        return 0x0000;
    }
}

/**
 * @brief This function returns the RGB565 colour of the first pixel of the image which
 *          is being latched to for a given the logo number. The pagenumber is global.
 *
 * @param logonumber int
 *
 * @return uint16_t
 *
 * @note Uses getBMPColor to read the actual image data.
 */
uint16_t getLatchImageBG(uint8_t row, uint8_t col)
{
    if ((pageNum >= 0) && (pageNum < NUM_PAGES)) {
        if ((row >= 0) && (row < BUTTON_ROWS) && (col >= 0) && (col < BUTTON_COLS)) {
            if (strcmp(menu[pageNum - 1].button[row][col].latchlogo, "/logos/") == 0) {
                return getBMPColor(menu[pageNum - 1].button[row][col].logo);
            }
            return getBMPColor(menu[pageNum - 1].button[row][col].latchlogo);
        }
        else {
            MSG_ERRORLN("[ERROR] getLatchImageBG: Invalid latch logo index");
            return 0x0000;
        }
    }
    else {
        MSG_ERRORLN("[ERROR] getLatchImageBG: Invalid pageNum");
        return 0x0000;
    }
}
