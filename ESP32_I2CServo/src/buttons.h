#ifndef BUTTONS_H
#define BUTTONS_H
#define SCREEN_BUTTON_PIN 21 // Button pin

#include <OneButton.h>

#include "display.h"
#include "motors.h"
#include "hall.h"
// Button Definitions
extern OneButton button; // Button object
extern int actionPage;
extern int pageOption;

extern int numContractions;
extern bool contractionDetected;
extern int lastContrationNum;


void initButtons();
void click();
void doubleClick();
void longPressStart();
void longPressStop();
void toggleHandPage(int pageOption);
void toggleGestureHand(int pageOption);
void getActionPage();

#endif // BUTTONS_H