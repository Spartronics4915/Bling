// Mostly copied from 2017 code.

#include <Adafruit_NeoPixel.h>

#include "DisplayStrip.h"

const int numLEDs = 150;
const int dirStripPin = 10;    // Din pin to Arduino pin 4
const int dirStripControlPin[3] {6,7,8};
const int decStripPin = 11;

int loopCounter = 0;

DisplayStrip dispStrip(numLEDs, dirStripPin, dirStripControlPin);

void setup()
{
    Serial.begin(119200);
    Serial.println("Wazza?");
    dispStrip.setup();
}

void loop()
{
    dispStrip.loop(loopCounter);
    loopCounter++;
    delay(5);
}
