//example for extracting individual functions from the 113 
//Mark E. table top sketch. Please give credit to MArk in your sketchs using his code.
//Start with the table.h header, add the function and add setup() amd loop().
//you may need to add additional routines as needed like fakenoise() in this sketch.


#include <LEDMatrix_22.h>
#include "Table.h"

/*------------------- create the total matrix panel array -------------------
If using led panels like 4x4 or 8x8 the you must define HAS_TILES and configure panel sizes in configuration_22.h
the Class "leds" can be changed to whatever you choose. Example: "myLeds" and all calls are "myLeds." as in myLeds.addLeds();
*/

cLEDMatrix leds;

//you may need this if any routines use FastLED "utilities."
#define XY(x, y) leds.mXY(x, y) //this sketch adresses all leds directly

//=============== will need to add a function or two as needed ======================
void fakenoise()
{
  faudio[0] = random(1, MIDLY);
  for (uint16_t i = 1; i < leds.matrixWidth; i++) {
    faudio[i] = faudio[i - 1] + random(10) - 7;
    faudio[i] = constrain(faudio[i ], 1, leds.matrixHeight - 2);
  }
  if (counter % 600 == 0) {
    faudio[random(leds.matrixWidth)] = constrain( (leds.matrixHeight / - random(MIDLY >> 2, MIDLY)), 2, leds.matrixHeight - 1);
  }
}


void setup() {
    Serial.begin(115200);
    delay(1000);

    leds.addLeds();       //initiate LEDMatrix

  driftx = random8(4, leds.matrixWidth - 4);//set an initial location for the animation center
  drifty = random8(4, leds.matrixHeight - 4);// set an initial location for the animation center
  mstep = byte( 256 / min((leds.matrixWidth - 1), 255)); //mstep is the step size to distribute 256 over an array the width of the matrix
  steper = random8(2, 8);// steper is used to modify h to generate a color step on each move
  lastmillis = millis();
  lasttest = millis();

  //all initialized variable from MarkE. Some may not be needed but left in for now.
  hue = random8();//get a starting point for the color progressions
  adio = false; // turn off audio
  if (!adio) mscale = 2.2;
  fakenoise();
  cangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation gives a float value between -1 and 1
  sangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation in the y direction gives a float value between -1 and 1
  for (uint16_t i = 0; i < mpatterns; i++)  //set up initial delay for animation timing
  waiter[i] = 10;
  //whatami();//this prints out the current status of stuff  //ALWAYS DELETE THIS
  leds.clear();
}

/*
 * Pattern 111 bubbles2, 
 * Time (s) 23, 
 * Step 2, 
 * Adjunct 9 Flip, 
 * flip2, 
 * flip3: 101 
 * fader 10 
 * pre waiter  10 
 * target FPS   15  
 * Actual FPS: 16.08  
 * delta: -1.1   
 * end waiter: 62
*/


void loop() {

  EVERY_N_MILLISECONDS(300) { //set the display speed so you can do something else
    bubbles2();
    leds.show();
  }
}

void bubbles2() {
  if (counter == 0)
  {
    howmany = random (MIDLX >> 1, MIDLX );

    for (uint16_t u = 0; u < howmany; u ++) {
      xfire[u] = random(MIDLX / 2, MIDLX * 3 / 2 );
      yfire[u]  = random(MIDLY / 2, MIDLY * 3 / 2);
      fcolor[u] = random8(); //color
      fcount[u] = random(3, 2 + (MIDLX >> 1) ); //radius
      fpeed[u] = random(1, 4); //speed
      xslope[u] = random(12, 52) + random(4) * 64; //angle avoiding square
      if (random8() > 128) {
        rimmer[u] = true;
        xbouncer[u] = false;
        ybouncer[u] = false;

      }
      else {
        rimmer[u] = false;
        xbouncer[u] = true;
        ybouncer[u] = true;
      }
    }
  }

  for (uint16_t u = 0; u < howmany; u++) {
    if (xbouncer[u])
      xfire[u] = xfire[u] + fpeed[u] * (sin8(xslope[u]) - 128.0) / 128.0;
    else
      xfire[u] = xfire[u] - fpeed[u] * (sin8(xslope[u]) - 128.0) / 128.0;

    if (ybouncer[u])
      yfire[u] = yfire[u] + fpeed[u] * (cos8(xslope[u]) - 128.0) / 128.0;
    else
      yfire[u] = yfire[u] - fpeed[u] * (cos8(xslope[u]) - 128.0) / 128.0;

    if (xfire[u] > leds.matrixWidth - fcount[u]   || xfire[u] < fcount[u]  )
      xbouncer[u] = !xbouncer[u];

    if (yfire[u] > leds.matrixHeight - fcount[u] || yfire[u] < fcount[u]  )
      ybouncer[u] = !ybouncer[u];

    leds.drawFilledCircle(xfire[u],  yfire[u] , fcount[u], CHSV(fcolor[u] + h, 255, 255));
    if (rimmer[u])
      leds.drawCircle(xfire[u],  yfire[u] , fcount[u], CHSV(fcolor[u] + h + 128, 255, 255));
    else
      leds.drawCircle(xfire[u],  yfire[u], fcount[u], CRGB::White);
  }
}
