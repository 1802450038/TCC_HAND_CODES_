#include <Arduino.h>
#include "emg.h"
#include "display.h"
#include "hand.h"
#include "prefs.h"

int POT_DIVIDER = 2; // Adjust this value based on your potentiometer's range
bool debug = false;
int INF_LIMIT = 1;
int SUP_LIMIT = 1;
int BASE_LIMIT = 1800;
int EMG_GESTURE = 0;
int TIME_OUT = 700;
unsigned long lastGestureMillis = 0;
unsigned long lastToggleMillis = 0;
int COOLDOWN_VAL = 0;

/*
Atualiza a variavel de gesto com base em um delay utilizando millis (assincrono)
*/
void incrementGesture(int gestureInterval)
{
  unsigned long currentMillis = millis();
  if (!HAND_STATE && (currentMillis - lastGestureMillis >= gestureInterval))
  {
    EMG_GESTURE += 1;
    if (EMG_GESTURE > 5)
    {
      EMG_GESTURE = 0;
    }
    lastGestureMillis = currentMillis;
  }
}

// Atualiza o estado da mão com base em um delay utilizando millis (assincrono)
void toggleStateTime(int stateInterval)
{
  unsigned long currentMillis = millis();
  if (!HAND_STATE && (currentMillis - lastToggleMillis >= stateInterval))
  {
    toggleHandState();
    lastToggleMillis = currentMillis;
  }
}

/*
Esta função recebe os valores do sinal já divididos em leitura alta e baixa
e avalia se algum deles permanece na zona de limite pelo periodo estipulado
e com isso permitidindo manter uma contraçào continua
Valor padrão timeOut deve ser algo entre 300 e 1500 milisegundos por padrão 700
*/
bool checkLimitsWithTimeout(int highOut, int lowOut, int highLim, int lowLim, unsigned long timeoutMillis)
{
  static unsigned long lastValidTime = 0;
  unsigned long currentMillis = millis();
  // Se highOut >= highLim ou lowOut <= lowLim, reseta o timer e retorna true
  if (highOut >= highLim || lowOut <= lowLim)
  {
    lastValidTime = currentMillis;
    return true;
  }

  // Se passou do tempo limite sem atingir os limites, retorna false
  if (currentMillis - lastValidTime >= timeoutMillis)
  {
    return false;
  }

  // Ainda dentro do tempo, retorna true
  return true;
}
// Default timeOut 700;

/*
Esta função separa os valores da leitura do sensor emg em dois, onde
highOutput -> valores acima do valor médio obtido pelo sensor
lowOutput -> valores abaixo do valor médio obtido pelo sensor
Eg. se o sensor tem uma faixa base entre 3000 e seus valores oscilam
entre 2500 e 3500, esta função ira isolar as variações e com isso base
nos limites superiores e inferiores ela irá criar duas saidas para que
possam ser consideradas durante as leitura de ativação e com isso
atualizar a variavél HAND_STATE
*/
void emgLines(int rawEmg, bool debug, int timeOut)
{
  int emg = rawEmg;

  static int highOutput = 0;
  static int lowOutput = 0;

  int emgValue = emg;

  if (emgValue > SUP_LIMIT)
  {
    emgValue = SUP_LIMIT;
  }

  if (emgValue < INF_LIMIT)
  {
    emgValue = INF_LIMIT;
  }

  int threshold = (SUP_LIMIT + INF_LIMIT) / 2;

  if (emgValue >= threshold)
  {
    highOutput = emgValue;
  }
  else
  {
    lowOutput = emgValue;
  }
  if (debug)
  {
    Serial.print(SUP_LIMIT);
    Serial.print(",");
    Serial.print(highOutput);
    Serial.print(",");
    Serial.print(emg);
    Serial.print(",");
    Serial.print(threshold);
    Serial.print(",");
    Serial.print(lowOutput);
    Serial.print(",");
  }

  if (checkLimitsWithTimeout(highOutput, lowOutput, SUP_LIMIT, INF_LIMIT, timeOut))
  {
    HAND_STATE = true;
    // return true;
  }
  else
  {
    HAND_STATE = false;
    // return false;
  }
  if (debug)
  {
    Serial.print(",");
    Serial.print(INF_LIMIT);
    Serial.print("Hand State : " + String(HAND_STATE ? "Closed" : "Open"));
    Serial.println();
  }
}

