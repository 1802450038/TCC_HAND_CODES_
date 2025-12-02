#ifndef MOTORS_H
#define MOTORS_H

#include <Adafruit_PWMServoDriver.h>
#include <ESP32Servo.h>
#include "display.h"
// Servos Definitions
extern Adafruit_PWMServoDriver pwm;


extern bool handState; // Hand state
#define SERVOMIN 75 // Minimum pulse length count
#define SERVOMAX 500  // Maximum pulse length count
// ANALOG 50Hz
// DIGITAL 60Hz
#define SERVO_FREQ 50 // Servo frequency (60 Hz)
// Feedback Pins
#define FEED_1_BACK_PIN 21
#define STOP_FEEDBACK 82
#define MED_FEEDBACK 85
#define MAX_FEEDBACK 90

void initPCA9685();
void initHand();
void movServoByAngle(int servo, int angle);

void initServoFeedBack();
void activeFeedBack(bool state);
void openHand();
void closeHand();
void toogleHandState(bool state);
void openFinger(int finger);
void closeFinger(int finger);
void openFingers(int *fingers);
void closeFingers(int *fingers);
void checkHandState(bool enableServo);
void feedbackIntensity(int val);
void toggleGestureHand(int gesture);

#endif // MOTORS_H