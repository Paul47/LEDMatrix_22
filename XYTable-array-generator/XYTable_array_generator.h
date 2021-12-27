//XYTable_array_generator

/*  XYTable[] array generator
* This sketch will create an lookup table for LEDMatrix_21 or similar Arduino libraries
*
*
*
* OUTPUT: will be on the serial terminal so you can review the results and make changes.
* SAVE: copy and paste the terminal output array to a file.
*/

#pragma once


enum MatrixType_t {
    HORIZONTAL_MATRIX,          //string goes in the same direction horizontallyin each row
    VERTICAL_MATRIX,            //string goes in the same direction vertically in each column
    HORIZONTAL_ZIGZAG_MATRIX,   //string of LEDs snakes left and right alternating 
    VERTICAL_ZIGZAG_MATRIX      //string of LEDs snakes up and down alternating 
};

String MatrixType[] = { "HORIZONTAL_MATRIX",
                        "VERTICAL_MATRIX",
                        "HORIZONTAL_ZIGZAG_MATRIX",
                        "VERTICAL_ZIGZAG_MATRIX " };

//what direction does the FIRST row go?
enum MatrixOrder_horizDir {         //direction of FIRST row
    LEFT_2_RIGHT,
    RIGHT_2_LEFT
};

String matrixHorizDir[] = { "LEFT_2_RIGHT",
                             "RIGHT_2_LEFT" };


//what direction does the FIRST column go?
enum MatrixOrder_vertDir {     //direction of FIRST column
    BOTTOM_UP,
    TOP_DOWN
};
String matrixVertDir[] = { "BOTTOM_UP",
                          "TOP_DOWN" };


//how are the blocks/cells organized in the matrix
enum BlockType_t {
    HORIZONTAL_BLOCKS,
    VERTICAL_BLOCKS,
    HORIZONTAL_ZIGZAG_BLOCKS,   //how are cells organized within the cell as well
    VERTICAL_ZIGZAG_BLOCKS
};
String block_Org[] = { "HORIZONTAL_BLOCKS",
                        "VERTICAL_BLOCKS",
                        "HORIZONTAL_ZIGZAG_BLOCKS",
                        "VERTICAL_ZIGZAG_BLOCKS" };

String hasBlocks[] = { "false", "true" };

