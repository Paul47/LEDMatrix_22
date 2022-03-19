//stepThruStrips for LEDMatrix_22
//This sketch works with any size matrix panel

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

    stepThruStrips();
}


/*
* stepThruStrips - light up each strip in each Bank
uses leds.show() instead of leds.show() for the extender
*/
void stepThruStrips() {
    uint8_t dly = 50;
    uint8_t row1 = 0;
    uint8_t row2 = 1;
    for (uint8_t j = 0; j < NUM_BANKS; j++) {

        for (int16_t y = row1 * MATRIX_TILE_HEIGHT; y < row2 * MATRIX_TILE_HEIGHT; y++) {
            for (int16_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                leds.drawPixel(x, y, CRGB::Green);
            }
            leds.show();
            FastLED.delay(dly);
        }
        for (int16_t y = row1 * MATRIX_TILE_HEIGHT; y < row2 * MATRIX_TILE_HEIGHT; y++) {
            for (int16_t x = MATRIX_TILE_WIDTH; x < 2 * MATRIX_TILE_WIDTH; x++) {
                leds.drawPixel(x, y, CRGB::Blue);
            }
            leds.show();
            FastLED.delay(dly);
        }
        for (int16_t y = row1 * MATRIX_TILE_HEIGHT; y < row2 * MATRIX_TILE_HEIGHT; y++) {
            for (int16_t x = 2 * MATRIX_TILE_WIDTH; x < 3 * MATRIX_TILE_WIDTH; x++) {
                leds.drawPixel(x, y, CRGB::Red);
            }
            leds.show();
            FastLED.delay(dly);
        }
        for (int16_t y = row1 * MATRIX_TILE_HEIGHT; y < row2 * MATRIX_TILE_HEIGHT; y++) {
            for (int16_t x = 3 * MATRIX_TILE_WIDTH; x < 4 * MATRIX_TILE_WIDTH; x++) {
                leds.drawPixel(x, y, CRGB::Yellow);
            }
            leds.show();
            FastLED.delay(dly);
        }
        row1++;
        row2++;
    }
}
