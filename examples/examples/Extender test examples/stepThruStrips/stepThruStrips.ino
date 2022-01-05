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

    stepThruStrips();
}


/*
* stepThruStrips - light up each strip in each Bank
uses leds.LEDShow() instead of FastLED.show() for the extender
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
            leds.LEDShow();
            FastLED.delay(dly);
        }
        for (int16_t y = row1 * MATRIX_TILE_HEIGHT; y < row2 * MATRIX_TILE_HEIGHT; y++) {
            for (int16_t x = MATRIX_TILE_WIDTH; x < 2 * MATRIX_TILE_WIDTH; x++) {
                leds.drawPixel(x, y, CRGB::Blue);
            }
            leds.LEDShow();
            FastLED.delay(dly);
        }
        for (int16_t y = row1 * MATRIX_TILE_HEIGHT; y < row2 * MATRIX_TILE_HEIGHT; y++) {
            for (int16_t x = 2 * MATRIX_TILE_WIDTH; x < 3 * MATRIX_TILE_WIDTH; x++) {
                leds.drawPixel(x, y, CRGB::Red);
            }
            leds.LEDShow();
            FastLED.delay(dly);
        }
        for (int16_t y = row1 * MATRIX_TILE_HEIGHT; y < row2 * MATRIX_TILE_HEIGHT; y++) {
            for (int16_t x = 3 * MATRIX_TILE_WIDTH; x < 4 * MATRIX_TILE_WIDTH; x++) {
                leds.drawPixel(x, y, CRGB::Yellow);
            }
            leds.LEDShow();
            FastLED.delay(dly);
        }
        row1++;
        row2++;
    }
}