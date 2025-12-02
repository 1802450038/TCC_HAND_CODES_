#ifndef BUTTONS_H
#define BUTTONS_H
#define SCREEN_BUTTON_D 23 // Button pin
#define SCREEN_BUTTON_U 5 // Button pin

#define SCREEN_BUTTON_L 18 // Button pin

#define SCREEN_BUTTON_R 15 // Button pin

#include <OneButton.h>

// Button Definitions
extern OneButton buttonR; // Button object
extern OneButton buttonL; // Button object

extern int INDEX_MENU;
extern int ACTION_PAGE;
extern int PAGE_OPTION;
extern bool IS_SUB_MENU;
extern bool UNLOCK_VAL;
extern int MENU_VARIATION;



void initButtons();
void btnTicks();

void clickR();
void clickL();
void clickU();
void clickD();
void doubleClickR();
void doubleClickL();
void doubleClickU();
void doubleClickD();
void longPressStartR();
void longPressStartL();
void longPressStartU();
void longPressStartD();
void longPressStopR();
void longPressStopL();
void longPressStopU();
void longPressStopD();


#endif // BUTTONS_H