//XYTable[] array generator

/*  XYTable[] array generator
Intro
This sketch will create a lookup table for LEDMatrix_21 or similar Arduino libraries

OUTPUT: will be on the serial terminal so you can review the results and make changes.
TO SAVE: copy and paste the terminal output array to a file.
*/

#include "XYTable_array_generator.h"

/*
 ================= for width and height ==========================

 Do you have an LED array to start with or an IRREGULAR layout? You can #include it in step 1.
If you have an irregular array, you should 1st create the array with missing LEDs here: 
FastLED XY Map Generator:  https:macetech.github.io/FastLED-XY-Map-Generator

    NOTE: make the array simple, left to right and top to bottom, no zigzag.
    We will make all these changes here.

    WHEN DESIGNING BLOCKS IN A MATRIX, COMBINATIONS CAN GIVE SURPRISING RESULTS.
    IF THE RESULT IS NOT WHAT YOU EXPECTS, REVIEW EACH STEP'S SETTINGS AGAIN!
*/

/*
STEPS
1.	 array name must be: 

    "const uint16_t  PROGMEM XYTable[][x] = {}" 
    or:
    "const uint16_t XYTable[][x] = {}" 
    The [x] length is required.
    PROGMEM is optional. Set PROG_MEM below as true or false to match
 */
#define HAVE_ARRAY false
#if HAVE_ARRAY
     #include "XYTable_LookUp.h"    //edit your file name here  
#endif

/*
2.	Set the overall Panel size in number of LEDs. 
    What direction does the FIRST row go?
    What direction does the FIRST column go?
    LEDMatrix uses a NEGATIVE value for reserved (right to left) 
    and (bottom to top).
| Parameter    | Description                                   |
| ------------ |-----------------------------------------------|
| MATRIX_WIDTH | width of matrix (negative for reverse order)  |
| MATRIX_HEIGHT| height of matrix  (negative for reverse order)|
*/
#define MATRIX_WIDTH  32    //use negative value for reversed (right to left)
#define MATRIX_HEIGHT 32     //use negative value for reversed (bottom to top)

/*
3.	How are the LEDs in the total matrix organized? (Regardless of and blocks 
    or cells making up the matrix).

>>>>> If using BLOCKs/cell to make up your matrix, this is n/a. See steps 6 and 7

*/
MatrixType_t matrix_type = HORIZONTAL_MATRIX;  //HORIZONTAL_MATRIX, VERTICAL_MATRIX, 
                                                       //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX

/* =============== BLOCKS options =================

4.	 LEDs make a BLOCK (cell), BLOCKS make up a MATRIX (panel). 
    if you have one long LED string in your display set HAS_BLOCK false 
    and ignore these BLOCK values
*/
#define HAS_BLOCKS  false     //Is this matrix made up of block/cells of LEDs?
    #define MATRIX_TILE_WIDTH  8 //width of each matrix BLOCK/CELL  (not total display)
    #define MATRIX_TILE_HEIGHT  8 // height of each matrix BLOCK/CELL

    #define MATRIX_TILE_H    4  // number of tiles arranged horizontally
    #define MATRIX_TILE_V    4  // number of tiles arranged vertically

/*
5. How are the leds organized inside the block/cell?
*/
    BlockType_t blockOrg = HORIZONTAL_BLOCKS;   //HORIZONTAL_BLOCKS, VERTICAL_BLOCKS, 
                                                       //HORIZONTAL_ZIGZAG_BLOCKS, VERTICAL_ZIGZAG_BLOCKS

/*
6. how are the block/cells organized in the matrix?
*/
    BlockType_t blocksInMatrix = HORIZONTAL_BLOCKS;   //HORIZONTAL_BLOCKS, VERTICAL_BLOCKS, 
                                                      //HORIZONTAL_ZIGZAG_BLOCKS, VERTICAL_ZIGZAG_BLOCKS
