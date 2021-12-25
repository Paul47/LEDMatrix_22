//block_copy.ino for LEDMatrix_22

//NOTE: This sketch requires a matrix at least 8x8

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

   #if HAS_EXTENDER
        leds.ExtInit(NUM_LEDS, NUM_BANKS, NUM_STRIPS, BRIGHTNESS);      //Extender - init params for Extender functions
    #else
        FastLED.addLeds<CHIPSET, DATA, CLOCK, COLOR_ORDER, DATA_RATE_MHZ(SPI_MHZ)>(leds[0], NUM_LEDS).setCorrection(CORRECTION);
     #endif
}


void loop() {

    leds.fillScreen(CHSV(180, 100, 100));       //dark color for contrast
    block_test();
}


//==================================== block routines ============================================

/*
Block rotuines can store and restore parts of the screen such as replacing background patterns
after writing over them.
NOTE: constructing the sprite buffer sprite uses ram memory so be careful of size and number
*/

#define NUM_BLOCKS 2                  //maximum nunber of blocks expected

/*
* color screen and draw an X
* block store the center of the screen where the lines cross
* change screen color
* restore block to same location
* move and retore block near top
*/
void block_test() {

    CRGB bg = CHSV(180, 100, 100);
    CRGB newColor = CRGB::Red;
    boolean success;
    int16_t x;
    int16_t y;

    leds.fillScreen(bg);       //dark color for contrast
    leds.drawLine(0, 0, 30, 30, CRGB::Yellow);
    leds.drawLine(0, 30, 30, 0, CRGB::Green);
     leds.LEDShow();   //for Extender - replace FastLED.show()
    delay(1000);

    //store the pixel block infomation
    uint8_t blockNum = 0;
    uint8_t width = 16;
    uint8_t height = 5;
    success = leds.blockInit(blockNum, width, height);

    if (success) {                           //proceed only if init valid
       x = 8;                               //store middle where lines cross
       y = 13;
        success = leds.blockStore(blockNum, x, y);
        if (success) {
            leds.fillScreen(newColor);
             leds.LEDShow();   //for Extender - replace FastLED.show()
            delay(1000);

            leds.blockRestore(blockNum);     //retore at same location
             leds.LEDShow();   //for Extender - replace FastLED.show()
            delay(1000);
  
            x = 1;
            y = 0;
            leds.blockRestore(blockNum, x, y);       //move and retore
             leds.LEDShow();   //for Extender - replace FastLED.show()
            delay(1000);
        }
    }
    leds.freeBlock(blockNum);
}
