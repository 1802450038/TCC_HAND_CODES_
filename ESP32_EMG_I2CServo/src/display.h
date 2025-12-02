#ifndef  DISPLAY_H
#define  DISPLAY_H

#define  OLED_ELABLED false

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)

extern Adafruit_SSD1306 display;
extern int maxGraphValue; // Maximum value for the graph
extern unsigned long lastUpdate;


void initDisplay();
void printDisplayLine(int line, String text);
void clearDisplayLine(int line);
void clearHalfDisplayLine(int line, int half);
void drawLineGraph(int value);

#endif // DISPLAY_H