/*
7. NEW OPTION NOT AVAILABLE IN LEDMatrix ONLY IF USING THIS TABLE LOOKUP METHOD
    These 2 flip the order of the tiles/BLOCKS in the matrix
    The LED order inside the blocks stay the same
    To flip everything in the matrix panel including the LEDs inside the blocks see step 3.
| Parameter    | Description                                   |
| ------------ |-----------------------------------------------|
| H_blockDir   | horizontal direction of led flow in the tile  |
| V_blockDir   | vertical direction of led flow in the tile    |
*/
    MatrixOrder_horizDir H_blockDir = LEFT_2_RIGHT;        //leds in tile LEFT_2_RIGHT, RIGHT_2_LEFT
    MatrixOrder_vertDir V_blockDir = TOP_DOWN;             //leds in tile TOP_DOWN, BOTTOM_UP

/*
8. DEBUGGING: Add h and v hash marks between blocks for easier viewing
  you can delete in an editor after copying
*/
#define TABLE_DIVIDERS true 

// ================= End BLOCKS =================

/*
9.     DONE - compile and run
*/

#define VERSION 1   //report version at bottom of printout
boolean error = false;  //CAPTURE A FEW ERRORS
#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.printl MACRO

/*
These flip THE ENTIRE MATRIX even LED order inside of BLOCKS if present
To change the order of BLOCKS see the blocks see step 7.
*/
#if (MATRIX_WIDTH < 0)
    MatrixOrder_horizDir horizDir = RIGHT_2_LEFT;        //LEFT_2_RIGHT, RIGHT_2_LEFT
    #define MATRIX_WIDTH_ABS             -(MATRIX_WIDTH)        //is negative so need postive for arrays
#else
    MatrixOrder_horizDir horizDir = LEFT_2_RIGHT;
    #define MATRIX_WIDTH_ABS             MATRIX_WIDTH
#endif
#if (MATRIX_HEIGHT < 0)
    MatrixOrder_vertDir vertDir = BOTTOM_UP;                //TOP_DOWN, BOTTOM_UP
    #define MATRIX_HEIGHT_ABS            -(MATRIX_HEIGHT)        //is negative
#else
    MatrixOrder_vertDir vertDir = TOP_DOWN;
    #define MATRIX_HEIGHT_ABS            MATRIX_HEIGHT
#endif

#define NUM_LEDS        MATRIX_WIDTH_ABS * MATRIX_HEIGHT_ABS 	//leds total on entire panel
#define LEDS_PER_BLOCK  MATRIX_TILE_WIDTH * MATRIX_TILE_HEIGHT

uint16_t mainArray[MATRIX_WIDTH_ABS][MATRIX_HEIGHT_ABS ];
uint16_t workingArray[MATRIX_WIDTH_ABS][MATRIX_HEIGHT_ABS ];

//configuration flags
boolean _hflag = true;
boolean _vflag = false;

void setup() {
    Serial.begin(115200);
    delay(1000);
    pt("====== XYTable[] Array Generator ======");
    pt("");

    mainProcessor();
}

void loop() {

}


/* ============================ logic ===============================

step 1. load array if #included  = build_sinple_table()
step 2. if not, build the full matrix array top/left to lower right = build_sinple_table()
If a SIMPLE array (w/o BLOCKS)
        step 3. convert row/column structure, and zizzag/not as needed = matrix_reorg()
        step 4. convert to horizontal/vertical direction as needed = matrix_direction()
        steps 5, 6, 7, 8 - skip
If BLOCKS
        step 5. build a block (a cell of LEDs that will be organized into the full matrix)
        step 6, organize leds inside the block/cell
        step 7. organize blocks in the matrix
        step 8. optionally print hash marks seperating blocks

step 9. done, report main array as table
*/

void mainProcessor() {
 
    if (HAS_BLOCKS) {
        if (HAVE_ARRAY) {
            pt("You cannot make a BLOCK array using a custom starting array");
        }
        else {
            build_block_table();    //start from scratch always
            if (error) {
                return;
            }
            blocks_in_matrix();     //fill out rest of matrix
            matrix_block_direction();        //flips block order, no change inside blocks
            matrix_direction();     //flips ENTIRE matrix direction, including LED order inside blocks
        }
    }
    else{
       build_sinple_table();          //continue with entire table
       matrix_reorg();               //turn
       matrix_direction();          //flips matrix directions
     }

    //move working to main
    run_report();       //print to terminal
}

