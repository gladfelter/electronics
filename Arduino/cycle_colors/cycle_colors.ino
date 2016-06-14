#include <Adafruit_NeoPixel.h>
#include <WS2812_Definitions.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 30; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for(int a = 3; a < 765; a+=31) {
    int stp = max(1, a / 30);
    stp = stp % 2 == 0 ? stp + 1 : stp;
    int minr = max(0, a - 510);
    for (int r = a % 2 == 0 ? minr : min(a, 256); a % 2 == 0 ? r < min(a, 256) : r >= minr; r = a % 2 == 0 ? r + stp : r - stp) {
      int ming = max(0, r - 255);
      for(int g = r % 2 == 0 ? ming : min(a-r, 255); r % 2 == 0 ? g < min(a-r, 256) : g >= ming; g = r % 2 == 0 ? g + stp : g - stp) {
          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          pixels.setPixelColor(0, pixels.Color(r,g,a-r-g)); // Moderately bright green color.
      
          pixels.show(); // This sends the updated pixel color to the hardware.
      
          delay(delayval); // Delay for a period of time (in milliseconds).
      }
    }
  }
}
