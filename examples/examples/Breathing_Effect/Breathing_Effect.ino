// Extra Example 2 - Breathing Effect 

//author unknown

#include <LEDMatrix_22.h>

#define VOLTS 5
#define MAX_AMPS 3600 //value in milliamps

#define DELAY   50     //set speed for "breathing rate"

cLEDMatrix leds;
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

void setup() {

    leds.addLeds();       //initiate LEDMatrix

    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
    leds.setBrightness(BRIGHTNESS);
    leds.clear();
}


void loop() { //Breathing effect!
   // strip();
    panel();

}

void strip(){
    for (int i=0; i<NUM_LEDS; i++) {
     pleds[i] = CRGB::Yellow; 
    }
    for (int i=0; i<BRIGHTNESS; i++) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }

    for (int i=BRIGHTNESS; i>=0; i--) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }

    //decrements down

    for (int i=NUM_LEDS - 1; i>=0; i--) { 
        pleds[i] = CRGB::Purple; 
    }
    for (int i=0; i<BRIGHTNESS; i++) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }
    for (int i=BRIGHTNESS; i>=0; i--) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }
}

void panel(){
    for (int x = 0; x < leds.matrixWidth; x++) {
        for (int y = 0; y < leds.matrixHeight; y++) {
            leds.drawPixel(x, y, CRGB::Yellow); 
        }
    }
    for (int i=0; i<BRIGHTNESS; i++) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }
    for (int i=BRIGHTNESS; i>=0; i--) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }

    //decrements down

    for (int x = 0; x < leds.matrixWidth; x++) {
        for (int y = 0; y < leds.matrixHeight; y++) {
            leds.drawPixel(x, y, CRGB::Purple); 
        }
    }
    for (int i=0; i<BRIGHTNESS; i++) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }
    for (int i=BRIGHTNESS; i>=0; i--) {
        leds.setBrightness(i);
        leds.show();
        FastLED.delay(DELAY);
    }
}
