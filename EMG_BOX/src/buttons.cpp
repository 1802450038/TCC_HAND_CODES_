#include "buttons.h"
#include "display.h"
#include "emg.h"
#include <Arduino.h>

OneButton buttonL(SCREEN_BUTTON_L, true); // Button object
OneButton buttonR(SCREEN_BUTTON_R, true); // Button object
OneButton buttonU(SCREEN_BUTTON_U, true); // Button object
OneButton buttonD(SCREEN_BUTTON_D, true); // Button object

int PAGE_OPTION = 99;
int ACTION_PAGE = 0;
bool UNLOCK_VAL = false;
int INDEX_MENU = 0;
bool IS_SUB_MENU = false;
int MENU_VARIATION = 0;

int numContractions = 0;
bool contractionDetected = false;

// Função para iniciar os botões
void initButtons()
{
  Serial.println("Init Buttons");
  buttonR.attachClick(clickR);                   // Attach click event
  buttonR.attachDoubleClick(doubleClickR);       // Attach double click event
  buttonR.attachLongPressStart(longPressStartR); // Attach double click event
  buttonR.attachLongPressStop(longPressStopR);   // Attach double click event

  buttonL.attachClick(clickL);                   // Attach click event
  buttonL.attachDoubleClick(doubleClickL);       // Attach double click event
  buttonL.attachLongPressStart(longPressStartL); // Attach double click event
  buttonL.attachLongPressStop(longPressStopL);   // Attach double click event

  buttonU.attachClick(clickU);                   // Attach click event
  buttonU.attachDoubleClick(doubleClickU);       // Attach double click event
  buttonU.attachLongPressStart(longPressStartU); // Attach double click event
  buttonU.attachLongPressStop(longPressStopU);   // Attach double click event

  buttonD.attachClick(clickD);                   // Attach click event
  buttonD.attachDoubleClick(doubleClickD);       // Attach double click event
  buttonD.attachLongPressStart(longPressStartD); // Attach double click event
  buttonD.attachLongPressStop(longPressStopD);   // Attach double click event
}

// Função para iniciar os botões e permitir as interrupções
void btnTicks()
{
  buttonR.tick(); // Update button state
  buttonL.tick(); // Update button state
  buttonU.tick(); // Update button state
  buttonD.tick(); // Update button state
}

void clickR()
{
  Serial.println("Click R");
  clearDisplay();
  PRINTED_INFO = false;
  if (!IS_SUB_MENU)
  {
    PAGE_OPTION = ACTION_PAGE;
    Serial.println(PAGE_OPTION);
    Serial.println("Submenu");
    IS_SUB_MENU = true;
  }
  else if (IS_SUB_MENU && !UNLOCK_VAL)
  {
    Serial.println("Unlock");
    UNLOCK_VAL = true;
    MENU_VARIATION = 0;
  }
}

void clickL()
{
  Serial.println("Click L");
  clearDisplay();
  PRINTED_INFO = false;
  if (IS_SUB_MENU && UNLOCK_VAL)
  {
    Serial.println("Lock");
    UNLOCK_VAL = false;
    MENU_VARIATION = 0;
  }
  else if (IS_SUB_MENU && !UNLOCK_VAL)
  {
    Serial.println("Homming");
    PAGE_OPTION = 99;
    Serial.println(PAGE_OPTION);
    IS_SUB_MENU = false;
  }
}

void clickU()
{
  Serial.println("Click U");
  clearDisplay();
  PRINTED_INFO = false;
  if (IS_SUB_MENU && UNLOCK_VAL)
  {
    MENU_VARIATION = 50;
    Serial.println("Menu variation +: " + String(MENU_VARIATION));
  }
  else if (!UNLOCK_VAL)
  {
    ACTION_PAGE -= 1;
  }
}

void clickD()
{
  Serial.println("Click D");
  clearDisplay();
  PRINTED_INFO = false;
  if (IS_SUB_MENU && UNLOCK_VAL)
  {
    MENU_VARIATION = -50;
    Serial.println("Menu variation -: " + String(MENU_VARIATION));
  }
  else if (!UNLOCK_VAL)
  {
    ACTION_PAGE += 1;
  }
}

void doubleClickR()
{
  Serial.println("Double Click R");
}

void doubleClickL()
{
  Serial.println("Double Click L");
}

void doubleClickU()
{
  Serial.println("Double Click U");
  clearDisplay();
  PRINTED_INFO = false;
  if (IS_SUB_MENU && UNLOCK_VAL)
  {
    MENU_VARIATION = +100;
    Serial.println("Menu variation -: " + String(MENU_VARIATION));
  }
}

void doubleClickD()
{
  Serial.println("Double Click D");
  clearDisplay();
  PRINTED_INFO = false;
  if (IS_SUB_MENU && UNLOCK_VAL)
  {
    MENU_VARIATION = -100;
    Serial.println("Menu variation -: " + String(MENU_VARIATION));
  }
}

void longPressStartR()
{
  Serial.println("Long Press Start R");
}

void longPressStartL()
{
  Serial.println("Long Press Start L");
}

void longPressStartU()
{
  Serial.println("Long Press Start U");
}

void longPressStartD()
{
  Serial.println("Long Press Start D");
}

void longPressStopR()
{
  Serial.println("Long Press Stop R");
}

void longPressStopL()
{
  Serial.println("Long Press Stop L");
}

void longPressStopU()
{
  Serial.println("Long Press Stop U");
}

void longPressStopD()
{
  Serial.println("Long Press Stop D");
}
