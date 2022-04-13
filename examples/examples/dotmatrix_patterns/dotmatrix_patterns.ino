/*
Example 60 of 100: FastLED matrix project dot matrix 16x16 on wokwi Arduino simulator
Source: @arcostasi on Wokwi channel from discord (https://discord.com/channels/787627282663211009/787630013658824707)
Wokwi Link: https://wokwi.com/arduino/projects/289981660621963784

from Mark Kriegsman
 https://gist.github.com/kriegsman/5adca44e14ad025e6d3b
*/

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
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

#define UPDATES_PER_SECOND 60

void setup() {
    leds.addLeds();
}

uint16_t XY(uint8_t x, uint8_t y) {     //need XY() for FastLED color utility blur2d
    return leds.mXY(x, y);
}

// Code above draw() only runs once, to set up our variables
uint32_t hue = 0;

const uint8_t kMatrixWidth = MATRIX_WIDTH;
const uint8_t kMatrixHeight = MATRIX_HEIGHT;
const uint8_t kBorderWidth = 2;

void draw() {
    // Apply some blurring to whatever's already on the matrix
    // Note that we never actually clear the matrix, we just constantly
    // blur it repeatedly. Since the blurring is 'lossy', there's
    // an automatic trend toward black -- by design.
    uint8_t blurAmount = 20; // beatsin8(2, 10, 255);
    blur2d(pleds, kMatrixWidth, kMatrixHeight, blurAmount);

    // Use two out-of-sync sine waves
    uint8_t i = beatsin8(27, kBorderWidth, kMatrixHeight - kBorderWidth);
    uint8_t j = beatsin8(41, kBorderWidth, kMatrixWidth - kBorderWidth);

    // Also calculate some reflections
    uint8_t ni = (kMatrixWidth - 1) - i;
    uint8_t nj = (kMatrixWidth - 1) - j;

    // The color of each point shifts over time, each at a different speed.
    uint16_t ms = millis();
    uint16_t ms2 = random8(0, 255);             //add more randomness

     pleds[XY(i, j)] += CHSV(ms / (11+ms2), 200, 255);
      pleds[XY(j, i)] += CHSV(ms / 13, 200, ms2);
      pleds[XY(ni, nj)] += CHSV(ms / (17 + ms2), 200, 255);
      pleds[XY(nj, ni)] += CHSV(ms / 29, 200, ms2);
      //  pleds[XY(i, nj)] += CHSV(ms / 37, 200, 255);  //more contrast of colors on blackwithout this
      pleds[XY(ni, j)] += CHSV(ms / 41, ms2, 255);

    leds.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void loop() {
    draw();
}
