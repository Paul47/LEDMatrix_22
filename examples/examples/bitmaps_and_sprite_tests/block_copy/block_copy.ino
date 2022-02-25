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

/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_BLOCKS and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;

//------------------------ Setup and loop ------------------------
void setup() {
    leds.addLeds();       //initiate LEDMatrix

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
