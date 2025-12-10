#ifndef HALLSENSOR_H
#define HALLSENSOR_H

#include "display.h"
#include "motors.h"


// Hall Sensor Definitions
// #define HALL_SENSOR_PIN_THUMB 33  // Hall sensor pin
#define HALL_SENSOR_PIN_THUMB A4  // Hall sensor pin
#define HALL_SENSOR_PIN_INDEX A3  // Hall sensor pin 2
#define HALL_SENSOR_PIN_MIDDLE A2 // Hall sensor pin 3
#define HALL_SENSOR_PIN_RING A1   // Hall sensor pin 4
#define HALL_SENSOR_PIN_LITTLE A0 // Hall sensor pin 5

extern int HALL_SENSOR_PIN_THUMB_THRESHOLD;
extern int HALL_SENSOR_PIN_INDEX_THRESHOLD;
extern int HALL_SENSOR_PIN_MIDDLE_THRESHOLD;
extern int HALL_SENSOR_PIN_RING_THRESHOLD;
extern int HALL_SENSOR_PIN_LITTLE_THRESHOLD;

#define NUM_READINGS 10
extern int readings[NUM_READINGS];
extern int readIndex;
extern long total;
extern int aisVal;

void initHallSensor();
int readAnalogHall(int pin);
bool checkPress(int hallValue, int threshold, int thresholdGain);
long smooth(int vPin, int threshold);
void displayHallSensorGraph();
void debugAnalog(int number);

#endif // HALLSENSOR_H