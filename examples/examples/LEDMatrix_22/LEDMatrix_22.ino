//LEDMatrix_22.ino



//useful for debugging
#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.printl MACRO

/* Configuring this library and FastLED requires a number of parameters and #defines
Configure your LED array in myConfiguration.h located in the  library folder.
This way you can reuse your configuration file(s) across all your sketches.
Consider renaming your configurations (and changing the #include "myConfiguration.h"
in the library folder.

Set serial terminal to 115200
*/

#include <LEDMatrix_22.h>
#include"TT_numbers.h"      //Tom Thumb number set 0-9

//init all dimensions to -1 as unused just for this sketch
int16_t X[5] = { -1, -1, -1, -1, -1 }; //ul,ur,ll,lr,center of matrix
int16_t Y[5] = { -1, -1, -1, -1, -1 };

//zero now but will resize in matrixSpecs()
//#define leds.numTiles = MATRIX_TILE_V
uint8_t* tileCentrx = (uint8_t*)malloc(sizeof(uint8_t) * 0);
uint8_t* tileCentry = (uint8_t*)malloc(sizeof(uint8_t) * 0);
uint8_t* tileULx = (uint8_t*)malloc(sizeof(uint8_t) * 0);
uint8_t* tileULy = (uint8_t*)malloc(sizeof(uint8_t) * 0);


/* DEBUGGING REPORT - sent to the serial terminal
To run report define RUN_REPORT, the Report Generator code will be included
below AFTER all the paramters are defined. Compile and upload this sketch
then run_report(); will execute at then end of setup.

>>>> Report Generator.cpp MUST BE INCLUDED BELOW just before sewtup()
after all parameters are defined <<<
*/
#define RUN_REPORT      //will open Serial.pprint when started in setup

//------------------- create the total matrix panel array -------------------
#if HAS_EXTENDER || HAS_BLOCKS 
    // leds in block/tile width, height      led flow IN THE TILE      # tiles in the matrix h, v    How tiles flow in the matrix
        cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, LEDS_IN_TILE, MATRIX_TILE_H_DIR, MATRIX_TILE_V_DIR, TILES_IN_MATRIX> leds;      //leds array configured
    #else
        cLEDMatrix<MATRIX_WIDTH_DIR, MATRIX_HEIGHT_DIR, MATRIX_TYPE> leds;  // create our matrix based on matrix definition only
    #endif

//if run report is defined, the report_Generator.cpp code will be included
//>>>> the code MUST BE INCLUDED HERE after all parameters are defined
#ifdef RUN_REPORT
#include ".\report_Generator\report_Generator.cpp"
#endif

//------------------------ Setup and loop ------------------------
void setup() {
    //if run report is defined, the report_Generator.cpp code will be included
    #ifdef RUN_REPORT
      Serial.begin(115200); //select your speed
      delay(1000);
      run_report();       //also open Serial port
    #endif

    leds.addLeds();     //no parameters required
    exerciseStart();
}

void loop() {


    uint16_t value = 0;

    if (Serial.available()) {
        char ch = Serial.read();
        switch (ch) {
        case '0'...'9':
            value = value * 10 + ch - '0';
            break;
        case 'a':
            pt("Run Report");
            run_report();       //also open Serial port
            pt("Done with report");
            value = 0;
            break;
        case 'b':
            pt("");
            //draw corner triangles
            TringleDraw();      //triangles at matrix corners
            pt("Done with triangle draw");
            value = 0;
            break;
        case 'c':
            pt("");
            drawTileDot();     //draW dots in tiles
            pt("Done with dot draw");
            value = 0;
            break;
        case 'd':
            pt("");
            stepThruTiles();    //number in each tile
            pt("Done with tiles");
            value = 0;
            break;
        case 'e':
            pt("");
            showBanks();
            pt("Done with banks");
            value = 0;
            break;
        case 'f':
            pt("");
            stepThruStrips();
            pt("Done done with strips");
            value = 0;
            break;
        case 'g':
            pt("");
            value = 0;
            pt("");
            menu();          //also come back here when done
            break;
        }
    }
}


