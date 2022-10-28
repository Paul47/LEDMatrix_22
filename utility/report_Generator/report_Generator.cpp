/* report_Generator.cpp
* Debugging report for LEDMatrix_22
* if not defines at top, exclude this code to save program memory space

Definitons used from the sketch:
    LED_MATRIX_WIDTH    //former LEDMatrix use negative value for reversed (right to left)
    LED_MATRIX_HEIGHT   //former LEDMatrix use negative value for reversed (bottom to top)
    HORIZ_DIR           //LEFT_2_RIGHT, RIGHT_2_LEFT
    VERT_DIR            //TOP_DOWN, BOTTOM_UP

    MATRIX_TILE_WIDTH   // width of EACH MATRIX "tile" (not total display)
    MATRIX_TILE_HEIGHT  // height of each matrix "tile"
    MATRIX_TILE_H       // number of matrices arranged horizontally
    MATRIX_TILE_V       // number of matrices arranged vertically
    LEDS_IN_TILE        //HORIZONTAL_MATRIX, VERTICAL_MATRIX,
                                //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX
       TILES_IN_MATRIX     //HORIZONTAL_TILES, VERTICAL_TILES,
                                //HORIZONTAL_ZIGZAG_TILES, VERTICAL_ZIGZAG_TILES
    NUM_STRIPS
    NUM_BANKS           // 1 to 4 "banks" for multiple led strips
    STRIPS_PER_BANK     //1 or more but 4 strips per Bank is the most effiicient use of the hardware
    LEDS_PER_BANK   //equally split the total number of leds across the number of active Banks
    LEDS_PER_STRIP  //equally split the number of leds in each banks into the number of strips in each Bank

    enableBank[NUM_BANKS]        //an array works best here
    stripStart[NUM_STRIPS]    //may need these segment points during programming
*/

void run_report();
String tilesInMatrix();
String ledsInTile();
String matrixTypeStr();
void listTiles();
void listBanks();
void listExtender();
void fmt(float input, byte columns, byte places);
void matrixDirection();
void mapOfTiles();
void  HTL();        //letters for direction ex: horizontal, top down, left 2 right
void  HTR();
void  HBL();
void  HBR();

void  VTL();
void  VTR();
void  VBL();
void  VBR();
void mapOfLedsInTile();
void  tHTL();        //letters for direction ex: horizontal, top down, left 2 right
void  tHTR();
void  tHBL();
void  tHBR();

void  tVTL();
void  tVTR();
void  tVBL();
void  tVBR();

#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.printl MACRO

void numberBank(uint8_t i); //used by report gen

void run_report() {

    pt("====== Reporting Enabled======");
    pt("");

    pt("Use this report to check that all your definitions are correct");
    pt("(if your panel is not working correctly it is likely a parameter is incorrect)")
    pt("");
    pt("======= Report ======");
    pt("");
    ptt("NUM_LEDS = "); ptt(NUM_LEDS); pt(" leds total in all strings");
    
    #ifdef XYTable_LookUp
        ptt("XYTable_LookUp = "); pt("Using a XYTable_LookUp table");     //attach to the lookup table in the library
    #endif
    
   #ifdef CLOCK_1
        pt("CLOCK_1 pin defined, so these are 2 wire LED strips");
    #else
        pt("No CLOCK pins, so these are 1 wire LED strips");
        #ifdef DATA_3
            if (DATA_3 == 17){
                pt("DATA_3 pin is 17, so this is a Teensy 4");
            }
            else if (DATA_3 == 10){
                pt("DATA_3 pin is 10, so this is a Teensy 3");
            }
            else{
                    pt("DATA_3 pin is not 10 or 17, so I don't recognize this MCU");
            }
        #else
             pt("DATA_3 pin is not defined, so I don't recognize this setup");
        #endif
        pt("");
    #endif                                             
    //ptt("COLOR_ORDER = "); pt(COLOR_ORDER);
    ptt("BRIGHTNESS = "); pt(BRIGHTNESS);
    ptt("NUM_LEDS = "); ptt(NUM_LEDS); pt(" leds total in all strings");
 
    #ifdef MATRIX_WIDTH                   
        ptt("MATRIX_WIDTH = "); ptt(MATRIX_WIDTH); pt(" leds across entire matrix panel");
        ptt("MATRIX_HEIGHT = "); ptt(MATRIX_HEIGHT); pt(" leds up/down entire matrix panel");
        ptt("MATRIX_TYPE = "); ptt(matrixTypeStr()); pt(" leds flow in each row");
        matrixDirection();
    #endif

    #ifdef XYTable_LookUp
    pt("XYTable_LookUp is enabled");
    pt("  Your lookup table overrides the HAS_TILES section of the Configuration header");  
    #else
        listTiles();                   
        mapOfLedsInTile();
        mapOfTiles();
    #endif

    listBanks();
    pt("");
    pt("===== Report End =====");
    pt("");

}

