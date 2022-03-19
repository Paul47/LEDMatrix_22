/* This example implements Conway's Game of Life on an
 *
 * Copyright (c) 2015-2017 Pololu Corporation.  For more information, see
 * http://www.pololu.com/
 * http://forum.pololu.com/
 *
 * support Pololu by frequenting their website
  *
 * modified for the LEDMatrix_22 library 2022
 * For more information about the Game of Life, see:
 *
 *     https://en.wikipedia.org/wiki/Game_of_Life
 *
 * The top of this file defines several settings which you can
 * edit.  Be sure to edit the width and height variables UP TO
 * your LED panel size.  
 * 
 * width and height are NOT necessarily the size of your led panel.
 * They are the bounds for the GofL environment and can be any size
 * 8x8 to the full size of your panel MATRIX_WIDTH x MATRIX_HEIGHT.
 *
 * By default, this example initializes each cell in the game to
 * a random state.  There are some other options for the initial
 * state of the game that you can use by editing the setup()
 * function. 
 * 
 * setupGlider(4, 3);              //try if panel is only 8x8. (x, y) is the starting location
 * setupLightweightSpaceship(4, 3);  //not very good, usually only 1 cell. (x, y) is the starting location
 * setupFromXBM();                   //BEST - good for larger panels 16x16 and above. cells spreadout quickly
 * setupRandom();                //use only if panel is larger than say 24 x 24 and above
 */

/* This example is meant for controlling large numbers of LEDs,
 * so it requires the FastGPIO library.  If you cannot use the
 * FastGPIO library, you can comment out the two lines below and
 * the example will still work, but it will be slow. */

//useful for debugging
#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.print MACRO

/* Configuring this library and FastLED requires a number of parameters and #defines
Configure your LED array in myConfiguration.h located in the  library folder.
This way you can reuse your configuration file(s) across all your sketches.
Consider renaming your configurations (and changing the #include "myConfiguration.h"
in the library folder.
*/

#include <LEDMatrix_22.h>

/*
----------- Set the size of the environment ---------------
*/

// Set the size of the LED panel (and the game).
const uint8_t width = MATRIX_WIDTH;   // up to MATRIX_WIDTH;
const uint8_t height = MATRIX_HEIGHT;  // up to MATRIX_HEIGHT;

const uint8_t analogRandomSeedPin = A0;

// Set whether the left and right edges are considered to
// border eachother.
const bool wraparoundX = true;

// Set whether the top and bottom edges are considered to border
// eachother.
const bool wraparoundY = true;

// Determines whether we display the age of the live cells using
// different colors.  If false, all live cells are the same
// color.
const bool showAge = true;

// Set the brightness to use (the maximum is 31).
//const uint8_t brightness = 1;

// Set how long to display each frame (in milliseconds).
#define FRAME_TIME  500

//To recycle periodically, set the time to restart
//(in milliseconds so set very high). Otherwise comment out in loop()
#define RESET_TIME 100000   //example 10 minutes is 10,000

// Create an object for writing to the LED strip.

const uint16_t cellCount = NUM_LEDS;

// Make a buffer for the game state.  Each cell gets one byte.
// Bit 7: 1 if the cell is alive currently.
// Bit 6: Temporarily 1 if the cell will be alive
//        in the next frame.
// Bits 0-5: the age of the cell, if alive currently,
//    capped at 64.  Otherwise, these will be 0.

uint8_t gameState[cellCount] = {0};

#define STATE_ALIVE       0x80
#define STATE_ALIVE_NEXT  0x40
#define STATE_DEAD        0x00
#define STATE_AGE_MASK    0x3F

/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_BLOCKS and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pt("start");

    leds.addLeds();       //initiate LEDMatrix
    leds.clear();

    // You can comment out and uncomment one of the other
    // calls if you want to see a different pattern used as the
    // initial state.

     //setupGlider(width/2, height/2);              //try if panel is only 8x8. (x, y) is the starting location
     // setupLightweightSpaceship(4, 3);  //not very good, usually only 1 cell. (x, y) is the starting location
    //setupFromXBM();                   //BEST - good for larger panels 16x16 and above. cells spreadout quickly
    setupRandom();                //use only if panel is larger than say 24 x 24 and above

    writeGameStateToLeds();
    //delay(2000);            //look at screen
}

void loop()
{

    EVERY_N_MILLISECONDS(FRAME_TIME) {
        updateGameState();
        writeGameStateToLeds();
    }

    EVERY_N_MILLISECONDS(RESET_TIME) {
        setupRandom();
    }

}

// Returns a reference to the byte representing the state of a
// specific cell.  If x or y are invalid, returns a reference to
// a dummy byte that is set to 0 (not alive).

