//stepThruOneBank for LEDMatrix_22
//This sketch will work with any size matrix panel

//useful for debugging
#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.printl MACRO

/* Configuring this library and FastLED requires a number of parameters and #fastled.show()
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
  for (uint8_t i = 0; i < NUM_BANKS; i++){
    stepThruOneBank(i);
  }
}

void stepThruOneBank(uint8_t Bank) {           // simple led step through 1 at a time
    int16_t y0 = Bank * 8;
    int16_t y1 = y0 + 8;
    leds.fillScreen(CRGB::Green);
    leds.LEDShow();
    for (int16_t y = y0; y < y1; y++) {
        for (int16_t x = 0; x < MATRIX_WIDTH; x++) {
            leds.drawPixel(x, y, CRGB::Red);
            leds.LEDShow(3-Bank, BRIGHTNESS);   //for Extender - replace FastLED.show()
            FastLED.delay(20);
            leds.drawPixel(x, y, CRGB::Black);
        }
        leds.LEDShow(3-Bank, BRIGHTNESS);  //clear last led this Bank();
    }
    //FastLED.clear(true);
}