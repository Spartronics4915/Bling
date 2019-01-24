#include "DisplayStrip.h"
#include "patAlternate.h"
#include "patFill.h"

//Colors
#define WHITE Adafruit_NeoPixel::Color(255, 255, 255)
#define YELLOW Adafruit_NeoPixel::Color(126, 112, 0)
#define BLUE Adafruit_NeoPixel::Color(10, 42, 125)

#define MAX 255 // Colors range from 0 to 255

#define NUM_PATTERNS 1
static pat *s_patterns[NUM_PATTERNS];

DisplayStrip::DisplayStrip(int numLEDs, int stripPin, const int ctlPins[3]) :
  m_ctlPins{ctlPins},
  m_pixels(numLEDs, stripPin, NEO_GRB + NEO_KHZ800)
{
  int i = 0;
  s_patterns[i++] = new patFill(m_pixels, WHITE, 5);
  s_patterns[i++] = new patAlternate(m_pixels, YELLOW,BLUE, 15, 12);
}



void DisplayStrip::setup()
{
  m_pixels.begin();
  m_pixels.setBrightness(80);
  for (int ctlPin : m_ctlPins) // Iterating through the pins
      pinMode(ctlPin, INPUT);
}

void DisplayStrip::loop(int loopCounter)
{
  for (int i(0); i < 3; i++)
  {
      if (digitalRead(m_ctlPins[i]) == LOW)
        m_states[i] = true;
      else
        m_states[i] = false;
  }
  m_pixels.show();
}