uint8_t& cellState(uint8_t x, uint8_t y)           //his modifys X and Y for wraping, then does mXY
{
    if (wraparoundX)                          ///<<<<<<<<< 255 flag to wrap
    {
        if (x == 0xFF) { x = width - 1; }        //??????? x wraps at 0xff >>>> 255 ??????????
        if (x == width) { x = 0; }
    }

    if (wraparoundY)
    {
        if (y == 0xFF) { y = height - 1; }       //??????? y wraps at 0xff >>>> 255 ??????????
        if (y == height) { y = 0; }
    }

    if (x >= width || y >= height)
    {
        static uint8_t dummy;                       //just return zero!
        dummy = 0;
        return dummy;
    }

    return gameState[leds.mXY(x,y)];             // MAPPING gameState is the CONTENTS at gameState[x,y] old gameState[y * width + x]; 
}
void setupRandom()
{
  randomSeed(analogRead(analogRandomSeedPin));      // see A0 above
  for (uint16_t i = 0; i < cellCount; i++)
  {
    gameState[i] = random(2) ? STATE_ALIVE : STATE_DEAD;
  }
}

// Sets up a glider that will travel in the positive x, positive
// y direction, with its nose starting at the specified
// coordinates.
void setupGlider(uint8_t x, uint8_t y)
{
  cellState(x, y) = STATE_ALIVE;
  cellState(x - 1, y) = STATE_ALIVE;
  cellState(x - 2, y) = STATE_ALIVE;
  cellState(x, y - 1) = STATE_ALIVE;
  cellState(x - 1, y - 2) = STATE_ALIVE;
}

// Sets up a light-weight space ship that will travel in the
// positive x direction, with its nose starting at the specified
// coordinates.
void setupLightweightSpaceship(uint8_t x, uint8_t y)
{
  cellState(x, y) = STATE_ALIVE;
  cellState(x - 1, y) = STATE_ALIVE;
  cellState(x - 2, y) = STATE_ALIVE;
  cellState(x - 3, y) = STATE_ALIVE;
  cellState(x - 4, y + 1) = STATE_ALIVE;
  cellState(x - 4, y + 3) = STATE_ALIVE;
  cellState(x - 1, y + 3) = STATE_ALIVE;
  cellState(x, y + 2) = STATE_ALIVE;
  cellState(x, y + 1) = STATE_ALIVE;
}

/* Include an XBM image.  XBM is an image format that is compatible
 * with C/C++ code and can store monochrome bitmaps.  This is
 * used by setupFromXBM().  The default image is an R pentomino, but
 * you can use an image editor such as GIMP to save a new image. */
#include "start.cpp"

/* Initializes the game state using an XBM image.  Uses
 * start_height, start_width, and start_bits, which are defined
 * in start.xbm, which is included above.  Places the image in
 * the upper left corner of the LED panel.  It is OK if the image
 * is not the same size as the LED panel. */
void setupFromXBM()
{
  const uint8_t bytesPerRow = (start_width + 7) / 8;
  for (uint8_t y = 0; y < start_height; y++)
  {
    for (uint8_t x = 0; x < start_width; x++)
    {
      if (start_bits[bytesPerRow * y + (x / 8)] >> (x % 8) & 1)
      {
        cellState(x, y) = STATE_ALIVE;
      }
      else
      {
        cellState(x, y) = STATE_DEAD;
      }
    }
  }
}


// Writes a specific cell to the LED panel.
void writeCellToLed(uint8_t cellState, uint8_t x2, uint8_t y2)        //ADDED x,y for mled
{
    int16_t ledIndex = leds.mXY(x2, y2);                                //ADDED x,y for mled

  if (cellState & STATE_ALIVE)
  {
    // The cell is alive.  Send a color that depends on its age.
    uint8_t age = cellState & STATE_AGE_MASK;

    if (age == 0 || !showAge)
    {
     // ledStrip.sendColor(0x00, 0x00, 0xFF, brightness); 
      leds.cLED[ledIndex].r = 0x00;
      leds.cLED[ledIndex].g = 0x00;
      leds.cLED[ledIndex].b = 0xFF;

    }
    else if (age == 1)
    {
      //ledStrip.sendColor(0x40, 0x00, 0xFF, brightness);
      leds.cLED[ledIndex].r = 0x40;
      leds.cLED[ledIndex].g = 0x00;
      leds.cLED[ledIndex].b = 0xFF;
    }
    else if (age == 2)
    {
     // ledStrip.sendColor(0xFF, 0x00, 0xFF, brightness);
      leds.cLED[ledIndex].r = 0xFF;
      leds.cLED[ledIndex].g = 0x00;
      leds.cLED[ledIndex].b = 0xFF;
    }
    else if (age == 3)
    {
     // ledStrip.sendColor(0xFF, 0x00, 0x40, brightness);
      leds.cLED[ledIndex].r = 0xFF;
      leds.cLED[ledIndex].g = 0x00;
      leds.cLED[ledIndex].b = 0x40;
    }
    else
    {
      // Fade from red to green as age goes from 4 to 63.
      uint8_t phase = (age - 4) * 255 / (63 - 4);
     // ledStrip.sendColor(0xFF - phase, phase, 0x00, brightness);  
      leds.cLED[ledIndex].r = 0xFF - phase;
      leds.cLED[ledIndex].g = phase;
      leds.cLED[ledIndex].b = 0x00;
      
    }
  }
  else
  {
    // The cell is dead.
    //ledStrip.sendColor(0, 0, 0, brightness);
    leds.cLED[ledIndex].r = 0;
    leds.cLED[ledIndex].g = 0;
    leds.cLED[ledIndex].b = 0;
  }
}

