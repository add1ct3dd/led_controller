#include "FastLED.h"

// Number of LED's we are running
#define NUM_LEDS 5
// Data pin we're outputting to
#define DATA_PIN 0
// Pin used for clock
#define CLOCK_PIN 2
// Pin for Button to toggle modes.
#define BUTTON_PIN 1
// Max brightness, less than 100% is better.
#define BRIGHTNESS          96
// FPS to run loop at.
#define FRAMES_PER_SECOND  120
// Simple colour order.
#define COLOR_ORDER RGB

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
      FastLED.addLeds<WS2812, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
      FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, sinelon, juggle };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
bool isFixed = false; // if the colour is fixed do not rotate hue..

void loop()
{
	// Call the current pattern function once, updating the 'leds' array
	gPatterns[gCurrentPatternNumber]();

	// send the 'leds' array out to the actual LED strip
	FastLED.show();
	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / FRAMES_PER_SECOND);

	// do some periodic updates
	EVERY_N_MILLISECONDS( 20 )
	{
    if (!isFixed) {
  		  gHue++;    // slowly cycle the "base color" through the rainbow
    }
	}

  if (digitalRead(BUTTON_PIN) == HIGH) {
    nextPattern();
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
	// add one to the current pattern number, and wrap around at the end
	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns );
}

void rainbow()
{
	// FastLED's built-in rainbow generator
	fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void sinelon()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy( leds, NUM_LEDS, 20);
	int pos = beatsin16(13, 0, NUM_LEDS);
	leds[pos] += CHSV( gHue, 255, 192);
}

void juggle()
{
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy( leds, NUM_LEDS, 20);
	byte dothue = 0;
	for( int i = 0; i < 8; i++)
	{
		leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}
