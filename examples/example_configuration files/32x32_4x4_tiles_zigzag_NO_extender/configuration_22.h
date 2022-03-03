
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
#define FASTLED_TEENSY4 //defined for DATA_RATE_MHZ() and FAST_SPI in fastSPI_ARM_MXRT1062.h
#define TEENSY_TRANS    //This enables SPI Tansactions if using SPI pins for output DATA and CLOCK

//======================== set up physical LED type, number ========================= 
//NOTE: NEOPIXEL is not recognized. USE THE ACTUAL LED TYPE because NEOPIXEL can be WS2811, WS2812, or WS282B.
#define CHIPSET             APA102   //TX1813	//WS2812, APA102 //see FastLED docs or examples for list
#define CLOCK_PIN_REQUIRED  true  //Does this LED need DATA + CLOCK??


//Is DATA_RATE_MHZ(SPI_MHZ) used w/ 2-wire leds requires a speed defined constant - AFFECTS SPEED EVEN IF SPI PINS NOT USED 
//	Example: APA102 is up to 24Mhz (predicted only) */
#define SPI_MHZ         24  //Too high a value causes individual led white flashes (sparkles)

#define COLOR_ORDER GRB		//Set the color order. Most 1-wire types like WS2812B are GRB.
#define CORRECTION  UncorrectedColor    //setCorrection type - see the FastLED manual or FastLED keywords.txt.
#define BRIGHTNESS  10  //1-255 CAUTION: Limit this. HIGH brightness can cause pixel breakup, and draws more current.

//set these optional parameters as needed or comment out
#define VOLTS 5
#define MAXIMUM_AMPS 50

//============ set up physical LED arrangement in overall matrix then blocks within the matrix ============= 
/*
	Set the overall Panel size in number of LEDs (POSITIVE VALUES ONLY).
    Previous LEDMatrix versions use a negative value for reserved (right to left)
    and (bottom to top). Use HORIZ_DIR and VERT_DIR below to do this.
*/
#define MATRIX_WIDTH    32   //former LEDMatrix use negative value for reversed (right to left)
#define MATRIX_HEIGHT   32  //former LEDMatrix use negative value for reversed (bottom to top)
#define NUM_LEDS        MATRIX_WIDTH * MATRIX_HEIGHT 	//the total number of LEDs in your display calculated

//if this is a simple matrix (no tiles/blocks), then define the flow of the led strip(s), otherwise ignore
#define MATRIX_TYPE     HORIZONTAL_MATRIX     //HORIZONTAL_MATRIX, VERTICAL_MATRIX, 
                                                   //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX };

//what direction does the FIRST row of LEDs in the matrix panel go (not within the tiles if you have them)? 
#define HORIZ_DIR      LEFT_2_RIGHT    //LEFT_2_RIGHT, RIGHT_2_LEFT
#define VERT_DIR       TOP_DOWN        //BOTTOM_UP, TOP_DOWN

//================== Select the data or data+clock pins =========================
/*   
If you are NOT using the LEDS Extender Shields, but want to use up to 4 separate led strips, 
set HAS_EXTENDER (below) to true, set the Banks = 1, and the NUM_STRIPS to your strips. 
Be sure to assign the DATA or DATA/CLOCK  pins correctly. Teensy boards limit the useable pins for 1-wire led strips. 

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
    #define DATA_3        10		  //	17			10				4
    #define DATA_4        26	      //	20			26				16
#endif

//================== tiles/blocks in the matrix panel =========================
/*
 1. Is your matrix panel made up of blocks/cells of LEDs? 
    If yes, set HAS_BLOCKS to true and ignore  this section.
 2. Are you using your own x,y lookup table? 
    If yes, enable XYTable_LookUp in LEDMatric_22.h and ignore this section.
 3. If NOT 1 or 2, use this section to describe your tile layout in you martix/panel.
    LEDMatrix will calculate the x,y offsets.
*/
#define HAS_BLOCKS  true
#if HAS_BLOCKS

    #define MATRIX_TILE_WIDTH   8               // width of EACH MATRIX "cell" (not total display)
    #define MATRIX_TILE_HEIGHT  8               // height of each matrix "cell" 
    #define MATRIX_TILE_H       4              // number of matrices arranged horizontally (positive value only)
    #define MATRIX_TILE_V       4             // number of matrices arranged vertically (positive value only)
    #define LEDS_IN_TILE        HORIZONTAL_ZIGZAG_MATRIX    //LED sequence within each tile:
                                                            //HORIZONTAL_MATRIX, VERTICAL_MATRIX,
                                                            //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX
    #define TILES_IN_MATRIX     HORIZONTAL_BLOCKS   //sequence of tiles in the entire panel
                                                            //HORIZONTAL_BLOCKS, VERTICAL_BLOCKS,
                                                            //HORIZONTAL_ZIGZAG_BLOCKS, VERTICAL_ZIGZAG_BLOCKS
    /*
        what direction does the FIRST row of LEDs in each individual tile go?
    */
    #define LEDS_HORIZ_DIR      LEFT_2_RIGHT    //LEFT_2_RIGHT, RIGHT_2_LEFT
    #define LEDS_VERT_DIR       TOP_DOWN        //BOTTOM_UP, TOP_DOWN
#endif
    //================================= end of Tiles/Blocks ==============================


//================================= end of USER DATA for Extender Control ==============================

//house keeping if no Extender present - set all to 1 with no Bank pins.
#if ! HAS_EXTENDER
    #define NUM_BANKS           1
    #define STRIPS_PER_BANK     1
    #define NUM_STRIPS          1
    #define LEDS_PER_BANK       NUM_LEDS/NUM_BANKS
    #define LEDS_PER_STRIP      LEDS_PER_BANK / STRIPS_PER_BANK 
    #define BANK_PIN_0          -1
    #define BANK_PIN_1          -1
    #define BANK_PIN_2          -1
    #define BANK_PIN_3          -1
#endif      // HAS_EXTENDER false

//housekeeping for matrix - need to set these 2 as NEGATIVE for FastLEDS cLEDMatrix if direction is "reversed"
#define LEFT_2_RIGHT true
#define RIGHT_2_LEFT false
#define BOTTOM_UP   true
#define TOP_DOWN    false
#if HORIZ_DIR == RIGHT_2_LEFT
    #define MATRIX_WIDTH_DIR    -(MATRIX_WIDTH)       //right to left requires negative defined contant value for LEDMatrix
#else
    #define MATRIX_WIDTH_DIR    MATRIX_WIDTH
#endif
#if (VERT_DIR == BOTTOM_UP)
    #define MATRIX_HEIGHT_DIR   -(MATRIX_HEIGHT)      //bottom up requires negative defined contant value for LEDMatrix
#else
    #define MATRIX_HEIGHT_DIR   MATRIX_HEIGHT
#endif

//need to set these 2 as NEGATIVE for cLEDMatrix if direction is "reversed"
#if LEDS_HORIZ_DIR == RIGHT_2_LEFT
#define MATRIX_TILE_H_DIR    -(MATRIX_TILE_H)       //right to left requires negative defined contant value for LEDMatrix
#else
#define MATRIX_TILE_H_DIR    MATRIX_TILE_H
#endif
#if (LEDS_VERT_DIR == BOTTOM_UP)
#define MATRIX_TILE_V_DIR   -(MATRIX_TILE_V)      //bottom up requires negative defined contant value for LEDMatrix
#else
#define MATRIX_TILE_V_DIR   MATRIX_TILE_V
#endif
//================== end of house keeping =====================
