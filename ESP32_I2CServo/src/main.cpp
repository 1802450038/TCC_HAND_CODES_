
#include <Arduino.h>
#include <Wire.h>
#include "display.h"
#include "buttons.h"
#include "motors.h"
#include "hall.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "smoothFilter.h" // Inclua a classe que criamos acima

#define OLED_ENABLE FALSE

// --- DEFINIÇÃO DOS PINOS E LIMITES ---
#define PIN_THUMB A4
#define PIN_INDEX A3
#define PIN_MIDDLE A2
#define PIN_RING A1
#define PIN_LITTLE A0

int THRESHOLD_THUMB = 2230;
int THRESHOLD_INDEX = 2320;
int THRESHOLD_MIDDLE = 0;
int THRESHOLD_RING = 2330;
int THRESHOLD_LITTLE = 2250;
// Instanciando os filtros (Apenas Pinos)
SmoothFilter filterThumb(PIN_THUMB);
SmoothFilter filterIndex(PIN_INDEX);
SmoothFilter filterMiddle(PIN_MIDDLE);
SmoothFilter filterRing(PIN_RING);
SmoothFilter filterLittle(PIN_LITTLE);

struct RawData
{
  int thumbVal;
  int indexVal;
  int middleVal;
  int ringVal;
  int littleVal;
};

RawData sharedData;          // Dados compartilhados
SemaphoreHandle_t dataMutex; // Cadeado de segurança

void readSensorsTask(void *parameter)
{
  for (;;)
  {
    // 1. Lê os valores suavizados (Isso leva alguns microssegundos)
    int t = filterThumb.read();
    int i = filterIndex.read();
    int m = filterMiddle.read();
    int r = filterRing.read();
    int l = filterLittle.read();

    // 2. Tenta pegar o cadeado para salvar na variável global
    if (xSemaphoreTake(dataMutex, (TickType_t)10) == pdTRUE)
    {
      sharedData.thumbVal = t;
      sharedData.indexVal = i;
      sharedData.middleVal = m;
      sharedData.ringVal = r;
      sharedData.littleVal = l;

      xSemaphoreGive(dataMutex); // Libera o cadeado IMEDIATAMENTE
    }

    // 3. Dorme por 10ms. Isso define a taxa de atualização (100Hz)
    // Essencial para o ESP32 não travar o Watchdog
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

int inc_thumb_pos = 0;
int inc_index_pos = 0;
int inc_middle_pos = 0;
int inc_ring_pos = 0;
int inc_little_pos = 0;
int inc_gesture = 0;
int inc_action_mode = 0;
bool inc_hand_state = false;
bool inc_feedback = false;
int inc_threshold_thumb = THRESHOLD_THUMB;
int inc_threshold_index = THRESHOLD_INDEX;
int inc_threshold_middle = THRESHOLD_MIDDLE;
int inc_threshold_ring = THRESHOLD_RING;
int inc_threshold_little = THRESHOLD_LITTLE;

typedef struct struct_inc_message
{
  int thumb_pos;
  int index_pos;
  int middle_pos;
  int ring_pos;
  int little_pos;
  int gesture;
  int action_mode;
  bool hand_state;
  bool feedback;
  int inc_threshold_thumb;
  int inc_threshold_index;
  int inc_threshold_middle;
  int inc_threshold_ring;
  int inc_threshold_little;
} struct_inc_message;

typedef struct struct_out_message
{
  int thumb_hall;
  int index_hall;
  int middle_hall;
  int ring_hall;
  int little_hall;
} struct_out_message;

String success;
struct_out_message OUT_DATA;
struct_inc_message INC_DATA;

esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0xe0, 0x5a, 0x1b, 0xa1, 0x33, 0x44};

void readMacAddress()
{
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK)
  {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  }
  else
  {
    Serial.println("Failed to read MAC address");
  }
}
// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0)
  {
    success = "Delivery Success :)";
  }
  else
  {
    success = "Delivery Fail :(";
  }
}
// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&INC_DATA, incomingData, sizeof(INC_DATA));
  inc_thumb_pos = INC_DATA.thumb_pos;
  inc_index_pos = INC_DATA.index_pos;
  inc_middle_pos = INC_DATA.middle_pos;
  inc_ring_pos = INC_DATA.ring_pos;
  inc_little_pos = INC_DATA.little_pos;
  inc_gesture = INC_DATA.gesture;
  inc_action_mode = INC_DATA.action_mode;
  inc_hand_state = INC_DATA.hand_state;
  inc_feedback = INC_DATA.feedback;
  inc_threshold_thumb = INC_DATA.inc_threshold_thumb;
  inc_threshold_index = INC_DATA.inc_threshold_index;
  inc_threshold_middle = INC_DATA.inc_threshold_middle;
  inc_threshold_ring = INC_DATA.inc_threshold_ring;
  inc_threshold_little = INC_DATA.inc_threshold_little;
}

