//introduction.ino for LEDMatrix_22

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

    leds.clear();
    Scottish_Flag();
    leds.clear();
    canvasTest();
    textTest();
    invertTest();
    printTest();
    StepThru();
    Cylon();
    flowingRainbow();

}

void flowingRainbow() {
 
    while (true) {
        uint32_t ms = millis();
        int32_t yHueDelta32 = ((int32_t)cos16(ms * (27 / 1)) * (350 / MATRIX_WIDTH));
        int32_t xHueDelta32 = ((int32_t)cos16(ms * (39 / 1)) * (310 / MATRIX_HEIGHT));
        RainbowDrawOneFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
        if (ms < 5000) {
            leds.setBrightness(scale8(BRIGHTNESS, (ms * 256) / 5000));
        }
        else {
            leds.setBrightness(BRIGHTNESS);
        }
         leds.show();   
    }
}


void RainbowDrawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8){
    byte lineStartHue = startHue8;
    for (byte y = 0; y < MATRIX_HEIGHT; y++) {
        lineStartHue += yHueDelta8;
        byte pixelHue = lineStartHue;
        for (byte x = 0; x < MATRIX_WIDTH; x++) {
            pixelHue += xHueDelta8;
            leds.drawPixel(x, y, CHSV(pixelHue, 255, 255));

        }
    }
}
 
//draw dots across x axis, changing color randomly for each row
void StepThru() {
     leds.clear();
    for (int16_t y = 0; y < MATRIX_HEIGHT; y++) {
        uint8_t seed = random(1, 255);
        for (int16_t x = 0; x < MATRIX_WIDTH; x++) {
            uint8_t pixelHue = seed;
            leds.drawPixel(x, y, CHSV(pixelHue, 255, 255));
             leds.show();   
            //FastLED.delay(30);
        }
    }
    leds.fillScreen(CRGB::Black);
}

void Cylon() {
    int16_t x, y;
     uint16_t value = 3000;  //default 250 ..8(xx) higher = more fade
     leds.fillScreen(CRGB::Black);

    for (int i = 0; i < 5; i++){
        // First slide the led in one direction
        for (x = 0; x < MATRIX_WIDTH; x++) {
            for (y = 0; y < MATRIX_HEIGHT; y++) {
                leds.drawPixel(x, y, CRGB::Red);
            }
             leds.show();   
            leds.fadeAll(value);
            delay(80);
        }

        for (x = MATRIX_WIDTH - 1; x > 0; x--) {
            for (y = 0; y < MATRIX_HEIGHT; y++) {
                leds.drawPixel(x, y, CRGB::Red); 
            }
             leds.show();   
            leds.fadeAll(value);
            delay(80);
        }
    }
    leds.fillScreen(CRGB::Black);
}

//simple test of GFX code 
void canvasTest(){
    CRGB color;

    leds.fillScreen(CRGB::Green);
     leds.show();   
    delay(1000);

    leds.setRotation(0); //0-3 in 90 deg steps

    color = CRGB::White;
    leds.drawLine(15, 0, MATRIX_WIDTH - 1, MATRIX_HEIGHT - 1, color);
     leds.show();   
    delay(1000);
    color = CRGB::Blue;
    leds.drawCircle(4, 4, 2, CRGB(255, 255, 0));
     leds.show();   
    delay(1000);
    color = CRGB::HotPink;
    leds.drawRectangle(8, 8, 16, 16, color);
     leds.show();   
    delay(1000);
    color = CRGB::Olive;
    leds.drawFilledRectangle(9, 9, 15, 15, color);
     leds.show();   
    delay(1000);
    color = CRGB::DarkCyan;
    leds.drawTriangle(0,10, 8, 16, 0, 16, color);
     leds.show();   
    delay(1000);

    CRGB bkg;
    unsigned char c;
    int8_t size;
    color = CRGB::Black;
    bkg = CRGB::FireBrick;
    c = char(65);
    size = 1;
    leds.drawChar(10, 20, c, color, bkg, size); 

     leds.show();   
    delay(2000);
    leds.setRotation(0); //reset
    leds.fillScreen(CRGB::Black);
}

//========================== Bitmaps ======================================

// Draw a 1 - bit image(bitmap) at the specified(x, y) position from the
// provided bitmap buffer using the specified
// foreground color (unset bits are transparent).
void drawBitmap(int16_t x, int16_t y, const uint8_t * bitmap, int16_t w, int16_t h, CRGB color) {
    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (i & 7) byte <<= 1;
            else      byte = pgm_read_byte(bitmap + j * byteWidth + i / 8);
            if (byte & 0x80) leds.drawPixel(x + i, y + j, color);
        }
    }
}

// Draw a 1 - bit image(bitmap) at the specified(zx, y) position from the
// provided bitmap buffer (must be PROGMEM memory) using the specified
// foreground (for set bits) and background (for clear bits) colors.
void drawBitmap(int16_t x, int16_t y, const uint8_t * bitmap, int16_t w, int16_t h, CRGB color, CRGB bg) {
    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (i & 7) byte <<= 1;
            else      byte = pgm_read_byte(bitmap + j * byteWidth + i / 8);
            if (byte & 0x80) leds.drawPixel(x + i, y + j, color);
            else            leds.drawPixel(x + i, y + j, bg);
        }
    }
}

