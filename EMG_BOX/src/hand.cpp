#include <Arduino.h>
#include "hand.h"
#include "prefs.h"

int THUMB_POS = 0;
int INDEX_POS = 0;
int MIDDLE_POS = 0;
int RING_POS = 0;
int LITTLE_POS = 0;
int GESTURE = 0;
int ACTION_MODE = 0;
bool HAND_STATE = false;
bool FEEDBACK_STATE = false;
int THRESHOLD_THUMB = 2270;
int THRESHOLD_INDEX = 2360;
int THRESHOLD_MIDDLE = 4096;
int THRESHOLD_RING = 2370;
int THRESHOLD_LITTLE = 2290;
int EMG_TOGGLE_TIME = 2000;
/*
Atualiza a variavél responsavel por controlara a posição dos dedos com base em valor
analogico recebido e convertido para uma faixa de angulo entre 0 e 180
*/ 
int setFingerPos(int finger, int value)
{
    int angle = map(value, 0, 4096, 180, 0);
    switch (finger)
    {
    case 0:
        THUMB_POS = angle;
        break;
    case 1:
        INDEX_POS = angle;
        break;
    case 2:
        MIDDLE_POS = angle;
        break;
    case 3:
        RING_POS = angle;
        break;
    case 4:
        LITTLE_POS = angle;
        break;
    default:
        break;
    }
    return angle;
}

//Atualiza a sensibilidade da presssão do dedo conforme o valor recebido
int setThresholdFinger(int finger, int value)
{
    switch (finger)
    {
    case 0:
        THRESHOLD_THUMB += (value / 10);
        break;
    case 1:
        THRESHOLD_INDEX += (value / 10);
        break;
    case 2:
        THRESHOLD_MIDDLE += (value / 10);
        break;
    case 3:
        THRESHOLD_RING += (value / 10);
        break;
    case 4:
        THRESHOLD_LITTLE += (value / 10);
        break;
    default:
        break;
    }
    initPrefs();
    storeValue("thresh_thumb", THRESHOLD_THUMB);
    storeValue("thresh_index", THRESHOLD_INDEX);
    storeValue("thresh_middle", THRESHOLD_MIDDLE);
    storeValue("thresh_ring", THRESHOLD_RING);
    storeValue("thresh_little", THRESHOLD_LITTLE);
    closePrefs();


    return value;
}

void updateToggleTime(int value)
{
    EMG_TOGGLE_TIME += value;
    initPrefs();
    storeValue("emg_toggle_time", EMG_TOGGLE_TIME);
    closePrefs();
}

void initHand(){
    initPrefs();
    THRESHOLD_THUMB = loadValue("thresh_thumb", 2270);
    THRESHOLD_INDEX = loadValue("thresh_index", 2360);
    THRESHOLD_MIDDLE = loadValue("thresh_middle", 4096);
    THRESHOLD_RING = loadValue("thresh_ring", 2370);
    THRESHOLD_LITTLE = loadValue("thresh_little", 2290);
    EMG_TOGGLE_TIME = loadValue("emg_toggle_time", 2000);
    closePrefs();
}

// Atualiza a variavel GESTURE conforme o valor recebido
void setGesture(int value)
{
    GESTURE = value;
}

// Atualiza a variavel HAND_STATE conforme o valor recebido
void setHandState(bool value)
{
    HAND_STATE = value;
}


// Atualiza a variavel FEEDBACK_STATE conforme o valor recebido
void setFeedbackState(bool value)
{
    FEEDBACK_STATE = value;
}