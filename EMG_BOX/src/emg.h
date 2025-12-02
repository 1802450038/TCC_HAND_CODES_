#ifndef EMG_H
#define EMG_H

// EMG Sensor Definitions
#define ANALOG_EMG_1_PIN 34 // EMG 1 Pin
#define ANALOG_POT A1

extern int POT_DIVIDER;
extern int INF_LIMIT;
extern int SUP_LIMIT;
extern int BASE_LIMIT;
extern int EMG_GESTURE;
extern int TIME_OUT;
extern bool debug; // Debug mode
extern unsigned long lastGestureMillis;
extern unsigned long lastToggleMillis;
extern int COOLDOWN_VAL;

void incrementGesture(int gestureInterval);
void toggleStateTime(int stateInterval);
bool checkLimitsWithTimeout(int highOut, int lowOut, int highLim, int lowLim, unsigned long timeoutMillis);
void emgVizualizer(bool debug);
void emgLines(int rawEmg, bool debug, int timeOut);
bool emgLinesReturn(int rawEmg, int timeOut);
// OLD
void updateLimiters(int potValue, int potDivider);
void updateLimiters(int value);
void initEMG();
void updateTimeOut(int potValue);
void toggleHandState();
void increaseLimit();
void decreaseLimit();
// 

// void emgProcessor(bool debug);
#endif // EMG_H