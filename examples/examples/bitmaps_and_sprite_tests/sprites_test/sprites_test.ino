//sprites_test.ino for LEDMatrix_22

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
    sprite_test();
}


//==========================================new sprite w/ direct draw from progMem ===========================

/*
drawing sprites - uses bitmap 24 bit routines.
sprites are drawn directly from progMem so does not need program space, but can be included in the program code.

Create bitmap arrays of retro arcade characters and store it in Flash memory in 24 bit color.
These are typically all forward in each row, but can be zigzag.
NOTE: width and height can be defined in the bitmap files, or defined by you in code. Example:
#define DIGDUG01_W  16
#define DIGDUG01_H  16
*/

#include "DigDug01.c"
#include "DigDug02.c"

#define NUM_SPRITES 4       //maximum nunber of sprits expected
CRGB* b_ptr[NUM_SPRITES];

struct SPRITES {
    const long* bitmap;         ///ptr to included bitmap
    int16_t w;
    int16_t h;
    boolean progMem;
    boolean zigzag;
} sprite[NUM_SPRITES];


void spriteInit(uint8_t spriteNum, int16_t w, int16_t h, const long* bitmap, boolean progMem, boolean zigzag) {

    sprite[spriteNum].bitmap = bitmap;
    sprite[spriteNum].w = w;
    sprite[spriteNum].h = h;
    sprite[spriteNum].progMem = progMem;
    sprite[spriteNum].zigzag = zigzag;

}

void drawSprite(uint8_t spriteNum, int16_t x, int16_t y, CRGB bg) {

    leds.drawBitmap24(x, y, sprite[spriteNum].bitmap, sprite[spriteNum].w, sprite[spriteNum].h, sprite[spriteNum].progMem, sprite[spriteNum].zigzag, bg);
 }


void eraseSprite(uint8_t spriteNum, int16_t x1, int16_t y1, CRGB bg) {

    for (int y = y1; y < sprite[spriteNum].h; y++) {
        for (int x = x1; x < sprite[spriteNum].w; x++) {
            leds.drawPixel(x, y, bg);
        }
    }
}


void sprite_test() {

    CRGB bg = CHSV(180, 100, 100);
    int16_t x;
    int16_t y;

    leds.fillScreen(bg);       //dark color for contrast
    leds.LEDShow();   //for Extender - replace FastLED.show()

    //store the sprite infomation
    uint8_t spriteNum = 0;
    //boolean progMem = true;
    // boolean zigzag = false;
    spriteInit(spriteNum, DIGDUG01_W, DIGDUG01_H, DigDug01, DIGDUG01_P, DIGDUG01_Z);   //1st sprite
    //leds.spriteInit(spriteNum, DIGDUG01_W, DIGDUG01_H, DigDug01, DIGDUG01_P, DIGDUG01_Z);   //1st sprite

    spriteNum = 1;
    spriteInit(spriteNum, DIGDUG02_W, DIGDUG02_H, DigDug02, DIGDUG02_P, DIGDUG02_Z);   //2nd sprite
    //leds.spriteInit(spriteNum, DIGDUG02_W, DIGDUG02_H, DigDug02, DIGDUG02_P, DIGDUG02_Z);   //2nd sprite

    CRGB transparent = 0;    //transparent background

     // position sprite in a good place in the matrix
    x = 0;
    y = 0;
    if (MATRIX_WIDTH > 8) {
        x = MATRIX_WIDTH / 4;
    }
    if (MATRIX_HEIGHT > 8) {
        y = MATRIX_HEIGHT / 4;
    }

    spriteNum = 0;
    drawSprite(spriteNum, x, y, transparent);        //call is nice and short
     leds.LEDShow();   //for Extender - replace FastLED.show()
    delay(1000);
    eraseSprite(spriteNum, x, y, bg);
    spriteNum = 1;
    drawSprite(spriteNum, x, y, bg);
     leds.LEDShow();   //for Extender - replace FastLED.show()
    delay(1000);
    eraseSprite(spriteNum, x, y, bg);
}


//==================================== block routines ============================================

/*
Block rotuines can store and restore parts of the screen such as replacing background patterns
after writing over them.
NOTE: constructing the sprite buffer sprite uses ram memory so be careful of size and number
*/

#define NUM_BLOCKS 2                  //maximum nunber of blocks expected

struct BLOCKS {
    int16_t xpos;
    int16_t ypos;
    CRGB* b_ptr;         ///ptr to memory block
    uint8_t w;
    uint8_t h;
    boolean progMem;        //future use?
} block[NUM_BLOCKS];


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

    //store the sprite infomation
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
