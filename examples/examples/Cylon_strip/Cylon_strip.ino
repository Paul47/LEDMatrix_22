//Cylon_strip.ino
// light up strip 1 down and back along (x), then strip 2, through each strip (y)

/*************************************
Strips and configursation.h

For 1 strip set x = striplengrh, y = 1.

For up to 16 strips in parallel use the LED Extender (see the LEDMatrix_22 wiki)
For example with 4 led strips, set:
  Banks = 1
  Strips per Bank = 4
  MATRIX_WIDTH (x) = leds per strip
  MATRIX_HEIGHT (y) = 4
  Now code strip 1 as x,1. Strip 2 as x,2 and so on.

  The other configuration parameters will also need to be set.
************************************/

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

//if run report is defined, the report_Generator.cpp code will be included
//>>>> the code MUST BE IMCLUDED HERE after all parameters are defined
#ifdef RUN_REPORT
    #include <.\report_Generator\report_Generator.cpp>
#endif
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

//------------------------ Setup and loop ------------------------
void setup() {
    //if run report is defined, the report_Generator.cpp code will be included
#ifdef RUN_REPORT
    Serial.begin(115200); //select your speed
    delay(1000);
    run_report();       //also open Serial port
#endif
	leds.addLeds();
}


// light up strip 1 down and back along (x), then strip 2, through each strip (y)
void loop() { 

	for (int y = 0; y < MATRIX_HEIGHT; y++){
		static uint8_t hue = 0;
		int16_t value = 3000;  //default 250 ..8(xx) higher = more fade
		// First slide the led in one direction
		for(int x = 0; x < MATRIX_WIDTH; x++) {
			// Set the i'th led to red 
			leds.drawPixel(x ,y, CHSV(hue++, 255, 255));
			// Show the leds
			leds.show();   //for Extender - replace leds.show() 
			// now that we've shown the leds, reset the i'th led to black
			// leds[i] = CRGB::Black;
			leds.fadeAll(value);
			// Wait a little bit before we loop around and do it again
			FastLED.delay(50);
		}

		// Now go in the other direction.  
		for(int x = (MATRIX_WIDTH)-1; x >= 0; x--) {
			// Set the i'th led to red 
			leds.drawPixel(x ,y, CHSV(hue++, 255, 255));
			// Show the leds
			leds.show();   //for Extender - replace leds.show()
			// now that we've shown the leds, reset the i'th led to black
			// leds[i] = CRGB::Black;
			leds.fadeAll(value);
			// Wait a little bit before we loop around and do it again
			FastLED.delay(50);
		}
	}
}