void matrixSpecs() {
    //set up variables for this matrix panel
    //4 corners - ul,ur,ll,lr
    X[0] = 0;
    Y[0] = 0;
    X[1] = leds.matrixWidth - 1;      //last
    Y[1] = 0;
    X[2] = 0;
    Y[2] = leds.matrixHeight - 1;
    X[3] = leds.matrixWidth - 1;
    Y[3] = leds.matrixHeight - 1;
    X[4] = (leds.matrixWidth / 2) - 1;    //center of matrix
    Y[4] = (leds.matrixHeight / 2) - 1;

    if (leds.numTiles) {         //resize now that we know m_numTiles size

        tileCentrx = (uint8_t*)realloc(tileCentrx, leds.numTiles);
        tileCentry = (uint8_t*)realloc(tileCentry, leds.numTiles);
        tileULx = (uint8_t*)realloc(tileULx, leds.numTiles);
        tileULy = (uint8_t*)realloc(tileULy, leds.numTiles);
    }

    //capture x,y in upper left corner of each tile
    uint8_t c = 0;          //tile count
    for (uint8_t j = 0; j < leds.tilesPerCol; j++) {
        for (uint8_t i = 0; i < leds.tilesPerRow; i++) {
            tileULx[c] = i * leds.tileWidth;
            tileULy[c] = j * leds.tileHeight;
            tileCentrx[c] = tileULx[c] + leds.tileWidth / 2;
            tileCentry[c] = tileULy[c] + leds.tileHeight / 2;
            c++;
        }
    }
}


void showBanks() {
    if (leds.hasExtender) {         //is it true?
        if (leds.numBanks == 0 || leds.stripsPerBank == 0) {
            pt("ERROR: No Banks or Strips defined");
            return;
        }
        char text[] = "B";
        leds.clear();
        stepThruBanks();        //add some color

        int16_t indexStart[16];

        for (uint8_t i = 0; i < leds.numBanks; i++) {
            indexStart[i] = i * leds.ledsPerBank;
        }

        for (uint8_t i = 0; i < leds.numBanks; i++) {
            int16_t y = indexStart[i] / leds.matrixWidth;
            int16_t x = 0;
            leds.setTextColor(CRGB::Black);
            leds.setCursor(x, y);

            char b[2];
            String str = String(i);
            str.toCharArray(b, 2);
            leds.print(b);      //must be char array
            leds.print(text);    //B for
            leds.LEDShow();
            FastLED.delay(1000);
        }
    }
    else {
        pt(">>>> NO Banks defined");
    }
}


void stepThruTiles() {
    if (leds.hasBlocks) {            //is it true?
        if (leds.tileWidth < 0 || leds.tileHeight < 0) {
            pt("ERROR: No tile sizes defined");
            return;
        }
        if (leds.tileWidth < 2 * NUM_W || leds.tileHeight < NUM_H) {
            pt("Tiles too small for numbers");
            return;
        }

        leds.clear();

        for (uint8_t i = 0; i < leds.numTiles; i++) {
            leds.drawRectangle(tileULx[i], tileULy[i], tileULx[i] + leds.tileWidth - 1, tileULy[i] + leds.tileHeight - 1, CRGB::DarkGreen); //Draw white frame around each tile
        }

        static uint8_t num[NUM_CHARS][NUM_H];      //from the included file
        for (uint8_t j = 0; j < 5; j++) {    //simplify the display loop
            num[0][j] = num0[j];
            num[1][j] = num1[j];
            num[2][j] = num2[j];
            num[3][j] = num3[j];
            num[4][j] = num4[j];
            num[5][j] = num5[j];
            num[6][j] = num6[j];
            num[7][j] = num7[j];
            num[8][j] = num8[j];
            num[9][j] = num9[j];
        }

        for (uint8_t i = 0; i < leds.numTiles; i++) {
            if (i < 10) {       //1 digit
                leds.drawBitmap(tileULx[i], tileULy[i], num[i], NUM_W, NUM_H, CRGB::Red + 100 * i);
            }
            if (i > 9) {        //2 digits
                if (leds.tileWidth < 2 * NUM_W) {
                    pt("Tiles too small for 2-digit numbers");
                    return;
                }
                uint8_t digitLeft = int(i / 10);
                leds.drawBitmap(tileULx[i], tileULy[i], num[digitLeft], NUM_W, NUM_H, CRGB::Red + 100 * i);     //#1 + digit
                leds.drawBitmap(tileULx[i] + NUM_W, tileULy[i], num[i - digitLeft * 10], NUM_W, NUM_H, CRGB::Red + 100 * i);
            }
            leds.LEDShow();
            FastLED.delay(8000/leds.numTiles);
        }
    }
    else {
        pt(">>>> NO Blocks defined");
    }   //HAS_BLOCKS
}



void drawTileDot() {       //display x,y in upper left corner of each tile
    if (leds.hasBlocks) {            //is it true?
        if (leds.tileWidth == 0 || leds.tileHeight == 0) {
            pt("ERROR: No tile sizes defined");
            return;
        }
        leds.clear();
        CRGB color = CRGB::DeepSkyBlue;

        for (uint8_t c = 0; c < leds.numTiles; c++) {
            leds.drawTriangle(tileULx[c], tileULy[c], tileULx[c] + 1, tileULy[c], tileULx[c], tileULy[c] + 1, color);
            leds.drawPixel(tileCentrx[c], tileCentry[c], CRGB::Pink);
        }
        leds.LEDShow();
        FastLED.delay(2000);
    }
    else {
        pt(">>>> NO Blocks defined");
    } //HAS_BLOCKS
}


