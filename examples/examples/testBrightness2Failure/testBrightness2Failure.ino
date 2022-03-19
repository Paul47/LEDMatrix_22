//testBrightness2Failure for LEDMatrix_22 library

//This sketch will work with any size matrix panel

//Count the number of brightness levels until your matrix panel leds breakup
//Adjust the speed (Mhz) and frames per second (fps) for the best balance

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

cLEDMatrix leds;

//------------------------ Setup and loop ------------------------
void setup() {

    leds.addLeds();     //no parameters required

}


void loop() {
    leds.clear();
    FastLED.delay(500);

    testBrightness2Failure();
}


/*

uses leds.setBrightness() - global BRIGHTNESS does not work with Controllers.
leds.fillScreen(), leds.clear(), leds.showColor() - these replace FastLED calls
*/
void testBrightness2Failure() {

    for (uint8_t i = 10; i < 255; i = i + 25) {
        leds.setBrightness(i);
        leds.showColor(CRGB::White);   //no show cmd needed.
        FastLED.delay(1000);
    }
    leds.setBrightness(BRIGHTNESS);
}
