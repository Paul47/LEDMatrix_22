
## XYTable[] array generator

**Intro**
This Arduino sketch will create an x,y lookup table for LED projects instead of writing and using mapping code. It uses LEDMatrix definitions (ex: HORIZONTAL_ZIGZAG_MATRIX) to define the LED mapping. 

The LED mapping apps I have found all have shortcoming on the size or layout of the matrix. Especially for tiles within the matrix like the popular 8x8 tiles. This sketch includes:

Up to 32k LEDs
Small to very large matrices – laid out in any direction with or w/o zigzag
Matrix can be made of tiles of any size that of any size– laid out in any direction with or w/o zigzag in the tile and tile layout within the matrix.
Produces a report on the Serial Terminal of the specified configuration and the resulting mapping array.
 Simple cut and paste into your header file.
Arduino code in small single purpose functions that are easy to modify

LEDMatrix reference:
https://jorgen-vikinggod.github.io/LEDMatrix has an excellent tutorial on mapping definitions.

__OUTPUT:__ will be on the serial terminal so you can review the results and make changes.
__TO SAVE:__ copy and paste the terminal output array to a file.

**Parameter List Guide**

================= for width and height ==========================

 Do you have an LED array to start with or an IRREGULAR layout? You can #include it in step 1.
If you have an irregular array, you should 1st create the array with missing LEDs here: 
FastLED XY Map Generator:  https:macetech.github.io/FastLED-XY-Map-Generator

    NOTE: make the array simple, left to right and top to bottom, no zigzag.
    We will make all these changes here.

WHEN DESIGNING TILES IN A MATRIX, COMBINATIONS CAN GIVE SURPRISING RESULTS.
IF THE RESULT IS NOT WHAT YOU EXPECTS, REVIEW EACH STEP'S SETTINGS AGAIN!

**STEPS**
1.	 array name must be: "const uint16_t  PROGMEM XYTable[][x] = {}" or:  
"const uint16_t XYTable[][x] = {}" The [x] length is required.
PROGMEM is optional. Set PROG_MEM below as true or false to match
 
    #define HAVE_ARRAY false
    #if HAVE_ARRAY
         #include "XYTable_LookUp.h"   		 edit your file name here  
    #endif
2.	Set the overall Panel size in number of LEDs. 
What direction does the FIRST row go?
What direction does the FIRST column go?
LEDMatrix uses a NEGATIVE value for reserved (right to left) and (bottom to top).
| Parameter    | Description                                   |
| ------------ |-----------------------------------------------|
| MATRIX_WIDTH | width of matrix (negative for reverse order)  |
| MATRIX_HEIGHT| height of matrix (negative for reverse order)|

    #define MATRIX_WIDTH 	 32    //use negative value for reversed (right to left)
    #define MATRIX_HEIGHT 	32     //use negative value for reversed (bottom to top)

3.	How are the LEDs in the total matrix organized? (Regardless of and tiles making up the matrix).

>>>>> If using TILEs to make up your matrix, this is n/a. See steps 6 and 7

    MatrixType_t matrix_type = HORIZONTAL_MATRIX; 
    Options: HORIZONTAL_MATRIX, VERTICAL_MATRIX, HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX


**================= TILES options =================**

4.	 LEDs make a TILE, TILES make up a MATRIX (panel). if you have one long LED string in your display set HAS_TILE false and ignore these TILE values

    #define HAS_TILES  true    		 Is this matrix made up of tiles of LEDs?
    #define MATRIX_TILE_WIDTH  8  		width of each matrix TILE  (not total display)
    #define MATRIX_TILE_HEIGHT  8 		height of each matrix TILE 

    #define MATRIX_TILE_H    4  	 the number of tiles arranged horizontally
    #define MATRIX_TILE_V    4   	the number of tiles arranged vertically

5.	 How are the LEDs organized inside the tile?

    TileType_t tileOrg = HORIZONTAL_TILES;  
    Options: HORIZONTAL_TILES, VERTICAL_TILES, HORIZONTAL_ZIGZAG_TILES, VERTICAL_ZIGZAG_TILES

6.	 How are the tiles organized in the matrix?

    TileType_t tilesInMatrix = HORIZONTAL_TILES;  
    Options:  HORIZONTAL_TILES, VERTICAL_TILES, HORIZONTAL_ZIGZAG_TILES, VERTICAL_ZIGZAG_TILES

7.	NEW OPTION NOT AVAILABLE IN LEDMatrix ONLY IF USING THIS TABLE LOOKUP METHOD
    These 2 flip the order of the tiles/TILES in the matrix
    The LED order inside the tiles stay the same
    To flip everything in the matrix panel including the LEDs inside the tiles see step 3.
| Parameter    | Description                                   |
| ------------ |-----------------------------------------------|
| H_tileDir   | horizontal direction of led flow in the tile  |
| V_tileDir   | vertical direction of led flow in the tile    |

    MatrixOrder_horizDir H_tileDir = LEFT_2_RIGHT;       Options: LEFT_2_RIGHT, RIGHT_2_LEFT
    MatrixOrder_vertDir V_tileDir = TOP_DOWN;             Options: TOP_DOWN, BOTTOM_UP

8.	DEBUGGING: Add h and v hash marks between tiles for easier viewing you can delete in an editor after copying.

    #define TABLE_DIVIDERS true 		Options: true, false


**================= End TILES =================**

**9.	DONE - compile and run**
