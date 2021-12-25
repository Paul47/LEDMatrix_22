
//configuration_22.h

/* --------------LEDMatrix_22 -------------------------
  LEDMatrix_22 updated previous by DrOldies 2021
  modified:  LEDMatrix V5 class by Aaron Liddiment (c) 2016
  modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
  date:      2016/04/27
*/

#pragma once


//#define FASTLED_TEENSY3 //no teensy4 enabled for DATA_RATE_MHZ()
#define FASTLED_TEENSY4 //defined for DATA_RATE_MHZ() and FAST_SPI in fastSPI_ARM_MXRT1062.h
//#define FASTLED_TEENSY3
#define TEENSY_TRANS    //This enables SPI Tansactions if using SPI pins for output DATA and CLOCK
#define FRAMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000/x)   //useful for controlling display updating

/* If XYTable_LookUp is defined below, use an external table named XYTable_LookUp.h,
in the library folder to map the LEDs in XYTable[y][x]
instead of calculating with mXY(x,y). The table is stored in PROGMEM.
*/
//#define XYTable_LookUp

/*
* Enable fonts here
* If true,  "gfxfont.h" and "glcdfont.c" will be included - must be in the library folder
*     and all character functions will be enabled
*/
#define ENABLE_FONTS true       //true/false


//======================== set up physical LED type, number ========================= 
#define COLOR_ORDER BGR
#define CHIPSET     APA102               //see FastLED docs or examples for list
#define CORRECTION  TypicalSMD5050      //TypicalSMD5050 //setCorrection type
                                        // TypicalSMD5050, TypicalLEDStrip,
                                        // Typical8mmPixel, TypicalPixelStrinp

//DATA_RATE_MHZ - APA102 is up to 24Mhz predicted only - WORKS EVEN IF SPI PINS NOT USED
#define SPI_MHZ         8   //8 working: 6. at 8 see individual led white flashes (sparkles)
//set Max Refresh Rate set in setup() after size set
#define refresh_fps     30  //working: 10-60    //default in chip defines is 400
#define BRIGHTNESS      10  //10  //working: 20 to 40. ??? with 2 power supplies up to 100


//============ set up physical LED arrangement in overall matrix then blocks within the matrix ============= 
/*
	Set the overall Panel size in number of LEDs (POSITIVE VALUES ONLY).
    Previous LEDMatrix versions use a negative value for reserved (right to left)
    and (bottom to top). Use HORIZ_DIR and VERT_DIR below to do this.
*/
#define MATRIX_WIDTH    16    //former LEDMatrix use negative value for reversed (right to left)
#define MATRIX_HEIGHT   16    //former LEDMatrix use negative value for reversed (bottom to top)

//if this is a simple matrix (no tiles/blocks), then define the flow of the led strip(s), otherwise ignore
#define MATRIX_TYPE     HORIZONTAL_MATRIX     //HORIZONTAL_MATRIX, VERTICAL_MATRIX, 
                                                    //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX };

//what direction does the FIRST row of LEDs in the matrix panel go? 
#define HORIZ_DIR      LEFT_2_RIGHT    //LEFT_2_RIGHT, RIGHT_2_LEFT
#define VERT_DIR       TOP_DOWN        //BOTTOM_UP, TOP_DOWN

//For a simple matrix use the SPI pins available, otherwise these are ignored
#define DATA_1    11
#define CLOCK_1   13

//the total number of LEDs in your display calculated
#define NUM_LEDS        MATRIX_WIDTH * MATRIX_HEIGHT 

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

#define MATRIX_TILE_WIDTH   8 // width of EACH MATRIX "cell" (not total display)
#define MATRIX_TILE_HEIGHT  8 // height of each matrix "cell" 
#define MATRIX_TILE_H       2  // number of matrices arranged horizontally (positive value only)
#define MATRIX_TILE_V       2  // number of matrices arranged vertically (positive value only)
#define LEDS_IN_TILE        HORIZONTAL_ZIGZAG_MATRIX    //HORIZONTAL_MATRIX, VERTICAL_MATRIX,
                                                        //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX
#define TILES_IN_MATRIX     HORIZONTAL_BLOCKS   //HORIZONTAL_BLOCKS, VERTICAL_BLOCKS,
                                                    //HORIZONTAL_ZIGZAG_BLOCKS, VERTICAL_ZIGZAG_BLOCKS
/*
    what direction does the FIRST row of LEDs in the tile go?
*/
#define LEDS_HORIZ_DIR      LEFT_2_RIGHT    //LEFT_2_RIGHT, RIGHT_2_LEFT
#define LEDS_VERT_DIR       TOP_DOWN        //BOTTOM_UP, TOP_DOWN
#endif
//================================= end of Tiles/Blocks ==============================

//================================= end of user input ==============================

//need to set these 2 as NEGATIVE for cLEDMatrix if direction is "reversed"
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