//draw trianles at each corner of matrix pointing to the corner
void TringleDraw() {
    leds.clear();

    CRGB color = CRGB::Red;
    int16_t x0, y0, x1, y1, x2, y2;

    x0 = X[0];
    y0 = Y[0];
    x1 = X[0] + 2;
    y1 = Y[0];
    x2 = X[0];
    y2 = Y[0] + 2;

    leds.drawRectangle(0, 0, leds.matrixWidth - 1, leds.matrixHeight - 1, CRGB::White); //Draw white frame
    //upper left
    leds.drawTriangle(x0, y0, x1, y1, x2, y2, color);
    leds.LEDShow();

    color = CRGB::Blue;
    x0 = X[1];
    y0 = Y[1];
    x1 = X[1] - 2;
    y1 = Y[1];
    x2 = X[1];
    y2 = Y[1] + 2;
    //upper right
    leds.drawTriangle(x0, y0, x1, y1, x2, y2, color);
    leds.LEDShow();

    color = CRGB::Green;
    x0 = X[2];
    y0 = Y[2];
    x1 = X[2] + 2;
    y1 = Y[2];
    x2 = X[2];
    y2 = Y[2] - 2;
    //lower left
    leds.drawTriangle(x0, y0, x1, y1, x2, y2, color);
    leds.LEDShow();

    color = CRGB::Yellow;
    x0 = X[3];
    y0 = Y[3];
    x1 = X[3] - 2;
    y1 = Y[3];
    x2 = X[3];
    y2 = Y[3] - 2;
    //lower right
    leds.drawTriangle(x0, y0, x1, y1, x2, y2, color);

    leds.LEDShow();
    FastLED.delay(2000);
}



//anything needed to initialize sketch here
void exerciseStart() {
    leds.clear();
    matrixSpecs();      //initialize matrix panel variables for this sketch
    printHello();    //let us know your working
    menu();          //also come back here when done
}

void menu() {
    //  leds.clear();   //clear matrix of last exercise
    //  leds.LEDShow();

    pt("*Sketh functionality*");
    pt("Locate and exercise your matrix panel, Extender Banks and Strips.");
    pt("This is an interactive menu on serial monitor to select functions:");
    pt("    If no blocks, menu stops after 'd' ");
    pt("    If no Extender, menu stops after 'e' ");
    pt("");
    pt("a. We will report your configuration.");
    pt("b. Draw triangles in starting corner and each corner of matrix");
    pt("IF HAS_BLOCKS");
    pt("c.    Draw arrows in starting corners and centers of tiles");
    pt("d.    Light up each block/tile AND print the number of the Block in sequence");
    pt("IF HAS_EXTENDER");
    pt("e.    Light up each Bank and number in sequence");
    pt("f.    Light up each strips");
    pt("g. Display this MENU");
    ptt("Select Exercise letter in cmd field");
}



void memoryTest() {
    //SetLEDArray(struct CRGB *pLED);	// Only used with externally defined LED arrays

    for (int16_t i = 0; i < 20; i++) {
        leds[i]->r = 255;
        leds[i]->g = 128;
        leds[i]->b = 32;
    }

    ptt("number of FastLED controllers = "); pt(FastLED.count());
    ptt("array size = ");  pt(FastLED.size());

    FastLED.setBrightness(20);
    ptt("Brightness = ");  pt(FastLED.getBrightness());

    pt("Step 1");
    pt("read back leds[]");
    for (int16_t i = 0; i < 20; i++) {
        ptt(leds[i]->r); ptt("\t");
        ptt(leds[i]->g); ptt("\t");
        ptt(leds[i]->b); pt("");
    }

    pt("Step 2");
    pt("read back from m_LED[]");
    for (int16_t i = 0; i < 20; i++) {
        ptt(leds.m_LED[i].r); ptt("\t");
        ptt(leds.m_LED[i].g); ptt("\t");
        ptt(leds.m_LED[i].b); pt("");
    }

    pt("Step 3");
    if (!leds.hasExtender) {     //if no extender or false  then no array --> locks up
        pt("Exetnder option is false. No Extender memory");
        return;
    }
    leds.LEDShow();                     //LOLAD e_LED array
    pt("read back from e_LED[]");
    for (int16_t i = 0; i < 20; i++) {
        ptt(leds.e_LED[i].r); ptt("\t");
        ptt(leds.e_LED[i].g); ptt("\t");
        ptt(leds.e_LED[i].b); pt("");
    }
}

