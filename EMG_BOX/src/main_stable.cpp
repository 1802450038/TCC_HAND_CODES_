// #include "buttons.h"
// #include "display.h"
// #include "emg.h"
// #include "hand.h"
// #include "now.h"
// #include "prefs.h"
// #include <Arduino.h>
// #include <WiFi.h>
// #include <esp_now.h>
// #include <esp_wifi.h>



// // -------------------------------------------------------------------------------

// // void updateMillis(int ms) {
// //   static unsigned long previousMillis = 0;
// //   unsigned long currentMillis = millis();
// //   if (currentMillis - previousMillis >= ms) {
// //     previousMillis = currentMillis;
// //   }
// // }

// // JUST FOR DEBUGGING PURPOSES
// void savePrefs() {
//   initPrefs();
//   storeValue("time_out", 700);
//   storeValue("sup_lim", 350);
//   storeValue("inf_lim", 300);
//   storeValue("pot_div", 2);
//   closePrefs();
// }

// void setup() {
//   Serial.begin(115200);
//   initButtons();
//   initHand();
//   initDisplay();
//   initEMG();
//   // delay(1000);
//   initNow();
//   menu();
// }


// // WorkLimits -> 1105 - 3525

// void loop() {
//   menu();
//   btnTicks();
//   sendPackMillis(300);
// }

