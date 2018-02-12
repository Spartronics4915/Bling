#include <Adafruit_NeoPixel.h>

#include "DirectionalStrip.h"
#include "DecorativeStrip.h"

const int numLEDs = 150;
const int dirStripPin = 10;    // Din pin to Arduino pin 4
const int dirStripControlPin = 6;
const int decStripPin = 11;

int loopCounter = 0;

DirectionalStrip dirStrip(numLEDs, dirStripPin, dirStripControlPin);
DecorativeStrip decStrip(numLEDs, decStripPin);

void setup()
{
    Serial.begin(119200);
    Serial.println("Hello LEDs");
    decStrip.setup();
    dirStrip.setup();
}

void loop()
{
    decStrip.loop(loopCounter);
    dirStrip.loop(loopCounter);
    loopCounter++;
    delay(5);
}
