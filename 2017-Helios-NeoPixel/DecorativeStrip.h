#include <Adafruit_NeoPixel.h>

class DecorativeStrip
{
public:
    DecorativeStrip(int numLEDs, int stripPin);
    void setup();
    void loop(int loopCounter);

private:
    Adafruit_NeoPixel m_pixels;
    int m_currentPatternId;
};
