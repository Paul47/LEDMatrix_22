/*
CircusMarquee - marquee lights as found around the edge of a circus, carnival, or theatre sign
https://gist.github.com/kriegsman/83325f561c033d9adbd31120b573deb9
*/

//THIS SKETCH CAN USE AN OPTIONAL ANALOG KINOB TO CHANGE DISPLAY SPEED
// 
// Lights that chase around the outside of a sign, as found
// at carnivals, circuses, and theatres.

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

// analog speed knob attached to analog input 2.
// undefine this to just used a fixed speed
//#define KNOB_PIN      A2

// 3 is normal (1/3rd of lights lit), but 4 or 5 work.
#define CYCLE_LENGTH 3

void setup() {
    Serial.begin(115200);
    delay(1000);


    leds.addLeds();       //initiate LEDMatrix
    #ifdef KNOB_PIN
        pinMode(KNOB_PIN, INPUT);
    #endif

    leds.clear();
}

void loop() {
   Marquee();
}

//using LEDMatrix_22 variables leds.matrixWidth and leds.matrixWidth
//rather than the defines MATRIX_WIDTH and MATRIX_HEIGHT

void Marquee(){
    CRGB color = CRGB(80, 50, 20);  //old incandesent light bulb tint

    int16_t x = 0;
    int16_t y = 0;
    uint8_t dir = 0;
    uint16_t perimeter = 2 * (leds.matrixWidth + leds.matrixWidth);     //entire outside edges
    uint8_t knob = 30;          //default speed or use analog input

    #ifdef KNOB_PIN
        knob = analogRead(KNOB_PIN) / 4;
    #endif
    knob = (knob & 0xF0) + 0x0F;
    uint16_t time_divider = knob * 4;

    // fade like the way incandescent bulbs fade
    fadeUsingColor(pleds, NUM_LEDS, CRGB(200, 185, 150));

    uint8_t stepN = (millis() / time_divider) % CYCLE_LENGTH;
    uint8_t modN = 0;

    for (uint16_t i = 0; i <= perimeter; i++) {     //perimeter
       modN = addmod8(modN, 1, CYCLE_LENGTH);
            // add some power to the bulbs that are lit right now
       switch (dir) {
       case 0:     //x+
           if (modN != stepN) leds.drawPixel(x, 0, color);
           x++;
           if (x >= leds.matrixWidth - 1)  dir = 1;
           break;
       case 1: //y+
           if (modN != stepN) leds.drawPixel(leds.matrixWidth - 1, y, color);
           y++;
           if (y >= leds.matrixHeight - 1)  dir = 2;
           break;
       case 2: //x-
           if (modN != stepN) leds.drawPixel(x, leds.matrixHeight - 1, color);
           x--;
           if (x <= 0)  dir = 3;
           break;
       case 3:  //y-
           if (modN != stepN) leds.drawPixel(0, y, color);
           y--;
           if (y <= 0)  dir = 0;
           break;
       }
    }
    leds.show();
}
