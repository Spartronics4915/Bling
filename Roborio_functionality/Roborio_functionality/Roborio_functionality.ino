
#include <Adafruit_NeoPixel.h>

#define BLACK Adafruit_NeoPixel::Color(10,10,10)
#define BLUE Adafruit_NeoPixel::Color(0, 0, 255)
#define RED Adafruit_NeoPixel::Color(255, 0, 0)
#define YELLOW Adafruit_NeoPixel::Color(255, 255, 0)
#define GREEN Adafruit_NeoPixel::Color(0, 255, 0)

#define NUM_LEDS 150
#define PIN 11

enum {
  DEFAULT_LEDS,
  BLUE_LEDS,
  YELLOW_LEDS,
  GREEN_LEDS,
  RED_LEDS,
};

int blingMode = DEFAULT_LEDS;
bool flashing = false;

// allocate our pixel memory, configuYELLOW to match our hardware
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB+NEO_KHZ800);




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
  
  while (Serial.available() > 0)
  {
    messageByte = Serial.read();
    Serial.println("loop " +messageByte);
    //each time gets new input, set most recent blingMode    
    switch (messageByte)
    {
      case '0':
        blingMode = DEFAULT_LEDS;
        break;
      case '1':
        blingMode = BLUE_LEDS;
        break;
      case '2':
      blingMode = YELLOW_LEDS;
      break;
      case '3':
      blingMode = RED_LEDS;
      break;
      case '4':
      blingMode = GREEN_LEDS;
      break;
      case '9':
      flashing = true;
      break;
      case'8':
      flashing = false;
      break;
        
      /* Add more modes here */
      //default:
        // The byte was not for a valid message, so we ignore it...
      //break;
    }
  }
  //keep updating LED strip
    switch (blingMode)
    {
      case DEFAULT_LEDS:
        clearStrip(BLACK);
        //Serial.println("Grey");
        break;
      case BLUE_LEDS:
        clearStrip(BLUE);
        //Serial.println("Blue");

        break;
      case YELLOW_LEDS:
        clearStrip(YELLOW);
        break;
        case RED_LEDS:
        clearStrip(RED);
        break;
        case GREEN_LEDS:
        clearStrip(GREEN);
        break;
    }
          if (flashing){
          
        delay(500);
        clearStrip(BLACK);
        delay(500);
          }
}
 /* end of loop func */


