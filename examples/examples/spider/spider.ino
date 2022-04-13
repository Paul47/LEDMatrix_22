/*
FastLED Arduino project Spider project on wokwi Arduino simulator
Source: @stepko on Wokwi channel 🙂from discord (https://discord.com/channels/787627282663211009/787630013658824707)
Wokwi Link: https://wokwi.com/arduino/projects/287701946731594253
*/


/* Configuring this library and FastLED requires a number of parameters and #defines
Configure your LED array in myConfiguration.h located in the  library folder.
This way you can reuse your configuration file(s) across all your sketches.
Consider renaming your configurations (and changing the #include "myConfiguration.h"
in the library folder.
*/

#include <LEDMatrix_22.h>

/* Special notes for LEDMatrix_22 conversion of this sketch
 1. We need a pointer to your leds[] array to access directly
    CRGB* pleds = leds.cLED;
    is added below. Now we change the array name leds and  leds[] to pleds and pleds[].

 2. blur2d (FastLED color utility reqires the function called XY(x, y).
   It's a virtual functikon so we can replace it with our own function.
   For LEDMatrix we need to use leds.mXY(x, y)

 3. Instead of FastLED's funtion XY(x, y) to map an led, we must use
    leds.mXY(x, y). This function handles all martix types (horiz, zigzag, tiled, etc).
*/

/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly


// Matrix size
#define HEIGHT  MATRIX_HEIGHT
#define WIDTH   MATRIX_WIDTH
#define myMATRIX_TYPE 1

//// ----------------------------- Spider ------------------------------
//(c)stepko
#define Speed 250
#define Koef 9 //1,3,5,7,9(standart),10,12
#define lines 7
#define Color 1 //By Kostyamat https://editor.soulmatelights.com/gallery/550
#define Fader 30 //By Kostyamat https://editor.soulmatelights.com/gallery/550
#define sx ((256 / (WIDTH / 2)) - (WIDTH / 16))
#define cy ((256 / (HEIGHT / 2)) - (HEIGHT / 16))

//local version of drawLine()
void drawLine(int x1, int y1, int x2, int y2,
  const CRGB & color) {
  int deltaX = abs(x2 - x1);
  int deltaY = abs(y2 - y1);
  int signX = x1 < x2 ? 1 : -1;
  int signY = y1 < y2 ? 1 : -1;
  int error = deltaX - deltaY;
  
  pleds[leds.mXY(x2, y2)] += color;
  while (x1 != x2 || y1 != y2) {
    pleds[leds.mXY(x1, y1)] += color;
    int error2 = error * 2;
    if (error2 > -deltaY) {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX) {
      error += deltaX;
      y1 += signY;
    }
  }
}


void draw() {
  fadeToBlackBy(pleds, NUM_LEDS, Fader);
  double t = millis() / (256 - Speed);
  for (uint8_t c = 0; c < lines; c++) {
    byte xx = sin8(t + (100 * c) * Koef) / sx;
    byte yy = cos8(t + (150 * c) * Koef) / cy;
    if (Color)
      drawLine(xx, yy, WIDTH - xx - 1, HEIGHT - yy - 1, CHSV(c * (256 / lines), 200, 255));
    else
      drawLine(xx, yy, WIDTH - xx - 1, HEIGHT - yy - 1, CHSV(0, 0, 255));
  }
  blur2d(pleds, WIDTH, HEIGHT, 32);
  delay(16);
}

void setup() {

    leds.addLeds();       //initiate LEDMatrix

}

void loop() {
  draw();
  leds.show();
} //loop

//blur2d (FastLED color utility reqires the function called XY(x, y).
//It's a virtual functikon so we can replace it with our own function.
//for LEDMatrix we need to use leds.mXY(x, y)
uint16_t XY(uint8_t x, uint8_t y) {
    leds.mXY(x, y);
}

