//memoryTest.ino
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
#include <gfxfont.h>
#include <glcdfont.c>

/* DEBUGGING REPORT - sent to the serial terminal
To run report define RUN_REPORT, the Report Generator code will be included
below AFTER all the paramters are defined. Compile and upload this sketch
then run_report(); will execute at then end of setup.

>>>> Report Generator.cpp MUST BE INCLUDED BELOW just before sewtup()
after all parameters are defined <<<
*/
//#define RUN_REPORT      //will open Serial.pprint when started in setup

/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_BLOCKS and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

//if run report is defined, the report_Generator.cpp code will be included
//>>>> the code MUST BE IMCLUDED HERE after all parameters are defined
#ifdef RUN_REPORT
    #include <.\report_Generator\report_Generator.cpp>
#endif

//------------------------ Setup and loop ------------------------
void setup() {
    //if run report is defined, the report_Generator.cpp code will be included
#ifdef RUN_REPORT
    Serial.begin(115200); //select your speed
    delay(1000);
    run_report();       //also open Serial port
#endif
    leds.addLeds();     //no parameters required
    memoryTest();
}


void loop() {

}


void memoryTest() {
    //SetLEDArray(struct CRGB *pLED);	// Only used with externally defined LED arrays

    for (int16_t i = 0; i < 20; i++) {
        leds[i]->r = 255;
        leds[i]->g = 128;
        leds[i]->b = 32;
    }


    ptt("number of FastLED controllers = "); pt(FastLED.count());
    ptt("array size = ");  pt(FastLED.size());

    FastLED.setBrightness(20);
    ptt("Brightness = ");  pt(FastLED.getBrightness());

    pt("Step 1");
    pt("read back leds[]");
    for (int16_t i = 0; i < 20; i++) {
        ptt(leds[i]->r); ptt("\t");
        ptt(leds[i]->g); ptt("\t");
        ptt(leds[i]->b); pt("");
    }

    pt("Step 2");
    pt("read back from cLED[]");
    for (int16_t i = 0; i < 20; i++) {
        ptt(leds.cLED[i].r); ptt("\t");
        ptt(leds.cLED[i].g); ptt("\t");
        ptt(leds.cLED[i].b); pt("");
    }

    pt("Step 3");
    #ifndef HAS_EXTENDER
        pt("Extender not defined. No Extender memory");
        return;
    #endif
    #if !EXTENDER      //if no extender or false  then no array --> locks up
        pt("Exetnder option is false. No Extender memory");
        return;
    #endif
    leds.LEDShow();                     //LOLAD e_LED array
    pt("read back from e_LED[]");
    for (int16_t i = 0; i < 20; i++) {
        ptt(leds.e_LED[i].r); ptt("\t");
        ptt(leds.e_LED[i].g); ptt("\t");
        ptt(leds.e_LED[i].b); pt("");
    }
}