String tilesInMatrix() {
    #ifdef HAS_TILES   //stub out if not
    #if HAS_TILES
        if (TILES_IN_MATRIX == HORIZONTAL_TILES) {
            return "HORIZONTAL_TILES";
        }
        else if (TILES_IN_MATRIX == VERTICAL_TILES) {
            return "VERTICAL_TILES";
        }
        else if (TILES_IN_MATRIX == HORIZONTAL_ZIGZAG_TILES) {
            return "HORIZONTAL_ZIGZAG_TILES";
        }
        else if (TILES_IN_MATRIX == VERTICAL_ZIGZAG_TILES) {
            return "VERTICAL_ZIGZAG_TILES";
        }
    #endif
    #endif
    return "";
}


String ledsInTile() {
    #ifdef LEDS_IN_TILE   //stub out if not
        if (LEDS_IN_TILE == HORIZONTAL_MATRIX) {
            return "HORIZONTAL_MATRIX";
        }
        else if (LEDS_IN_TILE == VERTICAL_MATRIX) {
            return "VERTICAL_MATRIX";
        }
        else if (LEDS_IN_TILE == HORIZONTAL_ZIGZAG_MATRIX) {
            return "HORIZONTAL_ZIGZAG_MATRIX";
        }
        else if (LEDS_IN_TILE == VERTICAL_ZIGZAG_MATRIX) {
            return "VERTICAL_ZIGZAG_MATRIX";
        }
    #endif
    return "";
}

String matrixTypeStr() {
    #ifdef MATRIX_TYPE      //stub if not a simple matrix                                                        
        if (MATRIX_TYPE == HORIZONTAL_MATRIX) {
            return "HORIZONTAL_MATRIX";
        }
        else if (MATRIX_TYPE == VERTICAL_MATRIX) {
            return "VERTICAL_MATRIX";
        }
        else if (MATRIX_TYPE == HORIZONTAL_ZIGZAG_MATRIX) {
            return "HORIZONTAL_ZIGZAG_MATRIX";
        }
        else if (MATRIX_TYPE == VERTICAL_ZIGZAG_MATRIX) {
            return "VERTICAL_ZIGZAG_MATRIX";
        }
    #endif     
   return "";
}

void listTiles() {
    #ifdef HAS_TILES   //defined?
    #if (!HAS_TILES)
        ptt("MATRIX_TYPE = "); ptt(matrixTypeStr()); pt(" mapping of LEDs thru the entire matrix panel");
    #else
        ptt("HAS_TILES = ");
        if (HAS_TILES == true) { pt("true"); }
        else { pt("false"); }
        ptt("   LEDS_IN_TILE = "); ptt(ledsInTile()); pt("        (direction of flow of LEDs INSIDE each tile/tile)");
        ptt("   MATRIX_TILE_WIDTH = "); ptt(MATRIX_TILE_WIDTH); pt("        (Number of LED columns horizontally inside each tile/tile)");
        ptt("   MATRIX_TILE_HEIGHT = "); ptt(MATRIX_TILE_HEIGHT); pt("        (Number of LED rows vertically inside each tile/tile)")
        ptt("   MATRIX_TILE_H = "); ptt(MATRIX_TILE_H); pt("                (Number of tiles across the entire matrix panel)");
        ptt("   MATRIX_TILE_V = "); ptt(MATRIX_TILE_V);  pt("                (Number of tiles or strips up/down the entire matrix panel)");

        ptt("   TILES_IN_MATRIX = "); ptt(tilesInMatrix()); pt("        (direction of flow of tiles/tiles thru the matrix panel)");
        #endif
        pt("");
    #endif

}


