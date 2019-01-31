#include <Adafruit_NeoPixel.h>

#define WAIT_TIME 20
#define NUM_ptrn 4
#define NUM_PIXELS 12

#define PRIMARY pixels.Color(255, 0, 0)
#define OFF pixels.Color(0, 0, 0)
#define SECONDARY pixels.Color(255, 255, 255)

const int pixelPin = 4;

Adafruit_NeoPixel pixels(30, 4, NEO_GRB + NEO_KHZ800);

const uint16_t pixelNum = pixels.numPixels();

typedef bool (*PatternFunction) (Adafruit_NeoPixel& strip);

enum Change
{
  CLOCKWISE,
  COUNTERCLOCKWISE
};

enum colorMode
{
  MAX,
  MIN
};

Change lastChange = CLOCKWISE;

namespace ptrn
{
  
  void color(Adafruit_NeoPixel& strip, int pos, int max=pixelNum, colorMode mode=MAX)
  {
    if (lastChange == CLOCKWISE)
    {
      strip.setPixelColor(pos + 1, OFF);
      strip.setPixelColor(pos - 1, PRIMARY);
    } else if (lastChange == COUNTERCLOCKWISE)
    {
      strip.setPixelColor(pos - 1, OFF);
      strip.setPixelColor(pos + 1, PRIMARY);
    }
    strip.setPixelColor(pos, PRIMARY);
    for (int i = 0; i < pixelNum; i ++)
    {
      if ((i != pos && i != pos + 1 && i != pos - 1) || ((i > max && mode == MAX) || (i <= max && mode == MIN)))
        strip.setPixelColor(i, OFF);
    }
    if (max != pixelNum)
    {
      strip.setPixelColor(max, SECONDARY);
      strip.setPixelColor(0, SECONDARY);
    }
    if (((pos <= max + 1 || pos >= pixelNum - 1) && mode == MAX) || (mode == MIN))
      strip.show();
  }
  bool clockwise(Adafruit_NeoPixel& strip) {
    lastChange = CLOCKWISE;
    static int position = 0;
    Serial.println("THROUGHPUTTING");
    position = ++position % pixelNum;
    ptrn::color(strip, position);
    return true;
  }
  bool clockwiseBreak(Adafruit_NeoPixel& strip) {
    lastChange = CLOCKWISE;
    static int position = 0;
    Serial.println("HOLDING");
    position = ++position % pixelNum;
    ptrn::color(strip, position, pixelNum / 2 - 1);
    return true;
  }
  bool clockwiseBreakFlip(Adafruit_NeoPixel& strip) {
    lastChange = CLOCKWISE;
    static int position = 0;
    Serial.println("HOLDING");
    position = ++position % pixelNum;
    ptrn::color(strip, position, pixelNum / 2, MIN);
    return true;
  }
  bool counterclockwise(Adafruit_NeoPixel& strip) {
    lastChange = COUNTERCLOCKWISE;
    static int position = 0;
    Serial.println("DROPPING");
    if (position == 0)
      position = pixelNum - 1;
    else
      position--;
    ptrn::color(strip, position);
    return true;
  }
  bool counterclockwiseBreak(Adafruit_NeoPixel& strip) {
    lastChange = COUNTERCLOCKWISE;
    static int position = 0;
    Serial.println("DROPPING");
    if (position == 0)
      position = pixelNum - 1;
    else
      position--;
    ptrn::color(strip, position, pixelNum / 2 - 1);
    return true;
  }
  bool counterclockwiseBreakFlip(Adafruit_NeoPixel& strip) {
    lastChange = COUNTERCLOCKWISE;
    static int position = 0;
    Serial.println("DROPPING");
    if (position == 0)
      position = pixelNum - 1;
    else
      position--;
    ptrn::color(strip, position, pixelNum / 2, MIN);
    return true;
  }
  bool off(Adafruit_NeoPixel& strip)
  {
    for (uint16_t i = 0; i < pixelNum; i++)
    {
      strip.setPixelColor(i, OFF);
    }
    pixels.show();
    Serial.println("OFF");
    return true;
  }
}

#define numberPatterns 7
PatternFunction patterns[numberPatterns] = 
{
  ptrn::off,
  ptrn::clockwise,
  ptrn::clockwiseBreak,
  ptrn::clockwiseBreakFlip,
  ptrn::counterclockwise,
  ptrn::counterclockwiseBreak,
  ptrn::counterclockwiseBreakFlip
};

int cycle()
{
  static int id = 0;
  static int currentState;
  currentState = digitalRead(2);
  static int previousState = currentState;
  if ((currentState == LOW && previousState != currentState) || (Serial.readStringUntil("\n") == "yeet\n"))
  {
    if (++id > numberPatterns)
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
  Serial.begin(115200);
  Serial.setTimeout(50);
  pixels.begin();
  ptrn::off(pixels);
}

void loop()
{
  Serial.println(Serial.readStringUntil("\n"));
  patterns[cycle()](pixels);
}
