#include <Adafruit_NeoPixel.h>

#define BLACK Adafruit_NeoPixel::Color(10,10,10)//actually a very dim white
#define BLUE Adafruit_NeoPixel::Color(0, 0, 255)
#define RED Adafruit_NeoPixel::Color(255, 0, 0)
#define YELLOW Adafruit_NeoPixel::Color(255, 255, 0)
#define GREEN Adafruit_NeoPixel::Color(0, 255, 0)
#define PURPLE Adafruit_NeoPixel::Color(128,0,128)

#define NUM_LEDS 150
#define PIN 11

enum {
  DEFAULT_LEDS,
  BLUE_LEDS,
  YELLOW_LEDS,
  GREEN_LEDS,
  RED_LEDS,
  PURPLE_LEDS,
};

int blingMode = DEFAULT_LEDS;
bool flashing = false;
bool fast_flashing = false;
bool fading = false;

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
      case '5':
      blingMode = PURPLE_LEDS;
      break;
      case '6':
      fading = true;
      break;
      case '7':
      fast_flashing = true;
      break;
      case'8':
      flashing = false;
      fast_flashing = false;
      fading = false;
      pixels.setBrightness(255);
      break;
      case '9':
      flashing = true;
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
        case PURPLE_LEDS:
        clearStrip(PURPLE);
        break;
    }
          if (flashing){
          
        fast_flashing = false;
        delay(500);
        clearStrip(BLACK);
        delay(500);
          }
          if (fast_flashing){

        flashing = false;
        delay(150);
        clearStrip(BLACK);
        delay(150);
          }
   if(fading){
  for(int brightness = 3; brightness<=255; brightness++){
    pixels.setBrightness(brightness);
    pixels.show();
    delay(0.5);  
  }
  
  for(int brightness = 255; brightness>=3; brightness--){
    pixels.setBrightness(brightness);
    pixels.show();
    delay(0.5);  
   
          

      }
   }
}
 /* end of loop func */