void listBanks(){
    #ifdef HAS_BANKS
    #if HAS_BANKS
            uint8_t strip = 0;  //strip loop from 0-3 but physical strips are 1-4
            int16_t ledCount;
            uint8_t col = 10;   //field widths for report
            if (NUM_STRIPS == 0 || NUM_BANKS == 0) {return;}
            pt("Strips Report");
            ptt("  NUM_STRIPS = "); pt(NUM_STRIPS);
            ptt("  LEDS_PER_STRIP = "); pt(LEDS_PER_STRIP);
            pt("");
            pt("Banks Report");
            ptt("  NUM_BANKS = "); pt(NUM_BANKS);
            ptt("  LEDS_PER_BANK = "); pt(LEDS_PER_BANK);
            listExtender();

            //offset into leds[] for each strip - used for memcopy LEDS[] to LEDS_2[]
            //NOTE: this index is reversed - from top down - thats  the way FastLED sets up the memory
            uint16_t stripStart[NUM_STRIPS];    //may need these segment points during programming
 
            /*  //old reversed from bottom  up
            uint8_t index = NUM_STRIPS - 1;
            for (int8_t i = 0; i < NUM_STRIPS; i++) {
                    stripStart[index] = i * LEDS_PER_STRIP;
                    index--;
            }
            */

            //segments from top down
            for (int8_t i = 0; i < NUM_STRIPS; i++) {
                stripStart[i] = i * LEDS_PER_STRIP;
            }



        #if CLOCK_PIN_REQUIRED  //2-wire leds
            ptt("      strip");  ptt(" | Data pin"); ptt(" | Clock pin"); ptt(" | StripStart"); pt(" |  StripEnd");
            pt("       ----------------------------------------");
            for (uint8_t i = 0; i < NUM_BANKS; i++) {
                ptt("Bank =  "); pt(i);
                ptt(" Enable Pin = "); numberBank(NUM_BANKS - 1 - i); pt("");
                    #if STRIPS_PER_BANK  > 0
                        fmt(strip+1, col, 0); fmt(DATA_1, col, 0);   fmt(CLOCK_1, col, 0);   fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP-1, col, 0); pt("");
                        strip++;
                    #endif
                    #if STRIPS_PER_BANK  > 1
                        fmt(strip+1, col, 0);  fmt(DATA_2, col, 0);   fmt(CLOCK_1, col, 0);   fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP-1, col, 0); pt("");
                        strip++;
                    #endif
                    #if STRIPS_PER_BANK  > 2
                        fmt(strip+1, col, 0); fmt(DATA_1, col, 0);   fmt(CLOCK_2, col, 0);   fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP-1, col, 0); pt("");
                        strip++;
                    #endif
                    #if STRIPS_PER_BANK  > 3
                        fmt(strip+1, col, 0);  fmt(DATA_2, col, 0);   fmt(CLOCK_2, col, 0);   fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP-1, col, 0); pt("");
                        strip++;
                    #endif
            }
          #else         //1-wire leds
            ptt("      strip");  ptt(" | Data pin"); ptt(" | StripStart"); pt(" |  StripEnd");
            pt("      ----------------------------------------");
            for (uint8_t i = 0; i < NUM_BANKS; i++) {
                 ptt("Bank =  "); pt(i);
                ptt(" Enable Pin = "); numberBank(NUM_BANKS - 1 - i); pt("");
                #if STRIPS_PER_BANK  > 0
                   fmt(strip + 1, col, 0); fmt(DATA_1, col, 0); fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP - 1, col, 0); pt("");
                   strip++;
                #endif
                #if STRIPS_PER_BANK  > 1
                   fmt(strip + 1, col, 0);  fmt(DATA_2, col, 0); fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP - 1, col, 0); pt("");
                  strip++;
                #endif
                #if STRIPS_PER_BANK  > 2
                  fmt(strip + 1, col, 0); fmt(DATA_3, col, 0); fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP - 1, col, 0); pt("");
                  strip++;
                #endif
                #if STRIPS_PER_BANK  > 3
                    fmt(strip + 1, col, 0);  fmt(DATA_4, col, 0); fmt(stripStart[strip], col, 0);  fmt(stripStart[strip] + LEDS_PER_STRIP - 1, col, 0); pt("");
                 strip++;
                #endif
            }
          #endif    
            ledCount = (strip) * LEDS_PER_STRIP;
            ptt("Strip LED count = "); pt(ledCount);
            ptt("NUM_LEDS = "); pt(NUM_LEDS);
            if (NUM_LEDS == ledCount) {
                pt("LED counts match");
            }else{
                pt("Error: LED COUNTS DO NOT MATCH - # of LEDs in strips or # of strips is incorrect!");
            }
    #endif      //#if HAS_BANKS
    #endif      //#ifdef HAS_BANKS                  
}


