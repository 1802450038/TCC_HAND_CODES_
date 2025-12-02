#include <Arduino.h>
#include "display.h"

#include "buttons.h"

#include "emg.h"

#include "hand.h"

bool PRINTED_INFO = false;

const unsigned char PROGMEM image_arrow_down_bits[] = {0x20, 0x20, 0x20, 0x20,
                                                       0xa8, 0x70, 0x20};

const unsigned char PROGMEM image_arrow_up_bits[] = {0x20, 0x70, 0xa8, 0x20,
                                                     0x20, 0x20, 0x20};

const byte PROGMEM rotateFrames[][128] = {
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 1, 132, 6, 32, 1, 144, 20, 32, 1, 128, 12, 48, 1, 196, 76, 16, 1, 128, 24, 24, 1, 145, 24, 12, 1, 128, 48, 6, 1, 196, 96, 3, 1, 128, 192, 1, 193, 145, 128, 0, 241, 135, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 1, 132, 6, 32, 1, 144, 20, 32, 1, 128, 12, 48, 1, 196, 76, 16, 1, 128, 24, 24, 1, 145, 24, 12, 1, 128, 48, 6, 1, 196, 96, 3, 1, 128, 192, 1, 193, 145, 128, 0, 241, 135, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 1, 132, 6, 32, 1, 144, 20, 32, 1, 128, 12, 48, 1, 196, 76, 16, 1, 128, 24, 24, 1, 145, 24, 12, 1, 128, 48, 6, 1, 196, 96, 3, 1, 128, 192, 1, 193, 145, 128, 0, 241, 135, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 1, 132, 6, 32, 1, 144, 20, 32, 1, 128, 12, 48, 1, 196, 76, 16, 1, 128, 24, 24, 1, 145, 24, 12, 1, 128, 48, 6, 1, 196, 96, 3, 1, 128, 192, 1, 193, 145, 128, 0, 241, 135, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 0, 132, 6, 32, 0, 144, 20, 32, 0, 128, 12, 48, 0, 196, 76, 16, 0, 192, 24, 24, 0, 209, 24, 12, 0, 192, 48, 6, 0, 68, 96, 3, 0, 64, 192, 1, 192, 81, 128, 0, 240, 71, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 0, 145, 6, 96, 0, 192, 6, 32, 0, 68, 6, 32, 0, 112, 20, 32, 0, 32, 12, 48, 0, 52, 76, 16, 0, 16, 24, 24, 0, 25, 24, 12, 0, 8, 48, 6, 0, 12, 96, 3, 0, 4, 192, 1, 192, 7, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 0, 209, 6, 96, 0, 112, 6, 32, 0, 60, 6, 32, 0, 14, 20, 32, 0, 7, 140, 48, 0, 1, 204, 16, 0, 0, 248, 24, 0, 0, 56, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 255, 254, 96, 0, 255, 254, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 56, 24, 1, 128, 120, 48, 1, 165, 204, 48, 1, 131, 4, 32, 1, 158, 4, 96, 1, 184, 4, 96, 1, 240, 6, 96, 1, 192, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 151, 128, 3, 1, 134, 192, 6, 1, 140, 96, 12, 1, 140, 48, 24, 1, 152, 24, 24, 1, 184, 8, 48, 1, 176, 12, 48, 1, 224, 4, 32, 1, 224, 4, 96, 1, 192, 4, 96, 1, 192, 6, 96, 1, 128, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 239, 0, 1, 129, 195, 128, 3, 1, 192, 192, 6, 1, 192, 96, 12, 1, 192, 48, 24, 1, 192, 24, 24, 1, 192, 8, 48, 1, 192, 12, 48, 1, 128, 4, 32, 1, 128, 4, 96, 1, 128, 4, 96, 1, 128, 6, 96, 1, 128, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 224, 15, 0, 1, 128, 3, 128, 3, 0, 0, 192, 6, 0, 0, 96, 12, 0, 0, 48, 24, 0, 0, 24, 24, 0, 0, 8, 48, 0, 0, 12, 48, 0, 0, 4, 32, 0, 0, 4, 96, 0, 0, 4, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 224, 15, 0, 1, 128, 3, 128, 3, 0, 0, 192, 6, 0, 0, 96, 12, 0, 0, 48, 24, 0, 0, 24, 24, 0, 0, 8, 48, 0, 0, 12, 48, 0, 0, 4, 32, 0, 0, 4, 96, 0, 0, 4, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 224, 15, 0, 1, 128, 3, 128, 3, 0, 0, 192, 6, 0, 0, 96, 12, 0, 0, 48, 24, 0, 0, 24, 24, 0, 0, 8, 48, 0, 0, 12, 48, 0, 0, 4, 32, 0, 0, 4, 96, 0, 0, 4, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 224, 15, 0, 1, 128, 3, 128, 3, 0, 0, 192, 6, 0, 0, 96, 12, 0, 0, 48, 24, 0, 0, 24, 24, 0, 0, 8, 48, 0, 0, 12, 48, 0, 0, 4, 32, 0, 0, 4, 96, 0, 0, 4, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 224, 15, 0, 1, 128, 3, 128, 3, 0, 0, 192, 6, 0, 0, 96, 12, 0, 0, 48, 24, 0, 0, 24, 24, 0, 0, 8, 48, 0, 0, 12, 48, 0, 0, 4, 32, 0, 0, 4, 96, 0, 0, 4, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 224, 15, 0, 1, 128, 3, 128, 3, 0, 0, 192, 6, 0, 0, 96, 12, 0, 0, 48, 24, 0, 0, 24, 24, 0, 0, 8, 48, 0, 0, 12, 48, 0, 0, 4, 32, 0, 0, 4, 96, 0, 0, 4, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 131, 128, 3, 1, 128, 192, 6, 1, 128, 96, 12, 1, 128, 48, 24, 1, 128, 24, 24, 1, 128, 8, 48, 1, 128, 12, 48, 1, 128, 4, 32, 1, 128, 4, 96, 1, 128, 4, 96, 1, 128, 6, 96, 1, 128, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 239, 0, 1, 129, 195, 128, 3, 1, 192, 192, 6, 1, 192, 96, 12, 1, 192, 48, 24, 1, 192, 24, 24, 1, 192, 8, 48, 1, 192, 12, 48, 1, 128, 4, 32, 1, 128, 4, 96, 1, 128, 4, 96, 1, 128, 6, 96, 1, 128, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 151, 128, 3, 1, 134, 192, 6, 1, 140, 96, 12, 1, 140, 48, 24, 1, 152, 24, 24, 1, 184, 8, 48, 1, 176, 12, 48, 1, 224, 4, 32, 1, 224, 4, 96, 1, 192, 4, 96, 1, 192, 6, 96, 1, 128, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 56, 24, 1, 128, 120, 48, 1, 165, 204, 48, 1, 131, 4, 32, 1, 158, 4, 96, 1, 184, 4, 96, 1, 240, 6, 96, 1, 192, 6, 96, 0, 0, 6, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 255, 254, 96, 0, 255, 254, 96, 0, 0, 6, 32, 0, 0, 6, 32, 0, 0, 4, 32, 0, 0, 12, 48, 0, 0, 12, 16, 0, 0, 24, 24, 0, 0, 24, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 0, 209, 6, 96, 0, 112, 6, 32, 0, 60, 6, 32, 0, 14, 20, 32, 0, 7, 140, 48, 0, 1, 204, 16, 0, 0, 248, 24, 0, 0, 56, 12, 0, 0, 48, 6, 0, 0, 96, 3, 0, 0, 192, 1, 192, 1, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 0, 145, 6, 96, 0, 192, 6, 32, 0, 68, 6, 32, 0, 112, 20, 32, 0, 32, 12, 48, 0, 52, 76, 16, 0, 16, 24, 24, 0, 25, 24, 12, 0, 8, 48, 6, 0, 12, 96, 3, 0, 4, 192, 1, 192, 7, 128, 0, 240, 7, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 0, 132, 6, 32, 0, 144, 20, 32, 0, 128, 12, 48, 0, 196, 76, 16, 0, 192, 24, 24, 0, 209, 24, 12, 0, 192, 48, 6, 0, 68, 96, 3, 0, 64, 192, 1, 192, 81, 128, 0, 240, 71, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 1, 132, 6, 32, 1, 144, 20, 32, 1, 128, 12, 48, 1, 196, 76, 16, 1, 128, 24, 24, 1, 145, 24, 12, 1, 128, 48, 6, 1, 196, 96, 3, 1, 128, 192, 1, 193, 145, 128, 0, 241, 135, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 1, 132, 6, 32, 1, 144, 20, 32, 1, 128, 12, 48, 1, 196, 76, 16, 1, 128, 24, 24, 1, 145, 24, 12, 1, 128, 48, 6, 1, 196, 96, 3, 1, 128, 192, 1, 193, 145, 128, 0, 241, 135, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 224, 0, 0, 63, 252, 0, 0, 225, 143, 0, 1, 129, 147, 128, 3, 1, 128, 192, 6, 1, 132, 96, 12, 1, 128, 48, 24, 1, 129, 24, 24, 1, 128, 8, 48, 1, 164, 76, 48, 1, 128, 4, 32, 1, 145, 20, 96, 1, 128, 4, 96, 1, 196, 70, 96, 1, 128, 6, 96, 1, 145, 6, 96, 1, 192, 6, 32, 1, 132, 6, 32, 1, 144, 20, 32, 1, 128, 12, 48, 1, 196, 76, 16, 1, 128, 24, 24, 1, 145, 24, 12, 1, 128, 48, 6, 1, 196, 96, 3, 1, 128, 192, 1, 193, 145, 128, 0, 241, 135, 0, 0, 63, 252, 0, 0, 7, 192, 0, 0, 0, 0, 0}};

// Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int maxGraphValue = 0; // Maximum value for the graph
unsigned long lastUpdate = 0;

static int animation_frame = 0;
void playAnimation(const byte animation[][512], int num_frames)
{
  Serial.println("Num frames : " + String(num_frames));
  Serial.println("Frame : " + String(animation_frame));
  display.fillRect(78, 120, 64, 64, TFT_BLACK);
  display.drawBitmap(78, 120, animation[animation_frame], 64, 64, TFT_WHITE);
  animation_frame += 1;
  if (animation_frame >= num_frames)
  {
    animation_frame = 0;
  }
}

void drawCooldownBar(int percentage)
{
  int x = 10;
  int y = 50;  // Posição Y na parte inferior da tela
  int w = 108; // Largura total
  int h = 8;   // Altura

  // Desenha a moldura
  display.drawRect(x, y, w, h, WHITE);

  // Calcula o preenchimento baseado na porcentagem (0 a 100)
  if (percentage > 100)
    percentage = 100;
  if (percentage < 0)
    percentage = 0;

  int fillWidth = map(percentage, 0, 100, 0, w - 4); // -4 para margem interna

  // Desenha o preenchimento ou limpa se for 0
  if (percentage > 0)
  {
    display.fillRect(x + 2, y + 2, fillWidth, h - 4, WHITE);
  }
  else
  {
    // Opcional: Escrever "READY" quando estiver em 0
    display.setCursor(x + 40, y - 10);
    display.setTextSize(1);
    display.print("READY");
  }
}

