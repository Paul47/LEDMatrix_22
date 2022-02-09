//bitmap_test.ino for LEDMatrix_22
//simple one color bitmaps using 0x0/0x01 images

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

    leds.addLeds();       //initiate LEDMatrix

}


void loop() {

    numbersTest();
   // format: drawBitmap(2, 2, bitmap, int16_t w, int16_t h, CRGB color)
}

//#include "TT_numbers_local_bits.h"      ///local memory 
#include "TT_numbers.h"      ///progmem or normal memory

void numbersTest() {
    uint8_t num[NUM_CHARS][NUM_H];      //from the included file
 
    for (uint8_t j = 0; j < 5; j++){    //simplify the display loop
        num[0][j] = num0[j];
        num[1][j] = num1[j];
        num[2][j] = num2[j];
        num[3][j] = num3[j];
        num[4][j] = num4[j];
        num[5][j] = num5[j];
        num[6][j] = num6[j];
        num[7][j] = num7[j];
        num[8][j] = num8[j];
        num[9][j] = num9[j];
    }

    //leds.setRotation(1);
    leds.clear();;
    FastLED.delay(2000);
    leds.drawRectangle(0, 0, MATRIX_WIDTH-1, MATRIX_HEIGHT-1, CRGB::White); //Draw white frame

    uint8_t x = 1, y = 1;
    for (uint8_t i = 0; i < 10; i++){
        leds.drawBitmap(x, y, num[i], NUM_W, NUM_H, CRGB::Red + 100 * i);        //ppd LOCAL from adafruit
        leds.LEDShow();
        delay(2000);
        x = x + NUM_W + 1;
        if (x > 20) {
            x = 1;
            y = y + NUM_H;
        }
    }
}

