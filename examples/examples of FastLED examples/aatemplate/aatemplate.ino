/* Display Template for FastLED
 *
 * By: Andrew Tuline
 * 
 * Modified by: Andrew Tuline
 *
 * Date: July, 2015
 *
 * This is a simple non-tileing FastLED display sequence template.
 *
 * "non-tileing" means code can execute during pauses caused by delay();
 * Use  EVERY_N_MILLISECONDS() {} for desplay timing even is code needs rearranged.
 */

 /* Configuring this library and FastLED requires a number of parameters and #defines
 Configure your LED array in Configuration.h located in the  library folder.
 This way you can reuse your configuration file(s) across all your sketches.
 Consider renaming your configurations (and changing the #include "myConfiguration.h"
 in the library folder.
 */

#include <LEDMatrix_22.h>


 /*------------------- create the total matrix panel array -------------------
 If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
 the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
 */

cLEDMatrix leds;

#define qsubd(x, b) ((x>b)?b:0)                               // Clip. . . . A digital unsigned subtraction macro. if result <0, then x=0. Otherwise, x=b.
#define qsuba(x, b) ((x>b)?x-b:0)                             // Level shift. . . Unsigned subtraction macro. if result <0, then x=0. Otherwise x=x-b.


// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                      // Overall brightness.

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Define variables used by the sequences.
int      twinkrate = 100;                                     // The higher the value, the lower the number of twinkles.
uint8_t  thisdelay =  10;                                     // A delay value for the sequence(s).
uint8_t   thisfade =   8;                                     // How quickly does it fade? Lower = slower fade rate.
uint8_t    thishue =  50;                                     // The hue.
uint8_t    thissat = 255;                                     // The saturation, where 255 = brilliant colours.
uint8_t    thisbri = 255;                                     // Brightness of a sequence.
bool       randhue =   1;                                     // Do we want random colours all the time? 1 = yes.



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
 
  leds.addLeds();       //initiate LEDMatrix

  currentBlending = LINEARBLEND;  
  
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()



void loop () {

  ChangeMe();                                                 // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-tileing delay to update/display the sequence.
    twinkle();
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    //static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

  leds.show();
  
  Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.
  
} // loop()



void twinkle() {
   if (random8() < twinkrate) leds.cLED[random16(NUM_LEDS)] += ColorFromPalette(currentPalette, (randhue ? random8() : thishue), 255, currentBlending);
   fadeToBlackBy(leds.cLED, NUM_LEDS, thisfade);
  
} // twinkle()



void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.

  uint8_t secondHand = (millis() / 1000) % 10;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case 0: thisdelay = 10; randhue = 1; thissat=255; thisfade=8; twinkrate=150; break;  // You can change values here, one at a time , or altogether.
      case 5: thisdelay = 100; randhue = 0;  thishue = random8(); thisfade=2; twinkrate=20; break;
      case 10: break;
    }
  }

} // ChangeMe()
