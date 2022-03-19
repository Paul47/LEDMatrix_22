//tile_copy.ino for LEDMatrix_22

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
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;

//------------------------ Setup and loop ------------------------
void setup() {
    leds.addLeds();       //initiate LEDMatrix

}


void loop() {

    leds.fillScreen(CHSV(180, 100, 100));       //dark color for contrast
    tile_test();
}


//==================================== tile routines ============================================

/*
Tile rotuines can store and restore parts of the screen such as replacing background patterns
after writing over them.
NOTE: constructing the sprite buffer sprite uses ram memory so be careful of size and number
*/

#define NUM_TILES 2                  //maximum nunber of tiles expected

/*
* color screen and draw an X
* tile store the center of the screen where the lines cross
* change screen color
* restore tile to same location
* move and retore tile near top
*/
void tile_test() {

    CRGB bg = CHSV(180, 100, 100);
    CRGB newColor = CRGB::Red;
    boolean success;
    int16_t x;
    int16_t y;

    leds.fillScreen(bg);       //dark color for contrast
    leds.drawLine(0, 0, 30, 30, CRGB::Yellow);
    leds.drawLine(0, 30, 30, 0, CRGB::Green);
     leds.show();   //for Extender - replace leds.show()
    delay(1000);

    //store the pixel tile infomation
    uint8_t tileNum = 0;
    uint8_t width = 16;
    uint8_t height = 5;
    success = leds.tileInit(tileNum, width, height);

    if (success) {                           //proceed only if init valid
       x = 8;                               //store middle where lines cross
       y = 13;
        success = leds.tileStore(tileNum, x, y);
        if (success) {
            leds.fillScreen(newColor);
             leds.show();   //for Extender - replace leds.show()
            delay(1000);

            leds.tileRestore(tileNum);     //retore at same location
             leds.show();   //for Extender - replace leds.show()
            delay(1000);
  
            x = 1;
            y = 0;
            leds.tileRestore(tileNum, x, y);       //move and retore
             leds.show();   //for Extender - replace leds.show()
            delay(1000);
        }
    }
    leds.freeTile(tileNum);
}
