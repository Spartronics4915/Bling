/*  Test program to show how to use NeoPixel LEDs
 *
 *  Connect Arduino to NeoPixel stick with 5V and Gnd for power,
 *  and pin D6 for control of the LEDs. Make sure that the
 *  Arduino is switched to 5V (not 3.3V) if it has a switch.
 *
 *
 */

/* Include files for libraries, etc. */
#include <Adafruit_NeoPixel.h>

/* Constants for configuring the program */
#define NUMBER_OF_LEDS  8
#define CONTROL_PIN 6

/*  Initialize the NeoPixel library
 *
 *  Adafruit_NeoPixel pixel_stick = Adafruit_NeoPixel(NUMBER_OF_LEDS, CONTROL_PIN, NEO_GRB + NEO_KHZ800);
 *
 *  Creates an objecct called 'pixel_stick'
 *  Parameter 1 = number of pixels in pixel_stick
 *  Parameter 2 = Arduino pin number (most are OK to use)
 *  Parameter 3 = pixel type flags, add together as needed:
 *    NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
 *    NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
 *    NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
 *    NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 *    NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
 */
Adafruit_NeoPixel pixel_stick = Adafruit_NeoPixel(NUMBER_OF_LEDS, CONTROL_PIN, NEO_GRB + NEO_KHZ800);

/*
 * The setup() function is automatically run one time when the power is applied
 * to the Arduino. Use it to set up pin functions and initialize libraries.
 */
void setup() {
  /* Start up (initialize) the NeoPixels */
  pixel_stick.begin();
  /* Set all the pixels to "off" by calling the method show() without any data */
  pixel_stick.show();
}

/*
 * The loop() function is automatically run over and over after the setup()
 * function finishes. It repeats forever. Use it to get the Arduino to do
 * all the things you want it to do.
 */
void loop() {
  /*
   * Some example procedures showing how to display to the pixels.
   * See the function definitions below the loop() to learn what
   * they are doing.
   * Note that the colors are made of three parts, red, green and blue.
   * The LED mixes different amounts of each to make the color your eye
   * sees. Each of thse "primary" colors can be given a different
   * intensity (brightness), from off (0) to full brightness (255).
   * Just put the number for the brightness you want in the parameter
   * field for the color. For example, wherever you see the Color()
   * method used, you can replace the RED_INTENSITY etc.
   * with numbers from 0 to 255:
   * pixel_stick.Color(RED_INTENSITY, GREEN_INTENSITY, BLUE_INTENSITY)
   */
  colorWipe(pixel_stick.Color(255, 0, 0), 50); // Red
  colorWipe(pixel_stick.Color(0, 255, 0), 50); // Green
  colorWipe(pixel_stick.Color(0, 0, 255), 50); // Blue

  // Send a theater pixel chase in...
  theaterChase(pixel_stick.Color(127, 127, 127), 50); // White
  theaterChase(pixel_stick.Color(127, 0, 0), 50); // Red
  theaterChase(pixel_stick.Color(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

/*
 * Fill the LEDs one at a time with the desired color,
 * and wait for some time between each new pixel. This
 * makes the color "crawl" onto the stick one LED at
 * a time.
 */
void colorWipe(uint32_t color, uint8_t milliseconds_to_wait) {
  for(uint16_t led_number=0; led_number<pixel_stick.numPixels(); led_number++) {
    pixel_stick.setPixelColor(led_number, color);
    pixel_stick.show();
    delay(milliseconds_to_wait);
  }
}

/*
 * Use the Wheel() function to display a spectrum of colors
 * across the pixels on the stick.
 */
void rainbow(uint8_t milliseconds_to_wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixel_stick.numPixels(); i++) {
      pixel_stick.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixel_stick.show();
    delay(milliseconds_to_wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixel_stick.numPixels(); i++) {
      pixel_stick.setPixelColor(i, Wheel(((i * 256 / pixel_stick.numPixels()) + j) & 255));
    }
    pixel_stick.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixel_stick.numPixels(); i=i+3) {
        pixel_stick.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixel_stick.show();

      delay(wait);

      for (uint16_t i=0; i < pixel_stick.numPixels(); i=i+3) {
        pixel_stick.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixel_stick.numPixels(); i=i+3) {
        pixel_stick.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixel_stick.show();

      delay(wait);

      for (uint16_t i=0; i < pixel_stick.numPixels(); i=i+3) {
        pixel_stick.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixel_stick.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixel_stick.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixel_stick.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