//==============================simple  reorganize entire table =============================

void build_sinple_table() {

#if (HAVE_ARRAY == true)
    uint16_t toX = 0;
    uint16_t toY = 0;
    copy_table_array(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS , toX, toY);        //copy an array block to a starting start x,y offset (for blocks
    pt("your array copied");
#else
    build_horiz_array(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );         //build a ENTIRE simple MAINarray
    pt("new array created");
#endif
}

//simple matrix, just reorg the matrix
void matrix_direction() {  
     pt("matrix direction started");
    switch (horizDir) {
    case LEFT_2_RIGHT: 
        pt("  top left-2-right do nothing");
        break;
    case RIGHT_2_LEFT:
        pt("  right-2-left-H swap");
        H_swap(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
        break;
    }
    switch (vertDir) {
    case TOP_DOWN:
        pt("  top down do nothing");
        break;
    case BOTTOM_UP:
        pt("  bottom up V swap");
        V_swap(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
        break;
    }
    pt("matrix direction completed");
}

//simple matrix, just reorg the DIRECTION matrix
void matrix_reorg() {
     pt("matrix reorg started");
    switch (matrix_type) {
    case HORIZONTAL_MATRIX:
        pt("  horiz matrix-do nothing");
        break;
    case VERTICAL_MATRIX:
        Diagonal_swap(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
        break;
    case HORIZONTAL_ZIGZAG_MATRIX:
        horiz_zig(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
        break;
    case VERTICAL_ZIGZAG_MATRIX:
        if (!_vflag) {
            pt("                         //ppd need flag for current h or v");
            Diagonal_swap(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
        }
        Vertical_zig(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS ); 
        break;
    }
    pt("matrix reorg completed");
}

//=================================== blocks ==========================

// build the leds inside the block(s)
void build_block_table() {
    pt("LEDs in block started");

    if (((MATRIX_TILE_WIDTH * MATRIX_TILE_H) != MATRIX_WIDTH_ABS) ||
        ((MATRIX_TILE_HEIGHT * MATRIX_TILE_V) != MATRIX_HEIGHT_ABS )) {
        pt("ERROR: Matrix tiles do not match matrix sizes - build_block_table");
        error = true;
        return;
    }
    else {
        pt("build horiz leds block");
        horiz_leds_block(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT);
        switch (blockOrg) {
        case HORIZONTAL_BLOCKS:
            //nothing more
            break;
        case VERTICAL_BLOCKS:
            vertical_leds_block(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT);
            break;
        case HORIZONTAL_ZIGZAG_BLOCKS:
            horiz_zigzag_ledsinBlock(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT);
            break;
        case VERTICAL_ZIGZAG_BLOCKS:
            vertrical_zigzag_ledsinBlock(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT);
            break;
        }
        work2main(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
    }
    pt("LEDs in block completed");
}


void horiz_leds_block(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    pt("horiz_leds_block start");
    build_horiz_array(x1, y1, x2, y2);
    pt("horiz_leds_block completed");
}

void vertical_leds_block(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    pt("vertical_leds_block start");
    Diagonal_swap(x1, y1, x2, y2);          
    pt("vertical_leds_block completed");
}

void horiz_zigzag_ledsinBlock(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    pt("horizontal_zigzag_block start");
    horiz_zig(x1, y1, x2, y2);
    pt("horizontal_zigzag_block completed");
}

void vertrical_zigzag_ledsinBlock(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    pt("vertical_zigzag_block start");
    horiz_zig(x1, y1, x2, y2);
    Diagonal_swap(x1, y1, x2, y2);
    pt("vertical_zigzag_block completed");
}

//organize the block inside the matrix
void blocks_in_matrix() {
    pt("blocks in matrix reorg started");
    switch (blocksInMatrix) {
    case HORIZONTAL_BLOCKS:
        h_block_copy();
        break;
    case VERTICAL_BLOCKS:
        v_block_copy();
        break;
    case HORIZONTAL_ZIGZAG_BLOCKS:
        h_zig_block_copy(); 
        break;
    case VERTICAL_ZIGZAG_BLOCKS:
        v_zig_block_copy(); 
        break;
    }
    pt("blocks in matrix reorg completed");
}

//reorg block order in matrix, LEDs in the blacks stay the same
//must check on both H and V
void matrix_block_direction() {
    pt("matrix block direction started");
    switch (H_blockDir) {
    case LEFT_2_RIGHT:
        pt("  top left-2-right do nothing");
        break;
    case RIGHT_2_LEFT:
        pt("  right-2-left-H swap");
        H_block_swap();
        break;
    }
    switch (V_blockDir) {
    case TOP_DOWN:
        pt("  top down do nothing");
        break;
    case BOTTOM_UP:
        pt("  bottom up V swap");
        V_block_swap();
        break;
    }
    pt("matrix block direction completed");
}

//1 2 3 4  -> 4 3 2 1
//5 6 7 8  -> 8 7 6 5 //inside blocks not changed
void H_block_swap() {
    pt("H_block_swap started");

    uint16_t toX, toY, frmX, frmY;     //target xand y
    uint16_t x1 = 0;    //original block start loc
    uint16_t y1 = 0;
    uint16_t block_num = 0;
    //make block size steps
    for (uint16_t y2 = y1; y2 < MATRIX_TILE_V; y2++) {        //copy block y number of blocks
        for (uint16_t x2 = x1; x2 < MATRIX_TILE_H; x2++) {       //copy block x number of blocks
            block_num = (MATRIX_TILE_H - 1) - x2;             //reverse horiz block order
            frmX = x2 * MATRIX_TILE_WIDTH;
            frmY = y2 * MATRIX_TILE_HEIGHT;
            toX = (block_num * MATRIX_TILE_WIDTH);
            toY = (y2 * MATRIX_TILE_HEIGHT);
            //test here
            copy_block_service(frmX, frmY, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, toX, toY);
        }
    }
    work2main(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
    pt("H_block_swap completed");
}

void V_block_swap() {
    pt("V_block_swap started");

    uint16_t toX, toY, frmX, frmY;     //target xand y
    uint16_t x1 = 0;    //original block start loc
    uint16_t y1 = 0;
    uint16_t block_num = 0;
    //make block size steps
    for (uint16_t x2 = x1; x2 < MATRIX_TILE_H; x2++) {       //copy block x number of blocks
        for (uint16_t y2 = y1; y2 < MATRIX_TILE_V; y2++) {        //copy block y number of blocks
            block_num = (MATRIX_TILE_V - 1) - y2;             //reverse vertical block order
            frmX = x2 * MATRIX_TILE_WIDTH;
            frmY = y2 * MATRIX_TILE_HEIGHT;
            toX = (x2 * MATRIX_TILE_WIDTH);
            toY = (block_num * MATRIX_TILE_HEIGHT);
            //test here
            copy_block_service(frmX, frmY, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, toX, toY);
        }
    }
    work2main(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
    pt("V_block_swap completed");
}

//copy_block_service(x1, y1, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, toX, toY);
//copy block from x1,y1 to x2,y2

void copy_block_service(uint16_t x1, uint16_t y1, uint16_t cellWidth, uint16_t cellHeight, uint16_t x2, uint16_t y2) {

    int16_t x, y, tox, toy;
    uint16_t ix = 0, iy = 0;  //offset into traget

    for (y = y1; y < y1 + cellHeight; y++) {
        ix = 0;
        for (x = x1; x < x1 + cellWidth; x++) {
            tox = x2 + ix;
            toy = y2 + iy;
            //test here
            workingArray[tox][toy] = mainArray[x][y];
            ix++;
        }
        iy++;
    }
}

/*
1 2 3 4
8 7 6 5
*/
void h_zig_block_copy() {
    pt("h_zig_block_copy started");

    uint16_t toX, toY;     //target xand y
    uint16_t toNum;      //array cotent offset value
    uint16_t x1 = 0;    //original block start loc
    uint16_t y1 = 0;
    uint16_t block_num = 0;
    //make block size steps
    for (uint16_t y2 = y1; y2 < MATRIX_TILE_V; y2++) {        //copy block y number of blocks
        for (uint16_t x2 = x1; x2 < MATRIX_TILE_H; x2++) {       //copy block x number of blocks
            if (y2 % 2) {            //odd row
               uint16_t xBack = (MATRIX_TILE_WIDTH - 1) - x2;       //reverse block number
                toX = (xBack * MATRIX_TILE_WIDTH);          //convert
                toY = (y2 * MATRIX_TILE_HEIGHT);
            }
            else {      //even row
                toX = (x2 * MATRIX_TILE_WIDTH);
                toY = (y2 * MATRIX_TILE_HEIGHT);
            }
            toNum = (block_num * LEDS_PER_BLOCK);
            copy_zero_block_array(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, toX, toY, toNum);
            block_num++;    //keep x increasing thru the next block
        }
    }
    work2main(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
    pt("h_zig_block_copy completed");
}


/*
1 8 9
2 7 10
3 6 11
4 5 12
*/
void v_zig_block_copy() {
    pt("v_zig_block_copy started");

   uint16_t toX, toY;     //target xand y
    uint16_t toNum;      //array cotent offset value
    uint16_t x1 = 0;    //original block start loc
    uint16_t y1 = 0;
    uint16_t block_num = 0;
    //make block size steps
     for (uint16_t x2 = x1; x2 < MATRIX_TILE_H; x2++) {       //copy block x number of blocks
        for (uint16_t y2 = y1; y2 < MATRIX_TILE_V; y2++) {       //copy block y number of blocks
            if (x2 % 2) {            //odd row
               uint16_t yBack = (MATRIX_TILE_HEIGHT - 1) - y2;       //reverse block number
                toX = (x2 * MATRIX_TILE_WIDTH);          //convert
                toY = (yBack * MATRIX_TILE_HEIGHT);
            }
            else {      //even row
                toX = (x2 * MATRIX_TILE_WIDTH);
                toY = (y2 * MATRIX_TILE_HEIGHT);
            }
            toNum = (block_num * LEDS_PER_BLOCK);
            copy_zero_block_array(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, toX, toY, toNum);
            block_num++;    //keep x increasing thru the next block
        }
    }
    work2main(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
    pt("v_zig_block_copy completed");
}

/*
    1 2 3 4
    5 6 9 8
*/
void h_block_copy() {
    pt("h_block_copy started");

    uint16_t toX, toY;     //target xand y
    uint16_t toNum;      //array cotent offset value
    uint16_t x1 = 0;    //original block start loc
    uint16_t y1 = 0;
    uint16_t block_num = 0;
    //make block size steps
    for (uint16_t y2 = y1; y2 < MATRIX_TILE_V; y2++) {        //copy block y number of blocks
        for (uint16_t x2 = x1; x2 < MATRIX_TILE_H; x2++) {       //copy block x number of blocks
            toX = (x2 * MATRIX_TILE_WIDTH);
            toY = (y2 * MATRIX_TILE_HEIGHT);
            toNum = (block_num * LEDS_PER_BLOCK);
            copy_zero_block_array(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, toX, toY, toNum);
            block_num++;    //keep x increasing thru the next block
        }
    }
    work2main(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
    pt("h_block_copy completed");
}

void copy_zero_block_array(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        uint16_t toX, uint16_t toY, uint16_t toNum) {

    for (uint16_t y = y1; y < y2; y++) {        //copy a block countents 
        for (uint16_t x = x1; x < x2; x++) { 
            workingArray[x + toX][y + toY] =
                mainArray[x][y] + toNum;
        }
    }
}

/*
 1  3
 2  4
*/
void v_block_copy() {
    pt("v_block_copy started");

    uint16_t toX, toY;     //target xand y
    uint16_t toNum;      //array cotent offset value
    uint16_t x1 = 0;    //original block start loc
    uint16_t y1 = 0;
    uint16_t block_num = 0;
    //make block size steps
    for (uint16_t x2 = x1; x2 < MATRIX_TILE_H; x2++) {       //copy block x number of blocks
        for (uint16_t y2 = y1; y2 < MATRIX_TILE_V; y2++) {        //copy block y number of blocks
            toX = (x2 * MATRIX_TILE_WIDTH);
            toY = (y2 * MATRIX_TILE_HEIGHT);
            toNum = (block_num * LEDS_PER_BLOCK);
            copy_zero_block_array(0, 0, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, toX, toY, toNum);
            block_num++;    //keep x increasing thru the next block
        }
    }
    work2main(0, 0, MATRIX_WIDTH_ABS, MATRIX_HEIGHT_ABS );
    pt("v_block_copy completed");
}

//copy an array block to a starting start x,y offset (for blocks)
void copy_table_array(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        uint16_t toX, uint16_t toY) {
#if (HAVE_ARRAY == true)
    uint16_t x, y;
    pt("copy_table_array start"); 
    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            mainArray[toX + x][toY + y] = XYTable[y][x];
        }
    }
    pt("copy_table_array completed"); 
#endif
}

//build MAIN array simple sequencial array of any size
void build_horiz_array(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y, i;
    i = 0;
    pt("build_horiz_array start"); 
    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            workingArray[x][y] = i;
            i++;
        }
    }
    work2main(x1, y1, x2, y2);
    pt("build_horiz_array completed"); 
}

//swap to working and return to MAIN array simple sequencial array of any size
void H_swap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y, i;
    uint16_t x_end;
    x_end = x2 - 1;
    pt("  H_swap start");   

    for (y = y1; y < y2; y++) {
        i = x_end;
        for (x = x1; x < x2; x++) {
            workingArray[i][y] = mainArray[x][y];
            i--;
        }
    }
    work2main(x1, y1, x2, y2);
       pt("  H_swap completed"); 
}

//swap to working and return to MAIN array simple sequencial array of any size
void V_swap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y, i;
    uint16_t y_end;
    y_end = y2 - 1;
   pt("  V_swap start"); 
    for (x = x1; x < x2; x++) {
        i = y_end;
        for (y = y1; y < y2; y++) {
             workingArray[x][i] = mainArray[x][y];
            i--;
        }
    }
    work2main(x1, y1, x2, y2);
    pt("  V_swap completed"); 
}

//make a copy of the working array to main
void work2main(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y;

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            mainArray[x][y] = workingArray[x][y];
        }
    }
}

//make a copy of the working array to main
void main2work(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y;

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            workingArray[x][y] = mainArray[x][y];
        }
    }
}

//swap to working and return to MAIN array simple sequencial array of any size
//swap diagonally so numbers go down each column
void Diagonal_swap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y, x_new, y_new, y2_new;
        
    pt("    diagonal_swap start"); 
    x_new = x1;
    y_new = y1;
    y2_new = y2;    //

    for (y = y1; y < y2; y++) {   //this is source array
        for (x = x1; x < x2; x++){ 
            workingArray[x_new][y_new] = mainArray[x][y];
            y_new++;
            if (y_new >= y2_new) {
                y_new = y1;
                x_new++;
            }
        }
    }
    work2main(x1, y1, x2, y2);
    pt("    diagonal_swap completed"); 
    _vflag = true;
}

