/*
FastLED Arduino project Lumi AA_Lines on wokwi Arduino simulator
Source: @costyn on Wokwi channel 🙂from discord (https://discord.com/channels/787627282663211009/787630013658824707)
Wokwi Link: https://wokwi.com/arduino/projects/284541784463245837
*/

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

  //this function needs to be declared 1st for some reason
  // from: https://github.com/FastLED/FastLED/pull/202
  CRGB ColorFromPaletteExtended(const CRGBPalette16& pal, uint16_t index, uint8_t brightness, TBlendType blendType);

#define WIDTH   MATRIX_WIDTH
#define HEIGHT  MATRIX_HEIGHT

CRGBPalette16 currentPalette = {
  0xFF0000, 0x7F0000, 0xAB5500, 0x552A00, 0xABAB00, 0x555500, 0x00FF00, 0x007F00,
  0x00AB55, 0x00552A, 0x0000FF, 0x00007F, 0x5500AB, 0x2A0055, 0xAB0055, 0x55002A
};


void setup() {
    leds.addLeds();       //initiate LEDMatrix
}

uint16_t XY(uint8_t x, uint8_t y) {
  if (x >= WIDTH) return NUM_LEDS;
  if (y >= HEIGHT) return NUM_LEDS;
  return y * WIDTH + x;
}


void loop()
{
  uint32_t ms = millis();

  // draw the background animation, just like the XYmatrix example
  // but with added distortion
  uint32_t yHueDelta = (int32_t)sin16(ms * 11) * 3;
  uint32_t xHueDelta = (int32_t)cos16(ms * 11) * 3;
  uint32_t startHue = ms << 8;
  uint32_t lineStartHue = startHue - (HEIGHT + 1) / 2 * yHueDelta;
  int16_t yd2 = sin16(ms * 3) / 4;
  int16_t xd2 = sin16(ms * 7) / 4;
  for (byte y = 0; y < HEIGHT; y++) {
    uint32_t pixelHue = lineStartHue - (WIDTH + 1) / 2 * xHueDelta;
    uint32_t xhd = xHueDelta;
    lineStartHue += yHueDelta;
    yHueDelta += yd2;
    for (byte x = 0; x < WIDTH; x++) {
        if (leds.inBounds) {
            pleds[leds.mXY(x, y)] = ColorFromPaletteExtended(currentPalette, pixelHue >> 7, 255, LINEARBLEND);
        }
      pixelHue += xhd;
      xhd += xd2;
    }
  }

   // change `effect` every second repeating: 0,1,2,3,4,5,0,1,2... 
  static uint8_t effect = 0;
  EVERY_N_MILLIS(1000) {
    if (++effect > 5) effect = 0;
  }


  leds.show();
}

// from: https://github.com/FastLED/FastLED/pull/202
CRGB ColorFromPaletteExtended(const CRGBPalette16& pal, uint16_t index, uint8_t brightness, TBlendType blendType) {
  // Extract the four most significant bits of the index as a palette index.
  uint8_t index_4bit = (index >> 12);
  // Calculate the 8-bit offset from the palette index.
  uint8_t offset = (uint8_t)(index >> 4);
  // Get the palette entry from the 4-bit index
  const CRGB* entry = &(pal[0]) + index_4bit;
  uint8_t red1   = entry->red;
  uint8_t green1 = entry->green;
  uint8_t blue1  = entry->blue;

  uint8_t blend = offset && (blendType != NOBLEND);
  if (blend) {
    if (index_4bit == 15) {
      entry = &(pal[0]);
    } else {
      entry++;
    }

    // Calculate the scaling factor and scaled values for the lower palette value.
    uint8_t f1 = 255 - offset;
    red1   = scale8_LEAVING_R1_DIRTY(red1,   f1);
    green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
    blue1  = scale8_LEAVING_R1_DIRTY(blue1,  f1);

    // Calculate the scaled values for the neighbouring palette value.
    uint8_t red2   = entry->red;
    uint8_t green2 = entry->green;
    uint8_t blue2  = entry->blue;
    red2   = scale8_LEAVING_R1_DIRTY(red2,   offset);
    green2 = scale8_LEAVING_R1_DIRTY(green2, offset);
    blue2  = scale8_LEAVING_R1_DIRTY(blue2,  offset);
    cleanup_R1();

    // These sums can't overflow, so no qadd8 needed.
    red1   += red2;
    green1 += green2;
    blue1  += blue2;
  }
  if (brightness != 255) {
    // nscale8x3_video(red1, green1, blue1, brightness);
    nscale8x3(red1, green1, blue1, brightness);
  }
  return CRGB(red1, green1, blue1);
}
