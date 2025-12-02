#include <Arduino.h>
#include <Preferences.h>
#include "prefs.h"

Preferences preferences;

void initPrefs()
{
    Serial.println("Init Prefs");
    preferences.begin("settings", false);
}

int loadValue(char *key, int defaultValue)
{
    Serial.println("Carregando valor : " + String(key));
    return preferences.getInt(key, defaultValue);
}

void storeValue(char *key, int value)
{
    Serial.println("Salvando valor : " + String(key));
    preferences.putInt(key, value);
}

void closePrefs()
{
    preferences.end();
}