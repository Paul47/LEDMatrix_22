/* aanimations
 *
 * By: Can't recall where I found this. Maybe Stefan Petrick.
 * 
 * Modified by: Andrew Tuline
 *
 * Date: January, 2017
 *
 * This sketch demonstrates how to blend between two animations running at the same time.
 * 
 */

 //useful for debugging
#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.printl MACRO

/* Configuring this library and FastLED requires a number of parameters and #defines
Configure your LED array in myConfiguration.h located in the  library folder.
This way you can reuse your configuration file(s) across all your sketches.
Consider renaming your configurations (and changing the #include "myConfiguration.h"
in the library folder.
*/

#include <LEDMatrix_22.h>


/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;                                      // Number of LED's.

// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

// have 3 independent CRGBs - 2 for the sources and one for the output
//CRGB leds[NUM_LEDS];  //1st  is leds.cLED[NUM_LEDS]
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];



void setup() {

  delay(1000);                                                // Power-up safety delay.
  Serial.begin(115200);                                        // Initialize serial port for debugging.

  leds.addLeds();       //initiate LEDMatrix


  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
} // setup()



void loop() {

  animationA();                                               // render the first animation into leds2   
  animationB();                                               // render the second animation into leds3

  uint8_t ratio = beatsin8(2);                                // Alternate between 0 and 255 every minute
  
  for (int i = 0; i < NUM_LEDS; i++) {                        // mix the 2 arrays together
    leds.cLED[i] = blend( leds2[i], leds3[i], ratio );
  }

  leds.show();

} // loop()



void animationA() {                                             // running red stripe.

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t red = (millis() / 10) + (i * 12);                    // speed, length
    if (red > 128) red = 0;
    leds2[i] = CRGB(red, 0, 0);
  }
} // animationA()



void animationB() {                                               // running green stripe in opposite direction.
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t green = (millis() / 5) - (i * 12);                    // speed, length
    if (green > 128) green = 0;
    leds3[i] = CRGB(0, green, 0);
  }
} // animationB()
