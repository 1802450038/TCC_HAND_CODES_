#include <Adafruit_PWMServoDriver.h>
#include <ESP32Servo.h>
#include "motors.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Servo feedBackServo1;

bool handState = false; // Hand state

void initHand()
{
  movServoByAngle(0, 30);
  delay(500);
  movServoByAngle(1, 30);
  delay(500);
  movServoByAngle(2, 30);
  delay(500);
  movServoByAngle(3, 30);
  delay(500);
  movServoByAngle(4, 30);
  delay(500);
  movServoByAngle(0, 170);
  delay(500);
  movServoByAngle(1, 170);
  delay(500);
  movServoByAngle(2, 170);
  delay(500);
  movServoByAngle(3, 170);
  delay(500);
  movServoByAngle(4, 170);
  delay(500);
}

void initServoFeedBack()
{
  feedBackServo1.attach(FEED_1_BACK_PIN);
  feedBackServo1.write(STOP_FEEDBACK);
}

bool startPCATransmission()
{
  Serial.println("Verificando PCA9685...");

  // 1. Garante que o barramento I2C iniciou
  // (Se você já iniciou o Wire no initDisplay, não precisa desta linha, mas não faz mal)
  Wire.begin();

  // 2. Tenta comunicar com o endereço padrão 0x40
  Wire.beginTransmission(0x40);
  byte error = Wire.endTransmission();

  if (error == 0)
  {
    Serial.println("PCA9685 Encontrado! Inicializando PWM...");

    pwm.begin();
    pwm.setPWMFreq(SERVO_FREQ); // Defina SERVO_FREQ (ex: 50 ou 60)
    delay(10);                  // Pequeno delay para estabilizar
    return true;
  }
  else
  {
    Serial.print("ERRO: PCA9685 nao encontrado no endereco 0x40. Erro: ");
    Serial.println(error);
    return false;
  }
}

void initPCA9685()
{
  delay(1000);
  Serial.println("Initializing PCA9685 HERE");
  startPCATransmission();
  delay(1000);
}

void toggleHandStateOpen()
{
  handState = true;
}

void toggleHandStateClose()
{
  handState = false;
}

void movServoByAngle(int servo, int angle)
{
  int pulseLength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  // Serial.println("Pulse length: " + String(pulseLength) + " Angle: " + String(angle) + " Servo: " + String(servo));
  pwm.setPWM(servo, 0, pulseLength);
}

void activeFeedBack(bool state)
{
  if (state)
  {
    movServoByAngle(8, 170);
  }
  else
  {
    movServoByAngle(8, 100);
  }
}

void feedbackIntensity(int val)
{
  feedBackServo1.write(val);
}

void openHand()
{

  Serial.println("Opening hand");
  movServoByAngle(0, 175);
  movServoByAngle(1, 175);
  movServoByAngle(2, 175);
  movServoByAngle(3, 175);
  movServoByAngle(4, 175);
}

void closeHand()
{
  Serial.println("Closing hand");
  movServoByAngle(0, 20);
  movServoByAngle(1, 20);
  movServoByAngle(2, 20);
  movServoByAngle(3, 20);
  movServoByAngle(4, 20);
}

void toogleHandState(bool state)
{
  if (state)
  {
    closeHand();
  }
  else
  {
    openHand();
  }
}
void openFinger(int finger)
{
  // pwm.setPWM(finger, 0, SERVOMIN);
  movServoByAngle(finger, 180);
}

void closeFinger(int finger)
{
  // pwm.setPWM(finger, 0, SERVOMAX);
  movServoByAngle(finger, 0);
}

void openFingers(int *fingers)
{
  for (int i = 0; i < sizeof(fingers) / sizeof(fingers[0]); i++)
  {
    pwm.setPWM(fingers[i], 0, SERVOMIN);
  }
}

void closeFingers(int *fingers)
{
  for (int i = 0; i < sizeof(fingers) / sizeof(fingers[0]); i++)
  {
    pwm.setPWM(fingers[i], 0, SERVOMAX);
  }
}

void toggleGestureHand(int gesture)
{
  if (gesture == 0)
  {
    Serial.println("Gesture - OPEN");
    movServoByAngle(0, 180);
    openFinger(0);
    openFinger(1);
    openFinger(2);
    openFinger(3);
    openFinger(4);
  }
  else if (gesture == 1)
  {
    Serial.println("Gesture - ONE");
    movServoByAngle(0, 0);
    closeFinger(0);
    openFinger(1);
    closeFinger(2);
    closeFinger(3);
    closeFinger(4);
  }
  else if (gesture == 2)
  {
    Serial.println("Gesture - TWO");
    closeFinger(0);
    openFinger(1);
    openFinger(2);
    closeFinger(3);
    closeFinger(4);
  }
  else if (gesture == 3)
  {
    Serial.println("Gesture - THREE");
    closeFinger(0);
    openFinger(1);
    openFinger(2);
    openFinger(3);
    closeFinger(4);
  }
  else if (gesture == 4)
  {
    Serial.println("Gesture - OK");
    closeFinger(1);
    closeFinger(0);
    openFinger(2);
    openFinger(3);
    openFinger(4);
  }
  else if (gesture == 5)
  {
    Serial.println("Gesture - THUMB");
    openFinger(0);
    closeFinger(1);
    closeFinger(2);
    closeFinger(3);
    closeFinger(4);
  }
}
