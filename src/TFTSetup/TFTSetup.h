#ifndef TFTSETUP_H
#define TFTSETUP_H

#include <Arduino.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

void initTFT();
void displayTime();

#endif
