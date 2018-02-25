#include <Adafruit_NeoPixel.h>

#define BLACK Adafruit_NeoPixel::Color(0,0,0)//actually a dim white
#define BLUE Adafruit_NeoPixel::Color(0, 0, 128)
#define RED Adafruit_NeoPixel::Color(128, 0, 0)
#define YELLOW Adafruit_NeoPixel::Color(128, 128, 0)
#define GREEN Adafruit_NeoPixel::Color(0, 128, 0)
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
bool spartronics_fade = false;
bool blue = true;

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
  Serial.println("Hello LEDS!");
}
void loop() {
  
  char messageByte=0;
  
  while (Serial.available() > 0)
  {
    messageByte = Serial.read();
    //Serial.println("loop " +messageByte);
    //each time gets new input, set most recent blingMode    
    switch (messageByte)
    {
      case 'a':
      blingMode = PURPLE_LEDS;
      break;
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
      spartronics_fade = true;
      break;
      case '6':
      fading = true;
      break;
      case '7':
      fast_flashing = true;
      break;
      case'8':
      //resets all effects
      flashing = false;
      fast_flashing = false;
      fading = false;
      spartronics_fade = false;
      pixels.setBrightness(100);
      break;
      case '9':
      flashing = true;
      break;
      /*case 'd':
      theaterChase();
      break;*/
      /* Add more modes here */
      //default:
      //break;        // The byte was not for a valid message, so we ignore it...

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
  for(int brightness = 3; brightness<=100; brightness++){
    pixels.setBrightness(brightness);
    pixels.show();
    delay(10);  
  }
  for(int brightness = 100; brightness>=3; brightness--){
    pixels.setBrightness(brightness);
    pixels.show();
    delay(10);
   
      }
   }
   if(spartronics_fade){
    
          if (blue){
          clearStrip(BLUE);
          blue = false;
  } else {
          clearStrip(YELLOW);
          blue = true; 
      }
      for(int brightness = 3; brightness<=100; brightness++){
    pixels.setBrightness(brightness);  
    pixels.show();
    delay(10);  
  }
  for(int brightness = 100; brightness>=3; brightness--){
    pixels.setBrightness(brightness);
    pixels.show();
    delay(10);
   
      }
   }
}
/* end of loop func */


/*void theaterChase(uint8_t wait) {
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixel_stick.numPixels(); i=i+3) {
        pixel_stick.setPixelColor(i+q, Wheel( (i) % 255));    //turn every third pixel on
      }
       pixel_stick.show();

      delay(wait);

      for (uint16_t i=0; i < pixel_stick.numPixels(); i=i+3) {
        pixel_stick.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}*/
 
