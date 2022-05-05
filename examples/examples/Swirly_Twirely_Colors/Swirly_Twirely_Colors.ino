//Swirly, Twirly Colors 
//author unknown

#include <LEDMatrix_22.h>

#define VOLTS 5
#define MAX_AMPS 500 //value in milliamps

#define DELAY_SPEED  100         //mimmisec delay of "sweep"

cLEDMatrix leds;
CRGB* pleds = leds.cLED;   //pointer to your leds[] array to access directly

void setup() {

    leds.addLeds();       //initiate LEDMatrix

    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
    leds.setBrightness(10);
    leds.clear();
}

void loop() { //Swirly, twirly effect
    //stringDisplay();
    panelDisplay();

}

void stringDisplay(){
    for (int i=0; i<NUM_LEDS; i++) {
        pleds[i] = CRGB(0,255 - 4*i,4*i);
        leds.show();
        FastLED.delay(DELAY_SPEED); //even shorter delay this time
    }

//decrements down from end of lights
  for (int i=NUM_LEDS - 1; i>=0; i--) {
    pleds[i] = CRGB(4*i,0,255-4*i);
    leds.show();
    FastLED.delay(DELAY_SPEED); //even shorter delay this time
  }
}

void panelDisplay(){
    for (int x = 0; x < leds.matrixWidth; x++) {
        for (int y = 0; y < leds.matrixHeight; y++) {
            leds.drawPixel(x, y, CRGB(0, 255 - leds.matrixWidth * x, 4 * x));      //force more color variation based on x dim
        }
        leds.show();
        FastLED.delay(DELAY_SPEED); //even shorter delay this time
    }

    //decrements down from end of lights
    for (int x = leds.matrixWidth - 1; x >= 0; x--) {
        for (int y = leds.matrixHeight - 1 ; y >= 0 ; y--) {
            leds.drawPixel(x, y, CRGB(4 * x, 0, 255 - leds.matrixWidth * x));      //force more color variation based on x dim

        }
        leds.show();
        FastLED.delay(DELAY_SPEED); //even shorter delay this time
    }

}
