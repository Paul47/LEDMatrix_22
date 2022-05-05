/* noise16_3
 * 
 * By: Andrew Tuline
 *
 * Date: January, 2017
 *
 * A 16 bit noise routine with palettes. This was adapted by a routine originally by Stefan Petrick.
 * 
 */

 /* Configuring this library and FastLED requires a number of parameters and #defines
 Configure your LED array in Configuration.h located in the  library folder.
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

uint8_t max_bright = 128;

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

uint8_t maxChanges = 24;                                      // Number of tweens between palettes.



void setup() {

  Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
 
  leds.addLeds();

  currentPalette = PartyColors_p;
  currentBlending = LINEARBLEND;  
  
  leds.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

} // setup()



void loop() {

    EVERY_N_MILLISECONDS(50) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
    }

    EVERY_N_SECONDS(5) {             // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
    }
    //choose a fill pattern
    noise16_3();
   // noise16_2();
    //noise16_1();

 
  leds.show();
  
} // loop()


void noise16_3() {                                            // no x/y shifting but scrolling along 

  uint16_t scale = 1000;                                       // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = millis()*2;                             // increment z linear

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 7;    // get the noise data and scale it down

    uint8_t index = sin8(noise*3);                            // map led color based on noise data
    uint8_t bri   = noise;

    pleds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  
} // noise16_3()

void noise16_2() {                                            // just moving along one axis = "lavalamp effect"

    uint16_t scale = 1000;                                       // the "zoom factor" for the noise

    for (uint16_t i = 0; i < NUM_LEDS; i++) {

        uint16_t shift_x = millis() / 10;                         // x as a function of time
        uint16_t shift_y = 0;

        uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
        uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
        uint32_t real_z = 4223;

        uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down

        uint8_t index = sin8(noise * 3);                            // map led color based on noise data
        uint8_t bri = noise;

        pleds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.

    }

} // noise16_2()

void noise16_1() {                                            // moves a noise up and down while slowly shifting to the side

    uint16_t scale = 1000;                                      // the "zoom factor" for the noise

    for (uint16_t i = 0; i < NUM_LEDS; i++) {

        uint16_t shift_x = beatsin8(5);                           // the x position of the noise field swings @ 17 bpm
        uint16_t shift_y = millis() / 100;                        // the y position becomes slowly incremented


        uint16_t real_x = (i + shift_x) * scale;                    // the x position of the noise field swings @ 17 bpm
        uint16_t real_y = (i + shift_y) * scale;                    // the y position becomes slowly incremented
        uint32_t real_z = millis() * 20;                          // the z position becomes quickly incremented

        uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down

        uint8_t index = sin8(noise * 3);                           // map LED color based on noise data
        uint8_t bri = noise;

        pleds[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    }

} // noise16_1()