void listExtender() {
    #ifdef HAS_BANKS
    #if HAS_BANKS
        pt(" ");
        ptt("  Bank Enable Pins = "); ptt(BANK_PIN_0);
        if (NUM_BANKS > 1) {
        #ifdef BANK_PIN_1
            ptt(", "); ptt(BANK_PIN_1);
        #endif
        }
        if (NUM_BANKS > 2) {
        #ifdef BANK_PIN_2
            ptt(", "); ptt(BANK_PIN_2);
        #endif
        }
        if (NUM_BANKS > 3) {
        #ifdef BANK_PIN_3
            ptt(", "); ptt(BANK_PIN_3);
        #endif
        }
        pt("");
        //must use the defined rather than the variales for these pins sins user may chang leds. to some other name
        #if CLOCK_PIN_REQUIRED  //these apply only to 2-wire leds
            #if defined DATA_1 && defined CLOCK_1
                ptt("  Bank Data Pins (Data/Clock) = "); ptt(DATA_1); ptt("/"); ptt(CLOCK_1); pt(" ");
                #if defined DATA_2 && defined CLOCK_2
                    ptt("  Bank Data Pins (Data/Clock) = "); ptt(DATA_2); ptt("/"); ptt(CLOCK_2); pt(" ");
                #endif
            #endif
        #else  //these apply only to 1-wire leds
            ptt("  Bank Data Pins = ");
            ptt(DATA_1); 
            if (STRIPS_PER_BANK > 1) {
            #ifdef DATA_2
                ptt(", "); ptt(DATA_2);
            #endif
            }
            if (STRIPS_PER_BANK > 2) {
            #ifdef DATA_3
                ptt(", "); ptt(DATA_3);
            #endif
            }
            if (STRIPS_PER_BANK > 3) {
            #ifdef DATA_4
                ptt(", "); ptt(DATA_4);
            #endif
            }
        #endif
        pt("");
    #endif  //HAS_BANKS
    #endif  //HAS_BANKS                     
}

//fmt(i, 3, 0);
void fmt(float input, byte columns, byte places) {
    char buffer[columns];
    dtostrf(input, columns, places, buffer);
    ptt(buffer);
}

void matrixDirection(){
    ptt("The 1st row of your panel is: ");
    if (HORIZ_DIR == LEFT_2_RIGHT) {   //LEFT_2_RIGHT, RIGHT_2_LEFT
        ptt("LEFT_2_RIGHT");
    }
    else {
        ptt("RIGHT_2_LEFT");
    }
    if (VERT_DIR == TOP_DOWN) {        //TOP_DOWN, BOTTOM_UP
        pt(" / TOP_DOWN");
    }
    else {
        pt(" / BOTTOM_UP")
    }
    pt("");
}

/* tiling combinations for mapping tiles in the matrix panel
  #define TILES_IN_MATRIX     VERTICAL_TILES   //sequence of tiles in the entire panel
                                               //HORIZONTAL_TILES, VERTICAL_TILES,
                                               //HORIZONTAL_ZIGZAG_TILES, VERTICAL_ZIGZAG_TILES
  #define HORIZ_DIR      LEFT_2_RIGHT   		//LEFT_2_RIGHT, RIGHT_2_LEFT
  #define VERT_DIR       TOP_DOWN       		//BOTTOM_UP, TOP_DOWN

pseudo-code:
TILES_IN_MATRIX  ==   HORIZONTAL_TILES || HORIZONTAL_ZIGZAG_TILES
    VERT_DIR  ==   TOP_DOWN
        HORIZ_DIR  ==    LEFT_2_RIGHT 
            TILES_IN_MATRIX  ==   HORIZONTAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==   HORIZONTAL_TILES

        HORIZ_DIR  ==    RIGHT_2_LEFT
            TILES_IN_MATRIX  ==   HORIZONTAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==   HORIZONTAL_TILES

    VERT_DIR  ==   BOTTOM_UP
        HORIZ_DIR  ==    LEFT_2_RIGHT
            TILES_IN_MATRIX  ==   HORIZONTAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==   HORIZONTAL_TILES

        HORIZ_DIR  ==    RIGHT_2_LEFT
            TILES_IN_MATRIX  ==   HORIZONTAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==   HORIZONTAL_TILES


TILES_IN_MATRIX  ==   VERTICAL_TILES || VERTICAL_ZIGZAG_TILES
    VERT_DIR  ==   TOP_DOWN
        HORIZ_DIR  ==    LEFT_2_RIGHT
            TILES_IN_MATRIX  ==   VERTICAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==   VERTICAL_TILES

        HORIZ_DIR  ==    RIGHT_2_LEFT
            TILES_IN_MATRIX  ==  VERTICAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==  VERTICALTILES

    VERT_DIR  ==   BOTTOM_UP
        HORIZ_DIR  ==    LEFT_2_RIGHT
            TILES_IN_MATRIX  ==   VERTICAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==   VERTICAL_TILES

        HORIZ_DIR  ==    RIGHT_2_LEFT
            TILES_IN_MATRIX  ==   VERTICAL_ZIGZAG_TILES
            TILES_IN_MATRIX  ==   VERTICAL_TILES

*/