//zigzag service function
void horiz_zig(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y;
    pt("horiz_zizag start"); 
     for (y = y1; y < y2; y++) {
         for (x = x1; x < x2; x++) {
             if (y % 2) {            //odd row
                 workingArray[(x2 - 1) - x][y] = mainArray[x][y];
             }
             else {
                 workingArray[x][y] = mainArray[x][y];
             }
         }
     }
     work2main(x1, y1, x2, y2);
      pt("horiz_zizag completed"); 
      _hflag = true;                //let us know this was done once already
}

//zigzag service function
void Vertical_zig(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int16_t x, y;
    pt("Vertical_zizag start"); 
    for (x = x1; x < x2; x++) {
        for (y = y1; y < y2; y++) {
            if (x % 2) {            //odd column
                workingArray[x][(y2 - 1) - y] = mainArray[x][y];
            }
            else {
                workingArray[x][y] = mainArray[x][y];
            }
        }
    }
    work2main(x1, y1, x2, y2);
    pt("Vertical_zizag completed"); 
    _vflag = true;
}

void run_report() {
    int16_t x, y;
    pt("");
    pt("======= Report ======");
    pt("");
    ptt("NUM_LEDS = "); pt(NUM_LEDS);
    ptt("HAVE_ARRAY = "); pt(HAVE_ARRAY);
    ptt("MATRIX_WIDTH = "); pt(MATRIX_WIDTH);       //display possible negative value
    ptt("MATRIX_HEIGHT  = "); pt(MATRIX_HEIGHT);
    if (!HAS_BLOCKS){
     ptt("matrix_type = "); pt(MatrixType[matrix_type]);
    }
    ptt("horizDir = "); pt(matrixHorizDir[horizDir]);
    ptt("vertDir = "); pt(matrixVertDir[vertDir]);

    if (HAS_BLOCKS) {
        ptt("HAS_BLOCKS = "); pt(hasBlocks[HAS_BLOCKS]);
        ptt("   MATRIX_TILE_WIDTH = "); pt(MATRIX_TILE_WIDTH);
        ptt("   MATRIX_TILE_HEIGHT = "); pt(MATRIX_TILE_HEIGHT);
        ptt("   MATRIX_TILE_H = "); pt(MATRIX_TILE_H);
        ptt("   MATRIX_TILE_V = "); pt(MATRIX_TILE_V);
        ptt("   blockOrg = "); pt(block_Org[blockOrg]);
        ptt("   blocksInMatrix = "); pt(block_Org[blocksInMatrix]);
        if (MATRIX_WIDTH_ABS > 64) {
            pt("");
            pt("WIDE ARRAYS WILL LOOK MESSY");
            pt("COPY TO AN EDITOR w/o text wrapping TO VIEW");
        }
                pt("");
        pt("===== Report End =====");
        pt("");
    }

    pt("/*");
    pt("XYTable_yx.h");
    pt("");
    pt("This table is layed out in X=horizontal in each row and Y=vertical rows.");  //<<< build this
    pt("this array is in logical VISUAL ORDER of rows and columns so NOTE:");
    pt("Addressing in your code is in y, x order XYTable[y][x] NOT x, y");
    pt("*/");
    ptt("const uint16_t  PROGMEM XYTable[]["); ptt(MATRIX_WIDTH_ABS); pt("] = {");    //<< width x or height y??
    //print tabel rows x w/ commas, comma at end of line
    uint16_t columns = 3;
    int16_t ledCount = 0;
    if (NUM_LEDS > 999) {
        columns = 4;
    }
    else if (NUM_LEDS > 9999){
        columns = 5;
    }
    for (y = 0; y < MATRIX_HEIGHT_ABS ; y++) {           //catch last line
        for (x = 0; x < MATRIX_WIDTH_ABS; x++) {
            if (HAS_BLOCKS && TABLE_DIVIDERS) {                //for easier fiewing -
                table_dividers(x, y, columns); 
            }
            fmt(mainArray[x][y], columns, 0);
            if (ledCount < NUM_LEDS - 1) {          //no comma after lasr entry
                ptt(",");
            }
             else {
             }
        }
        ledCount++;
        pt("");
     }
    pt("};");
    versionPrint();
}

void versionPrint() {
    ptt("Version ");
    pt(VERSION);        //set at top for ttesting
}

//fmt(i, 3, 0);
void fmt(float input, byte columns, byte places){
    char buffer[columns];
    dtostrf(input, columns, places, buffer);
    ptt(buffer);
}

void table_dividers(int16_t x, int16_t y, uint16_t columns) {
    static uint16_t y_cur = 0;
    if (! (x % (MATRIX_TILE_WIDTH )) && x > 0) {
        ptt("|");
    }
    if (!(y % (MATRIX_TILE_HEIGHT)) && y > 0 && y != y_cur) {
        for (uint16_t i = 0; i < (columns * MATRIX_WIDTH_ABS - 5); i++) {
            ptt("");
        }
        pt("");
        y_cur = y;
    }
}
