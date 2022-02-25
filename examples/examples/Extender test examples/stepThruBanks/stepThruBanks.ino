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

/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_BLOCKS and configure panel sizes in configuration_22.h
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
