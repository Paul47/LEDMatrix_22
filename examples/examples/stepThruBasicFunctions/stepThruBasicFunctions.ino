//stepThruBasicFunctions for LEDMatrix_22

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

//------------------- create the total matrix panel array -------------------
#if HAS_EXTENDER || HAS_BLOCKS 
        // leds in block/tile width, height      led flow IN THE TILE      # tiles in the matrix h, v    How tiles flow in the matrix
    cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, LEDS_IN_TILE, MATRIX_TILE_H_DIR, MATRIX_TILE_V_DIR, TILES_IN_MATRIX> leds;      //leds array configured
#else
    cLEDMatrix<MATRIX_WIDTH_DIR, MATRIX_HEIGHT_DIR, MATRIX_TYPE> leds;  // create our matrix based on matrix definition only
#endif

//------------------------ Setup and loop ------------------------
void setup() {

    leds.addLeds();     //no parameters required

}



void loop() {
    leds.clear();
    FastLED.delay(500);

    stepThruBasicFunctions();
}


/*
stepT hru Basic Functions
uses leds.setBrightness() - global BRIGHTNESS does not work with Controllers
leds.fillScreen(), leds.clear(), leds.showColor() - these replace FastLED calls
*/
void stepThruBasicFunctions() {
    leds.setBrightness(BRIGHTNESS);
    leds.fillScreen(CRGB::Green);
    leds.LEDShow();
    FastLED.delay(1000);
    leds.clear();          //no show cmd needed.
    FastLED.delay(1000);
    leds.showColor(CRGB::DarkCyan);   //no show cmd needed.
    FastLED.delay(1000);
    //test set brightness
    for (uint8_t i = 10; i < 100; i = i + 25) {
        leds.setBrightness(i);
        leds.showColor(CRGB::Yellow);   //no show cmd needed.
        FastLED.delay(1000);
    }
    leds.setBrightness(BRIGHTNESS);
}