void clearDisplay()
{
  display.fillScreen(BLACK);
}

void clearLine(int line)
{
  display.fillRect(0, line * TFT_LINE_HEIGHT, SCREEN_WIDTH, TFT_LINE_HEIGHT,
                   BLACK);
}

// Inicializa o display e suas funcões
void initDisplay()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();
}

void updateDisplay()
{
  display.clearDisplay();
  display.display();
}

void printDisplayLineSelected(int line, String text, bool rounded)
{
  if (rounded)
  {
    display.drawRoundRect(0, line * TFT_LINE_HEIGHT, SCREEN_WIDTH,
                          TFT_LINE_HEIGHT, 4, WHITE);
  }
  else
  {
    display.fillRect(0, line * TFT_LINE_HEIGHT, SCREEN_WIDTH, TFT_LINE_HEIGHT,
                     WHITE);
  }
  display.setCursor(4, (line * TFT_LINE_HEIGHT) + 1); // Set cursor position
  display.setTextColor(BLACK);
  display.print(text); // Print text
}

/* Imprime um texto em uma linha do display baseado em 10px por linha*/
void printDisplayLine(int line, String text, bool selected, bool rounded)
{
  display.setTextSize(TFT_TEXT_SIZE);
  if (selected)
  {
    if (rounded)
    {
      printDisplayLineSelected(line, text, true);
    }
    else
    {
      printDisplayLineSelected(line, text);
    }
  }
  else
  {
    display.setTextColor(WHITE);
    display.setCursor(2, (line * TFT_LINE_HEIGHT) + 1); // Set cursor position
    display.print(text);                                // Print text
  }
}

