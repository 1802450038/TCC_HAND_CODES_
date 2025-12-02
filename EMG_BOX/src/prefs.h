#ifndef PREFS_H
#define PREFS_H
#include <Preferences.h>

extern Preferences preferences;


void initPrefs();

int loadValue(char* key, int defaultValue);

void storeValue(char* key, int value);

void closePrefs();

#endif // PREFS_H