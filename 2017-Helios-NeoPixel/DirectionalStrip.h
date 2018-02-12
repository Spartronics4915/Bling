#include "Adafruit_NeoPixel.h"

class DirectionalStrip
{
public:
    DirectionalStrip(int numLEDs, int stripPin, int ctlPin);

    void setup();
    void loop(int loopCounter);

private:
    int m_ctlPin;
    Adafruit_NeoPixel m_pixels;
};