/* Limpa uma linha do display baseado em 10px por linha */
void clearDisplayLine(int line)
{
  display.setCursor(0, line * TFT_LINE_HEIGHT); // Set cursor position
  display.fillRect(0, line * TFT_LINE_HEIGHT, SCREEN_WIDTH, TFT_LINE_HEIGHT,
                   BLACK);
}

// Limpa uma metade de uma linha do display baseado em 10px por linha
void clearHalfDisplayLine(int line, int half)
{
  if (half == 0)
  {
    display.fillRect(0, line * TFT_LINE_HEIGHT, SCREEN_WIDTH / 2,
                     TFT_LINE_HEIGHT, TFT_BLACK);
  }
  else
  {
    display.fillRect(SCREEN_WIDTH / 2, line * TFT_LINE_HEIGHT, SCREEN_WIDTH / 2,
                     TFT_LINE_HEIGHT, TFT_BLACK);
  }
}

void emgPage()
{

  int potValue = MENU_VARIATION;

  if (ACTION_PAGE < 3)
  {
    ACTION_PAGE = 4;
  }
  else if (ACTION_PAGE > 4)
  {
    ACTION_PAGE = 3;
  }

  if (!PRINTED_INFO)
  {
    updateDisplay();
    printDisplayLine(ACTION_PAGE + 1, "", true, true);
    printDisplayLine(0, "1- EMG", true);
    if (UNLOCK_VAL)
    {
      if (ACTION_PAGE == 3)
      {

        updateLimiters(MENU_VARIATION);
        printDisplayLine(4, "Limits" + String(INF_LIMIT) + " - " + String(SUP_LIMIT), true);
        printDisplayLine(5, "Time out : " + String(TIME_OUT));
      }
      else if (ACTION_PAGE == 4)
      {
        updateTimeOut(potValue);
        printDisplayLine(5, "Limits  " + String(INF_LIMIT) + " -  " +
                                String(SUP_LIMIT));
        printDisplayLine(5, "Time out : " + String(TIME_OUT), true);
      }
    }
    else
    {
      printDisplayLine(4, "Limits  " + String(INF_LIMIT) + " -  " +
                              String(SUP_LIMIT));
      printDisplayLine(5, "Time out : " + String(TIME_OUT));
    }
    display.display();
    PRINTED_INFO = true;
  }
  else
  {
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 300)
    {
      previousMillis = currentMillis;
      clearDisplayLine(2);
      clearDisplayLine(3);
      printDisplayLine(2, "EMG Value: " + String(analogRead(ANALOG_EMG_1_PIN)));
      printDisplayLine(3, "Hand State :" +
                              String(HAND_STATE ? " Closed " : " Open"));
    }
    display.display();
  }
  // emgLines(false, false, TIME_OUT);
}

// Pagina para exibir os gestos atuais conforme incremento por gesto conforme
// estimulo
void gestureModePage()
{
  if (!PRINTED_INFO)
  {
    printDisplayLine(0, "2- GESTURE MODE", true);
    printDisplayLine(2, "GESTURE: " + String(GESTURE));
    switch (GESTURE)
    {
    case 0:
      printDisplayLine(3, "Hand open");

      break;
    case 1:
      printDisplayLine(3, "Hand One");

      break;
    case 2:
      printDisplayLine(3, "Hand Two");

      break;
    case 3:
      printDisplayLine(3, "Hand Three");
      break;
    case 4:
      printDisplayLine(3, "Hand OK");

      break;
    case 5:
      printDisplayLine(3, "Hand Thumb");

      break;
    }
    display.display();
    PRINTED_INFO = true;
  }
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 300)
  {
    previousMillis = currentMillis;
    clearDisplayLine(2);
    clearDisplayLine(3);
    clearDisplayLine(4);
    clearDisplayLine(5);
    drawCooldownBar(COOLDOWN_VAL);
  }
  display.display();
}

