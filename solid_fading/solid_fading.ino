

#include "FastLED.h"


#define NUM_LEDS 8
#define DATA_PIN 11
#define DELAY_TIME 6


#define FADING_COLOR CRGB(0, 50, 170)

// Define the array of leds
CRGB leds[NUM_LEDS];  

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int i=0 ; i < NUM_LEDS; i++) {
        leds[i] = FADING_COLOR;
  }
  FastLED.setBrightness(30);
}

void loop() {
  for(int brightness = 20; brightness<=155; brightness++){
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(DELAY_TIME);  
  }
  for(int brightness = 155; brightness>=20; brightness--){
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(DELAY_TIME);  
  }
}

void displayColor(CRGB color) {
  for (int i=0 ; i < NUM_LEDS; i++) {
        leds[i] = color;
  }
  FastLED.show();
}
