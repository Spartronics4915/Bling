#include <Adafruit_NeoPixel.h>

#define WaitPeriod 200
#define TimeoutPeriod 20

//Static loop variables:
  #define SLOOP(min, maxv) static int i = (min); static int j = (min); if (i >= maxv) { i = (min); } int max = (maxv);
  #define ENDSLOOP(min,maxv) j = i; if (i < (maxv)) {i++;}

#define PIN 4
#define NUM_PIXELS 12

#define RED Adafruit_NeoPixel::Color(255,0,0)
#define GREEN Adafruit_NeoPixel::Color(0, 255,0)
#define BLUE Adafruit_NeoPixel::Color(0,0,255)
#define YELLOW Adafruit_NeoPixel::Color(255,255,0)
#define OFF Adafruit_NeoPixel::Color(0,0,0)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
int numPixels = strip.numPixels();
void forwards()
{
  SLOOP(0, numPixels)
    strip.setPixelColor(j, OFF);
    strip.setPixelColor(i, RED);
    strip.show();
  ENDSLOOP(0, numPixels)
}

void backwards()
{
  SLOOP(0, numPixels)
    strip.setPixelColor(numPixels-j-1, OFF);
    strip.setPixelColor(numPixels-i-1, RED);
    strip.show();
  ENDSLOOP(0, numPixels)
}

void forwardsSp()
{
  SLOOP(0, numPixels)
    strip.setPixelColor(j, BLUE);
    strip.setPixelColor(i, YELLOW);
    strip.show();
  ENDSLOOP(0, numPixels)
}

void backwardsSp()
{
  SLOOP(0, numPixels)
    strip.setPixelColor(numPixels-j-1, BLUE);
    strip.setPixelColor(numPixels-i-1, YELLOW);
    strip.show();
  ENDSLOOP(0, numPixels)
}

void blood()
{
  static int k = 0;
  for (int i = 0; i < numPixels; i++)
  {
    if (i == 0)
    {
      k++;
      strip.show();
    }
    if ((i+k) % 2 == 0) {
      strip.setPixelColor(i, OFF);
    } else
      strip.setPixelColor(i, RED);
  }
}

void blinkSp()
{
  static int k = 0;
  for (int i = 0; i < numPixels; i++)
  {
    if (i == 0)
    {
      k++;
      strip.show();
    }
    if ((i+k) % 2 == 0) {
      strip.setPixelColor(i, BLUE);
    } else
      strip.setPixelColor(i, YELLOW);
  }
}

char currentCommand = '\0';

// Check for a new command byte, and return true if one is found
bool checkForCommand(void)
{
  bool result = false;
  char newCommand = '\0';
  
  // Preset our "new command" variable to be the same as the current command
  newCommand = currentCommand;
  // If there are any characters available on the serial port, read them
  while (Serial.available() > 0)
  {
    char tempCommand;
    tempCommand = Serial.read();
    if (isprint(tempCommand))
    {
      Serial.print("Got command: ");
      Serial.print(tempCommand);
      newCommand = tempCommand;
    }
  }

  // Only accept the command if it is alpha-numeric, and if it is different
  // from the current command being executed.
  if (isalnum(newCommand) && (newCommand != currentCommand))
  {
    result = true;
    currentCommand = newCommand;
    Serial.println(" VALID");
  }else if (newCommand == '\0')
  {
    Serial.println("");
  }
  return result;
}

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  Serial.println("Hello, -- HELP! I'M TRAPPED IN AN ARDUINO FACTORY!! -- LEDs!");
  strip.begin();
  strip.show();
}

void loop()
{
/**  digitalWrite(13, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(13, LOW);
  delay(1000); // Wait for 1000 millisecond(s)*/
  checkForCommand();
  switch(currentCommand) {
    case '0':
      forwards();
      break;
    case '1':
      backwards();
      break;
    case '2':
      forwardsSp();
      break;
    case '3':
      backwardsSp();
      break;
    case '4':
      blood();
      break;
    case '5':
      blinkSp();
      break;
    default:
      forwards();
  }
  delay(WaitPeriod);
}
