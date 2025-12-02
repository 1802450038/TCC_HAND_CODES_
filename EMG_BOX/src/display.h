#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define TFT_TEXT_SIZE_SMALL 1
#define TFT_TEXT_SIZE 1
#define TFT_TEXT_SIZE_LARGE 3
#define TFT_LINE_HEIGHT_SMALL 10
#define TFT_LINE_HEIGHT 10
#define TFT_LINE_HEIGHT_LARGE 30
#define TFT_WIDTH 128
#define TFT_HEIGHT 64
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)




#define TFT_BLACK  0x0000
#define TFT_WHITE 0xFFFF
#define TFT_RED 0xF800
#define TFT_GREEN 0x07E0
#define TFT_BLUE 0x001F
#define TFT_CYAN 0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW 0xFFE0
#define TFT_ORANGE 0xFC00


// Definição dos Pinos (a mesma da sua imagem)
#define TFT_DC     2
#define TFT_RST    4
#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_CS     -1 // Não usado
/*
TFT 240 x 320 pixels 2.0 inch ST7789 display wiring
Terminals on display's pcb from left to right

GND   GND
VDD   3.3V
SCL   18
SDA   23 (= "MOSI")
RST   4
DC    2
CS    9

*/
// const char* DIVIDER = "--------------------";

extern int maxGraphValue; // Maximum value for the graph
extern unsigned long lastUpdate;
extern bool PRINTED_INFO;


extern Adafruit_SSD1306 display;

void initDisplay();
void printDisplayLineSelected(int line, String text, bool rounded = false);
void printDisplayLine(int line, String text, bool selected = false,
                      bool rounded = false);
void clearDisplayLine(int line);
void clearHalfDisplayLine(int line, int half);
void drawLineGraph(int value);
void clearDisplay();
void menuPage();
void menu();



#endif // DISPLAY_H