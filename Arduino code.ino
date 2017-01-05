#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Mums Christmas lights

// Jay O'Dea, December 2016

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2
//#define CLK_PIN   100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    100
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255

//#define MAX_BRIGHTNESS 255      // Thats full on, watch the power!
//#define MIN_BRIGHTNESS 32       // set to a minimum of 25%


#define FRAMES_PER_SECOND  30


//const int brightnessInPin = A0;  // The Analog input pin that the brightness control potentiometer is attached to.


uint8_t hue = 32;
byte idex = 255;
byte meteorLength = 4;
int color;
int center = 0;
int step = -1;
int maxSteps = 5;
float fadeRate = 0.3;
int diff;

//background color
//uint32_t currentBg = random(256);
//uint32_t nextBg = currentBg;



//////////////////////////////////////////////////////////////////////////////////
void setup() {
  delay(3000); // 3 second delay for recovery

  
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}



typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, confetti, santa, juggle, rainbowlineWithGlitter, applause2, snow, rainbowWithGlitter, applause, rainbowb, bpmWithGlitter, neono, twinkle, meteorShower, sleigh};
//SimplePatternList gPatterns = { santa, juggle, rainbowb, neono };//ws2812b patterns
//SimplePatternList gPatterns = {juggle};
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gHue2 = 0;// rotating "base color" used by many of the patterns

uint8_t gHueDelta = 2;
uint8_t gHueDelta2 = -5;

/////////////////////////////////////////////////////////////////////////////////
  
void loop()
{
 // int mappedValue = map(analogRead(brightnessInPin), 0, 1023, 0, 255);
 //FastLED.setBrightness(constrain(mappedValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
 //int mappedHue;  
 
 // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(2000/FRAMES_PER_SECOND); 
  
  // do some periodic updates
  EVERY_N_MILLISECONDS( 300 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_MILLISECONDS( 50 ) { gHue2++; }
  
  EVERY_N_SECONDS( 120 ) { nextPattern(); } // change patterns periodically
 
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))



void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
///////////////////////////////////////////////////////////////////////////////////////RAINBOWS
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  gHue += gHueDelta;
}
void rainbowb() 
{  
  fill_rainbow( leds, NUM_LEDS, gHue, 0);
}

void rainbowc() 
{
  fill_rainbow( leds, NUM_LEDS, gHue2, 1.5);
  gHue2 += gHueDelta2;
}

//////////////////////////////////////////////////////////////////////////////////////GLITTERS

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random8(NUM_LEDS) ] += CRGB::White;
  }
}
void addGlitterb( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CHSV( gHue + random8(5), 255, 255);
  }
}
void addGlitters( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::Green;
  }
}
///////////////////////////////////////////////////////////////////////////////////////COMBIES
void snow() 
{
  snowy();
  addGlitters(255);
}
void rainbowWithGlitter() 
{
  rainbow();
  addGlitter(180);
}
void santa() 
{
  redsnow();
  addGlitter(255);
}
void neono() 
{
  rainbowc();
  addGlitters(255);
}
void bobbles() 
{
  confetti();
  addGlitters(190);
}
void sleigh() 
{
  tree();
  addGlitters(55);
}
void bpmWithGlitter() 
{  
  bpm();
  addGlitter(80);
}
void rainbowlineWithGlitter() 
{  
  rainbowb();
  addGlitter(180);
}
//////////////////////////////////////////////////////////////////////////////PATTERNS
void redsnow()
{
  
  fadeToBlackBy( leds, NUM_LEDS, 40);
  byte dothue = 0;
  for( int i = 0; i < 10; i++)
  {
    leds[beatsin16(i + 15, 0, NUM_LEDS)] |= CHSV(dothue, 255, 255);
    dothue += 255;
  }
}

