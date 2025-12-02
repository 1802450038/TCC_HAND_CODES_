#ifndef NOW_H
#define NOW_H
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>


extern uint8_t broadcastAddress[];
extern int inc_hall_thumb;
extern int inc_hall_index;
extern int inc_hall_middle;
extern int inc_hall_ring;
extern int inc_hall_little;

void updateMyData(bool debug);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void sendPackMillis(unsigned long interval);
void initNow();
void readMacAddress();


#endif // NOW_H