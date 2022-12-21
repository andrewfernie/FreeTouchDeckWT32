#pragma once
#include "FreeTouchDeckWT32.h"
uint16_t read16(fs::File &f);
uint32_t read32(fs::File &f);
unsigned long convertHTMLtoRGB888(char *html);
unsigned int convertRGB888ToRGB565(unsigned long rgb);
void convertRGB888toHTML(unsigned long rgb, char *html, unsigned int htmlsize);
unsigned long convertRGB565ToRGB888(unsigned int rgb);
unsigned int convertHTMLtoRGB565(char *html);
void convertRGB565toHTML(unsigned int rgb, char *html, unsigned int htmlsize);
// void drawBmpTransparent(const char *filename, int16_t x, int16_t y);
void drawBmp(const char *filename, int16_t x, int16_t y, uint8_t transparent);
int32_t readNbytesInt(File *p_file, int position, byte nBytes);
uint16_t getBMPColor(const char *filename);
uint16_t getImageBG(uint8_t page, uint8_t row, uint8_t col);
uint16_t getLatchImageBG(uint8_t page, uint8_t row, uint8_t col);
uint8_t drawLogoFromPSRAM(uint16_t *pImage, int16_t x, int16_t y, uint8_t transparent);
uint8_t loadBmpToPSRAM(const char *filename, uint16_t **pImage);