// Pagina para ajustar a sensibilidade da pressão nos dedos!
void touchSensePage()
{
  if (ACTION_PAGE < 0)
  {
    ACTION_PAGE = 5;
  }
  else if (ACTION_PAGE > 5)
  {
    ACTION_PAGE = 0;
  }

  if (!PRINTED_INFO)
  {
    if (UNLOCK_VAL)
    {
      setThresholdFinger(ACTION_PAGE, MENU_VARIATION);
      display.display();
    }
    printDisplayLine(0, "4- Touch sense", true);
    printDisplayLine(1, "Thum sense: " + String(THRESHOLD_THUMB));
    printDisplayLine(2, "Inde sense: " + String(THRESHOLD_INDEX));
    printDisplayLine(3, "Midd sense: " + String(THRESHOLD_MIDDLE));
    printDisplayLine(4, "Ring sense: " + String(THRESHOLD_RING));
    printDisplayLine(5, "Litt sense: " + String(THRESHOLD_LITTLE));
    printDisplayLine(ACTION_PAGE + 1, "", true, true);
    display.display();
    PRINTED_INFO = true;
  }
}

// Pagina para exibir o modo de alternar gesto conforme estimulo
void emgTogglePage()
{
  if (!PRINTED_INFO)
  {
    printDisplayLine(0, "4- EMG Toggle", true);
    printDisplayLine(3, "Time toogle: " + String(EMG_TOGGLE_TIME));

    display.display();
    PRINTED_INFO = true;
  }
  if (UNLOCK_VAL)
  {
    updateToggleTime(MENU_VARIATION);
    clearDisplayLine(3);
    printDisplayLine(3, "Time toogle: " + String(EMG_TOGGLE_TIME), true);
    display.display();
  }
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 300)
  {
    previousMillis = currentMillis;
    clearDisplayLine(2);
    printDisplayLine(2, "Hand State :" +
                            String(HAND_STATE ? " Closed " : " Open"));
    clearDisplayLine(4);
    clearDisplayLine(5);
    drawCooldownBar(COOLDOWN_VAL);
  }
  display.display();
  // toggleStateTime(EMG_TOGGLE_TIME);
}

/*
Pagina para exibir o controle manual dos dedos
OK DONE
*/
void fingersPage()
{
  if (ACTION_PAGE > 4)
  {
    ACTION_PAGE = 0;
  }
  else if (ACTION_PAGE < 0)
  {
    ACTION_PAGE = 4;
  }

  if (!PRINTED_INFO)
  {
    printDisplayLine(0, "1- FINGERS ANGLE", true);
    if (MENU_VARIATION < 0)
    {
      MENU_VARIATION = 400;
    }
    else if (MENU_VARIATION > 400)
    {
      MENU_VARIATION = 0;
    }

    String fingerNames[5] = {"THUMB", "INDEX", "MIDDLE", "RING", "PINKY"};

    int pot = map(MENU_VARIATION, 0, 400, 0, 4096);
    static int fingerPos = 0;
    static int frame = 11;
    printDisplayLine(5, "Angle: " + String(fingerPos));
    if (UNLOCK_VAL)
    {
      fingerPos = setFingerPos(ACTION_PAGE, pot);
      frame = map(fingerPos, 0, 180, 11, 3);
      printDisplayLine(5, "Angle: " + String(fingerPos), true);
    }
    display.drawBitmap(48, 16, rotateFrames[frame], 32, 32, WHITE);
    printDisplayLine(2, fingerNames[ACTION_PAGE]);
    display.display();

    display.display();
    PRINTED_INFO = true;
  }
}

void menuPage()
{
  if (ACTION_PAGE > 4)
  {
    ACTION_PAGE = 0;
  }
  else if (ACTION_PAGE < 0)
  {
    ACTION_PAGE = 4;
  }
  if (PRINTED_INFO == false)
  {
    printDisplayLine(0, "========MENU=======");
    printDisplayLine(1, "1- EMG");
    printDisplayLine(2, "2- FINGERS ANGLE");
    printDisplayLine(3, "3- GESTURE MODE");
    printDisplayLine(4, "4- TOUCH SENSE");
    printDisplayLine(5, "5- EMG TOGGLE");
    printDisplayLine(1 + ACTION_PAGE, "", true, true);
    display.display();
    // footerPage(0);
    PRINTED_INFO = true;
  }
}

void menu()
{
  ACTION_MODE = PAGE_OPTION;
  switch (PAGE_OPTION)
  {
  case 0:
    // ok done
    emgPage();
    break;
  case 1:
    // ok done
    fingersPage();
    break;
  case 2:
    //  todo
    gestureModePage();
    break;
  case 3:
    touchSensePage();
    break;
  case 4:
    emgTogglePage();
    break;
  case 99:
    // PRINTED_INFO = false;
    menuPage();
    break;
  }
}