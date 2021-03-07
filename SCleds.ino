#include <FastLED.h>


// Shawn Cruise
// 2021
//

// Requirements
// This code requires FastLED.  Developed against FastLED 3.4.0.
// http://fastled.io
// https://github.com/FastLED/FastLED
// Recommended to install FastLED first and get samples running before attempting this sketch.  This will ensure that your LEDs are displaying
// the colors correctly based on the ordering of RGB. Look at the samples and you will understand.  The code below may need to be modified if
// your LEDs are different.  The code below works with WS2812B strips.  
// Depending on your LED hardware you may have to adjust FastLED.addLeds() parameters in the setup() function below.
//
// Description
// Code derived from the FastLED samples provided.
// Pattern button selects a different pattern
// Style button selects a variation of the current pattern.
//
// Wiring
// Pin 3 to one side of momentary button designated to select the pattern
// Pin 5 to one side of momentary button designated to select the style within the same pattern
// Pin 7 to input for the ws2812b strip
// For each button connect resistor with one end on the side with the wire from the Arduino, other end connected to +5V.
// For each button connect side without arduino wire and resistor to ground.
// Connect +/- on ws2812b led strip to 5V power source.  Do not get 5V from Arduino because may not be enough amps for longer LED strips.
// Power Adruino from the 5V source and connect to common ground.
// *** For shorter strips you may be able to power Arduino from computer USB and LED strip from the Arduino 5V pin.  But be careful how many amps are being used.
//
// Notes
// -Considered running 2 different lenghts of LED strips with this code but it was very difficult.  Hard to keep different lengths syncronized.
//
// -You can connect multiple different LED strips to the same output pin.
//
// - On the ws2812b each LED is comprised of 3 smaller LEDs for red, blue and green.  Each of those consume a maximum of 20 milliamps at full brightness.
//   So each LED consumes 60 milliamps running at full brignt white.  Use that in your amp calculations.  Based on tests the most a 32 LED string used was
//   400 milliamps across all the pattererns below.  Ensure you are supplying enough amps for what you are trying to do.


// CHANGE THIS IF YOU WANT A DIFFERENT PATTERN INPUT PIN
const int patternPin = 3;


// CHANGE THIS IF YOU WANT A DIFFERENT STYLE INPUT PIN
const int stylePin = 5;


// CHANGE THIS IF YOU WANT A DIFFERENT LED OUTPUT PIN
const int ledPin = 7;


// CHANGE THIS TO MATCH THE NUMBER OF LEDS IN YOUR STRIP
const int numLeds = 32;

// MAY HAVE TO CHANGE THE LINE IN SETUP() THAT LOADS THE LEDS BASED ON YOUR SPECIFIC LED HARDWARE.  SEE FASTLED SAMPLES.

// globals for the current pattern and style
int pattern = 0;
int style = 0;

// structure to control the LED strip
CRGB leds[numLeds];

// rotating "base color" used by many of the patterns
uint8_t gHue = 0;

// rainbow palette used by some patterns
DEFINE_GRADIENT_PALETTE( rainbowpalette ) {
  0,     255, 0,  0,   //red
  36,   255, 165, 0,   //orange
  72,   255, 255, 0,  // yellow
  108,   0, 128, 0, //green
  144, 0, 0, 255, //blue
  180, 75, 0, 130, //indigo
  210, 238, 130, 238, // violet
  255, 238, 130, 238 // to the end
};


