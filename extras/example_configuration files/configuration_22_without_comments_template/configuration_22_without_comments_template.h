//configuration_22.h

/* --------------LEDMatrix_22 -------------------------
  LEDMatrix_22 updated previous by DrOldies 2021
  modified:  LEDMatrix V5 class by Aaron Liddiment (c) 2016
  modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
  date:      2016/04/27
*/

#pragma once

//Add aliases for new led types (not included in FastLED that you find work with existing FastLED types
#define TX1813		TM1829

//#define XYTable_LookUp

#define ENABLE_FONTS true       //true/false

//#define FASTLED_TEENSY3 //no teensy4 enabled for DATA_RATE_MHZ()
//#define FASTLED_TEENSY4 //defined for DATA_RATE_MHZ() and FAST_SPI in fastSPI_ARM_MXRT1062.h
//#define TEENSY_TRANS    //This enables SPI Tansactions if using SPI pins for output DATA and CLOCK
#define SPI_MHZ         12  //Too high a value causes individual led white flashes (sparkles)

//NOTE: NEOPIXEL is not recognized. USE THE ACTUAL LED TYPE because NEOPIXEL can be WS2811, WS2812, or WS282B.
#define CHIPSET             APA102  //TX1813	//WS2812, APA102 //see FastLED docs or examples for list
#define CLOCK_PIN_REQUIRED  true //Does this LED need DATA + CLOCK??
#define COLOR_ORDER BGR //GRB		//Set the color order. Most 1-wire types like WS2812B are GRB.
#define CORRECTION  UncorrectedColor    //setCorrection type - see the FastLED manual or FastLED keywords.txt.
#define BRIGHTNESS  20  //1-255 CAUTION: Limit this. HIGH brightness can cause pixel breakup, and draws more current.

#define VOLTS 5
#define MAXIMUM_AMPS 50

//#define HAS_TILES  true        //true/false for LED tiles

//#define HAS_BANKS  true    //true/false for more than one led strip (or Banks my LED Extender shields)

#define MATRIX_WIDTH    16    //former LEDMatrix use negative value for reversed (right to left)
#define MATRIX_HEIGHT   16  //former LEDMatrix use negative value for reversed (bottom to top)
#define NUM_LEDS        MATRIX_WIDTH * MATRIX_HEIGHT 	//the total number of LEDs in your display calculated

#define MATRIX_TYPE     HORIZONTAL_MATRIX          //HORIZONTAL_MATRIX, VERTICAL_MATRIX, 
    #define HORIZ_DIR     LEFT_2_RIGHT //LEFT_2_RIGHT, RIGHT_2_LEFT
    #define VERT_DIR      TOP_DOWN    //BOTTOM_UP, TOP_DOWN
	
#if CLOCK_PIN_REQUIRED     // 2-wire pin selection 
    //Select your DATA/CLOCK pins - if using the Extender Shield pin selections are limted
                                    //depends on how Teensy is rotated on the Extender board
										//Teensy3/4 with Extender	ESP32(typical)	
    #define DATA_1          1			//only 1 or 14 					2
    #define CLOCK_1         2			//only 2 or 15					0
    #define DATA_2          3			//only 3 or 16					4
    #define CLOCK_2         4			//only 4 or 17					16
#else   //1-wire DATA only. Teensy pins are limted to just a few
									  //Teensy4x	Teensy 3.5/3.6	ESP32(typical)
    #define DATA_1        1		      //	1     		1				2 
    #define DATA_2        8		      //	8			8				0
    #define DATA_3        17		  //	17			10				4
    #define DATA_4        20          //	20			26				16
#endif

#if HAS_TILES
    #define MATRIX_TILE_WIDTH   8               			// width of EACH MATRIX "tile" (not total display)
    #define MATRIX_TILE_HEIGHT  8               			// height of each matrix "tile" 
    #define MATRIX_TILE_H       2               			// number of matrices arranged horizontally (positive value only)
    #define MATRIX_TILE_V       2              				// number of matrices arranged vertically (positive value only)
    #define LEDS_IN_TILE        HORIZONTAL_ZIGZAG_MATRIX           //LED sequence within each tile:
    #define LEDS_HORIZ_DIR      LEFT_2_RIGHT  				//LEFT_2_RIGHT, RIGHT_2_LEFT
    #define LEDS_VERT_DIR       TOP_DOWN      			    //BOTTOM_UP, TOP_DOWN

    #define TILES_IN_MATRIX     HORIZONTAL_TILES         //sequence of tiles in the entire panel
#endif  //HAS_TILES
#if  HAS_BANKS
    #define NUM_BANKS           1       // 1 to 4 extender "banks"
    #define STRIPS_PER_BANK     2      //1 or more but 4 strips per Bank is the most efficient use of the hardware
#if CLOCK_PIN_REQUIRED  //2-wire		Teensy	ESP32(typical)	
    #define BANK_PIN_0          5       //	5	17
    #define BANK_PIN_1          6       //	6	5
    #define BANK_PIN_2          7       //	7	18
    #define BANK_PIN_3          8       //	8	19
#else                   //1-wire		Teensy	ESP32(typical)
    #define BANK_PIN_0          3  	    //	3   	17
    #define BANK_PIN_1          4       //	4		4
    #define BANK_PIN_2          5       //	5		5
    #define BANK_PIN_3          6       //	6		6
#endif

	#define NUM_STRIPS      STRIPS_PER_BANK * NUM_BANKS     //total number of strips used
    #define LEDS_PER_BANK       NUM_LEDS/NUM_BANKS      //equally split the total number of leds across the number of active Banks 
    #define LEDS_PER_STRIP      LEDS_PER_BANK / STRIPS_PER_BANK      //equally split the number of leds in each banks into the number of strips in each Bank
#endif  //HAS_BANKS true

//================================= end of USER DATA for Banks & Extender Control ==============================
