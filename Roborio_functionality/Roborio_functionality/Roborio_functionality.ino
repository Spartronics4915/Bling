
#include <Adafruit_NeoPixel.h>

#define BLACK Adafruit_NeoPixel::Color(10,10,10)
#define BLUE Adafruit_NeoPixel::Color(0, 50, 170)
//#define YELLOW Adafruit_NeoPixel::Color(255, 0, 0)
#define YELLOW Adafruit_NeoPixel::Color(90, 90, 0)

#define NUM_LEDS 150
#define PIN 11

bool flashing = false;

// allocate our pixel memory, configuYELLOW to match our hardware
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB+NEO_KHZ800);

enum {
  DEFAULT_LEDS,
  BLUE_LEDS,
  YELLOW_LEDS,
};


void clearStrip(uint32_t c)
{
   for(int i=0;i<NUM_LEDS;i++)
   {
      pixels.setPixelColor(i, c);
   }
   pixels.show();
}

void setup() {
  Serial.begin(9600);
  pixels.begin(); // initialize neopixel library
  clearStrip(BLACK);
}

void loop() {
  
  char messageByte=0;
  int blingMode = DEFAULT_LEDS;
  
  while (Serial.available() > 0)
  {
    messageByte = Serial.read();
    Serial.println("loop " +messageByte);
    switch (messageByte)
    {
      case '0':
        blingMode = DEFAULT_LEDS;
        clearStrip(BLACK);
        Serial.println("Grey");
        break;
      case '1':
        blingMode = BLUE_LEDS;
        clearStrip(BLUE);
        Serial.println("Blue");
        break;
      case '2':
      if(flashing){
        Serial.println("BLACK");
        flashing = false;
      }
      else{
        Serial.println("YELLOW");
        clearStrip(YELLOW);
        flashing = true;
      }
        //Serial.println("YELLOW");
        blingMode = YELLOW_LEDS;
        
        
        break;
      /* Add more modes here */
      default:
        // The byte was not for a valid message, so we ignore it...
      break;
    }
  }
} /* end of loop func */


