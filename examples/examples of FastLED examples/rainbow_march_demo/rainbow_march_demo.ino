/* rainbow_march_demo
 *
 * By: Andrew Tuline
 *
 * Date: March, 2015
 *
 * Rainbow marching up the strand. Pretty basic, but oh so popular, and we get a few options as well. No 'wheel' routine required.
 *
 */

/* Configuring this libraryand FastLED requires a number of parametersand #defines
Configure your LED array in Configuration.h located in the  library folder.
This way you can reuse your configuration file(s) across all your sketches.
Consider renaming your configurations(and changing the #include "myConfiguration.h"
in the library folder.
*/

#include <LEDMatrix_22.h>

    /*------------------- create the total matrix panel array -------------------
    If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
    the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
    */

cLEDMatrix leds;
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

// Initialize global variables for sequences
uint8_t thisdelay = 40;                                       // A delay value for the sequence(s)
uint8_t thishue = 0;                                          // Starting hue value.
int8_t thisrot = 1;                                           // Hue rotation speed. Includes direction.
uint8_t deltahue = 1;                                         // Hue change between pixels.
bool thisdir = 0;                                             // I use a direction variable instead of signed math so I can use it in multiple routines.


void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.

  leds.addLeds();       //initiate LEDMatrix

  leds.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA
  
} // setup()



void loop () {
  
  ChangeMe();
  
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-tileing delay to update/display the sequence.
    rainbow_march();
  }

  leds.show();

} // loop()



void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels. You would need to change the max_bright value.
  
  if (thisdir == 0) thishue += thisrot; else thishue-= thisrot;  // I could use signed math, but 'thisdir' works with other routines.
  fill_rainbow(pleds, NUM_LEDS, thishue, deltahue);            // I don't change deltahue on the fly as it's too fast near the end of the strip.
  
} // rainbow_march()



void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  
  uint8_t secondHand = (millis() / 1000) % 30;                // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisrot=1; deltahue=5; break;
      case  5: thisdir=-1; deltahue=10; break;
      case 10: thisrot=5; break;
      case 15: thisrot=5; thisdir=-1; deltahue=20; break;
      case 20: deltahue=30; break;
      case 25: deltahue=2; thisrot=5; break;
      case 30: break;
    }
  }
  
} // ChangeMe()
