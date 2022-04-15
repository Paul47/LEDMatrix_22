//THIS SKETCH WORKS BEST ON LARGE MATRIX PANELS LIKE 32X32 OR LARGER

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

// Matrix size
#define HEIGHT MATRIX_HEIGHT
#define WIDTH MATRIX_WIDTH

//// ----------------------------- Sinusoid ------------------------------
//(c)Stefan Petrick https://gist.github.com/StefanPetrick/dc666c1b4851d5fb8139b73719b70149
//https://editor.soulmatelights.com/gallery/587
//upd by Stepko
bool loadingFlag = true;

uint8_t speed = 15;
uint8_t scale = 55;
uint8_t amplitude = 255;
int8_t type = 5; //0-3 original/>3 other versions

static float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}
const int8_t semiHeightMajor  =  HEIGHT / 2 + (HEIGHT % 2);
const int8_t semiWidthMajor = WIDTH / 2  + (WIDTH % 2);
float e_s3_speed;
float e_s3_size;
float _scale;
void Sinusoid() {
  const uint8_t semiHeightMajor = HEIGHT / 2 + (HEIGHT % 2);
  const uint8_t semiWidthMajor = WIDTH / 2 + (WIDTH % 2);
  float e_s3_speed = 0.004 * speed + 0.015; // speed of the movement along the Lissajous curves
  float e_s3_size = fmap(amplitude, 1, 255, 3, 9); // amplitude of the curves
  uint8_t _scale = map8(scale, 50, 150);
  float time_shift = millis();
  switch (type) {
    case 0: //Sinusoid I
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy)))) / 32767.0);
          pleds[leds.mXY(x, y)].r = ~v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 134.3447 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 170.3884 * time_shift))) / 32767.0;
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy)))) / 32767.0);
          pleds[leds.mXY(x, y)].b = ~v;
          
        }
      }
      break;
    case 1: //it's not sinusoid II
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          CRGB color;
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (float(0.001 * time_shift * e_s3_speed) + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy)))) / 32767.0);
          color.r = ~v;
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 68.8107 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 65.534 * time_shift))) / 32767.0;
          v = 127 * (float(0.001 * time_shift * e_s3_speed) + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy)))) / 32767.0);
          color.r = (~v > color.r)? ~v : color.r;
          color.g = ~v >> 1;
          pleds[leds.mXY(x, y)] = color;
        }
      }
      break;
    case 2: //Sinusoid III
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy)))) / 32767.0);
          pleds[leds.mXY(x, y)].r = ~v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 68.8107 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 65.534 * time_shift))) / 32767.0;
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy)))) / 32767.0);
          pleds[leds.mXY(x, y)].g = ~v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 134.3447 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 170.3884 * time_shift))) / 32767.0;
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy)))) / 32767.0);
          pleds[leds.mXY(x, y)].b = ~v;
          
        }
      }
      break;
    case 3: //Sinusoid IV
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 100)) / 32767.0);
          pleds[leds.mXY(x, y)].r = ~v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 68.8107 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 65.534 * time_shift))) / 32767.0;
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 100)) / 32767.0);
          pleds[leds.mXY(x, y)].g = ~v;
          
          cx = (y - semiHeightMajor); // + float(e_s3_size * (sin16(e_s3_speed * 134.3447 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor); // + float(e_s3_size * (cos16(e_s3_speed * 170.3884 * time_shift))) / 32767.0;
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 100)) / 32767.0);
          pleds[leds.mXY(x, y)].b = ~v;
          
        }
      }
      break;
    case 4: //changed by stepko //anaglyph sinusoid
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 10)) / 32767.0);
          pleds[leds.mXY(x, y)].r = ~v;
          
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 1)) / 32767.0);
          pleds[leds.mXY(x, y)].b = ~v;
          
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 1)) / 32767.0);
          pleds[leds.mXY(x, y)].g = ~v;
          
        }
      }
      break;
    case 5: //changed by stepko //colored sinusoid
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 5)) / 32767.0);
          pleds[leds.mXY(x, y)].r = v;
          
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 10)) / 32767.0);
          pleds[leds.mXY(x, y)].b = v;
          
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 50)) / 32767.0);
          pleds[leds.mXY(x, y)].g = v;
          
        }
      }
      break;
    case 6: //changed by stepko //sinusoid in net
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 5)) / 32767.0);
          pleds[leds.mXY(x, y)].g = ~v;
          
          v = 127 * (1 + sinf(_scale * x * 10 + float(0.01 * time_shift * e_s3_speed)));
          pleds[leds.mXY(x, y)].b = ~v;
          
          v = 127 * (1 + sinf(_scale * y * 10 + float(0.011 * time_shift * e_s3_speed)));
          pleds[leds.mXY(x, y)].r = ~v;
          
        }
      }
      break;
    case 7: //changed by stepko //spiral
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          uint8_t v = 127 * (1 + sin16(127 * _scale * (2 * atan2(cy, cx) + hypot(cy, cx)) + (time_shift * e_s3_speed * 5)) / 32767.0);
          pleds[leds.mXY(x, y)].r = v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 68.8107 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 65.534 * time_shift))) / 32767.0;
          v = 127 * (1 + sin16(127 * _scale * (2 * atan2(cy, cx) + hypot(cy, cx)) + (time_shift * e_s3_speed * 5)) / 32767.0);
          pleds[leds.mXY(x, y)].b = v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 134.3447 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 170.3884 * time_shift))) / 32767.0;
          v = 127 * (1 + sin16(127 * _scale * (2 * atan2(cy, cx) + hypot(cy, cx)) + (time_shift * e_s3_speed * 5)) / 32767.0);;
          pleds[leds.mXY(x, y)].g = v;
          
        }
      }
      break;
    case 8: //changed by stepko //blobs
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          uint8_t v = 30 * (max(0, -hypot(cx, cy) + float(_scale * 0.07)));
          pleds[leds.mXY(x, y)].r = v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 68.8107 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 65.534 * time_shift))) / 32767.0;
          v = 30 * (max(0, -hypot(cx, cy) + float(_scale * 0.07)));
          pleds[leds.mXY(x, y)].b = v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 134.3447 * time_shift))) / 32767.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 170.3884 * time_shift))) / 32767.0;
          v = 30 * (max(0, -hypot(cx, cy) + float(_scale * 0.07)));
          pleds[leds.mXY(x, y)].g = v;
          
        }
      }
      break;
    case 9: //Fast Sinusoid III
      for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (127 - sin8(e_s3_speed * .48 * time_shift))) / 64.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (127 - cos8(e_s3_speed * .126 * time_shift))) / 64.0;
          int8_t v = sin8((_scale / 2) * sqrt(((cx * cx) + (cy * cy))));
          pleds[leds.mXY(x, y)].r = ~v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (127 - sin8(e_s3_speed * .168 * time_shift))) / 64.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (127 - cos8(e_s3_speed * .26 * time_shift))) / 64.0;
          v = sin8((_scale / 2) * sqrt(((cx * cx) + (cy * cy))));
          pleds[leds.mXY(x, y)].g = ~v;
          
          cx = (y - semiHeightMajor) + float(e_s3_size * (127 - sin8(e_s3_speed * .328 * time_shift))) / 64.0;
          cy = (x - semiWidthMajor) + float(e_s3_size * (127 - cos8(e_s3_speed * .416 * time_shift))) / 64.0;
          v = sin8((_scale / 2) * sqrt(((cx * cx) + (cy * cy))));
          pleds[leds.mXY(x, y)].b = ~v;
          
        }
      }
      break;
  }
}


void setup() {
     leds.addLeds();       //initiate LEDMatrix
    //pinMode(2, INPUT_PULLUP);                 //buton attached?
}

void loop() {       //timed pattern switch

     FRAMES_PER_SECOND(30) {     //alternate use of EVERY_N_MILLISECONDS - see configuration_22.h
        Sinusoid();
        leds.show();
    }
    EVERY_N_SECONDS(10) {   //alternate use of EVERY_N_MILLISECONDS from FastLED
        if (type >= 9) {
            type = 0;
        }
        else {
            type += 1;
        }
        speed = random8(5, 15);
        scale = random8(10, 55);
        amplitude = random8(100, 255);
    }
}

/*
void loop() {       //button press pattern switch
  bool buttonPressed = digitalRead(2) == LOW;
  Sinusoid();
  if (buttonPressed) {
    if (type >= 9) {
      type = 0;
    } else {
      type += 1;
    }
    leds.clear();
    delay(100);
  } else {
    leds.show();
  }
} //loop
*/

