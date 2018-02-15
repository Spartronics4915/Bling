#include "DirectionalStrip.h"

#define MAX 255
#define RAMP_LENGTH 38

DirectionalStrip::DirectionalStrip(int numLEDs, int stripPin, int ctlPin) :
    m_ctlPin(ctlPin),
    m_pixels(numLEDs, stripPin, NEO_GRB + NEO_KHZ800)
{
}

void
DirectionalStrip::setup()
{
  m_pixels.begin(); // Initializes the NeoPixel library
  m_pixels.setBrightness(80); // Value from 0 to 100%
  pinMode(m_ctlPin, INPUT_PULLUP);
}

// during our loop we write to all LEDs in our strip.
// The color-ramp pattern is a colored intensity ramp whose
// length is determined by RAMP_LENGTH (measured in LEDs).
void DirectionalStrip::loop(int loopCounter)
{
    int pinState = digitalRead(m_ctlPin);
    int nleds = m_pixels.numPixels();
    if(pinState == LOW)
    {
        for (int i=0; i<nleds; i++)
        {
            float v = ((i+loopCounter)%RAMP_LENGTH)/(float) RAMP_LENGTH;
            m_pixels.setPixelColor(i, (uint8_t)(MAX * v), 0, 0); // red ramp
        }
   }
   else
   {
        for(int i=0; i<nleds; i++)
        {
            float v = ((nleds - loopCounter+i)%RAMP_LENGTH)/(float) RAMP_LENGTH;
            m_pixels.setPixelColor(i, 0, (uint8_t)(MAX * v), 0); // green ramp
        }
    }
    m_pixels.show();
}
