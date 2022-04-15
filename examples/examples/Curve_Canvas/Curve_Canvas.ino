
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
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

//Idea from Ldir's F_lying
void drawPixelXYF(float x, float y, const CRGB & color) {
  if (x < 0 || y < 0 || x > ((float) MATRIX_WIDTH - 1) || y > ((float) MATRIX_HEIGHT - 1)) return;

  // extract the fractional parts and derive their inverses
  uint16_t xx = (x - (uint16_t) x) * 255, yy = (y - (uint16_t) y) * 255, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
#define WU_WEIGHT(a, b)((uint16_t)(((a) * (b) + (a) + (b)) >> 8))
  uint16_t wu[4] = {
    WU_WEIGHT(ix, iy),
    WU_WEIGHT(xx, iy),
    WU_WEIGHT(ix, yy),
    WU_WEIGHT(xx, yy)
  };
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint16_t i = 0; i < 4; i++) {
    uint16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    CRGB clr = leds.getPixel(xn, yn);

    if (xn < (uint16_t) MATRIX_WIDTH - 1 && yn < (uint16_t) MATRIX_HEIGHT - 1 && yn > 0 && xn > 0) {
      clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
      clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
      clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    } else {
      clr.r = qadd8(clr.r, (color.r * 85) >> 8);
      clr.g = qadd8(clr.g, (color.g * 85) >> 8);
      clr.b = qadd8(clr.b, (color.b * 85) >> 8);
    }
   leds.drawPixel(xn, yn, clr);
  }
#undef WU_WEIGHT
}

void drawCurve(float x,float y, float x2,float y2,float x3,float y3,CRGB col) 
{ 
    float xu = 0.0 , yu = 0.0 , u = 0.0 ; 
    for(u = 0.0 ; u <= 1.0 ; u += 0.01) 
    { 
        xu = pow(1-u,3)*x+3*u*pow(1-u,2)*x2+3*pow(u,2)*(1-u)*x3 
             +pow(u,3)*x3; 
        yu = pow(1-u,3)*y+3*u*pow(1-u,2)*y2+3*pow(u,2)*(1-u)*y3
            +pow(u,3)*y3; 
        drawPixelXYF(xu,yu,col); 
    } 
}
byte hue;
#define speed -6
void draw() {
  fadeToBlackBy(pleds, NUM_LEDS, 30);
    uint16_t x1 = beatsin8 (18+speed, 0, (MATRIX_WIDTH-2));
    uint16_t x2 = beatsin8 (23+speed, 0, (MATRIX_WIDTH-2)); 
    uint16_t x3 = beatsin8 (27+speed, 0, (MATRIX_WIDTH-2)); 

    uint16_t y1 = beatsin8 (20+speed, 0, (MATRIX_HEIGHT-2)); 
    uint16_t y2 = beatsin8 (26+speed, 0, (MATRIX_HEIGHT-2));
    uint16_t y3 = beatsin8 (15+speed, 0, (MATRIX_HEIGHT-2));

    drawCurve(x1,y1,x2,y2,x3,y3,CHSV(hue,255,255));
hue++;}

void setup() {
    leds.addLeds();
}

void loop() {
  draw();
  leds.show();
} //loop


uint16_t XY (uint16_t x, uint16_t y) {
    return leds.mXY(x, y);
}
