#include <Adafruit_NeoPixel.h>

#define WAIT_TIME 20
#define NUM_ptrn 4
#define NUM_PIXELS 12

#define WHITE pixels.Color(255, 0, 0)
#define TRAIL pixels.Color(127, 0, 0)

const int pixelPin = 4;

Adafruit_NeoPixel pixels(12, 4, NEO_GRB + NEO_KHZ800);

const uint16_t pixelNum = pixels.numPixels();

typedef bool (*PatternFunction) ();

namespace ptrn
{
  void color(int pos, int min=-1, bool ccw=false)
  {
    static uint16_t j;
    for (uint16_t i = 0; i < pixelNum; i++) 
    {
      uint16_t _i = ccw ? pixelNum - i : i;
      if (j < pixelNum)
      {
        pixels.setPixelColor(j, pixels.Color(0, 127, 0));
      if (_i == pos && ((_i <= min || _i == -1) && (ccw == false)))
      {
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        j = _i;
      }
      else
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      if (min >= 0 && _i != min)
        pixels.setPixelColor(min, pixels.Color(127, 127, 127));
      }
    }
    pixels.show();
  }
  bool clockwise() {
    static int position = 0;
    Serial.println("THROUGHPUTTING");
    position = ++position % pixelNum;
    ptrn::color(position);
    return true;
  }
  bool clockwiseBreak() {
    static int position = 0;
    Serial.println("HOLDING");
    position = ++position % pixelNum;
    ptrn::color(position, pixelNum / 2);
    return true;
  }
  bool counterclockwise() {
    static int position = 0;
    Serial.println("DROPPING");
    position = (--position < 0) ? pixelNum - 1 : position;
    ptrn::color(position);
    return true;
  }
  bool counterclockwiseBreak() {
    static int position = 0;
    Serial.println("DROPPING");
    position = (--position < 0) ? pixelNum - 1 : position;
    ptrn::color(position, pixelNum/2, true);
    return true;
  }
  bool off()
  {
    Serial.println("OFF");
    return true;
  }
}

PatternFunction patterns[] = 
{
  ptrn::off,
  ptrn::clockwise,
  ptrn::clockwiseBreak,
  ptrn::counterclockwise,
  ptrn::counterclockwiseBreak
};

int cycle()
{
  static int id = 0;
  static int currentState;
  currentState = digitalRead(2);
  static int previousState = currentState;
  if (currentState == LOW && previousState != currentState)
  {
    if (++id >= 5)
    {
      id = 0;
    }
  }
  previousState = currentState;
  return id;
}

void setup() 
{
  pinMode(2, INPUT_PULLUP);
  Serial.begin(119200);
  pixels.begin();
}

void loop()
{
  patterns[cycle()]();
  delay(50);
}