// Writes the overall state of the game to the LED panel.
void writeGameStateToLeds()
{
 // ledStrip.startFrame();
  for (uint8_t y = 0; y < height; y++)
  {
    if (y & 1)
    {
      // For odd rows, write the row without reversing it.
      for (uint8_t x = 0; x < width; x++)
      {
          writeCellToLed(cellState(x, y), x, y);
      }
    }
    else
    {
      // For even rows, write the row in reverse order because
      // the LED panel uses a serpentine layout.
      for (uint8_t x = 0; x < width; x++)
      {
          writeCellToLed(cellState(width - 1 - x, y), width - 1 - x, y);
      }
    }
  }
  leds.show();               //display ALL
//  ledStrip.endFrame(cellCount);
}

void updateGameState()
{
  // In this first pass, we update bit 6 of each cell, setting it
  // to 1 if the cell will be alive in the next frame and 0 if
  // the cell will be dead.  We also update the age for surviving
  // live cells.
  for (uint8_t y = 0; y < height; y++)
  {
    for (uint8_t x = 0; x < width; x++)
    {
      uint8_t neighborCount =
        (cellState(x - 1, y - 1) >> 7) +
        (cellState(x - 1, y + 0) >> 7) +
        (cellState(x - 1, y + 1) >> 7) +
        (cellState(x + 0, y - 1) >> 7) +
        (cellState(x + 0, y + 1) >> 7) +
        (cellState(x + 1, y - 1) >> 7) +
        (cellState(x + 1, y + 0) >> 7) +
        (cellState(x + 1, y + 1) >> 7);

      if (cellState(x, y) & STATE_ALIVE)
      {
        // The cell is alive.
        if (neighborCount < 2 || neighborCount > 3)
        {
          // The cell dies on the next frame
          // (clear the STATE_ALIVE_NEXT bit and the age).
          cellState(x, y) = STATE_ALIVE;
        }
        else
        {
          // The cell survives to the next generation.
          // Increment its age and record survival.
          uint8_t age = cellState(x, y) & STATE_AGE_MASK;
          if (age < STATE_AGE_MASK) { age += 1; }
          cellState(x, y) = STATE_ALIVE | STATE_ALIVE_NEXT | age;
        }
      }
      else
      {
        // The cell is dead.
        if (neighborCount == 3)
        {
          // The cell will be born on the next frame.
          cellState(x, y) = STATE_ALIVE_NEXT;
        }
        else
        {
          // The cell remains dead.
          cellState(x, y) = STATE_DEAD;
        }
      }
    }
  }

  // In this second pass, we update the actual liveness state
  // (in bit 7) by copying bit 6 to bit 7.
  for (uint16_t i = 0; i < cellCount; i++)
  {
    if (gameState[i] & STATE_ALIVE_NEXT)
    {
      gameState[i] |= STATE_ALIVE;
    }
    else
    {
      gameState[i] &= ~STATE_ALIVE;
    }
  }
}



/* ------------------ fumction mapping -----------------------
oid setup
  setup();				one of several options to seed the array


void loop()
  writeGameStateToLeds();
 	void writeGameStateToLeds()						Loop through entire array
		void writeCellToLed	...						Use current cell status/state...
		  array = (cellState(x, y),x ,y) 	...converting x,y to array index
			leds.cLED[leds.mXY(x, y)].r,g,b 				...To choose led color for each pixel in the array
		leds.show()								Now display state of the game to the LED panel
		
	updateGameState();					for entire array update the cell status/state array for all neighbors
		array index = cellState(x, y)	return array index w/ or w/o wrapping as x or y = 255
*/
