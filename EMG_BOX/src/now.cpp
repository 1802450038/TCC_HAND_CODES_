#include "now.h"
#include "hand.h"

// uint8_t broadcastAddress[] = {0x18, 0x8b, 0x0e, 0x93, 0x24, 0x20};
uint8_t broadcastAddress[] = {0x18,0x8b,0x0e,0x94,0x13,0xd0};
// 0x18,0x8b,0x0e,0x94,0x13,0xd0
// 18:8b:0e:94:13:d0
int inc_hall_thumb = 0;
int inc_hall_index = 0;
int inc_hall_middle = 0;
int inc_hall_ring = 0;
int inc_hall_little = 0;

typedef struct struct_out_message {
    int thumb_pos;
    int index_pos;
    int middle_pos;
    int ring_pos;
    int little_pos;
    int gesture;
    int actionMode;
    bool hand_state;
    bool feedback;
    int threshold_thumb;
    int threshold_index;
    int threshold_middle;
    int threshold_ring;
    int threshold_little;
} struct_out_message;

typedef struct struct_inc_message {
    int thumb_hall;
    int index_hall;
    int middle_hall;
    int ring_hall;
    int little_hall;
} struct_inc_message;

String success;
struct_inc_message INC_DATA;
struct_out_message OUT_DATA;
esp_now_peer_info_t peerInfo;

// Atualiza as variavéis da struct de saida
void updateMyData(bool debug) {

  OUT_DATA.feedback = FEEDBACK_STATE;
  OUT_DATA.hand_state = HAND_STATE;
  OUT_DATA.thumb_pos = THUMB_POS;
  OUT_DATA.index_pos = INDEX_POS;
  OUT_DATA.middle_pos = MIDDLE_POS;
  OUT_DATA.ring_pos = RING_POS;
  OUT_DATA.little_pos = LITTLE_POS;
  OUT_DATA.gesture = GESTURE;
  OUT_DATA.actionMode = ACTION_MODE;
  OUT_DATA.threshold_thumb = THRESHOLD_THUMB;
  OUT_DATA.threshold_index = THRESHOLD_INDEX;
  OUT_DATA.threshold_middle = THRESHOLD_MIDDLE;
  OUT_DATA.threshold_ring = THRESHOLD_RING;
  OUT_DATA.threshold_little = THRESHOLD_LITTLE;

  if (debug) {
    Serial.println("\n\n========DEBUG FOR updateMyData()========");
    Serial.println("Data UPDATE");
    Serial.println("Feedback state: " + String(FEEDBACK_STATE));
    Serial.println("Hand state: " + String(HAND_STATE));
    Serial.println("Thumb pos: " + String(THUMB_POS));
    Serial.println("Index pos: " + String(INDEX_POS));
    Serial.println("Middle pos: " + String(MIDDLE_POS));
    Serial.println("Ring pos: " + String(RING_POS));
    Serial.println("Little pos: " + String(LITTLE_POS));
    Serial.println("Gesture: " + String(GESTURE));
    Serial.println("Action mode: " + String(ACTION_MODE));
    Serial.println("Threshold thumb: " + String(THRESHOLD_THUMB));
    Serial.println("Threshold index: " + String(THRESHOLD_INDEX)); 
    Serial.println("Threshold middle: " + String(THRESHOLD_MIDDLE));
    Serial.println("Threshold ring: " + String(THRESHOLD_RING));
    Serial.println("Threshold little: " + String(THRESHOLD_LITTLE));
    Serial.println("========================================");
  }
}

void initNow() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  delay(10);
}

// Função de retorno para quando algum dado é enviado através do espNow
void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" :
  // "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

// Função para enviar os dados utilizando delay em millis (Assincrono)
void sendPackMillis(unsigned long interval) {
  // static unsigned long lastSendTime = 0;
  // if (millis() - lastSendTime >= interval) {
    // lastSendTime = millis();
    // Send message via ESP-NOW
    updateMyData(false);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&OUT_DATA,
    sizeof(OUT_DATA)); if (result == ESP_OK)
    {
      // Serial.println("Sent with success");
    }
    else
    {
      // Serial.println("Error sending the data");
    }
  // }
}