//////////////////////////////////////////////////////////////////////////
void tree()
{
 
  fadeToBlackBy( leds, NUM_LEDS, 10);
  byte dothue =  beatsin16( 16, 0, 255);
  for( int i = 0; i < 16; i++)
  {
    leds[beatsin8(i + 9, 0, NUM_LEDS)] |= CHSV(dothue, 255, 255);
    dothue += 255;
  }
}
////////////////////////////////////////////////////////////////////////////
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue2 + random8(NUM_LEDS), 210, 255);   
}
////////////////////////////////////////////////////////////////////////////
void applause()
{
  static uint16_t lastPixel = 0;
  fadeToBlackBy( leds, NUM_LEDS, 16);
  leds[lastPixel] = CHSV(random8(HUE_BLUE, HUE_RED), 255, 255);
  lastPixel = random16(NUM_LEDS);
  leds[lastPixel] = CRGB::Green;
}
/////////////////////////////////////////////////////////////////////////////
void applause2()
{
  static uint16_t lastPixel = 0;
  static uint16_t firstPixel = 0;
  fadeToBlackBy( leds, NUM_LEDS, 4);
  leds[lastPixel] = CHSV(random16(HUE_RED, HUE_GREEN), 255, 255);
  leds[firstPixel] = CHSV(random8(HUE_BLUE, HUE_GREEN), 199, 255);
  lastPixel = random8(NUM_LEDS);
  firstPixel = random16(NUM_LEDS);
  leds[lastPixel] = CRGB::Red;
  leds[lastPixel] = CRGB::Blue;
}
/////////////////////////////////////////////////////////////////////////////
void snowy()
{
  fadeToBlackBy( leds, NUM_LEDS, 25);
  byte dothue = 0;
  for( int i = 0; i < 25; i++)
  {
    leds[beatsin16(i + 9, 0, NUM_LEDS)] |= CHSV(dothue, 0, 240);
    dothue += 64;
  }
}
/////////////////////////////////////////////////////////////////////////////
void juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++)
  {
    leds[beatsin16(i + 1, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
//////////////////////////////////////////////////////////////////////////
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 60;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin88( BeatsPerMinute, 0, 240);
  for( int i = 0; i < NUM_LEDS; i++)   //9948
  {
    leds[i] = ColorFromPalette(palette, gHue + (i * 7), beat - gHue + (i * 40));
  }
}
/////////////////////////////////////////////////////////////////////////////////////
void bpm2()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 30;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin88( BeatsPerMinute, 7, 240);
  for( int i = 0; i < NUM_LEDS; i++)   //9948
  {
    leds[i] = ColorFromPalette(palette, gHue + (i * 13), beat - gHue + (i * 100));
  }
}
////////////////////////////////////////////////////////////////////////////////////
void twinkle() {
  int i = random16(NUM_LEDS);                                           
  if (i < NUM_LEDS) leds[i] = CHSV(random16(255), random8(255),(255));             
  for (int j = 0; j < NUM_LEDS; j++) leds[j].fadeToBlackBy(25);
  LEDS.show();                                                
                                              
  LEDS.delay(50);                                       
}
//////////////////////////////////////////////////////////////////////////////////////
void meteorShower(){
  // slide all the pixels down one in the array
  memmove8( &leds[1], &leds[0], (NUM_LEDS - 1) * 3 );

  // increment the meteor display frame
  idex++;
  // make sure we don't drift into space
  if ( idex > meteorLength ) {
    idex = 0;
    // cycle through hues in each successive meteor tail
    hue += 32;  
  }

  // this switch controls the actual meteor animation, i.e., what gets placed in the
  // first position and then subsequently gets moved down the strip by the memmove above
  switch ( idex ) {
  case 0:
    leds[0] = CRGB(0,200,0); 
    break;
  case 1:
    leds[0] = CHSV((hue - 20), 255, 210); 
    break;
  case 2:
    leds[0] = CHSV((hue - 22), 255, 180); 
    break;
  case 3:
    leds[0] = CHSV((hue - 23), 255, 150); 
    break;
  case 4:
    leds[0] = CHSV((hue - 24), 255, 110); 
    break;
  case 5:
    leds[0] = CHSV((hue - 25), 255, 90); 
    break;
  case 6:
    leds[0] = CHSV((hue - 26), 160, 60); 
    break;
  case 7:
    leds[0] = CHSV((hue - 27), 140, 40); 
    break;
  case 8:
    leds[0] = CHSV((hue - 28), 120, 20); 
    break;
  case 9:
    leds[0] = CHSV((hue - 29), 100, 20); 
    break;
  default:
    leds[0] = CRGB::Black; 
  }

  // show the blinky
  FastLED.show();  
  // control the animation speed/frame rate
  delay(100);
}
void rainbow_beat() {
  uint8_t beatA = beatsin8(5, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(15, 4, 20);                        // Delta hue between LED's
  fill_rainbow(leds, NUM_LEDS, beatA, beatB);                 // Use FastLED's fill_rainbow routine.
}
/////////////////////////////////////////////////////////////////////////////////
