
//configuration_22.h

/* --------------LEDMatrix_22 -------------------------
  LEDMatrix_22 updated previous by DrOldies 2021
  modified:  LEDMatrix V5 class by Aaron Liddiment (c) 2016
  modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
  date:      2016/04/27
*/

#pragma once

//here's a utility so we can use frames-per-second which makes more sense than every_millisecond
#define FRAMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000/x)   //useful for controlling display updating

//Add aliases for new led types (not included in FastLED that you find work with existing FastLED types
#define TX1813		TM1829

/* If XYTable_LookUp is defined below, use an external table named XYTable_LookUp.h,
in the library folder to map the LEDs in XYTable[y][x] instead of 
calculating with mXY(x,y). The table is stored in PROGMEM. For irregular led
arrays, look at the LEDMatrix manual for details.
*/
//#define XYTable_LookUp

/*
* Enable fonts here
* If true,  "gfxfont.h" and "glcdfont.c" will be included - must be in the library folder
*     and all character functions will be enabled
*/
#define ENABLE_FONTS true       //true/false

//====================== If Teensy - MUST define here ============================
//#define FASTLED_TEENSY3 //no teensy4 enabled for DATA_RATE_MHZ()
//#define FASTLED_TEENSY4 //defined for DATA_RATE_MHZ() and FAST_SPI in fastSPI_ARM_MXRT1062.h
//#define TEENSY_TRANS    //This enables SPI Tansactions if using SPI pins for output DATA and CLOCK

//======================== set up physical LED type, number ========================= 
//NOTE: NEOPIXEL is not recognized. USE THE ACTUAL LED TYPE because NEOPIXEL can be WS2811, WS2812, or WS282B.
#define CHIPSET             APA102  //TX1813	//WS2812, APA102 //see FastLED docs or examples for list
#define CLOCK_PIN_REQUIRED  true //Does this LED need DATA + CLOCK??


//Is DATA_RATE_MHZ(SPI_MHZ) used w/ 2-wire leds requires a speed defined constant - AFFECTS SPEED EVEN IF SPI PINS NOT USED 
//	Example: APA102 is up to 24Mhz (predicted only) */
#define SPI_MHZ         8  //Too high a value causes individual led white flashes (sparkles)

#define COLOR_ORDER BGR //GRB		//Set the color order. Most 1-wire types like WS2812B are GRB.
#define CORRECTION  UncorrectedColor    //setCorrection type - see the FastLED manual or FastLED keywords.txt.
#define BRIGHTNESS  50  //1-255 CAUTION: Limit this. HIGH brightness can cause pixel breakup, and draws more current.

//set these optional parameters as needed or comment out
#define VOLTS 5
#define MAXIMUM_AMPS 50

//Is your matrix panel made up of tiles of LEDs such a ADafruit NEOPIXEL 8x8 tiles?
//If true complete Sections #1 and #2

#define HAS_TILES  false        //true/false for LED tiles

//setup number of me LED Extender Shield PCBs and LED "strips" in each bank in Section #3.
//See the documentation for how to use these Extenders to support up to 16 LED strips and thousands of leds.
//If true, complete Sections #1, #2, and #3

#define HAS_EXTENDER  false    //true/false for LED Extender shields 

//Section #1. ======= set up physical LED arrangement in overall matrix then tiles within the matrix ============= 
/*
	Set the overall Panel size in number of LEDs (POSITIVE VALUES ONLY).
    Previous LEDMatrix versions use a negative value for reserved (right to left)
    and (bottom to top). Use HORIZ_DIR and VERT_DIR below to do this.
     MATRIX_TYPE must match your hardware or will get trash on the panel.
*/
#define MATRIX_WIDTH    8    //former LEDMatrix use negative value for reversed (right to left)
#define MATRIX_HEIGHT   8  //former LEDMatrix use negative value for reversed (bottom to top)
#define NUM_LEDS        MATRIX_WIDTH * MATRIX_HEIGHT 	//the total number of LEDs in your display calculated
/*
    Define the flow of the leds in the full led Matrix panel 
    If the panel has tiles, ignore the flow of leds in each tile for now. The tiles are
    organized either horizontally in rows or vertically in columns. If tiled, start with HORIZONTAL_MATRIX.
*/
#define MATRIX_TYPE     HORIZONTAL_MATRIX          //HORIZONTAL_MATRIX, VERTICAL_MATRIX, 
                                                   //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX
/*
    what direction does the FIRST row of LEDs in the MATRIX PANEL flow?
    these may be in one large matrix or the first tile if you are using them
    This is also the entry point of the data signal. Example: top left or bottom right.
*/
#define HORIZ_DIR     LEFT_2_RIGHT   //LEFT_2_RIGHT, RIGHT_2_LEFT
#define VERT_DIR      TOP_DOWN      //BOTTOM_UP, TOP_DOWN

//================== Select the data or data+clock pins =========================
/*   
If you are NOT using the LEDS Extender Shields, but want to use up to 4 separate led strips, 
set HAS_EXTENDER (below) to true, set the Banks = 1, and the NUM_STRIPS to your strips. 
Be sure to assign the DATA or DATA/CLOCK  pins correctly. Teensy boards limit the useable pins for 1-wire led strips. 

If using the hardware Extender Shield choose DATA and CLOCK pins in the bank (all banks use the same pins)
The same data/clock pins are used for all Banks, and made active by the BANK_PIN. 
Alternate pins (14-17) depend on how Teensy is rotated on the Extender board

For 1-wire leds, it appears that only some Teensy pins will work as DATA lines.
Usable pins: 
Teensy LC:   1, 4, 5, 24 
Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8) 
Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48 
Teensy 3.6:  1, 5, 8, 10, 26, 32, 33 
Teensy 4.0:  1, 8, 14, 17, 20, 24, 29, 39
Teensy 4.1:  1, 8, 14, 17, 20, 24, 29, 35, 47, 53
More details are here: https://github.com/PaulStoffregen/WS2812Serial

For 2-wire leds, select your DATA/CLOCK pins
If using the Extender shield pin selections are limited to these:
         Teensy pin
DATA_1   1    or    14**
CLOCK_1  2    or    15**
DATA_2   3    or    16**
CLOCK_2  4    or    17**
**If Teensy is rotated 180 deg on the Extender board

Teensy 4.1 hardware SPI - this MCU has 2 sets of SPI ports.
These may deliver faster display rates.
         Teensy SPI pin
DATA_1   11
CLOCK_1  13
DATA_2   26
CLOCK_2  27
*/

#if CLOCK_PIN_REQUIRED     // 2-wire pin selection 
    //Select your DATA/CLOCK pins - if using the Extender shield pin selections are limted
                                    //depends on how Teensy is rotated on the Extender board
										//Teensy3/4 with Extender	ESP32(typical)	
    #define DATA_1          1			//only 1 or 14 					2
    #define CLOCK_1         2			//only 2 or 15					0
    #define DATA_2          3			//only 3 or 16					4
    #define CLOCK_2         4			//only 4 or 17					16
#else   //1-wire DATA only. Teensy pins are limted to just a few
        //if other MCU change as desired
									  //Teensy4x	Teensy 3.5/3.6	ESP32(typical)
    #define DATA_1        1		      //	1     		1				2 
    #define DATA_2        8		      //	8			8				0
    #define DATA_3        17		  //	17			10				4
    #define DATA_4        20          //	20			26				16
#endif

//Section #2.=========== tiles in the matrix panel =========================
//left out for ths example
//Section #3. ========= setup number of extenders and LED "strips" in each bank =========================
//left out for ths example

//================================= end of USER DATA for Extender Control ==============================
