
/* rainbow_march

By: Andrew Tuline

Date: Nov, 2014

Updated: June, 2018

Update:

1.1 Remove original delay and added EVERY_N_MILLISECONDS
1.2 Remove EVERY_N_MILLISECONDS and use millis() for timing. Also demonstrate beat8 and beatsin8.


Description

Rainbow marching up the strand. Very basic, but oh so popular. Oh look, we don't need to add a 'wheel' routine.

If you want something really funky, try generating sequences with palettes, especially palettes you can change on the flly.

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
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

void setup() {

  Serial.begin(115200);

  leds.addLeds();       //initiate LEDMatrix

  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);              // FastLED 2.1 Power management set at 5V, 500mA

} // setup()



void loop () {

  rainbow_march(200, 10);
 leds.show();
  
} // loop()



void rainbow_march(uint8_t thisdelay, uint8_t deltahue) {     // The fill_rainbow call doesn't support brightness levels.

  uint8_t thishue = millis()*(255-thisdelay)/255;             // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.
  
// thishue = beat8(50);                                       // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
// thishue = beatsin8(50,0,255);                              // This can change speeds on the fly. You can also add these to each other.
  
  fill_rainbow(pleds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.

} // rainbow_march()
