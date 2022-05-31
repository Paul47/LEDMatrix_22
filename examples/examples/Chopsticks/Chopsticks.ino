
//chopsticksexample for LEDMatrix_22

//NOTE: This example is best run on a matrix panel 32x32 or larger

/*
draw chopsticks, circles, or triangles with fading.
display speed and fading rate are set in each subroutine.
*/

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

/* DEBUGGING REPORT - sent to the serial terminal
To run report define RUN_REPORT, the Report Generator code will be included
below AFTER all the paramters are defined. Compile and upload this sketch
then run_report(); will execute at then end of setup.

>>>> Report Generator.cpp MUST BE INCLUDED BELOW just before sewtup()
after all parameters are defined <<<
*/
//#define RUN_REPORT      //will open Serial.pprint when started in setup
											 
/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly


//if run report is defined, the report_Generator.cpp code will be included
//>>>> the code MUST BE INCLUDED HERE after all parameters are defined
#ifdef RUN_REPORT
    #include ".\utility\report_Generator\report_Generator.cpp"#endif
#endif

//------------------------ Setup and loop ------------------------
  void setup() {
	      //if run report is defined, the report_Generator.cpp code will be included
    #ifdef RUN_REPORT
      Serial.begin(115200); //select your speed
      delay(1000);
     // run_report();       //also open Serial port
    #endif

    leds.addLeds();       //initiate LEDMatrix
    leds.clear();
}

void loop() {

    combined();         //all 3
    //chopsticks();
    //circles();
    //tria();

}


void chopsticks() {
    uint16_t Fader = 50;
    uint16_t Speed = 50;

    while (true) {
        uint8_t hue = random8(0, 255);
        uint8_t x1 = random8(0, leds.matrixWidth);
        uint8_t y1 = random8(0, leds.matrixHeight);
        uint8_t x2 = random8(0, leds.matrixWidth);
        uint8_t y2 = random8(0, leds.matrixHeight);

        uint8_t thickness = random8(1, 3); 

        fadeToBlackBy(pleds, leds.numLeds, Fader);

        for (uint8_t i = 0; i < thickness; i++) {
            leds.drawLine(x1, y1, x2, y2, CHSV(hue, 255, 255));
            x1++;
            x2++;
        }
        leds.show();
        FastLED.delay(Speed);
    }
}

void circles() {
    uint8_t Fader = 50;
    uint16_t Speed = 50;

    while (true) {
        uint8_t hue = random8(0, 255);
        uint8_t x1 = random8(0, leds.matrixWidth);
        uint8_t y1 = random8(0, leds.matrixHeight);
        uint8_t rad = random8(0, max(leds.matrixHeight, leds.matrixWidth));
        uint8_t thickness = random8(1, 3);                 //circle thickness

        fadeToBlackBy(pleds, leds.numLeds, Fader);

        for (uint8_t i = 0; i < thickness; i++) {
            leds.drawCircle(x1, y1, rad, CHSV(hue, 255, 255));
            rad--;
        }
        leds.show();
       FastLED.delay(Speed);
    }
}

void tria() {
    uint8_t Fader = 50;
    uint16_t Speed = 50;
    int8_t Scale = 1.2;       //let triangle sides go off matrix
    int8_t ScaleN = 4;       //let triangle sides go off matrix

    while (true) {
        int16_t hue = random16(0, 255);
        int16_t x1 = random16(-leds.matrixWidth / ScaleN, Scale * leds.matrixWidth);
        int16_t y1 = random16(-leds.matrixHeight / ScaleN, leds.matrixHeight);
        int16_t x2 = random16(-leds.matrixWidth / ScaleN, leds.matrixWidth);
        int16_t y2 = random16(-leds.matrixHeight / ScaleN, Scale * leds.matrixHeight);
        int16_t x3 = random16(-leds.matrixWidth / ScaleN, Scale * leds.matrixWidth);
        int16_t y3 = random16(-leds.matrixHeight / ScaleN, leds.matrixHeight);

        uint8_t thickness = random8(1, 3);                 // thickness

        fadeToBlackBy(pleds, leds.numLeds, Fader);

        for (uint8_t i = 0; i < thickness; i++) {
            leds.drawTriangle(x1, y1, x2, y2, x3, y3, CHSV(hue, 255, 255));
            x1++;
            x2++;
            x3++;
        }
        leds.show();
        FastLED.delay(Speed);
    }
}



void combined() {
    uint8_t Fader = 100;     //speed up fade for combined
    uint16_t Speed = 100;    //slow down for combined
    int8_t Scale = 1.2;      //let triangle sides go off matrix
    int8_t ScaleN = 4;       //let triangle sides go off matrix
    uint8_t thickness = random8(1, 3);
    fadeToBlackBy(pleds, leds.numLeds, Fader);

    uint8_t hue = random8(0, 255);
    uint8_t x1 = random8(0, leds.matrixWidth);
    uint8_t y1 = random8(0, leds.matrixHeight);
    uint8_t x2 = random8(0, leds.matrixWidth);
    uint8_t y2 = random8(0, leds.matrixHeight);

    for (uint8_t i = 0; i < thickness; i++) {
        leds.drawLine(x1, y1, x2, y2, CHSV(hue, 255, 255));
        x1++;
        x2++;
    }
 
    hue = random8(0, 255);
    uint8_t rad = random8(0, max(leds.matrixHeight, leds.matrixWidth));

    for (uint8_t i = 0; i < thickness; i++) {
        leds.drawCircle(x1, y1, rad, CHSV(hue, 255, 255));
        rad--;
    }

    int16_t x1t = random16(-leds.matrixWidth / ScaleN, Scale * leds.matrixWidth);
    int16_t y1t = random16(-leds.matrixHeight / ScaleN, leds.matrixHeight);
    int16_t x2t = random16(-leds.matrixWidth / ScaleN, leds.matrixWidth);
    int16_t y2t = random16(-leds.matrixHeight / ScaleN, Scale * leds.matrixHeight);
    int16_t x3t = random16(-leds.matrixWidth / ScaleN, Scale * leds.matrixWidth);
    int16_t y3t = random16(-leds.matrixHeight / ScaleN, leds.matrixHeight);

    hue = random8(0, 255);

    for (uint8_t i = 0; i < thickness; i++) {
        leds.drawTriangle(x1t, y1t, x2t, y2t, x3t, y3t, CHSV(hue, 255, 255));
        x1t++;
        x2t++;
        x3t++;
    }
    leds.show();
    FastLED.delay(Speed);
}