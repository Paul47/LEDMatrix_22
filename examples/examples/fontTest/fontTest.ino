//fontTest.ino will run on any size panel

//useful for debugging
#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.printl MACRO

/* Configuring this library and FastLED requires a number of parameters and #defines
Configure your LED array in myConfiguration.h located in the  library folder.
This way you can reuse your configuration file(s) across all your sketches.
Consider renaming your configurations (and changing the #include "myConfiguration.h"
in the library folder.
*/

#include <LEDMatrix_22.h>

#include ".\fonts\TomThumb.h"
#include ".\fonts\Picopixel.h"

#define NUM_cFONTS      10
struct C_FONTS {
    uint8_t fn;
    uint8_t xOffset;  //positive number from expected x to printed x. usually 0
    uint8_t yOffset;  //positive number from expected y to printed y. usually = font height
    uint8_t width;
    uint8_t height;
    boolean space;        //char space?
    GFXfont ff;
};

C_FONTS c_fonts[NUM_cFONTS];        //registeer up to NUM_cFONTS fonts.

/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;

//------------------------ Setup and loop ------------------------
void setup() {

    leds.addLeds();       //initiate LEDMatrix

}


void loop() {

    cFTest();   //This test creates a structure array to load an use multiple custom fonts.

    //fontTest(TomThumb, text);     //simple font test with a small font
    //textTest();               //These two use the default font
    //printTest();
}

//=================================== text testing section ============================
/*
CHARACTERS
     drawChar( x,  y,  c,  color,  bg, size)
    size_t  = write(uint8_t)

//Set (text)
    setFont(const GFXfont *f = NULL);
     setCursor( x,  y)
     setTextColor( c)
     setTextColor( c,  bg)
     setTextSize(s)
     setTextWrap(boolean)

 //    -------------------------------------------
void setCursor(int16_t x, int16_t y);
void setTextColor(CRGB c);
void setTextColor(CRGB c, CRGB bg);
void setTextSize(uint8_t s);
void setTextWrap(boolean w);
void setRotation(uint8_t r);
void cp437(boolean x=true);
void setFont(const GFXfont *f = NULL);
void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void drawChar(int16_t x, int16_t y, unsigned char c, CRGB color, CRGB bg, uint8_t size);
size_t write(char char);
void print(char text[]);
*/

void textTest() {

    //leds.setTextColor(CRGB::Red);
    leds.setTextColor(CRGB::Red, CRGB::Black);
    leds.setTextSize(1);
    leds.setTextWrap(false);
    char my_str[] = "Hello";

    for (uint8_t r = 0; r < 4; r++) {
        leds.setRotation(r);
         FastLED.clear(true);
         leds.setCursor(1, 10);
        for (uint8_t i = 0; i < sizeof(my_str); i++) {
            leds.write(my_str[i]);
             leds.show();   
            delay(1000);
        }
        leds.clear();
    }
    leds.setRotation(0); //0-3 in 90 deg steps
    leds.fillScreen(CRGB::Black);
}

void printTest() {
    int16_t x;
    char text[] = "HELLO WORLD!";

    leds.fillScreen(CRGB::Black);
     leds.show();   
    leds.setTextWrap(0);
    for (x = MATRIX_WIDTH; x > -55; x--) {
        leds.setTextColor(CRGB::OrangeRed);
        leds.setCursor(x, MATRIX_HEIGHT / 4);
        leds.print(text);
        delay(200);
        leds.setTextColor(CRGB::Black);
        leds.setCursor(x, MATRIX_HEIGHT/4);
        leds.print(text);
    }
    char text1[] = "Wrapping";
    leds.clear();
     leds.show();   
    leds.setTextWrap(1);
    for (x = -55; x < MATRIX_WIDTH; x++) {
        leds.setTextColor(CRGB::Blue);
        leds.setCursor(x, MATRIX_HEIGHT / 2);
        leds.print(text1);
        delay(200);
        leds.setTextColor(CRGB::Black);
        leds.setCursor(x, MATRIX_HEIGHT/2);
        leds.print(text1);
    }
    leds.fillScreen(CRGB::Black);
}

