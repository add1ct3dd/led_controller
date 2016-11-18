#include "FastLED.h"

// Number of LED's we are running
#define NUM_LEDS 4
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
SimplePatternList gPatterns = {
  rainbow,        // 0
  sinelon,        // 1
  juggle,         // 2
  solidgreen,     // 3
  solidblue,      // 4
  solidpurple,    // 5
  solidred,       // 6
  solidyellow,    // 7
  sinelonextra,   // 8
  thruster        // 9
};

uint8_t gCurrentPatternNumber = 9; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
bool gIsHueFixed = false; // if the colour is fixed do not rotate hue..
uint8_t gLastLED = 0;

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
    if (!gIsHueFixed) { // i
  		  gHue++;    // slowly cycle the "base color" through the rainbow
    }
	}

  // If the button has been pressed change the pattern.
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
	int pos = beatsin16(20, 0, NUM_LEDS);
	leds[pos] += CHSV( gHue, 255, 192);
}

void sinelonextra()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy( leds, NUM_LEDS, 20);
	int pos = (beatsin16(20, 0, (NUM_LEDS+2))-1);
  if (pos >= 0) {
  	leds[pos] += CHSV( gHue, 255, 192);
  }
}

void thruster() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(50, 0, NUM_LEDS);

  if (pos > gLastLED) {
    leds[pos-1] += CHSV( gHue, 255, 192);
  }

  gLastLED = pos;
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

void solidgreen() {
  gIsHueFixed = true;
  gHue = 0;
  sinelon();
}

void solidblue() {
  gIsHueFixed = true;
  gHue = 160;
  sinelon();
}

void solidpurple() {
  gIsHueFixed = true;
  gHue = 130;
  sinelon();
}

void solidred() {
  gIsHueFixed = true;
  gHue = 96;
  sinelon();
}

void solidyellow() {
  gIsHueFixed = true;
  gHue = 87;
  sinelon();
}

// 0 =  green
// 96 = red
// 100 = pink
// 130 = purple
// 160 = blue
// 230 = lightblue