//simple led step through 1 at a time
void stepThruBanks() {
    static uint8_t here = 0;        //cabge on return call 3 ways
    leds.clear();
    FastLED.delay(1000);
    for (int16_t y = 0; y < leds.tileHeight; y++) {
        for (int16_t x = 0; x < leds.matrixWidth; x++) {
            leds.drawPixel(x, y, CRGB::Green);
        }
        if (here == 1) { leds.LEDShow(); FastLED.delay(500); }
    }
    if (here == 0) { leds.LEDShow(); FastLED.delay(1000); }

    for (int16_t y = leds.tileHeight; y < 2 * leds.tileHeight; y++) {
        for (int16_t x = 0; x < leds.matrixWidth; x++) {
            leds.drawPixel(x, y, CRGB::Blue);
        }
        if (here == 1) { leds.LEDShow(); FastLED.delay(500); }
    }
    if (here == 0) { leds.LEDShow(); FastLED.delay(1000); }

    for (int16_t y = 2 * leds.tileHeight; y < 3 * leds.tileHeight; y++) {
        for (int16_t x = 0; x < leds.matrixWidth; x++) {
            leds.drawPixel(x, y, CRGB::Red);
        }
        if (here == 1) { leds.LEDShow(); FastLED.delay(500); }
    }
    if (here == 0) { leds.LEDShow(); FastLED.delay(1000); }

    for (int16_t y = 3 * leds.tileHeight; y < 4 * leds.tileHeight; y++) {
        for (int16_t x = 0; x < leds.matrixWidth; x++) {
            leds.drawPixel(x, y, CRGB::Yellow);
        }
        if (here == 1) { leds.LEDShow(); FastLED.delay(500); }
    }
    if (here == 0) { leds.LEDShow(); FastLED.delay(1000); }

    here++;
    if (here > 2) { here = 0; }
}

void stepThruStrips() {

    if (leds.hasExtender) {
        uint16_t dly = 1000;
        uint8_t color = 75;
        int16_t index = 0;
        leds.clear();

        //easiest to fill memory directly rather than by x,y conversions
        for (uint8_t j = 0; j < leds.numBanks; j++) {
            for (uint8_t k = 0; k < leds.stripsPerBank; k++) {
                for (int16_t m = 0; m < leds.ledsPerStrip; m++) {
                    index = j * leds.ledsPerBank + k * leds.ledsPerStrip + m;
                    leds.m_LED[index] = CHSV(color,255,255);
                }
                color = (color + 40) % 255;
                leds.LEDShow();
                FastLED.delay(dly);
            }
        }
    }
    else {
        pt(">>>> NO Strips enabled");
    }
}




//=================================== text testing section ============================
/*
CHARACTERS
     drawChar( x,  y,  c,  color,  bg, size)
    size_t  = write(uint8_t)

//Set (text)
    SetFont(const GFXfont *f = NULL);
     SetCursor( x,  y)
     SetTextColor( c)
     SetTextColor( c,  bg)
     SetTextSize(s)
     SetTextWrap(boolean)

 //    -------------------------------------------
void SetCursor(int16_t x, int16_t y);
void SetTextColor(CRGB c);
void SetTextColor(CRGB c, CRGB bg);
void SetTextSize(uint8_t s);
void SetTextWrap(boolean w);
void SetRotation(uint8_t r);
void cp437(boolean x=true);
void SetFont(const GFXfont *f = NULL);
void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void drawChar(int16_t x, int16_t y, unsigned char c, CRGB color, CRGB bg, uint8_t size);
size_t write(uint8_t);
void print(char text[]);
*/
//show and intro s
void printHello() {
    int16_t x = 0;
    char text[] = "HELLO";
    leds.clear();
    leds.setTextColor(CRGB::Yellow);
    leds.setCursor(0,0);
    leds.print(text);
    leds.LEDShow();
}
void printTest() {
    int16_t x = 0;
    char text[] = "HELLO WORLD!";

    leds.clear();
    leds.LEDShow();
    leds.setTextWrap(0);
    for (x = leds.matrixWidth; x > -55; x--) {
        leds.setTextColor(CRGB::OrangeRed);
        leds.setCursor(x, leds.matrixHeight / 4);
        leds.print(text);
        delay(200);
        leds.setTextColor(CRGB::Black);
        leds.setCursor(x, leds.matrixHeight / 4);
        leds.print(text);

    }
    char text1[] = "Wrapping";
    leds.clear();
    leds.LEDShow();
    leds.setTextWrap(1);
    for (x = -55; x < leds.matrixWidth; x++) {
        leds.setTextColor(CRGB::Blue);
        leds.setCursor(x, leds.matrixHeight / 2);
        leds.print(text1);
        delay(200);
        leds.setTextColor(CRGB::Black);
        leds.setCursor(x, leds.matrixHeight / 2);
        leds.print(text1);
    }
    leds.clear();;
}