void fontTest(const GFXfont tt, char text[]) {

        int16_t x=0, y=0;
        x = 0;
        y = 0;

        leds.setRotation(0);
        leds.setTextWrap(false);             //font wrapping with custom fonts does not always work correctly.
                                            //use write() each charter, check position, and wrap yourself

        GFXfont ff[10];
        ff[0] = TomThumb;

       //const GFXfont tt = TomThumb;

        leds.setFont(&ff[0]);                //must use pointer &
        leds.setTextSize(1);
        leds.setTextColor(CRGB::Green);
        leds.setCursor(x, y + 5);           //with custom fonts, char x,y is bottom left - subtract font height
        //print text
        leds.clear();
       // char text[] = "Hello World!";
        leds.print(text);
        FastLED.delay(1000);            //surprise! delay includes a show - no need to do twice

        //clear this text - must reposition, change color, then print
        leds.setCursor(x, y + 5);
        leds.setTextColor(CRGB::Black);
        leds.print(text);

        //new text
        leds.setCursor(x, y + 5);
        leds.setTextColor(CRGB::Red);
        char text1[] = "ABCDEF";
        leds.print(text1);
        FastLED.delay(1000);            //surprise! delay includes a show - no need to do twice
}


/* Custom Adafruit_GFX compatible fonts
*
  1. #define NUM_cFONTs to the number of fonts to be used
  2. cFInit() to register the fonts
        Set up up to NUM_cFONTS custom fonts.
        Assign name fontName a number 0-NUMcFONTS-1
        Add any needed x,y offsets (for example, custom fonts y coord is usually bottm left, while the default fonts are uppeer left.
        Add the font width and height.
  3. Set the normal FastLED text settings before printing, ex: color, rotation, wrap.
  4. cFPrint() to print
*/



/*
set up up to NUM_cFONTS custom fonts.
Assign name fontName a number 0-NUMcFONTS-1
Add any needed x,y offsets (for example, custom fonts y coord is usually bottm left, while the default fonts are uppeer left.
Add the font width and height.
*/
void cFInit(uint8_t fn, const GFXfont fontName, uint8_t xOffset, uint8_t yOffset, uint8_t width, uint8_t height) {

    if (fn > NUM_cFONTS) { return; }
    //set up an struct array for these values for each font registered
    c_fonts[fn].fn = fn;
    c_fonts[fn].xOffset = xOffset;
    c_fonts[fn].yOffset = yOffset;
    c_fonts[fn].width = width;
    c_fonts[fn].height = height;
    c_fonts[fn].ff = fontName;
}


void cFTest() {
    int16_t x;
    int16_t y;

    leds.setTextSize(1);              //Only text sizs = 1 for cutom fonts
    leds.setRotation(0);
    leds.setTextWrap(false);             //font wrapping with custom fonts does not always work correctly.
                                         //use write() each charter, check position, and wrap yourself
    //print text
    leds.clear();
    FastLED.delay(1000);            //surprise! delay includes a show - no need to do twice

    uint8_t fn = 0;
    leds.setTextColor(CRGB::Green);
    const GFXfont fontName0 = TomThumb;
    uint8_t xOffset = 0;
    uint8_t yOffset = 5;
    uint8_t width = 3;
    uint8_t height = 5;
    cFInit(fn, fontName0, xOffset, yOffset, width, height);

    fn = 1;
    const GFXfont fontName1 = Picopixel;
    xOffset = 0;
    yOffset = 6;
    width = 3;
    height = 6;
    cFInit(fn, fontName1, xOffset, yOffset, width, height);

    char text[] = "Hello World!";

    //  print testing font 0
    x = 3;
    y = 3;
    fn = 0;
    leds.setTextColor(CRGB::Red);
    cFPrint(fn, x, y, text);
    FastLED.delay(1000);            //surprise! delay includes a show - no need to do twice

   //clear this text - must reposition, change color, then print
    x = 3;                          //reset the position
    y = 3;
    fn = 0;
    leds.setTextColor(CRGB::Black);
    cFPrint(fn, x, y, text);
    FastLED.delay(1000);            //surprise! delay includes a show - no need to do twice
/*
    //  print testing font 1
    x = 0;
    y = 8;
    fn = 1;
    leds.setTextColor(CRGB::Blue);
    cFPrint(fn, x, y, text);
    FastLED.delay(1000);            //surprise! delay includes a show - no need to do twice

   //clear this text - must reposition, change color, then print
    x = 0;                  //reset the position to the beginning
    y = 8;
    fn = 1;
    leds.setTextColor(CRGB::Black);
    cFPrint(fn, x, y, text);
    FastLED.delay(1000);            //surprise! delay includes a show - no need to do twice
*/

}


void cFPrint(uint8_t fn, uint8_t x, uint8_t y, char text[]) {

    leds.setFont(&c_fonts[fn].ff);     //must use pointer &
    leds.setCursor(x + c_fonts[fn].xOffset, y + c_fonts[fn].yOffset);
    leds.print(text);
}
