#include <Arduino.h>
#include "buttons.h"

OneButton button(SCREEN_BUTTON_PIN, true); // Button object
int actionPage = 0;
int pageOption = 0;

int numContractions = 0;
bool contractionDetected = false;

void click()
{
  // Serial.println("Single Click");
  pageOption++;
}

void doubleClick()
{
  // Serial.println("Double Click");
  pageOption--;
  if (pageOption < 0)
  {
    pageOption = 0; // Reset to the last option
  }
}

void longPressStart()
{
  // Serial.println("Long Press Start");
}

void longPressStop()
{
  // Serial.println("Long Press Stop");
  pageOption = 0;
  actionPage++;
  if (actionPage > 4)
  {
    actionPage = 0;
  }
}

void toggleHandPage(int pageOption)
{

  switch (pageOption)
  {
  case 0:
    printDisplayLine(1, "Open Hand");
    printDisplayLine(2, "Page Option -> 0");
    openHand();
    break;
  case 1:
    printDisplayLine(1, "Close Hand");
    printDisplayLine(2, "Page Option -> 1");
    closeHand();
    break;
  }
  // Serial.print("Page Option -> ");
  // Serial.print(pageOption);
  // Serial.println();
  display.display();
}

int lastContrationNum = 0;


void getActionPage()
{
  if (actionPage < 0)
  {
    actionPage = 0; // Reset to the first page
  }
  if (actionPage > 3)
  {
    actionPage = 0; // Reset to the first page
  }
  switch (actionPage)
  {
  case 0:
    display.clearDisplay(); // Clear the display
    printDisplayLine(0, "Deb Hall");
    if (pageOption > 4)
    {
      pageOption = 0;
    }
    debugAnalog(pageOption);
    break;
  case 1:
    display.clearDisplay(); // Clear the display
    printDisplayLine(0, "Btn Hand Page");
    if (pageOption > 1)
    {
      pageOption = 0;
    }
    toggleHandPage(pageOption);
    break;
  case 2:
    display.clearDisplay(); // Clear the display
    printDisplayLine(0, "Gesture Hand Page");
    if (pageOption > 4)
    {
      pageOption = 0;
    }
    toggleGestureHand(pageOption);
    break;
  case 3:
    display.clearDisplay(); // Clear the display
    printDisplayLine(0, "Emg Hand Page");
    printDisplayLine(1, "Contracao Num : " + String(numContractions));
    printDisplayLine(2, "Hand State : " + handState ? "Open" : "Close");
    if (handState)
    {
      openHand();
    }
    else
    {
      closeHand();
    }
    

    break;
  default:
    Serial.println("Default Action Page");
    pageOption = 0;         // Reset to the first page
    display.clearDisplay(); // Clear the display
    break;
  }
  display.display(); // Display the text
}
