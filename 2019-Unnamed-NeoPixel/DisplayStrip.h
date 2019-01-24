#include <Adafruit_NeoPixel.h>

class DisplayStrip
{
public:
    DisplayStrip(int numLEDs, int stripPin, const int ctlPin[3]);

    void setup();
    void loop(int loopCounter);

private:
    bool m_states[3] {false, false, false};
    int m_ctlPins[3];
    Adafruit_NeoPixel m_pixels;
};