void updateThresholds()
{
  THRESHOLD_THUMB = inc_threshold_thumb;
  THRESHOLD_INDEX = inc_threshold_index;
  THRESHOLD_MIDDLE = inc_threshold_middle;
  THRESHOLD_RING = inc_threshold_ring;
  THRESHOLD_LITTLE = inc_threshold_little;
}
// 1205 3425
void prepareDataToSend()
{
  OUT_DATA.thumb_hall = analogRead(HALL_SENSOR_PIN_THUMB);
  OUT_DATA.index_hall = analogRead(HALL_SENSOR_PIN_INDEX);
  OUT_DATA.middle_hall = analogRead(HALL_SENSOR_PIN_MIDDLE);
  OUT_DATA.ring_hall = analogRead(HALL_SENSOR_PIN_RING);
  OUT_DATA.little_hall = analogRead(HALL_SENSOR_PIN_LITTLE);
}

void setupNow()
{
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void setup()
{
  // initServoFeedBack();

  initHallSensor();
  Serial.begin(115200); // Initialize serial communication
  delay(1000);
  Serial.println("Initializing");
  analogReadResolution(12);
  initPCA9685();
  // delay(500);
  // closeHand();
  delay(1000);
  openHand();
  delay(1000);
  Serial.println("Setup HALL");
  setupNow();
  dataMutex = xSemaphoreCreateMutex();
  xTaskCreate(
      readSensorsTask,
      "ReadSensors",
      4096,
      NULL,
      1,
      NULL);

  Serial.println("Sistema Iniciado com FreeRTOS Estavel.");
  delay(500);
}

bool isPressed(int val, int threshold)
{
  return (val < threshold);
}

void action_mode()
{
  switch (inc_action_mode)
  {
  case 0:
    Serial.println("Modo EMG");
    toogleHandState(inc_hand_state);
    break;
  case 1:
    Serial.println("Modo angulo");
    movServoByAngle(0, inc_thumb_pos);
    movServoByAngle(1, inc_index_pos);
    movServoByAngle(2, inc_middle_pos);
    movServoByAngle(3, inc_ring_pos);
    movServoByAngle(4, inc_little_pos);
    break;
  case 2:
    toggleGestureHand(inc_gesture);
    break;
  case 3:
    Serial.print("Modo Touch");

  case 4:
    Serial.print("Modo Toggle");
    toogleHandState(inc_hand_state);
  }
}

String getActionModeName(int action)
{
  if (action == 0)
  {
    return String("EMG - MODE");
  }
  else if (action == 1)
  {
    return String("ANGLE - MODE");
  }
  else if (action == 2)
  {
    return String("GESTURE - MODE");
  }
  else if (action == 3)
  {
    return String("TOGGLE - MODE");
  }
  else if (action == 4)
  {
    return String("DEBUG - MODE");
  }
  return "ERROR NOT DEFINED: " + String(action);
}

// // Display Functions
void updateValues(int freqMillis)
{
  static unsigned long lastSendTime = 0;

  if (millis() - lastSendTime >= freqMillis)
  {
    action_mode();
    // prepareDataToSend();
    updateThresholds();
    Serial.println("Thumb pos: " + String(inc_thumb_pos));
    Serial.println("Index pos: " + String(inc_index_pos));
    Serial.println("Middle pos: " + String(inc_middle_pos));
    Serial.println("Ring pos: " + String(inc_ring_pos));
    Serial.println("Little pos: " + String(inc_little_pos));
    Serial.println("Gesture : " + String(inc_gesture));
    Serial.println("Action mode: " + String(inc_action_mode) + " " + getActionModeName(inc_action_mode));
    Serial.println("Hand state: " + String(inc_hand_state));
    Serial.println("Feedback: " + String(inc_feedback));
    Serial.println("Thumb_threshold: " + String(THRESHOLD_THUMB));
    Serial.println("Index_threshold: " + String(THRESHOLD_INDEX));
    Serial.println("Middle_threshold: " + String(THRESHOLD_MIDDLE));
    Serial.println("Ring_threshold: " + String(THRESHOLD_RING));
    Serial.println("Little_threshold: " + String(THRESHOLD_LITTLE));
    lastSendTime = millis();
  }
}

void debugHallValues()
{
  Serial.print(OUT_DATA.thumb_hall);
  Serial.print(",");
  Serial.print(OUT_DATA.index_hall);
  Serial.print(",");
  Serial.print(OUT_DATA.middle_hall);
  Serial.print(",");
  Serial.print(OUT_DATA.ring_hall);
  Serial.print(",");
  Serial.println(OUT_DATA.little_hall);
}

void debugServo()
{
  for (size_t i = 0; i < 180; i++)
  {
    movServoByAngle(8, i);
    Serial.println("Angle: " + String(i));
    delay(300);
  }
}
void debugThresholds(bool validValues, RawData currentValues)
{
  if (validValues)
  {
    Serial.print("T:");
    Serial.print(":");
    Serial.print(THRESHOLD_THUMB);
    Serial.print(":");
    Serial.print(HALL_SENSOR_PIN_THUMB_THRESHOLD);
    Serial.print(":");
    Serial.print(currentValues.thumbVal);
    Serial.print(isPressed(currentValues.thumbVal, THRESHOLD_THUMB) ? "(ON) " : "(of) ");

    Serial.print("I:");
    Serial.print(currentValues.indexVal);
    Serial.print(isPressed(currentValues.indexVal, THRESHOLD_INDEX) ? "(ON) " : "(of) ");

    Serial.print("M:");
    Serial.print(currentValues.middleVal);
    Serial.print(isPressed(currentValues.middleVal, THRESHOLD_MIDDLE) ? "(ON) " : "(of) ");
    Serial.print("R:");
    Serial.print(currentValues.ringVal);
    Serial.print(isPressed(currentValues.ringVal, THRESHOLD_RING) ? "(ON) " : "(of) ");

    Serial.print("L:");
    Serial.print(currentValues.littleVal);
    Serial.println(isPressed(currentValues.littleVal, THRESHOLD_LITTLE) ? "(ON)" : "(of)");
  }
}

void loop()
{

  // readMacAddress();
  // debugServo();

  updateValues(300);

  RawData currentReads;
  bool dadosValidos = false;

  // // 1. Pega os dados da Task de forma segura
  if (xSemaphoreTake(dataMutex, (TickType_t)100) == pdTRUE)
  {
    currentReads = sharedData; // Copia rápida
    dadosValidos = true;
    xSemaphoreGive(dataMutex);
  }

  if (dadosValidos)
  {
    // debugThresholds(dadosValidos, currentReads);
    if (isPressed(currentReads.littleVal, THRESHOLD_LITTLE) || isPressed(currentReads.ringVal, THRESHOLD_RING) || isPressed(currentReads.indexVal, THRESHOLD_INDEX) || isPressed(currentReads.thumbVal, THRESHOLD_THUMB))
    {
      activeFeedBack(true);
    }
    else
    {
      activeFeedBack(false);
    }
  }
  else
  {
    Serial.println("Erro: Task ocupada ou travada!");
  }
}