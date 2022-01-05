
//Cylon_22 example for LEDMatrix_22

//NOTE: This example will run on any size matrix panel

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

//												 
//------------------- create the total matrix panel array -------------------
//If using led panels like 4x4 or 8x8 the you must define HAS_BLOCKS and configure panel sizes in configuration_22.h
//the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
#if HAS_BLOCKS 
  // leds in block/tile width, height      led flow IN THE TILE      # tiles in the matrix h, v    How tiles flow in the matrix
cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, LEDS_IN_TILE, MATRIX_TILE_H_DIR, MATRIX_TILE_V_DIR, TILES_IN_MATRIX> leds;      //leds array configured
#else
cLEDMatrix<MATRIX_WIDTH_DIR, MATRIX_HEIGHT_DIR, MATRIX_TYPE> leds;  // create our matrix based on matrix definition only
#endif

//------------------------ Setup and loop ------------------------
void setup() {

    leds.addLeds();       //initiate LEDMatrix

}

void loop() {
    leds.clear();
    FastLED.delay(500);

    Cylon_22();
}

/* 
Cylon_22 - Modified slightly to use multiple controllers
uses: leds.fadeAll(),  leds.fillAll(), leds.drawPixel()
*/
void Cylon_22() {
    //static uint8_t hue = 0;
    uint16_t value = 3000;  //default 250 ..8(xx) higher = more fade
    leds.fillScreen(CRGB::Black);

    for (int i = 0; i < 5; i++) {
        // First slide the led in one direction
        for (int16_t x = 0; x < MATRIX_WIDTH; x++) {
            for (int16_t y = 0; y < MATRIX_HEIGHT; y++) {
                leds.drawPixel(x, y, CRGB::Red);
            }
            leds.LEDShow(BRIGHTNESS); 
            leds.fadeAll(value);
            FastLED.delay(80);
        }

        for (int16_t x = MATRIX_WIDTH - 1; x > 0; x--) {
            for (int16_t y = 0; y < MATRIX_HEIGHT; y++) {
                leds.drawPixel(x, y, CRGB::Red);
            }
            leds.LEDShow(BRIGHTNESS);
            leds.fadeAll(value);
            FastLED.delay(80);
        }
    }
    leds.fillScreen(CRGB::Black);
}
