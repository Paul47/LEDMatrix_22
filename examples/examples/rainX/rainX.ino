//Digital Rain implementation
//Original varsion from: Yaroslaw Turbin 24.08.2020
//https://vk.com/ldirko (not English)
//https://www.reddit.com/user/ldirko/ (English)

//NOTE: This example will run on any size matrix panel

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
If using led panels like 4x4 or 8x8 the you must define HAS_BLOCKS and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;

CRGB* pleds = leds.m_LED;   //pointer to your leds[] array to access directly

//----------rain variations------------------

boolean oneColor = true;    //if true set myHue next for color, else mulit-random colors
uint8_t  myHue = 200;       //changes color
uint8_t saturation = 150;   //100 - 200 works well
const uint8_t dripSpeed = 100;    //speed down panel - cannot chAge once set
const uint8_t changePattern = 30; // - cannot chAge once set
uint8_t order = 2;  //how orderly are the pixels? - 2 =streams, 255 = chaotic
uint8_t dripLen = 40;  //5 - 80 seem to work
uint8_t dropBlur = 1;  //1 - 80 needs larger array for higher values

byte rain[NUM_LEDS];

void setup() {
  leds.addLeds();       //initiate LEDMatrix
  rain_init();
  leds.clear();

  Serial.begin(115200);
  delay(1000);
}

void loop() {
    static uint8_t changeMode = 0;

    EVERY_N_MILLISECONDS(5000) {

        pt(changeMode);

        switch (changeMode) {
        case 0:
            oneColor = true;    //if true set myHue next for color, else mulit-random colors
            myHue = 200;       //changes color
            saturation = 150;
            order = 2;
        case 1:
            oneColor = false;        //changed
            myHue = 200;        //na
            saturation = 150;
            order = 2;
            changeMode++;
            break;
        case 2:
            oneColor = true;
            myHue = 110;
            saturation = 150;
            changeMode++;
            break;
        case 3:
            oneColor = false;
            myHue = 200;
            saturation = 150;
            order = 200;        //changed
            changeMode++;
            break;
        case 4:
            oneColor = true;
            myHue = 245;        //changed
            saturation = 150;
            order = 2;
            changeMode++;
            break;
        case 5:
            oneColor = true;
            myHue = 50;        //changed
            saturation = 150;
            order = 2;
            changeMode++;
            break;
        case 6:
            oneColor = false;        //changed
            myHue = 230;
            saturation = 150;
            order = 100;        //changed
            dropBlur = 255; //changed
            changeMode++;
            break;  
        case 7:         //reset
            oneColor = true;
            myHue = 200; 
            saturation = 150; 
            order = 2;
            dropBlur = 1;
            changeMode = 0;
            break;
        }
     }

  EVERY_N_MILLISECONDS(dripSpeed) {
     rain_updaterain();
     leds.LEDShow();
  }
  EVERY_N_MILLISECONDS(changePattern) {
      rain_changepattern();
  }
} //loop

void rain_changepattern () {
  int rand1 = random16 (NUM_LEDS);
  int rand2 = random16 (NUM_LEDS);
  if ((rain[rand1] == 1) && (rain[rand2] == 0) )   //simple get two random dot 1 and 0 and swap it,
  {
    rain[rand1] = 0;  //this will not change total number of dots
    rain[rand2] = 1;
  }
} //rain_changepattern

void rain_init() {                               //init array of dots. run once
  for (int i = 0; i < NUM_LEDS; i++) {
    if (random8(12) == 0) {          //2=mixed colors all pixels, 20 = sparce rain
      rain[i] = 1;  //random8(20) number of dots. decrease for more dots
    }
    else {
      rain[i] = 0;
    }
  }
} //rain_init

void rain_updaterain() {
    static uint8_t step = 1;

    if ( ! oneColor) {
        myHue = random8(255);         //mixed colors in each column
    }
    for (byte i = 0; i < leds.matrixWidth; i++) {
        for (byte j = 0; j < leds.matrixHeight; j++) {
            byte layer = rain[leds.mXY(i, ((j + step + random8(order) + leds.matrixHeight) % leds.matrixHeight))];   //fake scroll based on shift coordinate
            //random8(2) adds glitchy look
            if (layer) {
            // orighinal leds[leds.mXY((leds.matrixWidth - 1) - i, (leds.matrixHeight - 1) - j)] = CHSV(110, 255, saturation);
            leds.drawPixel((leds.matrixWidth - 1) - i, (leds.matrixHeight - 1) - j, CHSV(myHue, 255, saturation));
            }
        }
    }
    step ++;
    //array pointyer CRGB* required - use either pleds or leds.m_LED or leds[0]
     fadeToBlackBy(pleds, NUM_LEDS, dripLen);    // leds[0]
     blurRows(leds.m_LED, leds.matrixWidth, leds.matrixHeight, dropBlur); 
} //rain_updaterain

//XY() required by FastLED colorutils.cpp functions
int16_t XY(uint8_t x, uint8_t y) {
    return leds.mXY(x, y);
}