#ifdef HAS_TILES   //is it even defined?
#if HAS_TILES

    void mapOfTiles() {     //print table w/ tile positons in the matrix panel

        pt("Map of tiles in your matrix panel");
        pt("H=Horiz, T=TopDown, L=LeftRight, V=Vert, B=BottomUp, R=RightLeft");
        ptt("Layout: ");
        if ((TILES_IN_MATRIX == HORIZONTAL_TILES) || (TILES_IN_MATRIX == HORIZONTAL_ZIGZAG_TILES)) {
            if (VERT_DIR == TOP_DOWN) {
                if (HORIZ_DIR == LEFT_2_RIGHT) { 
                    pt("HTL");
                    HTL();       //letters for direction ex: horizontal, top down, left 2 right
                }
                else {  /*HORIZ_DIR == RIGHT_2_LEFT*/ 
                    pt("HTR");
                    HTR();
                }
            }
            if (VERT_DIR == BOTTOM_UP) {
                if (HORIZ_DIR == LEFT_2_RIGHT) { 
                    pt("HBL");
                    HBL();
                }
                else {  /*HORIZ_DIR == RIGHT_2_LEFT*/
                    pt("HBR");
                    HBR();
                }
            }
        }

        if ((TILES_IN_MATRIX == VERTICAL_TILES) || (TILES_IN_MATRIX == VERTICAL_ZIGZAG_TILES)) {
            if (VERT_DIR == TOP_DOWN) {
                if (HORIZ_DIR == LEFT_2_RIGHT) { 
                    pt("VTL");
                    VTL();
                }
                else {  /*HORIZ_DIR == RIGHT_2_LEFT*/ 
                    pt("VTR");
                   VTR();
                }
            }
            if (VERT_DIR == BOTTOM_UP) {
                if (HORIZ_DIR == LEFT_2_RIGHT) { 
                    pt("VBL");
                    VBL();
                }
                else {  /*HORIZ_DIR == RIGHT_2_LEFT*/ 
                    pt("VBR");
                   VBR();
                }
            }
        }
    }

    void  HTR() {   //letters for direction ex: horizontal, top down, left 2 right
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles

        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {        //foward y
            for (int8_t x = MATRIX_TILE_H - 1; x >= 0; x--) {    //reverse x
                if (TILES_IN_MATRIX == HORIZONTAL_ZIGZAG_TILES) {
                    if (y % 2) {            //odd row
                        index[x][y] = (y * MATRIX_TILE_V) + (MATRIX_TILE_H - 1) - x;
                    }
                    else {  /* HORIZONTAL_TILES */
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
         }
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void  HBL() {   //letters for direction ex: horizontal, top down, left 2 right
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles
        for (int8_t y = MATRIX_TILE_V - 1; y >= 0; y--) {    //reverse y
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {  //forward x
                if (TILES_IN_MATRIX == HORIZONTAL_ZIGZAG_TILES) {
                    if (y % 2) {            //odd row
                        index[x][y] = (y * MATRIX_TILE_V) + (MATRIX_TILE_H - 1) - x;
                    }
                    else {  /* HORIZONTAL_TILES */
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void  HBR() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles
        for (int8_t y = MATRIX_TILE_V - 1; y >= 0; y--) {    //reverse y
            for (int8_t x = MATRIX_TILE_H - 1; x >= 0; x--) {    //reverse x
                if (TILES_IN_MATRIX == HORIZONTAL_ZIGZAG_TILES) {
                    if (y % 2) {            //odd row
                        index[x][y] = (y * MATRIX_TILE_V) + (MATRIX_TILE_H - 1) - x;
                    }
                    else {  /* HORIZONTAL_TILES */
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void  VTR() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles
        for (int8_t x = MATRIX_TILE_H - 1; x >= 0; x--) {    //reverse x
            for (int8_t y = 0; y < MATRIX_TILE_V; y++) {        //foward y
                if (TILES_IN_MATRIX == VERTICAL_ZIGZAG_TILES) {
                    if (x % 2) {            //odd column
                        index[x][y] = (x * MATRIX_TILE_H) + (MATRIX_TILE_V - 1) - y;
                    }
                    else {
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }
    void  VBL() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles
        for (int8_t x = 0; x < MATRIX_TILE_H; x++) {        //forw3ard x
            for (int8_t y = MATRIX_TILE_V - 1; y >= 0; y--) {    //reverse y
                if (TILES_IN_MATRIX == VERTICAL_ZIGZAG_TILES) {
                    if (x % 2) {            //odd column
                        index[x][y] = (x * MATRIX_TILE_H) + (MATRIX_TILE_V - 1) - y;
                    }
                    else {
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }
    void  VBR() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles
        for (int8_t x = MATRIX_TILE_H - 1; x >= 0; x--) {    //reverse x
            for (int8_t y = MATRIX_TILE_V - 1; y >= 0; y--) {    //reverse y
                if (TILES_IN_MATRIX == VERTICAL_ZIGZAG_TILES) {
                    if (x % 2) {            //odd column
                        index[x][y] = (x * MATRIX_TILE_H) + (MATRIX_TILE_V - 1) - y;
                    }
                    else {
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void HTL() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {        //forward y
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {    //forward x
                if (TILES_IN_MATRIX == HORIZONTAL_ZIGZAG_TILES) {
                    if (y % 2) {            //odd row
                        index[x][y] = (y * MATRIX_TILE_V) + (MATRIX_TILE_H - 1) - x;
                    }
                    else {  /* HORIZONTAL_TILES */
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void VTL() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_H][MATRIX_TILE_V];   //total number of tiles
            for (int8_t x = 0; x < MATRIX_TILE_H; x++) {            //forward x
                for (int8_t y = 0; y < MATRIX_TILE_V; y++) {        //forward y
                    if (TILES_IN_MATRIX == VERTICAL_ZIGZAG_TILES) {
                        if (x % 2) {            //odd column
                            index[x][y] = (x * MATRIX_TILE_H) + (MATRIX_TILE_V - 1) - y;
                        }
                        else {
                            index[x][y] = count;
                        }
                    }
                    else {
                        index[x][y] = count;
                    }
                    count++;
                }
            }
            for (int8_t y = 0; y < MATRIX_TILE_V; y++) {
                for (int8_t x = 0; x < MATRIX_TILE_H; x++) {
                    fmt(index[x][y], columns, 0);
                }
                pt("");
            }
            pt("");
    }
 
#else       /* stubs */  //#ifs are OUTSIDE the functions so need stubs to avoid errors
    void mapOfTiles() {  }
    void  HTL() { }       //letters for direction ex: horizontal, top down, left 2 right
    void  HTR() { }
    void  HBL() { }
    void  HBR() { }

    void  VTL() { }
    void  VTR() { }
    void  VBL() { }
    void  VBR() { }
#endif  //if HAS_TILES
#endif //ifdef HAS_TILES

/*  leds in TILE combinations for mapping leds in TILES

    #define LEDS_IN_TILE        HORIZONTAL_MATRIX          //LED sequence within each tile:
                                                        //HORIZONTAL_MATRIX, VERTICAL_MATRIX,
                                                        //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX
    #define LEDS_HORIZ_DIR      LEFT_2_RIGHT   	//LEFT_2_RIGHT, RIGHT_2_LEFT
    #define LEDS_VERT_DIR       TOP_DOWN       	//BOTTOM_UP, TOP_DOWN

pseudo-code:
LEDS_IN_TILE   ==   HORIZONTAL_MATRIX || HORIZONTAL_ZIGZAG_MATRIX
    LEDS_VERT_DIR  ==   TOP_DOWN
        LEDS_HORIZ_DIR  ==    LEFT_2_RIGHT
            LEDS_IN_TILE   ==   HORIZONTAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==   HORIZONTAL_MATRIX

        LEDS_HORIZ_DIR  ==    RIGHT_2_LEFT
            LEDS_IN_TILE   ==   HORIZONTAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==   HORIZONTAL_MATRIX

    LEDS_VERT_DIR  ==   BOTTOM_UP
        LEDS_HORIZ_DIR  ==    LEFT_2_RIGHT
            LEDS_IN_TILE   ==   HORIZONTAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==   HORIZONTAL_MATRIX

        LEDS_HORIZ_DIR  ==    RIGHT_2_LEFT
            LEDS_IN_TILE   ==   HORIZONTAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==   HORIZONTAL_MATRIX


LEDS_IN_TILE   ==   VERTICAL_MATRIX || VERTICAL_ZIGZAG_MATRIX
    LEDS_VERT_DIR  ==   TOP_DOWN
        LEDS_HORIZ_DIR  ==    LEFT_2_RIGHT
            LEDS_IN_TILE   ==   VERTICAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==   VERTICAL_MATRIX

        LEDS_HORIZ_DIR  ==    RIGHT_2_LEFT
            LEDS_IN_TILE   ==  VERTICAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==  VERTICALMATRIX

    LEDS_VERT_DIR  ==   BOTTOM_UP
        LEDS_HORIZ_DIR  ==    LEFT_2_RIGHT
            LEDS_IN_TILE   ==   VERTICAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==   VERTICAL_MATRIX

        LEDS_HORIZ_DIR  ==    RIGHT_2_LEFT
            LEDS_IN_TILE   ==   VERTICAL_ZIGZAG_MATRIX
            LEDS_IN_TILE   ==   VERTICAL_MATRIX
*/

    //=====================================================


/*
TILES_IN_MATRIX   	LEDS_IN_TILE
HORIZ_DIR  	        LEDS_HORIZ_DIR
VERT_DIR  		    LEDS_VERT_DIR
MATRIX_TILE_H       MATRIX_TILE_WIDTH
MATRIX_TILE_V       MATRIX_TILE_HEIGHT
_TILES              _MATRIX
*/

#ifdef HAS_TILES   //is it even defined?
#if HAS_TILES      //is it true

    void mapOfLedsInTile() {

        pt("Map of LEDs in your tiles");
        pt("H=Horiz, T=TopDown, L=LeftRight, V=Vert, B=BottomUp, R=RightLeft");
        ptt("Layout: ");
        if ((LEDS_IN_TILE == HORIZONTAL_MATRIX) || (LEDS_IN_TILE == HORIZONTAL_ZIGZAG_MATRIX)) {
            if (LEDS_VERT_DIR == TOP_DOWN) {
                if (LEDS_HORIZ_DIR == LEFT_2_RIGHT) {
                    pt("HTL");
                    tHTL();       //letters for direction ex: horizontal, top down, left 2 right
                }
                else {  /*LEDS_HORIZ_DIR == RIGHT_2_LEFT*/
                    pt("HTR");
                    tHTR();
    }
            }
            if (LEDS_VERT_DIR == BOTTOM_UP) {
                if (LEDS_HORIZ_DIR == LEFT_2_RIGHT) {
                    pt("HBL");
                    tHBL();
                }
                else {  /*LEDS_HORIZ_DIR == RIGHT_2_LEFT*/
                    pt("HBR");
                    tHBR();
                }
            }
        }

        if ((LEDS_IN_TILE == VERTICAL_MATRIX) || (LEDS_IN_TILE == VERTICAL_ZIGZAG_MATRIX)) {
            if (LEDS_VERT_DIR == TOP_DOWN) {
                if (LEDS_HORIZ_DIR == LEFT_2_RIGHT) {
                    pt("VTL");
                    tVTL();
                }
                else {  /*LEDS_HORIZ_DIR == RIGHT_2_LEFT*/
                    pt("VTR");
                    tVTR();
                }
            }
            if (LEDS_VERT_DIR == BOTTOM_UP) {
                if (LEDS_HORIZ_DIR == LEFT_2_RIGHT) {
                    pt("VBL");
                    tVBL();
                }
                else {  /*LEDS_HORIZ_DIR == RIGHT_2_LEFT*/
                    pt("VBR");
                    tVBR();
                }
            }
        }
    }


    void  tHTR() {   //letters for direction ex: horizontal, top down, left 2 right
        byte columns = 5;
        uint8_t count = 0;
        uint8_t rowStart = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {        //foward y
            rowStart = count;
            for (int8_t x = MATRIX_TILE_WIDTH - 1; x >= 0; x--) {    //reverse x
                if (LEDS_IN_TILE == HORIZONTAL_ZIGZAG_MATRIX) {
                    if (y % 2) {            //even row
                        index[x][y] = (rowStart + x);
                    }
                    else { 
                        index[x][y] = count;
                    }
                }
                else { /* HORIZONTAL_MATRIX */
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void  tHBL() {   //letters for direction ex: horizontal, top down, left 2 right
        byte columns = 5;
        uint8_t count = 0;
        uint8_t rowStart = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t y = MATRIX_TILE_HEIGHT - 1; y >= 0; y--) {    //reverse y
            rowStart = count; //row start
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {  //forward x
                if (LEDS_IN_TILE == HORIZONTAL_ZIGZAG_MATRIX) {
                    if (y % 2) {            //even row
                        index[x][y] = count;
                       
                    }
                    else {
                       // index[x][y] = count;
                        index[x][y] = (rowStart + MATRIX_TILE_WIDTH - 1 - x);
                    }
                }
                else {  /* HORIZONTAL_MATRIX */
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void  tHBR() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t rowStart = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t y = MATRIX_TILE_HEIGHT - 1; y >= 0; y--) {    //reverse y
            rowStart = count; //row start
            for (int8_t x = MATRIX_TILE_WIDTH - 1; x >= 0; x--) {    //reverse x
                if (LEDS_IN_TILE == HORIZONTAL_ZIGZAG_MATRIX) {
                    if (y % 2) {            //even row
                       // index[x][y] = (y * MATRIX_TILE_HEIGHT) + (MATRIX_TILE_WIDTH - 1) - x;
                        index[x][y] = count;
                    }
                    else {
                        index[x][y] = (rowStart + MATRIX_TILE_WIDTH - 1 + x);
                    }
                }
                else {  /* HORIZONTAL_MATRIX */
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void  tVTR() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t x = MATRIX_TILE_WIDTH - 1; x >= 0; x--) {    //reverse x
            for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {        //foward y
                if (LEDS_IN_TILE == VERTICAL_ZIGZAG_MATRIX) {
                    if (x % 2) {            //odd column
                        index[x][y] = (x * MATRIX_TILE_WIDTH) + (MATRIX_TILE_HEIGHT - 1) - y;
                    }
                    else {
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }
    void  tVBL() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t colStart = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {        //forw3ard x
            colStart = count;
            for (int8_t y = MATRIX_TILE_HEIGHT - 1; y >= 0; y--) {    //reverse y
                if (LEDS_IN_TILE == VERTICAL_ZIGZAG_MATRIX) {
                    if (x % 2) {            //even column
                        index[x][y] = (colStart  + y);
                    }
                    else {
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }
    void  tVBR() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t colStart = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t x = MATRIX_TILE_WIDTH - 1; x >= 0; x--) {    //reverse x
            colStart = count;
            for (int8_t y = MATRIX_TILE_HEIGHT - 1; y >= 0; y--) {    //reverse y
                if (LEDS_IN_TILE == VERTICAL_ZIGZAG_MATRIX) {
                    if (x % 2) {            //even column
                        index[x][y] = count;
                    }
                    else {
                        index[x][y] = (colStart + y);
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void tHTL() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t rowStart = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {        //forward y
            rowStart = count;
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {    //forward x
                if (LEDS_IN_TILE == HORIZONTAL_ZIGZAG_MATRIX) {
                    if (y % 2) {            //even row
                        index[x][y] = (rowStart + MATRIX_TILE_WIDTH - 1 - x);
                    }
                    else {
                        index[x][y] = count;
                    }
                }
                else {  /* HORIZONTAL_MATRIX */
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

    void tVTL() {
        byte columns = 5;
        uint8_t count = 0;
        uint8_t index[MATRIX_TILE_WIDTH][MATRIX_TILE_HEIGHT];   //total number of tiles
        for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {            //forward x
            for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {        //forward y
                if (LEDS_IN_TILE == VERTICAL_ZIGZAG_MATRIX) {
                    if (x % 2) {            //odd column
                        index[x][y] = (x * MATRIX_TILE_WIDTH) + (MATRIX_TILE_HEIGHT - 1) - y;
                    }
                    else {
                        index[x][y] = count;
                    }
                }
                else {
                    index[x][y] = count;
                }
                count++;
            }
        }
        for (int8_t y = 0; y < MATRIX_TILE_HEIGHT; y++) {
            for (int8_t x = 0; x < MATRIX_TILE_WIDTH; x++) {
                fmt(index[x][y], columns, 0);
            }
            pt("");
        }
        pt("");
    }

#else       /* stubs */  //#ifs are OUTSIDE the functions so need stubs to avoid errors
    void mapOfTiles() {  }
    void  tHTL() { }       //letters for direction ex: horizontal, top down, left 2 right
    void  tHTR() { }
    void  tHBL() { }
    void  tHBR() { }

    void  tVTL() { }
    void  tVTR() { }
    void  tVBL() { }
    void  tVBR() { }
#endif  //if HAS_TILES 
#endif //ifndef HAS_TILES 

//quick clug to list bank numbers in reverse order
void numberBank(uint8_t i){
    if (NUM_BANKS == 1) {
        pt(BANK_PIN_0)
    }
    else if (NUM_BANKS == 2) {
        if (i == 0) {                  //again forced to cluge because of defines
            pt(BANK_PIN_1);
        }
        else if (i == 1) {
            pt(BANK_PIN_0);
        }
    }
    else if (NUM_BANKS == 3) {
        if (i == 0) {                  //again forced to cluge because of defines
            pt(BANK_PIN_2);
        }
        else if (i == 1) {
            pt(BANK_PIN_1);
        }
        else {
            pt(BANK_PIN_0);
        }
    }
    else if (NUM_BANKS == 4) {
        if (i == 0) {                  //again forced to cluge because of defines
            pt(BANK_PIN_3);
        }
        else if (i == 1) {
            pt(BANK_PIN_2);
        }
        else if (i == 2) {
            pt(BANK_PIN_1);
        }
        else {
            pt(BANK_PIN_0);
        }
    }
}