// function to check for button presses
// if the pattern button is pressed then it increments pattern and resets the style to 0
// if the style button is pressed then it just increments the style
bool buttonPress ()
{
  if (digitalRead(patternPin) == LOW)
  {
    while (digitalRead(patternPin) == LOW)
    {

    }
    pattern++;
    style = 0;
    delay(500);
    return true;
  }

  else if (digitalRead(stylePin) == LOW)
  {
    while (digitalRead(stylePin) == LOW)
    {

    }
    style++;
    delay(500);
    return true;
  }

  else
  {
    return false;
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 0
// Initial pattern shown when powered on
// Single dot moving back and forth
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void ShowCylonSingle()
{

  CRGB styleColor;

  bool randColor = false;
  bool hueAdjust = false;
  bool hueSlowAdjust = false;

  // choose the color based on the style
  switch (style % 8)
  {
    case (0):
      hueAdjust = true;  // changes the hue each time the dot moves to a new led
      break;

    case (1):
      hueSlowAdjust = true;  // changes the hue when the dot reaches the end of the strip.
      break;

    case (2):
      styleColor = CRGB::Red;
      break;

    case (3):
      styleColor = CRGB::Blue;
      break;

    case (4):
      styleColor = CRGB::White;
      break;

    case (5):
      styleColor = CRGB::OrangeRed;
      break;

    case (6):
      styleColor = CRGB::Aqua;
      break;

    case (7):
      styleColor = CRGB::Gold;
      break;

  }


  while (!buttonPress())
  {
    for (int i = 0; i < numLeds; i++)  // start to end
    {

      if (hueAdjust)
      {
        leds[i] += CHSV( gHue++, 255, 192);
      }
      else if (hueSlowAdjust)
      {
        leds[i] += CHSV( gHue, 255, 192);
      }
      else {
        leds[i] = styleColor;
      }

      if ((i == 0) || (i == (numLeds - 1)))
      {
        gHue++;
      }

      FastLED.show();

      // Wait a little bit
      delay(50);

      // turn it off
      leds[i] = CRGB::Black;

      if (buttonPress())
      {
        return;
      }
    }


    for (int j = numLeds - 1; j >= 0 ; j--) {  // end to start

      if (hueAdjust)
      {
        leds[j] += CHSV( gHue++, 255, 192);
      }
      else if (hueSlowAdjust)
      {
        leds[j] += CHSV( gHue, 255, 192);
      }
      else {
        leds[j] = styleColor;
      }

      if ((j == 0) || (j == (numLeds - 1)))
      {
        gHue++;
      }

      FastLED.show();

      // Wait a little bit
      delay(50);

      // turn it off
      leds[j] = CRGB::Black;
      if (buttonPress())
      {
        return;
      }
    }
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 1
// Shows a fading streak from the begining to end
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


void ShowStreak()
{
  //like Cylon but one way streak from begining to end
  int pos = 0;
  int pos2 = 0;

  CRGB styleColor;
  CHSV randomColor;

  bool hueAdjust = false;
  bool hueSlowAdjust = false;

  // choose the style
  switch (style % 8)
  {
    case (0):
      hueAdjust = true;  // increment hue every led
      break;

    case (1):
      hueSlowAdjust = true; // increment hue at the end of the strip
      break;

    case (2):
      styleColor = CRGB::Red;
      break;

    case (3):
      styleColor = CRGB::Blue;
      break;

    case (4):
      styleColor = CRGB::White;
      break;

    case (5):
      styleColor = CRGB::OrangeRed;
      break;

    case (6):
      styleColor = CRGB::Aqua;
      break;

    case (7):
      styleColor = CRGB::Gold;
      break;

  }

  while (!buttonPress())
  {
    fadeToBlackBy( leds, numLeds, 15);
    pos = beatsin16( 25, 0, numLeds - 1 );
    if ((pos > pos2) || (pos == 0))
    {
      if (hueAdjust)
      {
        leds[pos] += CHSV( gHue++, 255, 192);
      }
      else if (hueSlowAdjust)
      {
        leds[pos] += CHSV( gHue, 255, 192);
      }

      else {
        leds[pos] += styleColor;
      }

      if ((pos == 0) || (pos == (numLeds - 1)))
      {
        gHue++;
      }

    }
    FastLED.show();
    pos2 = pos;
    delay(20);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 2
// Regular cylon effect
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


void ShowCylon()
{

  CRGB styleColor;
  CHSV randomColor;

  bool randColor = false;
  bool hueAdjust = false;
  bool hueSlowAdjust = false;

  int pos = 0;

  // select the current style
  switch (style % 8)
  {
    case (0):
      hueAdjust = true; // increment hue on each led
      break;

    case (1):
      hueSlowAdjust = true;  // increment hue at end of strip
      break;

    case (2):
      styleColor = CRGB::Red;
      break;

    case (3):
      styleColor = CRGB::Blue;
      break;

    case (4):
      styleColor = CRGB::White;
      break;

    case (5):
      styleColor = CRGB::OrangeRed;
      break;

    case (6):
      styleColor = CRGB::Aqua;
      break;

    case (7):
      styleColor = CRGB::Gold;
      break;
  }


  while (!buttonPress())
  {
    fadeToBlackBy( leds, numLeds, 15);

    int pos = beatsin16( 13, 0, numLeds - 1 );

    if (hueAdjust)
    {
      leds[pos] += CHSV( gHue++, 255, 192);
    }
    else if (hueSlowAdjust)
    {
      leds[pos] += CHSV( gHue, 255, 192);
    }

    else {
      leds[pos] += styleColor;
    }

    if ((pos == 0) || (pos == (numLeds - 1)))
    {
      gHue++;
    }

    FastLED.show();

    delay(20);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 3
// 2 cyclon effects on the same led strip crossing each other in the middle.
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void ShowCylonSmash()
{
  CRGB styleColor1;
  CRGB styleColor2;

  bool hueAdjust = false;
  bool hueSlowAdjust = false;

  // set the style
  switch (style % 9)
  {
    case (0):
      hueAdjust = true;
      break;

    case (1):
      hueSlowAdjust = true;
      break;

    case (2):
      styleColor1 = CRGB::Red;
      styleColor2 = CRGB::Blue;
      break;

    case (3):
      styleColor1 = CRGB::Blue;
      styleColor2 = CRGB::OrangeRed;
      break;

    case (4):
      styleColor1 = CRGB::Gold;
      styleColor2 = CRGB::Green;
      break;

    case (5):
      styleColor1 = CRGB::OrangeRed;
      styleColor2 = CRGB::White;
      break;

    case (6):
      styleColor1 = CRGB::Aqua;
      styleColor2 = CRGB::White;
      break;

    case (7):
      styleColor1 = CRGB::Fuchsia;
      styleColor2 = CRGB::White;
      break;

    case (8):
      styleColor1 = CRGB::OrangeRed;
      styleColor2 = CRGB::OrangeRed;
      break;

  }




  while (!buttonPress())
  {
    fadeToBlackBy( leds, numLeds, 30);
    int pos1 = beatsin16( 13, 0, numLeds - 1 );

    int pos2 = abs(pos1 - numLeds) - 1;

    if (hueAdjust)
    {
      leds[pos1] += CHSV( gHue++, 255, 192);
      leds[pos2] += CHSV( gHue++, 255, 192);
    }
    else if (hueSlowAdjust)
    {
      leds[pos1] += CHSV( gHue, 255, 192);
      leds[pos2] += CHSV( gHue, 255, 192);
    }

    else {
      leds[pos1] += styleColor1;
      leds[pos2] += styleColor2;
    }


    if ((pos1 == 0) || (pos1 == (numLeds - 1)) || (pos2 == 0) || (pos2 == (numLeds - 1)))
    {
      gHue++;
    }

    FastLED.show();

    delay(20);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 4
// increase the hue and randomly sets an led to that hue.  
// all leds gradually increase hue
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void ShowConfetti()
{
  while (!buttonPress())
  {
    fadeToBlackBy( leds, numLeds, 10);
    int pos = random16(numLeds);
    leds[pos] += CHSV( gHue + random8(64), 200, 255);
    FastLED.show();
    gHue++;
    delay(50);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 5
// scrolling rainbow with option to have sparkles
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void ShowRainbow()
{

  bool sparkle = false;

  switch (style % 2)
  {
    case 0:
      break;

    case 1:
      sparkle = true;
      break;
  }


  while (!buttonPress())
  {
    fill_rainbow( leds, numLeds, gHue, 7);
    if (sparkle)
    {
      if ( random8() < 80) {
        leds[ random16(numLeds) ] += CRGB::White;
      }
    }
    FastLED.show();
    gHue++;
    delay(10);
  }
}



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 6
// Fire pattern with 5 options.
// 0 red fire with variable black at end
// 1 blue fire with variable black at end
// 2 rainbow fire with no back at end
// 3 red fire with no black at end
// 4 blue fire with no black at end
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////



void ShowFire()
{

  // COOLING: How much does the air cool as it rises?
  // Less cooling = taller flames.  More cooling = shorter flames.
  // Default 50, suggested range 20-100
  #define COOLING  55

  // SPARKING: What chance (out of 255) is there that a new spark will be lit?
  // Higher chance = more roaring fire.  Lower chance = more flickery fire.
  // Default 120, suggested range 50-200.
  #define SPARKING 120

  static byte heat[numLeds];

  CRGBPalette16 gPal;

  switch (style % 5)
  {
    case 0:  // red fire with black at ends sometimes
      gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
      break;

    case 1:  // blue fire with black at ends sometimes
      gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
      break;

    case 2:  // rainbow fire with no black at ends
      gPal = rainbowpalette;  //defined at the top of sketch
      break;

    case 3:  // red fire with no black at ends
      gPal = CRGBPalette16( CRGB::Red, CRGB::OrangeRed, CRGB::Yellow,  CRGB::White);
      break;

    case 4:  // blue fire with no black at ends
      gPal = CRGBPalette16( CRGB::DarkBlue, CRGB::Blue, CRGB::Aqua,  CRGB::White);
      break;

  }

  while (!buttonPress())
  {
    // Step 1.  Cool down every cell a little
    for ( int i = 0; i < numLeds; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / numLeds) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for ( int k = numLeds - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if ( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160, 255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for ( int j = 0; j < numLeds; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      pixelnumber = j;
      leds[pixelnumber] = color;
    }

    FastLED.show();

    delay(15);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 7
// Effect with light balls moving around
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void ShowJuggle()
{
  while (!buttonPress())
  {
    fadeToBlackBy( leds, numLeds, 20);
    byte dothue = 0;
    for ( int i = 0; i < 8; i++) {
      leds[beatsin16( i + 7, 0, numLeds - 1 )] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
    FastLED.show();
    gHue++;
    delay(10);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// Pattern 8
// Effect with stripes moveing back and forth
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void showBPM()
{

  while (!buttonPress())
  {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < numLeds; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    FastLED.show();
    gHue++;
    delay(15);
  }

}


void setup()
{
//  Serial.begin(9600);

  // set the pins
  pinMode(patternPin, INPUT);
  pinMode(stylePin, INPUT);
  pinMode(ledPin, OUTPUT);

  // create the LEDs
  FastLED.addLeds<WS2812B, ledPin, GRB>(leds, numLeds);  // MAY HAVE TO CHANGE THIS BASED ON LED HARDWARE.  SEE FASTLED SAMPLES.

  // initialize the pattern and style
  pattern = 0;
  style = 0;
}

void loop()
{

  // pattern keeps incrementing
  // mod 9 gives 0 - 8
  switch (pattern % 9)  
  {

    case 0:
      ShowCylonSingle();
      break;

    case 1:
      ShowStreak();
      break;

    case 2:
      ShowCylon();
      break;

    case 3:
      ShowCylonSmash();
      break;

    case 4:
      ShowConfetti();
      break;

    case 5:
      ShowRainbow();
      break;

    case 6:
      ShowFire();
      break;

    case 7:
      ShowJuggle();
      break;

    case 8:
      showBPM();
      break;
  }

}
