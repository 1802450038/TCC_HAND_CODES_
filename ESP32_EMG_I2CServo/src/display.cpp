#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int maxGraphValue = 0; // Maximum value for the graph
unsigned long lastUpdate = 0;

void initDisplay()
{
  if (OLED_ELABLED)
  {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); // Initialize OLED display
    display.clearDisplay();                              // Clear the display
    display.setTextSize(1);                              // Set text size
    display.setTextColor(SSD1306_WHITE);
    Serial.println("Initializing OLED");
  } else {
    Serial.println("OLED Disabled");
  }
}

void printDisplayLine(int line, String text)
{
  if (OLED_ELABLED)
  {
    display.clearDisplay();
    display.setCursor(0, line * 10); // Set cursor position
    display.print(text);             // Print text
    display.display();
  }
}

void clearDisplayLine(int line)
{
  if (OLED_ELABLED)
  {
    display.setCursor(0, line * 10); // Set cursor position
    display.fillRect(0, line * 10, SCREEN_WIDTH, 10, SSD1306_BLACK);
  }
}

void clearHalfDisplayLine(int line, int half)
{
  if (OLED_ELABLED)
  {
    if (half == 0)
    {
      display.fillRect(0, line * 10, SCREEN_WIDTH / 2, 10, SSD1306_BLACK);
    }
    else
    {
      display.fillRect(SCREEN_WIDTH / 2, line * 10, SCREEN_WIDTH / 2, 10, SSD1306_BLACK);
    }
  }
}

void drawLineGraph(int value)
{
  if (OLED_ELABLED)
  {
    if (value > maxGraphValue)
    {
      maxGraphValue = value; // Update the maximum value
    }

    // printDisplayLine(0, "Max V-> " + String(maxGraphValue));

    static int graph[SCREEN_WIDTH] = {0}; // Array to store graph values
    static int xPos = 0;                  // Current position in the graph

    int mappedValue = map(value, 0, 100, SCREEN_HEIGHT - 1, 0);
    graph[xPos] = mappedValue;
    display.fillRect(0, 20, SCREEN_WIDTH, 10, SSD1306_BLACK);

    for (int i = 0; i < SCREEN_WIDTH - 1; i++)
    {
      display.drawLine(i, graph[i], i + 1, graph[i + 1], SSD1306_WHITE);
    }
    xPos = (xPos + 1) % SCREEN_WIDTH;
  }
}
