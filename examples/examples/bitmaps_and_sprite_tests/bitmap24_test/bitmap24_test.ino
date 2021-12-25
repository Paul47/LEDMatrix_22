//bitmap24_test.ino for LEDMatrix_22
//NOTE: This sketch requires a matrix at least 16x16

//========================== Bitmaps ======================================

#define BOMBJACK01_W  16
#define BOMBJACK01_H  16
#define BOMBJACK01_Z    true    //zigzag
#define BOMBJACK01_P    false   //progmem

const long BombJack01[] =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x0099ff, 0x000000, 0x000000, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x000000, 0x000000, 0x0099ff, 0x000000, 0x000000,
0x000000, 0x000000, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x0099ff, 0x0099ff, 0x0099ff, 0xffffff, 0xffffff, 0x0099ff, 0xffffff, 0xffffff, 0x0099ff, 0x0099ff, 0x0099ff, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x0099ff, 0x0099ff, 0xffffff, 0x000000, 0x0099ff, 0x000000, 0xffffff, 0x0099ff, 0x0099ff, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x0099ff, 0xffffff, 0x000000, 0x0099ff, 0x000000, 0xffffff, 0x0099ff, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffcc99, 0xffcc99, 0xffcc99, 0xffcc99, 0xffcc99, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0xffcc99, 0xffcc99, 0xffcc99, 0xffcc99, 0xffcc99, 0xffffff, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0x0099ff, 0x0099ff, 0x0099ff, 0xffff00, 0x0099ff, 0x0099ff, 0x0099ff, 0xffffff, 0x000000, 0x000000, 0x000000,
0x000000, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000,
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0x000000,
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xffffff, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

//zigzag
#define BOMBJACK02_W  16
#define BOMBJACK02_H  16
#define BOMBJACK02_Z    true    //zigzag
#define BOMBJACK02_P    false  //progmem

const long BombJack02[] =
{
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x0099ff, 0x000000, 0x000000, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x000000, 0x000000, 0x0099ff, 0x000000, 0x000000,
0x000000, 0x000000, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x0099ff, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x0099ff, 0x0099ff, 0x0099ff, 0xffffff, 0x000000, 0x0099ff, 0x000000, 0xffffff, 0x0099ff, 0x0099ff, 0x0099ff, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x0099ff, 0x0099ff, 0xffffff, 0x000000, 0x0099ff, 0x000000, 0xffffff, 0x0099ff, 0x0099ff, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x0099ff, 0xffffff, 0xffffff, 0xffcc99, 0xffffff, 0xffffff, 0x0099ff, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xff0000, 0xffcc99, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffcc99, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xffffff, 0xffcc99, 0xffcc99, 0xffcc99, 0xffcc99, 0xffcc99, 0xffffff, 0xff0000, 0xff0000, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xffffff, 0x000000, 0x000000, 0x000000,
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0x000000, 0x000000,
0x000000, 0x000000, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0x000000, 0x000000,
0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0x0099ff, 0x0099ff, 0x0099ff, 0xffff00, 0x0099ff, 0x0099ff, 0x0099ff, 0xffffff, 0xffffff, 0xffffff, 0x000000,
0x000000, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0x000000,
0x000000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xff0000, 0xffffff, 0xffffff, 0xffffff, 0xffffff,
0x000000, 0x000000, 0x000000, 0xffffff, 0xffffff, 0xff0000, 0xff0000, 0xffffff, 0xff0000, 0xff0000, 0xffffff, 0x000000, 0xffffff, 0xffffff, 0x000000, 0x000000,
0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};



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
    if (leds.matrixWidth < 16 || leds.matrixHeight < 16) {
        ptt("Matrix should be 16x16 or larger");
    }
    bitmap24_test1();
    bitmap24_test2();
}
#include "DigDug01.c"
#include "DigDug02.c"
#include "Qbert01.c"
#include "Qbert02.c"

//this is in progMEM loaded via a #include
//DIGDUG01_Z is false so lines are all left to right
//DIGDUG02_W, and DIGDUG02_H are from the bitmap file

void bitmap24_test1() {
    leds.setRotation(0); //0-3 in 90 deg steps

    //store the background infomation
    uint8_t blockNum = 0;
    int16_t wd = DIGDUG02_W;
    int16_t wh = DIGDUG02_H;
    //get set to save the background
    if (!leds.blockInit(blockNum, wd, wh)) return;  //leave if can't getr storsge

    // set up the graphic image info
    int16_t x, y = 0;
    uint8_t step = 1 + leds.matrixWidth / 16;     //atleast 8 steps across any matrix, and atleast 1 pixel each step.
    x = -wd - 1;        //start way off screen

    while (x < leds.matrixWidth + 1) {
        leds.blockStore(blockNum, x, y);    //store only if enough memory
        leds.drawBitmap24(x, y, DigDug01, wd, wh, 0);   //0=transparent background
        leds.LEDShow();   //for Extender - replace FastLED.show()();
        delay(500);
        leds.blockRestore(blockNum);     //retore at same location
        x = x + step;       //go across
        leds.blockStore(blockNum, x, y);    //store only if enough memory
        leds.drawBitmap24(x, y, DigDug02, wd, wh, 0);
        leds.LEDShow();   //for Extender - replace FastLED.show()();
        delay(500);
        leds.blockRestore(blockNum);
        x = x + step;       //go across
    }
   leds.freeBlock(blockNum);        //done with block
 }

// this one is in the code above - NOT in progMem
//the image is NOT stored as zigzag.
//each appearance is on different background w/ transperant background for the bitmap!
 void bitmap24_test2() {
     if (leds.matrixWidth < 16 || leds.matrixHeight < 16) return;

     leds.setRotation(0); //0-3 in 90 deg steps
     //store the background infomation
     uint8_t blockNum = 0;
     // set up the graphic image info
    int16_t wd = BOMBJACK01_W;
    int16_t wh = BOMBJACK01_H;
    //get set to save the background
    if (!leds.blockInit(blockNum, wd, wh)) return;  //leave if can't getr storsge
    int16_t x, y, min = 0, maxx = leds.matrixWidth - 8, maxy = leds.matrixHeight - 8;
    uint8_t bg;
    uint8_t loop = 0;
    while (loop < 10) {
        x = random(min, maxx);
        y = random(min, maxy);
        bg = random(0, 255);
        leds.fillScreen(CHSV(bg, 255, 100));    //each appearance is on different background

        leds.blockStore(blockNum, x, y);    //store only if enough memory
        leds.drawBitmap24(x, y, BombJack01, wd, wh, BOMBJACK01_P, BOMBJACK01_Z, 0);   //0=transparent background
        leds.LEDShow();   //for Extender - replace FastLED.show()();
        delay(500);

        leds.drawBitmap24(x, y, BombJack02, wd, wh, BOMBJACK02_P, BOMBJACK02_Z, 0);
        leds.LEDShow();   //for Extender - replace FastLED.show()();
        delay(500);
        leds.blockRestore(blockNum);     //retore at same location
        loop++;
    }
}