bool emgLinesReturn(int rawEmg, int timeOut)
{
  int emg = rawEmg;
  static int highOutput = 0;
  static int lowOutput = 0;

  int emgValue = emg;
  if (emgValue > SUP_LIMIT)
  {
    emgValue = SUP_LIMIT;
  }

  if (emgValue < INF_LIMIT)
  {
    emgValue = INF_LIMIT;
  }

  int threshold = (SUP_LIMIT + INF_LIMIT) / 2;

  if (emgValue >= threshold)
  {
    highOutput = emgValue;
  }
  else
  {
    lowOutput = emgValue;
  }
  if (checkLimitsWithTimeout(highOutput, lowOutput, SUP_LIMIT, INF_LIMIT, timeOut))
  {
    // HAND_STATE = true;
    return true;
  }
  else
  {
    // HAND_STATE = false;
    return false;
  }
}

// Funcão para visualizar os valores do emg
void emgVizualizer(bool debug)
{
  int emgValue = analogRead(ANALOG_EMG_1_PIN);

  if (emgValue < INF_LIMIT)
  {
    emgValue = INF_LIMIT;
  }
  else if (emgValue > SUP_LIMIT)
  {
    emgValue = SUP_LIMIT;
  }

  if (debug)
  {
    Serial.print(SUP_LIMIT);
    Serial.print(",");
    Serial.print(emgValue);
    Serial.print(",");
    Serial.println(INF_LIMIT);
  }
}

/*
Função para atualizar os limites superiores e inferiores com base no valor
recebido no potenciometro e um fator de divisão desse valor lido para
atenuar o incremento ou decremento
*/

void updateLimiters(int value)
{

  SUP_LIMIT = SUP_LIMIT - ((value * -1) / 2); // Adjust upper limit
  INF_LIMIT = INF_LIMIT + ((value * -1) / 2); // Adjust lower limit
  initPrefs();
  storeValue("sup_lim", SUP_LIMIT);
  storeValue("inf_lim", INF_LIMIT);
  closePrefs();
}

void increaseLimit(int potValue, int potDivider)
{
  SUP_LIMIT += 50; // Adjust upper limit based on
  INF_LIMIT -= 50; //; // Adjust lower limit based on potentiometer value
  initPrefs();
  storeValue("sup_lim", SUP_LIMIT);
  storeValue("inf_lim", INF_LIMIT);
  closePrefs();
}

void decreaseLimit(int potValue, int potDivider)
{
  SUP_LIMIT -= 50; // Adjust upper limit based on
  INF_LIMIT += 50; //; // Adjust lower limit based on potentiometer value
  initPrefs();
  storeValue("sup_lim", SUP_LIMIT);
  storeValue("inf_lim", INF_LIMIT);
  closePrefs();
}

void updateTimeOut(int potValue)
{
  TIME_OUT += potValue;
  initPrefs();
  storeValue("time_out", TIME_OUT);
  closePrefs();
}

// Funcào para iniciar o sensor emg
void initEMG()
{
  initPrefs();
  TIME_OUT = loadValue("time_out", 701);
  SUP_LIMIT = loadValue("sup_lim", 1);
  INF_LIMIT = loadValue("inf_lim", 1);
  POT_DIVIDER = loadValue("pot_div", 2);
  Serial.println("EMG Init with : ");
  Serial.println(" Time Out : " + String(TIME_OUT));
  Serial.println(" Sup Lim : " + String(SUP_LIMIT));
  Serial.println(" Inf Lim : " + String(INF_LIMIT));
  Serial.println(" Pot Div : " + String(POT_DIVIDER));
  delay(1000);
  pinMode(ANALOG_EMG_1_PIN, INPUT);
}

// Funcão apra trocar o estado da mão
void toggleHandState()
{
  HAND_STATE = !HAND_STATE;
}