// drawBitmap() variant for RAM-resident (not PROGMEM) bitmaps.
void drawBitmap(int16_t x, int16_t y,
    uint8_t* bitmap, int16_t w, int16_t h, CRGB color) {

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (i & 7) byte <<= 1;
            else      byte = bitmap[j * byteWidth + i / 8];
            if (byte & 0x80) leds.drawPixel(x + i, y + j, color);
        }
    }
}

// drawBitmap() variant w/background for RAM-resident (not PROGMEM) bitmaps.
void drawBitmap(int16_t x, int16_t y,
    uint8_t* bitmap, int16_t w, int16_t h, CRGB color, CRGB bg) {

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (i & 7) byte <<= 1;
            else      byte = bitmap[j * byteWidth + i / 8];
            if (byte & 0x80) leds.drawPixel(x + i, y + j, color);
            else            leds.drawPixel(x + i, y + j, bg);
        }
    }
}


//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void drawXBitmap(int16_t x, int16_t y,
    const uint8_t* bitmap, int16_t w, int16_t h, CRGB color) {

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (i & 7) byte >>= 1;
            else      byte = pgm_read_byte(bitmap + j * byteWidth + i / 8);
            if (byte & 0x01) leds.drawPixel(x + i, y + j, color);
        }
    }
}

void invertTest() {
    char text[] = "inverse test";
    leds.fillScreen(CRGB::ForestGreen);
    leds.setTextColor(CRGB::Black);
    leds.setCursor(0,0);
    leds.print(text);
     leds.show();   
    delay(2000);
    leds.invertDisplay();
     leds.show();   
    delay(2000);
    leds.invertDisplay();
     leds.show();   
    delay(2000);
    leds.fillScreen(CRGB::Blue);
     leds.show();   
    delay(2000);
    leds.invertSquare(10, 10, 21, 21);
     leds.show();   
    delay(2000);
    leds.invertSquare(12, 12, 19, 19);
     leds.show();   
    delay(2000);
    leds.setRotation(0); //0-3 in 90 deg steps
    leds.fillScreen(CRGB::Black);
}


void Scottish_Flag() {
    leds.drawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(0, 0, 255));
    leds.drawRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
    leds.drawLine(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
    leds.drawLine(0, 1, leds.Width() - 1, leds.Height() - 2, CRGB(255, 255, 255));
    leds.drawLine(0, leds.Height() - 1, leds.Width() - 1, 0, CRGB(255, 255, 255));
    leds.drawLine(0, leds.Height() - 2, leds.Width() - 1, 1, CRGB(255, 255, 255));
     leds.show();   
    delay(5000);

    // Japanese Flag
    leds.drawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
    uint16_t r = min((leds.Width() - 1) / 2, (leds.Height() - 1) / 2) - 1;
    leds.drawFilledCircle((leds.Width() - 1) / 2, (leds.Height() - 1) / 2, r, CRGB(255, 0, 0));
     leds.show();   
    delay(5000);

    // Norwegian Flag
    int16_t x = (leds.Width() / 4);
    int16_t y = (leds.Height() / 2) - 2;
    leds.drawFilledRectangle(0, 0, x, y, CRGB(255, 255, 255));
    leds.drawFilledRectangle(0, 0, x - 1, y - 1, CRGB(255, 0, 0));
    leds.drawFilledRectangle(x + 3, 0, leds.Width() - 1, y, CRGB(255, 255, 255));
    leds.drawFilledRectangle(x + 4, 0, leds.Width() - 1, y - 1, CRGB(255, 0, 0));
    leds.drawFilledRectangle(0, y + 3, x, leds.Height() - 1, CRGB(255, 255, 255));
    leds.drawFilledRectangle(0, y + 4, x - 1, leds.Height() - 1, CRGB(255, 0, 0));
    leds.drawFilledRectangle(x + 3, y + 3, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
    leds.drawFilledRectangle(x + 4, y + 4, leds.Width() - 1, leds.Height() - 1, CRGB(255, 0, 0));
    leds.drawLine(0, y + 1, leds.Width() - 1, y + 1, CRGB(0, 0, 255));
    leds.drawLine(0, y + 2, leds.Width() - 1, y + 2, CRGB(0, 0, 255));
    leds.drawLine(x + 1, 0, x + 1, leds.Height() - 1, CRGB(0, 0, 255));
    leds.drawLine(x + 2, 0, x + 2, leds.Height() - 1, CRGB(0, 0, 255));
     leds.show();   
    delay(5000);
    leds.shiftLeft();
    leds.fillScreen(CRGB::Black);
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



void textTest() {

    //leds.setTextColor(CRGB::Red);
    leds.setTextColor(CRGB::Red, CRGB::Black);
    leds.setTextSize(1);
    leds.setTextWrap(false);
    char my_str[] = "Hello";

    for (uint8_t r = 0; r < 4; r++) {
        leds.setRotation(r);
         FastLED.clear(true);
         if (MATRIX_WIDTH < 16){
             leds.setCursor(1, 1);
         }
         else {
            leds.setCursor(1, 10);
         }
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
