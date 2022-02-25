//flowingRandow_22 for LEDMatrix_22 library

//NOTE: this example will run on any size matrix panel]

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
    leds.clear();
    FastLED.delay(500);

    flowingRainbow_22();


}

/*
NOTE: function has been modified to work with Banks. It also works with
standard LEDMatrix layouts.
FastLED controllers do not use global BRIGHTNESS, so this routine is
modified by capturing brightness changes in gBrightness, then passing
this value to the LEDShow() function.
*/
void flowingRainbow_22() {
    uint8_t gBrightness;
    uint32_t intnesity = 500;       //try 5 to 500k - changes color intensities
    while (true) {
        uint32_t ms = millis();
        int32_t yHueDelta32 = ((int32_t)cos16(ms * (27 / 1)) * (350 / MATRIX_WIDTH));
        int32_t xHueDelta32 = ((int32_t)cos16(ms * (39 / 1)) * (310 / MATRIX_HEIGHT));
        RainbowDrawOneFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
        if (ms < intnesity) {
            gBrightness = (scale8(BRIGHTNESS, (ms * 256) / 5000));
        }
        else {
            gBrightness = BRIGHTNESS;
        }
        leds.LEDShow(gBrightness);
    }
}

void RainbowDrawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8) {
    byte lineStartHue = startHue8;
    for (byte y = 0; y < MATRIX_HEIGHT; y++) {
        lineStartHue += yHueDelta8;
        byte pixelHue = lineStartHue;
        for (byte x = 0; x < MATRIX_WIDTH; x++) {
            pixelHue += xHueDelta8;
            leds.drawPixel(x, y, CHSV(pixelHue, 255, 255));
        }
    }
}
