
//Tabletop.ino for LEDMJatrix_22 
//This version modiefied from code by Mark Estes who modified it from MarcMerlin
//this version has audio input turned off unless you are specific matrix size

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
#include "Table.h"

//------------------- create the total matrix panel array -------------------
#if HAS_EXTENDER || HAS_BLOCKS 
        // leds in block/tile width, height      led flow IN THE TILE      # tiles in the matrix h, v    How tiles flow in the matrix
cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, LEDS_IN_TILE, MATRIX_TILE_H_DIR, MATRIX_TILE_V_DIR, TILES_IN_MATRIX> leds;      //leds array configured
#else
cLEDMatrix<MATRIX_WIDTH_DIR, MATRIX_HEIGHT_DIR, MATRIX_TYPE> leds;  // create our matrix based on matrix definition only
#endif

//------------------------ Setup and loop ------------------------
void setup() {

    Serial.begin(115200);
    delay(1000);

#if HAS_EXTENDER
    /* 
    If you are using 2-wire leds (APA102 or similar) and the Dr. Oldies LED Strip Extenders, 
    define HAS_EXTENDER as true and configure the Extender section settings in Configuration_22.h.
    This is the only call you need to complete sewtup of the matrix is this line of leds.ExtInit
    */
    leds.ExtInit(NUM_LEDS, NUM_BANKS, NUM_STRIPS, BRIGHTNESS);      //Extender - init params for Extender functions
#else
    /*
    LEDMatrix_22 uses a slightly different .addLeds format then FastLED sketches. 
    FastLED requires different .addLeds format depending on the type of led: 1-wirw, 2-wire, or Neopixel
    Here are a few exmaples:
    1. LEDs requiring DATA + CLOCK such as APA102. Also called 2-wire LEDs
         FastLED.addLeds<CHIPSET, DATA, CLOCK, COLOR_ORDER, DATA_RATE_MHZ(SPI_MHZ)>(leds[0], NUM_LEDS).setCorrection(CORRECTION);   //defines need to be set in config_22.h
 
    2. LEDs that have no CLOCK such as NEOpixel or WS2812x. Also called 1-wire LEDs

    2a. If CHIPSET == NEOPIXEL, use this version. NOT Allowed with Teensyx MCUs
         Use the fomat in c. below, and use the real led type name WS2811, WS2812, or SK6812.
            FastLED.addLeds<NEOPIXEL, DATA>(leds[0], NUM_LEDS);
        2b. All other processors do not require this NUM_STRIPS constant
                FastLED.addLeds<CHIPSET, DATA, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(CORRECTION);   //defines need to be set in config_22.h
        2c. Teensy MCU boards
             #define (FASTLED_TEENSY4) or #define (FASTLED_TEENSY3) in Configuration_22.h
             Teensy MCU requires a NUM_STRIPS parameter at the beginning of the .addLeds call.
             FastLED requires the number of strips (NUM_STRIPS) as the 1st .addLeds parameter. 
             NUM_STRIPS is usually 1 unless you are doing parallel led strips.
                #ifndef NUM_STRIPS        
                   #define NUM_STRIPS 1    //in case NUM_STRIPS is not defined in configuration_22.h
                #endif
                FastLED.addLeds<NUM_STRIPS, CHIPSET, DATA, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(CORRECTION);   //defines need to be set in config_22.h
                //                  ^ for TEENSYx whithout NUM_STRIPS all leds on full white
     */

#endif

  driftx = random8(4, leds.matrixWidth - 4);//set an initial location for the animation center
  drifty = random8(4, leds.matrixHeight - 4);// set an initial location for the animation center
  mstep = byte( 256 / min((leds.matrixWidth - 1), 255)); //mstep is the step size to distribute 256 over an array the width of the matrix
  steper = random8(2, 8);// steper is used to modify h to generate a color step on each move
  lastmillis = millis();
  lasttest = millis();

  hue = random8();//get a starting point for the color progressions
  adio = false; // turn off audio
  if (!adio) mscale = 2.2;
  fakenoise();
  cangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation gives a float value between -1 and 1
  sangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation in the y direction gives a float value between -1 and 1
  for (uint16_t i = 0; i < mpatterns; i++)  //set up initial delay for animation timing
    waiter[i] = 10;
  whatami();//this prints out the current status of stuff
  leds.clear();
  smile2();// make one frame of the smile 2 pattern
  leds.LEDShow();
  delay(3000);
  //zoro();//helpful with matrix mapping and wiring
  //delay(60000);//helpful with pixel mapping

}

char readchar;

void loop()
{
  hue += 1;//increment the color basis
  h = hue;  //set h to the color basis
  if (counter % shifty == 0) driftx =  driftx + cangle;//move the x center every so often
  if (driftx > (leds.matrixWidth - MIDLX / 2))//change directin of drift if you get near the right 1/4 of the screen
    cangle = 0 - abs(cangle);
  if (driftx < MIDLX / 2)//change directin of drift if you get near the right 1/4 of the screen
    cangle = abs(cangle);
  if ((counter + shifty / 2) % shifty == 0) drifty =  drifty + sangle;//move the y center every so often
  if (drifty > ( leds.matrixHeight - MIDLY / 2))// if y gets too big, reverse
    sangle = 0 - abs(sangle);
  if (drifty < MIDLY / 2) {// if y gets too small reverse
    sangle = abs(sangle);
  }

  driftx = constrain(driftx, MIDLX - MIDLX / 3, MIDLX + MIDLX / 3);//constrain the center, probably never gets evoked any more but was useful at one time to keep the graphics on the screen....
  drifty = constrain(drifty, MIDLY - MIDLY / 3, MIDLY - MIDLY / 3);
  // test for frame rate,  every 15 frames
  // test for frame rate,  every 15 frames
  if (counter % 15 == 0) {
    fps = 15000.00 / (millis() - lasttest);
    lasttest = millis();
  //ledMatrix_22 - this code locks up the sketch at pattern #9 - just leave it out
    // adjust frame rate if it is too high or low
    if (fps > targetfps + 10)
      waiter[pattern] += 2;
    else if (fps > targetfps)
      waiter[pattern]++;
    if (fps <= targetfps && waiter[pattern] > 0)
      waiter[pattern]--;
    else if (fps < targetfps - 10 && waiter[pattern] > 2)
      waiter[pattern] -= 2;
//LEDMatrix_22
  }

  // this calls the pre frame fade,based on a byte "bfade" which is mostly random, but sometimes assignes for a specific effect
  switch (bfade)
  {
    case 0:
      lfado(3);//almost none  3/256 or 1.17%
      break;
    //cases 5,6,and 7 cause the fade to be non symetric, favoring one color over others, this adds a bit of fun to the ghosts
    case 5:
      bluefado(8 + (random8() >> 2));
      break;
    case 6:
      redfado(8 + (random8() >> 2));
      break;
    case 7:
      greenfado(8 + (random8() >> 2));
      break;

    default:
      lfado(bfade << 4);// 1 = 16,  4 = 64, this sets the amont of fade as a fraction over 256  range is 16/256 (6.25%) i.e. not much  to 64/256 (25%)or considerable
      break;
  }

  runpattern();//got generate a updated screen
  counter++;//increment the counter which is used for many things
  leds.LEDShow();
  delay(waiter[pattern]);//frame rate control
  if (Serial.available()) readchar = Serial.read(); else readchar = 0;          //s3 serial input check for user selection
  if (readchar > 31 || millis() > lastmillis + dwell)                           //when to change patterns - if so, gosub newpattern()
  {
    Serial.print("  Actual FPS: ");
    Serial.print (fps, 2);
    Serial.print("  delta: ");
    Serial.print(targetfps - fps, 1);
    Serial.print("   end waiter: ");
    Serial.println(waiter[pattern]);
    newpattern();
  }
}

void newpattern()//generates all the randomness for each new pattern
{
  int16_t new_pattern = 0;
  // Allows keeping a pattern index for selecting bestof patterns
  static uint8_t local_pattern = 0;

#ifndef BESTPATTERNS       
  local_pattern = pattern;
#endif

  if (readchar) {
    while ((readchar >= '0') && (readchar <= '9')) {         //s3 process selected pattern
      new_pattern = 10 * new_pattern + (readchar - '0');
      readchar = 0;
      if (Serial.available()) readchar = Serial.read();
    }

    if (new_pattern) {
      Serial.print("Got new pattern via serial ");
      Serial.println(new_pattern);
      local_pattern = new_pattern;
    } else {
      Serial.print("Got serial char ");
      Serial.println(readchar);
    }
  }

  if (readchar == 'n') { local_pattern++; Serial.println("Serial => next"); }
  else if (readchar == 'p') { local_pattern--; Serial.println("Serial => previous"); }
  else if (mixit) {//when set to true, plays the patterns in random order, if not, they increment, I start with increment and eventually flip this flag to make the progression random
    local_pattern = random(mpatterns);
    if (!adio)//this skips the audio based patterns unless audio is enabled
      while (pattern >= 29 && pattern <= 44)
        local_pattern = random(mpatterns);
  }
  else if (!new_pattern) {
    local_pattern ++;
  }

#ifdef BESTPATTERNS                                //bestpatterns - use the bestpattern index to limit selection
  // wrap around from 0 to last pattern.
  if (!new_pattern) {
    if (local_pattern >= 250) local_pattern = numbest-1;
    if (local_pattern >= numbest) local_pattern = 0;

    pattern = bestpatterns[local_pattern];
    Serial.print("Mapping best pattern idx ");
    Serial.print(local_pattern);
    Serial.print(" to ");
    Serial.println(pattern);
  } else {
    // In bestof mode, a specific pattern called by number is not sticky
    // next time around, the next bestof pattern will play
    pattern = new_pattern;
  }
#else
  if (local_pattern >= 250) local_pattern = lastpatindex-1;
  if (local_pattern >= lastpatindex) local_pattern = 0;

  pattern = local_pattern;
#endif

  leds.clear();

  targetfps = random(20, 30);
  bfade = random(1, 8);
  dot = random(2, 6);// controls the size of a circle in many animations
  if (max(leds.matrixHeight, leds.matrixWidth) >  64) dot += 4;
  if (max(leds.matrixHeight, leds.matrixWidth) > 128) dot += 4;
  
  adjunct = (random(3, 11));//controls which screen wide effect is used if any
  dwell = 1000 * (random(20, 40));//set how long the pattern will play

  ringdelay = random(30, 90);//sets how often one of the effects will happen
  bringdelay = random(70, 105);//sets how often one of the effects will happen

  counter = 0;// reset the counter with each new pattern
  steper = random(2, 8);//color tempo
  shifty = random (3, 12);//how often the drifter moves

  // sets up a few boolean switches for showing some effects or not
  blackringme = false;
  ringme = false;
  blender = random8();
  if (random8() > 180)
  {
    if (random8() > 120)
      blackringme = true;
    else
      ringme = true;
  }

  flip = false;
  flip2 = false;
  flip3 = false;
  if (random8() > 127)
    flip = true;
  if (random8() > 127)
    flip2 = true;
  if (random8() > 127)
    flip3 = true;

  hue += random(64);//picks the next color basis
  h = hue;
  // new drift factors for x and y drift
  cangle = (sin8(random(25, 220)) - 128.0) / 128.0;
  sangle = (sin8(random(25, 220)) - 128.0) / 128.0;
  whatami();//write to screen the lebels (if present) and set some parameters specific to the given pattern
  runpattern();//generate the first frame of the new pattern 

}


void whatami()// set some parameters specific to the pattern and send some data to the serial port for trouble shooting/ tweaking
{
  Serial.print("Pattern ");
  Serial.print(pattern);
  Serial.print(" ");
  lastmillis = millis();
  switch (pattern)
  {
    case 0:
      Serial.print("Dhole ");//label
      adjunct = 2;//specify the screen effect as 2 which in this case means HorizontalMirror()  followed by VerticalMirror()  if not specified, it will be what ever was assigned randomly for this pattern
      break;
    case 1:
      Serial.print("Inca ");
      adjunct = 3;
      break;
    case 2:
      Serial.print("Ringo ");
      adjunct = 3;
      break;

    case 3:
      Serial.print("Diag ");
      targetfps = 40;
      break;
    case 4:
      Serial.print("Rhole ");
      targetfps = 40;
      //  adjunct = 0;// as an alternative to this, simply do not call "adjuster()" when calling the specific pattern, either one works,
      break;
    case 5:
      Serial.print("Drft ");
      targetfps = 40;
      break;
    case 6:
      adjunct = 0;
      bfade = 0;

      Serial.print("DRIP ");
      ringme = false;
      blackringme = false;
      break;
    case 7:
      Serial.print("Volc ");
      targetfps = 40;
      break;
    case 8:
      Serial.print("Pym ");
      targetfps = 40;
      break;
    case 9:
      Serial.print("Pyms ");
      if (flip2) steper = steper * 4;
      break;
    case 10:

      driftx = MIDLX;//pin the animation to the center
      drifty = MIDLY;

      Serial.print("Corn ");
      break;
    case 11:
      adjunct = 0;
      targetfps = random(10, 40);
      Serial.print("Wt warp ");
      break;
    case 12:
      Serial.print("Fzy ");
      bfade = 0;
      targetfps = random (5, 15);
      break;
    case 13:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Conf ");
      targetfps = random (5, 15);
      break;
    case 14:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("spire2 ");
      break;
    case 15:
      Serial.print("streaker ");
      adjunct = 0;
      break;
    case 16:
      Serial.print("Sol FB ring ");
      adjunct = 0;
      bfade = constrain(bfade, 1, 3);
      targetfps = 15;
      break;
    case 17:
      Serial.print("Warp ");
      adjunct = 0;
      targetfps = 15;
      break;
    case 18:
      adjunct = 0;
      Serial.print("Siny ");
      break;
    case 19:
      //adjunct = 0;
      Serial.print("Sinx ");
      targetfps = 40;
      break;
    case 20:
      adjunct = 0;
      Serial.print("Trip ");
      targetfps = 40;
      break;
    case 21:
      adjunct = 0;
      Serial.print("Spire3 ");
      targetfps = 40;
      break;
    case 22:
      adjunct = 5;
      steper = mstep;
      Serial.print("BarG mirror");
      break;
    case 23:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("audio ");
      break;
    case 24:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Barg");
      steper = mstep;
      break;
    case 25:
      Serial.print("spire ");
      adjunct = 0;
      targetfps = 40;
      break;
    case 26:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Sinx ");
      steper = mstep;
      targetfps = 40;
      break;
    case 27:
      adjunct = 0;
      targetfps = random (5, 15);
      Serial.print("Conf1&2");
      break;

    case 28:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Barg3");
      steper = mstep;
      break;

    case 29:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Raudio");

      break;

    case 30:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Audio 2: RECT");

      break;

    case 31:

      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Audio3");

      break;

    case 32:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("Audio");

      break;

    case 33:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("RAudio 2");
      break;

    case 34:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("RAudio 3");
      break;

    case 35:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("RAaudio 4");
      break;

    case 36:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("RAudio 5");
      break;

    case 37:

      ringme = false;
      blackringme = false;
      Serial.print("RAudio");
      break;

    case 38:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("RAudio 6");
      break;

    case 39:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("raudio 2 and raudio 3");
      break;

    case 40:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("raudio 6");
      break;

    case 41:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("RAudio 4");
      break;

    case 42:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      Serial.print("RAudio 5");
      break;

    case 43:

      ringme = false;
      blackringme = false;
      Serial.print("RAudio 2");
      break;

    case 45:
      targetfps = random(5, 10);
      bfade = random (0, 3);
      break;

    case 49:
      targetfps = random(4, 8);
      break;

    case 50:
      targetfps = 60;
      break;
    case 51:
      targetfps = 15;
      bfade = 2;
      break;
    case 52:
      targetfps = 15;
      bfade = 3;
      break;
    case 53:
      targetfps = random (10, 40);
      bfade = 2;
      break;
    case 54:
      targetfps = random (5, 30);
      bfade = 1;
      break;
    case 55:
      targetfps = 50;
      break;
    case 56:
      Serial.print("bounce ");
      break;
    case 58:
      Serial.print("sld bbox ");
      adjunct = 0;
      break;
// Remove delay on pattern only based on ringer/bkringer
    case 60:
      ringdelay = 0;
      bringdelay = 0;
      break;
    case 62:
      targetfps = 30;
      break;
    case 65:
      targetfps = random (5, 15);
      break;
    case 70:
      Serial.print("spin2 ");
      //targetfps = random (5, 15);
    break; case 71:
      Serial.print("xspin ");
      //targetfps = random (5, 15);
      break;
    case 72:
      Serial.print("homer ");
      targetfps = random (5, 15);
      break;
    case 74:
      Serial.print("fireball w/ eff ");
      // targetfps = random (5, 15);
      break;
    case 75:
      targetfps = 60;
      break;
    case 76:
      targetfps = random(5, 12);
      break;
    case 77:
      Serial.print("whtwarp w/ eff ");
      targetfps = random(5, 12);
      break;
    case 80:
      Serial.print("starz ");
      targetfps = random(5, 15);
      break;
    case 84:
      targetfps = random(15, 35);
      break;
    case 86:
      Serial.print("starbounce ");
      //targetfps = random (5, 15);
      break;

    case 88:
      Serial.print("triforce ");
      break;
    case 89:
      targetfps = random(6, 20);
      break;
    case 92:
      targetfps = 10;

      bfade = 0;
      adjunct = 0;
      Serial.print("DRIP2 ");
      ringme = false;
      blackringme = false;

      break;
    case 93:
      adjunct = 0;
      Serial.print("spoke");
      ringme = false;
      blackringme = false;
      targetfps = random (5, 15);
      break;
    case 94:
      targetfps = 15;
      break;

    case 95:
      bfade = 0;
      targetfps = 10;
      break;
    case 98:
      targetfps = random (5, 15);
      break;
    case 99:
      targetfps = random (5, 15);
      break;
    case 101:
      Serial.print("conf4 warp");
      targetfps = random(5, 15);
      break;
    case 104:
      Serial.print("circlearc");
      //targetfps = random(5, 15);
      break;
    case 105:
      targetfps = 60;
      break;
    case 106:
      targetfps = 20;
      break;
    case 107:
      targetfps = random(5, 15);
      break;
    case 108:
      targetfps = random(5, 10);
      if (adjunct < 2) adjunct += 2;
      break;

    case 109:
      targetfps = random(10, 20);
      break;
    case 110:
      Serial.print("bubbles");
      targetfps = random(15, 25);
      bfade = 10;//lots
      break;
    case 111:
      Serial.print("bubbles2");
      targetfps = random(10, 20);
      bfade = 10;//lots
      break;
    case mpatterns-1:
      Serial.print("seasick5");
      targetfps = random(10, 20);
      //bfade = 10;//lots
      break;

    default:
      adjunct = 0;
      ringme = false;
      blackringme = false;
      break;
  }
  Serial.print(", Time (s) ");
  Serial.print((dwell) / 1000, 1);
  Serial.print(", Step ");
  Serial.print(steper);
  Serial.print(", Adjunct ");
  Serial.print(adjunct);
  Serial.print(" Flip, flip2, flip3: ");
  Serial.print(flip);
  Serial.print(flip2);
  Serial.print(flip3);
  Serial.print(" fader " );
  Serial.print(bfade);
  Serial.print(" pre waiter  " );
  Serial.print(waiter[pattern]);
  Serial.print(" target FPS   " );
  Serial.print(targetfps);
#ifdef ARDUINOONPC
  Serial.println(""); // ArduinoOnPC doesn't output a line before newline, so output it early.
#endif
}
void runpattern() {//here the actuall effect is called based on the pattern number,  
                   //sometimes more than one is called, sometimes the logical switches, 
                  //dictate what is called
  switch (pattern) {
    case 0:
     // Diamondhole();      //not very interesting
      if (flip2)
        bkstarer();
      else
        boxer();

      adjuster();// apply the screenwide effect
      break;
    case 1:
      Inca();
      if (flip3)bkringer();
      else
      bkboxer();
      leds.QuadrantBottomTriangleMirror();
      break;
    case 2:
      Ringo();
      if (blackringme)bkringer();
      else
        bkboxer();
      if (ringme)ringer();
      else
        boxer();
      if (flip)
        leds.QuadrantBottomTriangleMirror();
      break;
    case 3:
      diagonally();
      adjuster();
      break;
    case 4:
      Roundhole();
      if (flip3)
        bkstarer();
      else
        bkringer();
      // boxer();
      adjuster();
      break;
    case 5:
      drifter();
      if (flip2)
      {
        if (blackringme)bkringer();
        else
          bkboxer();
        if (ringme)ringer();
        else
          boxer();
      }
      adjuster();
      break;
    case 6:
      drip();
      break;
    case 7:
      volcano();
      if (flip2)
      {
        if (blackringme)bkringer();
        else
          bkboxer();
        if (ringme)ringer();
        else
          boxer();
      }
      adjuster();
      break;
    case 8:
      pyrimid();
      if (flip2)
      {
        if (blackringme)bkringer();
        else
          bkboxer();
        if (ringme)ringer();
        else
          boxer();
      }
      adjuster();
      break;
    case 9:
      solidpyrimid();
      if (flip2)
      {
        if (blackringme)bkringer();
        else
          bkboxer();
        if (ringme)ringer();
        else
          boxer();
      }
      adjuster();
      break;
    case 10:
      corner();
      driftx = MIDLX;
      drifty = MIDLY;
#if 0
      if (blackringme)bkringer();
      else
        bkboxer();
      if (ringme)ringer();
      else
        boxer();
#endif
      bkringer();
      ringer();
      //bkboxer();
      //boxer();
 //     adjuster();
      break;
    case 11:
      whitewarp();

#if 0
      if (ringme)ringer();
      else
        starer();
#endif
      ringer(); // exploding circles
//      starer(); // exploding triangle
      break;
    case 12:
      fuzzy();
      break;
    case 13:

      confetti();

      break;
    case 14:

      if (flip3 && flip2)
        ringer();
      spire2();
      if (flip && flip2) adjuster();
      break;
    case 15:
      //solid();

      streaker();
      break;
    case 16:


      fireball();
      ringer();
      break;
    case 17:


      warp();
      if (flip && flip3)
        nringer(counter % leds.matrixWidth);
      else if (flip2)
        starer();
      break;
    case 18:

      siny();
      if (flip && flip3) adjuster();
      break;
    case 19:

      sinx();
      if (flip) adjuster();
      break;
    case 20:

      triple();
      if ((flip && flip2) || (flip && flip3) ) adjuster();
      break;
    case 21:

      spire3();
      if (flip && flip2 && flip3) adjuster();
      break;

    case 22:
      Bargraph();
      adjuster();
      break;
    case 23:

      roger();
      if (flip && (flip2 || flip3)) adjuster();
      break;

    case 24:
      Bargraph();
      break;

    case 25:
      spire();
      //if (flip3)
        adjuster();
      break;

    case 26:
      sinx();
      if (flip2 && flip3)
        bkstarer();
      break;

    case 27:
      confetti4();
      confetti2();

      break;
    case 28:
      if (flip2 && flip3)
        solid2();
      else if (flip)solid();
      Bargraph();
      break;

    // start of audio sectionnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn

    case 29:
      Raudio();
      break;

    case 30:
      audio2();
      break;

    case 31:
      audio3();
      break;

    case 32:
      audio();
      break;

    case 33:
      Raudio2();
      break;

    case 34:
      Raudio3();
      break;


    case 35:
      Raudio4();
      break;

    case 36:
      Raudio5();
      break;

    case 37:
      Raudio();
      adjuster();
      break;

    case 38:

      whitewarp();
      Raudio6();

      break;

    case 39:
      Raudio3();
      Raudio2a();
      break;

    case 40:
      Raudio6();
      adjuster();
      break;

    case 41:
      Raudio4();
      break;

    case 42:
      Raudio5();
      break;

    case 43:
      Raudio2();
      adjuster();
      break;

    case 44:
      Raudio4();
      Raudio2();
      break;
    // end of audio sectionnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn

    case 45:
      confetti2();
      if (flip2)
        confetti4();
      else
        confetti3();

      break;


    case 46:
      if (flip2) bkringer();
      spin2();
      break;

    case 47:
      streaker();
      if (flip && flip2 ) confetti4();
      else
        confetti2();
      break;

    case 48:

      if (flip && flip2 ) confetti4();
      fireball();
      break;


    case 49:
      confetti2();

      break;

    case 50:
      seasick3();
      break;

    case 51:

      magictime();
      if (flip3)
        bkringer();
      else
        bkboxer();
      break;

    case 52:
      rmagictime();
      bkboxer();
      starer();
      if (flip && !flip2)
        adjuster();
      break;

    case 53:

      rmagictime();
      boxer();
      bkstarer();

      adjuster();
      break;

    case 54:


      splat();

      break;

    case 55:
      seasick2();
      break;

    case 56:

      bounce();
      if (flip2 && flip3)
        ringer();
      if (!flip3 && flip)
        bkboxer();
      break;

    case 57:

      bounce();
      if (flip && flip2 && flip3) adjuster();
      break;

    case 58:
      boxer();
      bkringer();
      break;


    case 59:
      fireball();
      if (flip2 && flip) ringer();
      if (flip3) bkringer();

      break;

    case 60:

      if (flip3)
        bkringer();
      else
        bkboxer();
      //FastLED.delay(10);
      if (flip2)
        ringer();
      else
        starer();
      //adjuster(); // this never does anything
      break;


    case 61:
      starer();
      bkboxer();
      //FastLED.delay(20);
      break;

    case 62:

      if (flip)
        boxer();
      else
        ringer();
      if (flip2)
        bkstarer();
      else
        bkboxer();
      if (flip3)
        warp();
      else {
        confetti2();
        adjuster();
      }
      break;

    case 63:
      ringer();
      bkstarer();
      confetti4();
      adjuster();
      break;

    case 64:
      triple();
      if (flip2)ringer();
      break;

    case 65:
      if (flip && flip2)
        smile();
      else if (flip && !flip2)
        smile2();
      else if (!flip && flip2)
        smile3();
      else
        smile4();
      break;

    case 66:
      hypnoduck();
      break;

    case 67:
      hypnoduck2();
      break;

    case 68:
      roger();
      adjuster();
      //FastLED.delay(20);
      break;


    case 69:
      if (flip3) {
        starer();
        bkringer();
      }
      else {
        bkboxer();
        ringer();
      }
      adjuster();
      break;

    case 70:
      if (flip2) boxer(); // outward going box
      else if (flip3) bkringer(); // back collapsing circles
      spin2();
      if (!flip && flip2 && !flip3) adjuster();
      break;

    case 71:
      xspin();
      if (flip3) adjuster();
      break;

    case 72:
      homer();
      break;

    case 73:
      homer2();
      break;

    case 74:
      if (flip2) bkstarer(); else bkringer();
      fireball();
      adjuster();
      break;

    case 75:
      seasick();
      break;

    case 76:
      confetti3();
      //FastLED.delay(10 + random8() >> 3);
      break;

    case 77:
      if (flip2) bkstarer(); else bkringer();
      whitewarp();
      break;

    case 78:
      solid2();
      drawtriangle();
      break;

    case 79:
      starz3();
      break;

    case 80:
      starz();
      break;

    case 81:
      starz2();
      break;

    case 82:
      swirl2();
      break;

    case 83:
      bkstarer();
      boxer();
      adjuster();
      break;

    case 84:
      fireball();
      break;

    case 85:
      wheelz();
      break;

    case 86:
      starbounce();
      break;

    case 87:
      wheelz2();
      break;

    case 88:
      triforce();
      adjuster();

      break;

    case 89:
      swirl();
      break;

    case 90:
      swirl4();
      break;

    case 91:
      starer();
      bkboxer();
      break;

    case 92:

      drip();
      mirror();
      break;

    case 93:
      spoker();
      break;

    case 94:
      swirl3();
      break;

    case 95:

      drip();
      mirror();
      break;

    case 96:
      triangler();
      bkringer();
      break;

    case 97:
      if (flip)
        bfade = 1;

      xspin();
      if (flip2 && flip3)
        bkstarer();
      break;

    case 98:
      swirl5();
      break;

    case 99:
      spoker3();
      break;

    case 100:
      starer();
      bkboxer();
      adjuster();

      break;


    case 101:
      confetti4();
      warp();
      break;

    case 102:
      seasick4();
      break;

    case 103:
      hypnoduck3();
      break;

    case 104:
      circlearc();
      break;

    case 105:
      hypnoduck4();
      break;

    case 106:
      circlearc();

      break;

    case 107:
      if (flip)
        rmagictime();
      triangler();
      bkboxer();
      confetti2();
      adjuster();// hey long time no see....

      break;


    case 108:
      whitewarp();
      rmagictime();
      if (flip && !flip2 && flip3)
        bkringer();
      break;

    case 109:
      warp();
      xspin();
      break;

    case 110:
      if (flip3) solid2();
      bubbles();
      break;

    case 111:
      if ( !flip2)
        solid2();
      bubbles2();
      break;

    case mpatterns-1:
      seasick5();
      break;

    default:
      warp();
      if (flip2)
        ringer();
      else starer();
      adjuster();
      break;
  }
  ////FastLED.delay(frames);

}



void Diamondhole()//eff 0
{
  // ** Fill LED's with diagonal stripes
  for (uint16_t x = 0; x < (leds.matrixWidth  + leds.matrixHeight); x++)
  {
    leds.drawLine(x - leds.matrixHeight, leds.matrixHeight - 1, x, 0, CHSV(h * 2 + x * steper, 255, 255 - leds.matrixHeight * 2 + 2 * x));

  }

}

void Inca()//eff 1
{
  // ** Fill LED's with horizontal stripes
    for (uint16_t y = 0; y < leds.matrixHeight; y += 3) // += dot)
  {
    leds.drawLine(0, y, leds.matrixWidth  - 1, y, CHSV(h * 2 + y * steper , 255, 255));
    leds.drawLine(0, y+1, leds.matrixWidth - 1, y, CHSV(h * 2 + y * steper, 255, 255));
  }

}

void Ringo()// eff 2
{
  for (uint16_t x = 0; x < (leds.matrixWidth * 2); x += dot)
  {
    leds.drawLine(x - leds.matrixHeight, leds.matrixHeight - 1, x, 0, CHSV(h + x * steper / 3 , 255, 255));

  }

}

void diagonally()// eff 3
{

  for (uint16_t y = 0; y <= leds.matrixHeight * 2 / 3  + 4 ; y += dot)
  {
    leds.drawCircle(driftx, drifty , leds.matrixHeight * 2 / 3 + 3 - y, CHSV(h * 3 + y * 2, 255, 255 - y));
  }
}


void Roundhole()// eff4
{
  solid();
  for (uint16_t y = 0; y < byte(leds.matrixHeight < 255 ? leds.matrixHeight : 255) ; y += dot)
  {
    leds.drawCircle(driftx, drifty, leds.matrixHeight - y, CHSV(h * 2 + y * steper, 255, 255));

  }
}

void drifter()//pattern=5
{

  if (flip2 || flip3)
    solid();
  for (uint16_t y = 0; y <= leds.matrixHeight  + 1 ; y += dot - 1)
  {
    leds.drawFilledCircle(driftx, drifty , leds.matrixHeight  + 5 - y, CHSV(h + y * steper, 255, 255));
  }
}




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


void volcano()//pattern=7
{
  if (flip3 || flip2)
    solid2();
  for (uint16_t y = 0; y <  leds.matrixWidth  ; y += dot - 1)
  {
    leds.drawFilledCircle(driftx , drifty  ,  leds.matrixWidth  - y, CHSV(h + y * steper, 255, 255));

  }

}

void pyrimid()//pattern=8
{
    solid2();
  for (int16_t y = leds.matrixWidth; y > 1; y -= dot)   //ledMatrix_22 made y int from unint or can lock up
  {
    leds.drawFilledCircle(driftx, drifty,  y, CHSV(h + 85 + y * steper, 255, 255));

  }
}

void solidpyrimid()//pattern=9
{
  if (counter == 0)
    rr = random(200, 255);
  for (int16_t y = leds.matrixWidth; y > 0; y -= dot - 1)   //ledMatrix_22 made y int from unint or can lock up
  {
    leds.drawRectangle(0 , 0,  leds.matrixWidth - y + 1, leds.matrixHeight - y + 1, CHSV(h + y * steper, rr, 255));
  }
}

void solid2()
{
  if (counter == 0)
    rr = random8();
  leds.drawFilledRectangle(0 , 0,  leds.matrixWidth, leds.matrixHeight, CHSV(rr -  h, 255, 90));
}

void solid()
{
  if (counter == 0)
    rr = random8();
  leds.drawFilledRectangle(0 , 0,  leds.matrixWidth, leds.matrixHeight, CHSV(rr + h, 255, 90));
}


void fuzzy() {  ///pattern 12

  leds.drawPixel(random(leds.matrixWidth), random(leds.matrixHeight), CHSV(3 * h, 255, 255));
  leds.drawPixel(random(leds.matrixWidth), random(leds.matrixHeight), CHSV(3 * h + (random8() >> 4), 200 + random(55), 255));
  leds.drawPixel(random(leds.matrixWidth), random(leds.matrixHeight), CHSV(3 * h - (random8() >> 4), 200 + random(55), 255));
  leds.drawPixel(random(leds.matrixWidth), random(leds.matrixHeight), CHSV(3 * h - (random8() >> 4), 200 + random(55), 255));
}
 
void corner()//pattern=10
{
  leds.drawFilledRectangle(0 , 0,  leds.matrixWidth, leds.matrixHeight, CHSV(h, 255, 255));
  h += steper;

  for (uint16_t y = 0; y <= leds.matrixWidth / 2 - 1 ; y++)
  {
    leds.drawFilledCircle(MIDLX, MIDLY, (leds.matrixWidth / 2 + 1 - y) / 3, CHSV(h  + y * steper, 255, 255));
    leds.drawFilledCircle(0, 0, leds.matrixWidth / 2 - y, CHSV(64 + h + y * steper, 255, 255));
    leds.drawFilledCircle(0, leds.matrixHeight - 1, leds.matrixWidth / 2 - y, CHSV(h - 64 + y * steper, 255, 255));
    leds.drawFilledCircle(leds.matrixWidth  - 1, 0, leds.matrixWidth / 2 - y, CHSV(h - 64 + y * steper, 255, 255));
    leds.drawFilledCircle(leds.matrixWidth  - 1, leds.matrixHeight - 1, leds.matrixWidth / 2 - y, CHSV(h + 64 + y * steper, 255, 255));
  }
}

void fireball() {// with colored dots in pairs sometimes
  if (counter == 0)
  {
    ccoolloorr = random8();
    howmany = random(leds.matrixWidth , leds.matrixWidth * 2);
    for (uint16_t u = 0; u < howmany; u++) {
      xfire[u] = driftx;
      yfire[u] =   drifty;
      fpeed[u] = random(1, 5);
      blender = random8();
      yslope[u] = (cos8(blender) - 128.0) / 128.0;
      xslope[u] = (sin8(blender) - 128.0) / 128.0;

      if (flip)
        fcolor[u] = ccoolloorr + random(42);
      else
        fcolor[u] = random8();
    }
  }

  for (uint16_t u = 0; u < howmany; u++) {
    leds.drawPixel(xfire[u], yfire[u], CHSV(fcolor[u], 255, 255));
    xfire[u] = xfire[u] + xslope[u];
    yfire[u] = yfire[u] + yslope[u];
    fcount[u] = 0;
    if (yfire[u] < 0 || yfire[u] > leds.matrixWidth - 1 || xfire[u] < 0 || xfire[u] >  leds.matrixWidth - 1) {
      xfire[u] = driftx;
      yfire[u] = drifty;
      if (flip)
        fcolor[u] = ccoolloorr + random(16);
      else
        fcolor[u] = random8();

      blender = random8();
      yslope[u] = (cos8(blender) - 128.0) / 128.0;
      xslope[u] = (sin8(blender) - 128.0) / 128.0;

    }

  }
}

void streaker() {
  if (counter == 0)
  {
    solid();
    ccoolloorr = random8();
    howmany = random(leds.matrixWidth * 2 / 3, leds.matrixWidth * 3 / 2);
    for (uint16_t u = 0; u < howmany; u++) {
      xfire[u] = driftx;
      yfire[u] = drifty;
      fpeed[u] = random(1, 5);
      blender = random8();
      yslope[u] = (cos8(blender) - 128.0) / 128.0;
      xslope[u] = (sin8(blender) - 128.0) / 128.0;
      if (flip)fcolor[u] = ccoolloorr + random(42);
      else
        fcolor[u] = random8();

    }
  }

  for (uint16_t u = 0; u < howmany; u++)
  {
    leds.drawPixel(xfire[u], yfire[u], CHSV(fcolor[u], 255, 255));
    xfire[u] = xfire[u] + xslope[u] * fpeed[u];
    yfire[u] = yfire[u] + yslope[u] * fpeed[u];
    fcount[u] = 0;

    if (yfire[u] < 0 || yfire[u] >  leds.matrixWidth - 1 || xfire[u] < 0 || xfire[u] >  leds.matrixWidth - 1)
    {
      xfire[u] = driftx;
      yfire[u] = drifty;
      if (flip)
        fcolor[u] = ccoolloorr + random(16);
      else
        fcolor[u] = random8();
      blender = random8();
      fpeed[u] = random(1, 5);
      yslope[u] = (cos8(blender) - 128.0) / 128.0;
      xslope[u] = (sin8(blender) - 128.0) / 128.0;
    }
  }
}

void ringer() {
  if (counter >= ringdelay)
  {
    if (counter - ringdelay <= leds.matrixWidth)
    {
      if (flip || flip2)
        leds.drawCircle(driftx, drifty , counter - ringdelay, CHSV( h + 85, 255, 255));
      else
        leds.drawCircle(driftx, drifty , counter - ringdelay, CRGB::White);
    }
    else
      ringdelay = counter + random(30, 80);
  }
}

void bkringer() {
  if (counter >= bringdelay)
  {
    if (counter - bringdelay <= (unsigned int) leds.matrixWidth  + 13)
    {
      leds.drawCircle(driftx, drifty , (leds.matrixWidth + 12 - counter + bringdelay), CHSV(h + 60, 255, 255));
      leds.drawCircle(driftx, drifty, (leds.matrixWidth + 8 - counter + bringdelay), CHSV(h + 70 , 255, 255));
      leds.drawCircle(driftx, drifty , (leds.matrixWidth + 4 - counter + bringdelay), CHSV(h + 80 , 255, 255));
      leds.drawCircle(driftx, drifty , (leds.matrixWidth  - counter + bringdelay), CHSV(h + 90 , 255, 255));
    }
    else {
      bringdelay = counter + random(60, 120);

    }
  }
}

void sinx() {
  if (counter == 0) {
    radius2 = random((MIDLX >> 1) + 1, MIDLX + 4);
    radius3 = random (3, MIDLX >> 1);
  }

  leds.drawCircle(driftx + ((radius3 * (cos8(h * 3) - 128)) >> 7), drifty + ((radius3 * (sin8(h * 3) - 128)) >> 7), radius2, CHSV(h , 255, 255));
  leds.drawCircle(driftx + (radius3 * ((cos8(h * 3 + 128) - 128)) >> 7) , drifty + ((radius3 * (sin8(h * 3 + 128) - 128)) >> 7), radius2, CHSV(h + 85 , 255, 255));
  if (flip3) {
    leds.drawCircle(driftx + ((radius3 * (cos8(h * 3) - 128)) >> 7), drifty + ((radius3 * (sin8(h * 3) - 128)) >> 7), radius2 / 2, CHSV(h + 85 , 255, 255));
    leds.drawCircle(driftx + (radius3 * ((cos8(h * 3 + 128) - 128)) >> 7) , drifty + ((radius3 * (sin8(h * 3 + 128) - 128)) >> 7), radius2 / 2, CHSV(h  , 255, 255));
  }
}

void drip() {
  if (counter == 0)
  {
    // leds.drawFilledRectangle(0, 0, leds.matrixWidth - 1, leds.matrixHeight - 1, CHSV(h * 2, 255, 100));
    xslope[0] = random8();
    if (flip2)
      xslope[0] = 42;
    for (howmany = 1; howmany < leds.matrixWidth; howmany++)
    {
      xslope[howmany] = random8();//color
      while (abs(xslope[howmany] - xslope[howmany - 1]) > 20)
        xslope[howmany] = random8();
    }

  }
  for (xx = 0; xx < leds.matrixWidth ; xx++)
  {
    if (random8() < 48)
      for (yy = 0; yy < leds.matrixHeight - 1; yy++)
      {
          leds.m_LED[leds.mXY(xx, yy)] = leds.m_LED[leds.mXY(xx, yy + 1)];
      }
    if (random8() > 128)
      leds.drawPixel(xx, leds.matrixHeight - 1,  CHSV(xslope[xx], 255, 255));
    else
      leds.drawPixel(xx, leds.matrixHeight - 1, CRGB::Black);
  }
}

void mirror() {
  for (xx = 0; xx < leds.matrixWidth; xx++)
    for (yy = 0; yy < MIDLY; yy++)
        leds.m_LED[leds.mXY(xx, yy)] =  leds.m_LED[leds.mXY(xx, leds.matrixHeight - yy - 1)];

}

void roger()
{
  if (counter == 0)
    ccoolloorr = random8();
  if (blender < 85) {
    leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(h  + 128, 255, 190));
    leds.drawFilledCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2) , CHSV(h , 255, 255));

    leds.drawLine(MIDLX, leds.matrixHeight - 2, 0, 3, CRGB::Black);
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 1, 3, CRGB::Black);
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 2, 3, CRGB::Black);
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 3, 3, CRGB::Black);
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 4, 3, CRGB::Black);
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 5, 3, CRGB::Black);
    leds.drawFilledCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2) - 1, CRGB::Black);
    leds.HorizontalMirror();
  }
  else if (blender > 170) {
    leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(h + 84 , 255, 190));

    leds.drawCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2), CHSV(h  - 84, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 0, 3, CHSV(h  + 30, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 1, 3, CHSV(h  + 15, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 2, 3, CHSV(h  , 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 3, 3, CHSV(h  - 15, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 4, 3, CHSV(h  - 30, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 5, 3, CHSV(h  - 45, 255, 255));
    leds.drawCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2), CRGB::Black);

    leds.HorizontalMirror();
  }
  if (blender >= 85 && blender <= 170)
  {
    // leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CRGB::Black);
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 0, 3, CHSV(ccoolloorr  + 45, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 1, 3, CHSV(ccoolloorr  + 22, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 2, 3, CHSV(ccoolloorr  , 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 3, 3, CHSV(ccoolloorr  - 15, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 4, 3, CHSV(ccoolloorr  - 30, 255, 255));
    leds.drawLine(MIDLX, leds.matrixHeight - 2, 5, 3, CHSV(ccoolloorr  - 45, 255, 255));
    if ((counter / 200) % 2 == 0) {
      leds.drawFilledCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2), CHSV(ccoolloorr - 128, 255, 55 + counter % 200));
      leds.drawFilledCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2) - 1, CHSV(ccoolloorr - 128, 255, 255 - counter % 200));
    }
    else {
      leds.drawFilledCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2), CHSV(ccoolloorr - 128, 255, 255 - counter % 200));
      leds.drawFilledCircle(MIDLX, MIDLY * .6666, (MIDLY >> 2) - 1, CHSV(ccoolloorr - 128, 255, 55 + counter % 200));
    }
    leds.HorizontalMirror();
  }
}

void confetti() {
  if (random8() < 224)
    leds.drawCircle(random(leds.matrixWidth), random(leds.matrixHeight), random(1, 7), CHSV(random8(), 255, 255));
  else
    leds.drawCircle(random(leds.matrixWidth), random(leds.matrixHeight), random(1, 7), CHSV(h + 128 , 255, 255));
}

void confetti3() {

  if (random8() < 224)
    leds.drawCircle(random(leds.matrixWidth), random(leds.matrixHeight), random(1, 7), CHSV(random8(), 255, 255));
  else
    leds.drawFilledCircle(random(leds.matrixWidth), random(leds.matrixHeight), random(1, 7), CHSV(h + 128 , 255, 255));
}

void confetti4() {
    leds.m_LED[leds.mXY(random(leds.matrixWidth), random(leds.matrixHeight))] += CHSV(random8(), 255, 255);
    leds.m_LED[leds.mXY(random(leds.matrixWidth), random(leds.matrixHeight))] += CHSV(random8(), 255, 255);
}

void confetti2() {
  if (random8() > blender)
      leds.m_LED[leds.mXY(random(leds.matrixWidth), random(leds.matrixHeight))] += CHSV(h + random(32) + 128, 255, 255);
  else
    leds.drawFilledCircle(random(leds.matrixWidth), random(leds.matrixHeight), random(1, 9), CHSV(h + random(32) + 128 , 255, 255));
}



void bubbles() {
  if (counter == 0)
  {
    howmany = random (MIDLX >> 1, MIDLX );

    for (uint16_t u = 0; u < howmany; u++) {
      xfire[u] = random(leds.matrixWidth);
      yfire[u]  = random(leds.matrixHeight);
      fcolor[u] = random8(); //color
      fpeed[u] = random(1, 7); //speed

      fcount[u] = random(3, MIDLX >> 1); //radius
      if (random8() > 128)
        rimmer[u] = true;
      else
        rimmer[u] = false;
    }
  }

  for (uint16_t u = 0; u < howmany; u++) {

    leds.drawFilledCircle(xfire[u],  yfire[u] - fcount[u] / 2, fcount[u], CHSV(fcolor[u], 255, 255));
    if (rimmer[u])
      leds.drawCircle(xfire[u],  yfire[u] - fcount[u] / 2, fcount[u], CHSV(fcolor[u] + 87, 255, 255));
    else
      leds.drawCircle(xfire[u],  yfire[u] - fcount[u] / 2, fcount[u], CHSV(fcolor[u] - 87, 255, 255));





    if (counter % fpeed[u] == 0)
    {
      if (u % 2 == 0)
        yfire[u]++;
      else
        xfire[u]++;
    }
    if (yfire[u] > leds.matrixHeight + fcount[u] + 2  || xfire[u] > leds.matrixWidth + fcount[u] + 2  )
    {
      if (u % 2 == 0)
      {
        xfire[u] = random(3, leds.matrixWidth - 3);
        yfire[u] =   0;
      }
      else
      {
        yfire[u] = random(3, leds.matrixHeight - 3);
        xfire[u] =   0;
      }
      fcolor[u] = random8(); //color
      fpeed[u] = random(1, 7); //speed
      fcount[u] = random(2, MIDLX >> 1); //radius

      if (random8() > 128)
        rimmer[u] = true;
      else
        rimmer[u] = false;
    }
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

void sticks() {

  ccoolloorr = random8();
  if (random8() > 22)
  { leds.drawLine(driftx, drifty, random(leds.matrixWidth), random(leds.matrixHeight), CHSV(ccoolloorr, 255, 255));
    leds.drawLine(driftx, drifty, random(leds.matrixWidth), random(leds.matrixHeight), CHSV(ccoolloorr, 255, 255));
    leds.drawLine(driftx, drifty, random(leds.matrixWidth), random(leds.matrixHeight), CHSV(ccoolloorr, 255, 255));
  }
  else
  { leds.drawLine(driftx, drifty, random(leds.matrixWidth), random(leds.matrixHeight), CRGB::White);
    leds.drawLine(driftx, drifty, random(leds.matrixWidth), random(leds.matrixHeight), CRGB::White);
    leds.drawLine(driftx, drifty, random(leds.matrixWidth), random(leds.matrixHeight), CRGB::White);
  }
}

void  Bargraph() {
  if (counter == 0)
    for (uint16_t hhh = 0; hhh < leds.matrixWidth ; hhh++) {
      fcolor[hhh] = random(MIDLY / 2, leds.matrixHeight - MIDLY / 2 - 1);
      if (pattern == 22)
        fcolor[hhh] = random(MIDLY >> 2, MIDLY - 1);
      fpeed[hhh] = random(4, 9);
      fcount[hhh] = 0;
      poffset = random8();
    }


  for (uint16_t hhh = 0; hhh < leds.matrixWidth ; hhh++)
  {
    if (fcount[hhh] > fpeed[hhh])
    {
      if (pattern == 22) {
        if (fcolor[hhh] > MIDLY / 2)
          fcolor[hhh] = fcolor[hhh] - 5 + random(9);
        else
          fcolor[hhh] = fcolor[hhh] - 3 + random(9);

        fcolor[hhh] = constrain(fcolor[hhh], 1, MIDLY - 1);
      }

      else {
        if (fcolor[hhh] > MIDLY)
          fcolor[hhh] = fcolor[hhh] - 5 + random(9);
        else
          fcolor[hhh] = fcolor[hhh] - 3 + random(9);

        fcolor[hhh] = constrain(fcolor[hhh], 1, leds.matrixHeight - 1);

      }

      fcount[hhh] = 0;
      fpeed[hhh] = random(4, 9);
    }
    else
      fcount[hhh]++;

    leds.drawLine(hhh, 0, hhh, fcolor[hhh], CHSV(h + poffset + hhh * steper, 255, 255));

  }
}

void adjuster() {  // applies the screen wide effect
  switch (adjunct) {
    case 0://no effect
      break;
    case 1://no effect
      break;
    case 2:
      leds.HorizontalMirror();
      leds.VerticalMirror();
      break;
    case 3:
      leds.QuadrantBottomTriangleMirror();
      break;
    case 4:
      leds.HorizontalMirror();
      break;
    case 5:
      leds.VerticalMirror();
      break;
    case 6:
      leds.QuadrantRotateMirror();
      break;
    case 7:
      leds.TriangleTopMirror();
      break;
    case 8:
      leds.QuadrantMirror();
      break;

    default:// no effect
      break;
  }
}

void spire2() {

  if (counter == 0)
  {
    radius =  leds.matrixWidth / 2 - 3;
    flip = true;
    radius2 =  5;
    flip2 = false;
    dot = dot + 4 + random(5);
  }

  if (h % 24 == 0)
  {
    if (radius < 4)
      flip = 1 - flip;
    if (radius > leds.matrixWidth / 2)
      flip = 1 - flip;
    if (flip)
      radius --;
    else
      radius++;

    if (radius2 < 4)
      flip2 = 1 - flip2;
    if (radius2 > leds.matrixWidth / 2)
      flip2 = 1 - flip2;
    if (flip2)
      radius2 --;
    else
      radius2++;
  }

  float xer = driftx + radius * (cos8(h * 2) - 128.0) / 128.0;
  float yer = drifty + radius * (sin8(h * 2) - 128.0) / 128.0;
  leds.drawFilledCircle(xer, yer, dot, CHSV(h , 255, 255));
  if (flip3)  leds.drawCircle(xer, yer, dot + 1, CRGB::White);
  xer = driftx - radius2 * (cos8( h * 2) - 128.0) / 128.0;
  yer = drifty - radius2 * (sin8( h * 2) - 128.0) / 128.0;
  leds.drawFilledCircle(xer, yer, dot, CHSV(h + 128, 255, 255));
  if (flip3)  leds.drawCircle(xer, yer, dot + 1, CRGB::White);
  xer = driftx + radius * (cos8(h * 2 + 87) - 128.0) / 128.0;
  yer = drifty + radius * (sin8( h * 2 + 87) - 128.0) / 128.0;
  leds.drawFilledCircle(xer, yer, dot, CHSV(h + 87, 255, 255));
  if (flip3)  leds.drawCircle(xer, yer, dot + 1, CRGB::White);
  xer = driftx - radius2 * (cos8( h * 2 + 87) - 128.0) / 128.0;
  yer = drifty - radius2 * (sin8( h * 2 + 87) - 128.0) / 128.0;
  leds.drawFilledCircle(xer, yer, dot, CHSV(h + 128 + 87, 255, 255));
  if (flip3)  leds.drawCircle(xer, yer, dot + 1, CRGB::White);

  xer = driftx + radius * (cos8(h * 2 + 174) - 128.0) / 128.0;
  yer = drifty + radius * (sin8(h * 2 + 174) - 128.0) / 128.0;
  leds.drawFilledCircle(xer, yer, dot, CHSV(h + 174, 255, 255));
  if (flip3)  leds.drawCircle(xer, yer, dot + 1, CRGB::White);
  xer = driftx - radius2 * (cos8( h * 2 + 174) - 128.0) / 128.0;
  yer = drifty - radius2 * (sin8( h * 2 + 174) - 128.0) / 128.0;
  leds.drawFilledCircle(xer, yer, dot, CHSV(h + 128 + 174, 255, 255));
  if (flip3)  leds.drawCircle(xer, yer, dot + 1, CRGB::White);
}

void spire3() {

  if (counter == 0)
  {
    radius =  MIDLX + 7;
    flip = true;
    radius2 =  5;
    flip2 = false;
    dot = dot + 3 + random(4);
    howmany = random (4, 8);
  }


  if (h % 16 == 0)
  {

    if (radius < 4)
      flip = 1 - flip;
    if (radius > 8 + leds.matrixWidth / 2)
      flip = 1 - flip;
    if (flip)
      radius --;
    else
      radius++;


    if (radius2 < 4)
      flip2 = 1 - flip2;
    if (radius2 > 8 + leds.matrixWidth / 2)
      flip2 = 1 - flip2;
    if (flip2)
      radius2 --;
    else
      radius2++;
  }
  for (uint16_t i = 0; i < howmany; i++) {
    float xer = driftx + radius * (cos8(2 * h + i * 256 / howmany) - 128.0) / 128.0;
    float yer = drifty + radius * (sin8(2 * h + i * 256 / howmany) - 128.0) / 128.0;
    if ( flip3)
      leds.drawFilledCircle(xer, yer, dot, CHSV(h + i * 256 / howmany, 255, 255));
    else
      leds.drawCircle(xer, yer, dot, CHSV(h + i * 256 / howmany, 255, 255));
    xer = driftx - radius2 * (cos8(2 * h + 128 + i * 256 / howmany) - 128.0) / 128.0;
    yer = drifty - radius2 * (sin8(2 * h + 128 + i * 256 / howmany) - 128.0) / 128.0;
    if (flip3)
      leds.drawFilledCircle(xer, yer, dot, CHSV(h + 128 + i * 256 / howmany, 255, 255));
    else
      leds.drawCircle(xer, yer, dot, CHSV(h + 128 + i * 256 / howmany, 255, 255));

  }
}

void spire() {

  if (counter == 0)
  {
    radius =  leds.matrixWidth / 2 - 3;
    flip = true;
    radius2 =  5;
    flip2 = false;
    dot = dot + 3 + random(5);
  }

  if (h % 16 == 0)
  {
    if (radius < 5)
      flip = 1 - flip;
    if (radius > leds.matrixWidth / 2)
      flip = 1 - flip;
    if (flip)
      radius --;
    else
      radius++;

    if (radius2 < 5)
      flip2 = 1 - flip2;
    if (radius2 > leds.matrixWidth / 2)
      flip2 = 1 - flip2;
    if (flip2)
      radius2 --;
    else
      radius2++;
  }

  float xer = driftx + radius * (cos8(2 * h) - 128.0) / 128.0;
  float yer = drifty + radius * (sin8(2 * h) - 128.0) / 128.0;
  leds.drawCircle(xer, yer, dot, CHSV(h, 255, 255));
  xer = driftx - radius * (cos8(2 * h) - 128.0) / 128.0;
  yer = drifty - radius * (sin8(2 * h) - 128.0) / 128.0;
  leds.drawCircle(xer, yer, dot, CHSV(h + 128, 255, 255));

  xer = driftx + radius * (cos8(2 * h + 87) - 128.0) / 128.0;
  yer = drifty + radius * (sin8(2 * h + 87) - 128.0) / 128.0;
  leds.drawCircle(xer, yer, dot, CHSV(h + 85, 255, 255));
  xer = driftx - radius * (cos8(2 * h + 87) - 128.0) / 128.0;
  yer = drifty - radius * (sin8(2 * h + 87) - 128.0) / 128.0;
  leds.drawCircle(xer, yer, dot, CHSV(h - 85, 255, 255));

  xer = driftx + radius * (cos8(2 * h + 43) - 128.0) / 128.0;
  yer = drifty + radius * (sin8(2 * h + 43) - 128.0) / 128.0;
  leds.drawCircle(xer, yer, dot, CHSV(h + 43, 255, 255));
  xer = driftx - radius * (cos8(2 * h + 43) - 128.0) / 128.0;
  yer = drifty - radius * (sin8(2 * h + 43) - 128.0) / 128.0;
  leds.drawCircle(xer, yer, dot, CHSV(h - 43, 255, 255));
}

void triple() {
  if (counter == 0) {
    radius2 = random(leds.matrixWidth / 4 , leds.matrixWidth / 2 - 4 );
    radius3 = random (radius2 / 2, radius2);
    ringdelay = ringdelay * random(2, 5);
  }
  leds.drawCircle(driftx + ((radius3 * (cos8(h * dot) - 128)) >> 7), drifty + ((radius3 * (sin8(h * dot) - 128)) >> 7), radius2, CHSV(2 * h , 255, 255));
  leds.drawCircle(driftx + ((radius3 * (cos8(h * dot + 85) - 128) ) >> 7), drifty + ((radius3 * (sin8(h * dot + 85 ) - 128) ) >> 7), radius2, CHSV(2 * h + 85 , 255, 255));
  leds.drawCircle(driftx + (( radius3 * (cos8(h * dot - 85) - 128)) >> 7), drifty + ((radius3 * (sin8(h * dot - 85 ) - 128) ) >> 7), radius2, CHSV(2 * h - 85  , 255, 255));
}

void siny() {

  if (counter == 0) {
    radius2 = random(leds.matrixWidth >> 2, (leds.matrixWidth >> 1) - 4);
    radius3 = random (4, MIDLX - 4 );
    ringdelay = ringdelay * random(2, 5);
  }
  leds.drawCircle(MIDLX + ((radius3 * (cos8(-h * 3) - 128)) >> 7), MIDLY + ((radius3 * (sin8(-h * 3) - 128)) >> 7), radius2 + 1, CHSV(h * 2 , 255, 255));
  leds.drawCircle(MIDLX + ((radius3 * (cos8(h * 3 + 128) - 128)) >> 7), MIDLY + ((radius3 * (sin8(-h * 3 + 128) - 128)) >> 7), radius2 - 1, CHSV(h * 2 + 128 , 255, 255));
}


void whitewarp() {
  if (counter == 0 )
  {
    howmany = random (MIDLX * 3 / 2, leds.matrixWidth + 4 );

    for (int i = 0; i < howmany; i++) {
      fcount[i] = random8(); //angle
      fcolor[i] = random8();
      fpeed[i] = random(2, 12);
      xfire[i] = driftx;
      yfire[i] = drifty;

    }
  }

  for (int i = 0; i < howmany; i++)
  {
    xfire[i] = xfire[i] + fpeed[i] / 4.0 * (sin8(fcount[i] + h ) - 128.0) / 128.0;
    yfire[i] = yfire[i] + fpeed[i] / 4.0 * (cos8(fcount[i] + h ) - 128.0) / 128.0;

    if (!flip2)
      leds.drawPixel(xfire[i], yfire[i], CRGB::White);
    else
      leds.drawPixel(xfire[i], yfire[i], CHSV(fcolor[i] , 255, 255));

    if (xfire[i] < 0 || yfire[i] < 0 || xfire[i] > leds.matrixWidth || yfire[i] > leds.matrixHeight) {
      xfire[i] = driftx;
      yfire[i] = drifty;
      fcount[i] = random8(); //angle
      fcolor[i] = random8();;
      fpeed[i] = random8(2, 12);
    }
  }
  if (!flip2)
    leds.drawPixel(xfire[howmany - 1], yfire[howmany - 1], CHSV(fcolor[howmany - 1] , 255, 255));
  else
    leds.drawPixel(xfire[howmany - 1], yfire[howmany - 1], CRGB::White);
}

void warp() {
  if (counter == 0 )
  {
    howmany = random (MIDLX * 7 / 5, leds.matrixWidth * 7 / 5);

    for (int i = 0; i < howmany; i++) {
      fcount[i] = random8(); //angle
      fcolor[i] = blender + random(45);//color
      fpeed[i] = random(2, 12);
      xfire[i] = driftx;
      yfire[i] = drifty;

    }
  }

  for (int i = 0; i < howmany; i++)
  {
    xfire[i] = xfire[i] + (fpeed[i] / 4.0) * (sin8(fcount[i] + h ) - 128.0) / 128.0;
    yfire[i] = yfire[i] + ( fpeed[i] / 4.0) * (cos8(fcount[i] + h ) - 128.0) / 128.0;
    if (!flip)
      leds.drawPixel(xfire[i], yfire[i], CHSV(fcolor[i], 255, 255));
    else if (!flip2)
      leds.drawPixel(xfire[i], yfire[i], CRGB::White);
    else
      leds.drawPixel(xfire[i], yfire[i], CHSV(blender*64/howmany, 255, 255));

    if (xfire[i] < 0 || yfire[i] < 0 || xfire[i] > leds.matrixWidth || yfire[i] > leds.matrixHeight) {
      xfire[i] = driftx;
      yfire[i] = drifty;
      fcount[i] = random8(); //angle
      fcolor[i] = random8();;
      fpeed[i] = random8(2, 8);
    }
  }
  if (!flip2)
    leds.drawPixel(xfire[howmany - 1], yfire[howmany - 1], CHSV(blender*64/howmany , 255, 255));
  else
    leds.drawPixel(xfire[howmany - 1], yfire[howmany - 1], CRGB::White);
}

void lfado(int16_t bbc)
{
  for ( uint16_t hhh = 0; hhh < leds.matrixWidth ; hhh++)
    for (uint16_t jjj = 0; jjj < leds.matrixHeight ; jjj++)
     leds.m_LED[leds.mXY(hhh, jjj)].fadeToBlackBy(bbc);//% = bbc/255
}  



void redfado(int16_t bbc) {
  for ( uint16_t hhh = 0; hhh < leds.matrixWidth ; hhh++)
    for (uint16_t jjj = 0; jjj < leds.matrixHeight ; jjj++)
      if (flip2)
        leds.m_LED[leds.mXY(hhh, jjj)] -= CRGB(random(bbc / 2), random(bbc), random(bbc));
      else
        leds.m_LED[leds.mXY(hhh, jjj)] -= CRGB(random(bbc / 2), random(bbc / 2), random(bbc));
}

void greenfado(int16_t bbc) {
  for ( uint16_t hhh = 0; hhh < leds.matrixWidth ; hhh++)
    for (uint16_t jjj = 0; jjj < leds.matrixHeight ; jjj++)
      if (!flip3)
        leds.m_LED[leds.mXY(hhh, jjj)] -= CRGB(random(bbc ), random(bbc / 2), random(bbc));
      else
        leds.m_LED[leds.mXY(hhh, jjj)] -= CRGB(random(bbc ), random(bbc / 2), random(bbc / 2));
}

void bluefado(int16_t bbc) {
  for ( uint16_t hhh = 0; hhh < leds.matrixWidth ; hhh++)
    for (uint16_t jjj = 0; jjj < leds.matrixHeight ; jjj++)
      if (flip2)
        leds.m_LED[leds.mXY(hhh, jjj)] -= CRGB(random(bbc ), random(bbc), random(bbc / 2));
      else
        leds.m_LED[leds.mXY(hhh, jjj)] -= CRGB(random(bbc / 2 ), random(bbc), random(bbc / 2));

}

void audio()
{
  for (int i = 0; i < leds.matrixHeight; i++)
  {
    leds.drawLine(i, 0, i, faudio[i], CHSV(i * mstep + 2 * h, 255, 255));
    if (faudio[i] > leds.matrixHeight * 0.7 )
      leds.drawLine(i, faudio[i] - 2, i, faudio[i] ,  CHSV(i * mstep + (2 * h) + 128, 255, 255));
  }
  audioprocess();
}

void audio2()
{
  for (int i = 0; i < leds.matrixWidth; i++)
  {
    leds.drawLine(i, 0, i, faudio[i] , CHSV(i * mstep, 255, 255));
    if (faudio[i] > leds.matrixHeight * 0.50 )
      leds.drawLine(i, faudio[i] - 4, i, faudio[i] ,  CHSV(i * mstep - 85, 255, 255));
    if (faudio[i] > leds.matrixHeight * 0.80 )
      leds.drawLine(i, faudio[i] - 2, i, faudio[i] ,  CHSV(i * mstep + 85, 255, 255));
  }
  audioprocess();
}

void audio3()// different on big hits
{
  for (int i = 0; i < leds.matrixWidth; i++)
  {
    leds.drawLine(i, 0, i, faudio[i], CHSV(h * 2, 255, 255));
    if (faudio[i] > leds.matrixHeight * 0.75 )
      leds.drawLine(i, 0, i, faudio[i], CHSV(h * 2 - 42, 255, 255));

  }
  audioprocess();
}

void Raudio()  // rotating ring,  dots  , freq rotates, colors do not
{
  for (int i = 0; i < leds.matrixWidth ; i++) {
    xangle =  (sin8(i * mstep  +  h) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  +  h) - 128.0) / 128.0;
    leds.drawPixel(MIDLX + xangle * (leds.matrixWidth / 2 - faudio[i] / mscale) , MIDLY + yangle * (leds.matrixHeight / 2 - faudio[i] / mscale), CHSV(i * mstep , 255, 255));
  }
  audioprocess();
}

void Raudio2()// ring, lines, colors rotate, freq does not rotate
{

  for (int i = 0; i < leds.matrixWidth ; i++) {
    xangle =  (sin8(i * mstep  ) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  ) - 128.0) / 128.0;
    leds.drawLine(MIDLX + MIDLX * xangle, MIDLY + MIDLY * yangle, MIDLX + xangle * (MIDLX - faudio[i] / (2 * mscale)) , MIDLY + yangle * (MIDLY - faudio[i] / (2 * mscale)), CHSV(i * mstep , 255, 255));
  }
  audioprocess();
}

void Raudio2a()// ring, lines, colors rotate, freq does not rotate
{

  for (int i = 0; i < leds.matrixWidth ; i++) {
    xangle =  (sin8(i * mstep  + h) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  + h) - 128.0) / 128.0;
    leds.drawLine(MIDLX + MIDLX * xangle, MIDLY + MIDLY * yangle, MIDLX + xangle * (MIDLX - faudio[i] / (2 * mscale)) , MIDLY + yangle * (MIDLY - faudio[i] / (2 * mscale)), CHSV(i * mstep +  h, 255, 255));
  }
  audioprocess();
}

void Raudio3()//star shaped colors rotate
{
  for (int i = 0; i < leds.matrixWidth ; i++) {
    xangle =  (sin8(i * mstep  + h) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  + h) - 128.0) / 128.0;
    leds.drawLine(MIDLX + xangle * 2 , MIDLY + yangle * 2 , MIDLX  + xangle * ( 2 + (faudio[i] ) / (mscale)) , MIDLY  + yangle * ( 2 + (faudio[i] ) / (mscale)), CHSV(i * mstep , 255, 255));
  }
  //leds.drawFilledRectangle(MIDLX - 1, MIDLY - 1, MIDLX + 1, MIDLY + 1, CHSV(h, 255, 255));
  audioprocess();
}

void Raudio4() // multi color  ring witht variable height outward non rotational frequency but colors rotate
{
  if (counter == 0)  {
    poffset = random(leds.matrixWidth >> 2, (leds.matrixWidth >> 1) - 3);
    if (pattern == 70)
      poffset = MIDLX / 2 + 4;
    else
      poffset = 0;
  }

  for (int i = 0; i < leds.matrixWidth  ; i++) {
    xangle =  (sin8(i * mstep  ) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  ) - 128.0) / 128.0;
    leds.drawLine(MIDLX + (MIDLX / 4) * xangle, MIDLY + (MIDLY / 4)* yangle, MIDLX + xangle * (MIDLX / 4 + faudio[i] / (mscale)) , MIDLY + yangle * (MIDLY / 4 + faudio[i] / (mscale)), CHSV(i * mstep , 255, 255));
  }
  audioprocess();
}

void Raudio5()  // multi color  ring witht variable height
{
  for (uint16_t i = 0; i < leds.matrixWidth ; i++) {
    xangle =  (sin8(i * mstep  - h) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  - h) - 128.0) / 128.0;
    leds.drawLine(MIDLX + MIDLX * xangle, MIDLY + MIDLY * yangle, MIDLX + xangle * (MIDLX - faudio[i] / (mscale )) , MIDLY + yangle * (MIDLY - faudio[i] / (mscale )), CHSV(i * mstep  + 2 * h, 255, 255));
  }
  audioprocess();
}

void Raudio6()  // color ring of dots, roughly circular ,
{
  for (int i = 0; i < leds.matrixWidth ; i++) {
    xangle =  (sin8(i * mstep  - h ) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  - h) - 128.0) / 128.0;
    leds.drawPixel( driftx + xangle * (3 + leds.matrixWidth / 2 - faudio[i] / mscale) , drifty + yangle * (3 + leds.matrixHeight / 2 - faudio[i] / mscale), CHSV(i * mstep , 255, 255));
  }
  audioprocess();
}

void spin()  // x meterior moving in a circle to center
{
  if (counter == 0) {
    ccoolloorr = random8();
    dot = random(2, 6);
    poffset = 0;
    howmany = random(3, 9);
    raad = 256.0 / howmany;
  }

  if (counter % leds.matrixWidth - 5 == 0) {
    poffset += dot;
    ccoolloorr = ccoolloorr + random(64);
    howmany = random(3, 8);
    raad = 256.0 / howmany;
    dot = random(2, 6);
  }

  for (int q = 0; q < howmany; q++) {
    xangle =  (sin8(-3 * h + poffset + raad * q) - 128.0) / 128.0;
    yangle =  (cos8(-3 * h + poffset + raad * q) - 128.0) / 128.0;
    if (flip2)
      leds.drawFilledCircle(driftx + xangle * (leds.matrixWidth + dot - counter % leds.matrixWidth), drifty + yangle * (leds.matrixWidth + dot - counter % leds.matrixHeight) , dot, CHSV(ccoolloorr, 255, 255));
    else

      leds.drawCircle(driftx + xangle * (leds.matrixWidth + dot - counter % leds.matrixWidth), drifty + yangle * (leds.matrixWidth + dot - counter % leds.matrixHeight) , dot, CHSV(ccoolloorr, 255, 255));
  }

}

void xspin()  // many fingers moving in arc to middle
{
  if (counter == 0) {
    ccoolloorr = random8();
    dot = random(3, 6);
    poffset = 0;
    hhowmany = random(3, 9);
    raad = 256.0 / hhowmany;
  }

  if (counter % leds.matrixWidth - 5 == 0) {
    poffset += dot;
    ccoolloorr = ccoolloorr + random(64);
    hhowmany = random(3, 8);
    raad = 256.0 / hhowmany;
  }

  for (int q = 0; q < hhowmany; q++) {
    xangle =  (sin8(3 * h + poffset + raad * q) - 128.0) / 128.0;
    yangle =  (cos8(3 * h + poffset + raad * q) - 128.0) / 128.0;

    leds.drawFilledCircle(driftx + xangle * (leds.matrixWidth + dot - counter % leds.matrixWidth), drifty + yangle * (leds.matrixWidth + dot - counter % leds.matrixHeight) , dot, CHSV(ccoolloorr, 255, 255));
  }
  if (flip) {
    if (counter % leds.matrixWidth - 2 == 0)
      leds.drawFilledCircle(driftx  , drifty  , dot , CHSV(ccoolloorr + 21, 255, 255));
    if (counter % leds.matrixWidth - 3 == 0)
      leds.drawFilledCircle(driftx  , drifty  , dot - 1, CHSV(ccoolloorr + 42, 255, 255));
    if (counter % leds.matrixWidth - 4 == 0)
      leds.drawFilledCircle(driftx  , drifty  , dot - 2, CHSV(ccoolloorr + 63, 255, 255));
  }
}

void spin2()  // 4 meteriorw moving in ovals
{
  driftx = MIDLX;
  drifty = MIDLY;
  xangle =  (sin8(2 * (h )) - 128.0) / 128.0;
  yangle =  (cos8(2 * (h )) - 128.0) / 128.0;

  leds.drawFilledCircle(MIDLX + xangle * (MIDLX - 9), MIDLY + yangle * (MIDLY - 1) , dot, CHSV(h + 128, 255, 255));
  xangle =  (sin8(2 * (h ) + 64) - 128.0) / 128.0;
  yangle =  (cos8(2 * (h ) + 64) - 128.0) / 128.0;
  leds.drawFilledCircle(MIDLX + xangle * (MIDLX - 1), MIDLY + yangle * (MIDLY - 9) , dot, CHSV(h , 255, 255));
  xangle =  (sin8(2 * (h ) + 128) - 128.0) / 128.0;
  yangle =  (cos8(2 * (h ) + 128) - 128.0) / 128.0;
  leds.drawFilledCircle(MIDLX + xangle * (MIDLX - 9), MIDLY + yangle * (MIDLY - 1) , dot, CHSV(h + 64, 255, 255));
  xangle =  (sin8(2 * (h ) - 64) - 128.0) / 128.0;
  yangle =  (cos8(2 * (h ) - 64) - 128.0) / 128.0;
  leds.drawFilledCircle(MIDLX + xangle * (MIDLX - 1), MIDLY + yangle * (MIDLY - 9) , dot, CHSV(h - 64, 255, 255));
}

void magictime()
{
  if (counter == 0)
  {
    locusx = driftx;
    locusy = drifty;
    raad = random(leds.matrixWidth / 2, leds.matrixWidth - 10);
    ringdelay = 50;
  }
  ccoolloorr = ccoolloorr - 4 * steper;
  if (raad > 1)
  {
    leds.drawFilledCircle(locusx, locusy , raad, CHSV(ccoolloorr , 255, 255));
    raad = raad - 1;
  }

  if (raad == 2)
  {
    ringdelay = 20;
    // leds.drawFilledCircle(locusx, locusy , raad, CHSV(ccoolloorr , 255, 255));
    //  leds.drawPixel( locusx, locusy, CRGB::White);
    raad = raad - 1;
  }
  if (raad == 1) {
    ringdelay--;
  }

  if (ringdelay == 0)
  {
    raad = random(leds.matrixWidth / 2, leds.matrixWidth - 10);
    locusx = driftx;
    locusy = drifty;
    ringdelay = random(40, 80);
  }
}

void rmagictime()
{

  if (counter == 0)
  {
    locusx = driftx;
    locusy = drifty;
    raad = 1;
    ringdelay = random(30, 60);
    // ccoolloorr = random8();
  }

  if (raad < leds.matrixWidth - dot)
  {
    leds.drawCircle(driftx, drifty , raad, CHSV(ccoolloorr + h , 255, 255));
    leds.drawCircle(driftx, drifty , raad + 1, CHSV(ccoolloorr + h , 255, 255));
    leds.drawCircle(driftx, drifty , raad - 1, CHSV(ccoolloorr + h , 255, 255));
    raad++;
  }

  if (raad == leds.matrixWidth - dot) {
    ringdelay--;
    //ringdelay = constrain(ringdelay, 0, 20);
  }

  if (ringdelay == 0)
  {
    raad = 1;
    locusx = driftx;
    locusy = drifty;
    ringdelay = random(50, 70);
    ccoolloorr = random(8);
  }
}


void splat()
{
  xsizer = random(leds.matrixWidth / 5, leds.matrixWidth / 3);
  ysizer = random(leds.matrixWidth / 5, leds.matrixWidth / 3);
  raad =  random(leds.matrixWidth / 5, leds.matrixWidth / 3);
  locusx = random(4, leds.matrixWidth - 5);
  locusy = random(4, leds.matrixWidth - 5);

  ccoolloorr = random8();
  leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, random(180, 255), random(180, 255)));
  leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, 255, random(180, 255)));
  leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, random(180, 255), random(180, 255)));
  leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, 255, random(180, 255)));
  leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, random(180, 255), random(180, 255)));
  leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, 255, random(180, 255)));
  leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, random(180, 255), random(180, 255)));

  switch (random(10))
  {
    case 0:
      leds.drawRectangle(locusx, locusy , locusx + xsizer , locusy + ysizer, CHSV(ccoolloorr , 255, 255));
      break;
    case 1:
      leds.drawCircle(locusx, locusy , raad, CHSV(ccoolloorr , 255, 255));
      break;
    case 2:
      leds.drawFilledRectangle(locusx, locusy , locusx + xsizer , locusy + ysizer, CHSV(ccoolloorr , 255, 255));
      break;

    case 3:
      leds.drawFilledCircle(locusx, locusy , raad, CHSV(ccoolloorr , 255, 255));
      break;
    case 4:
      leds.drawFilledRectangle(locusx, locusy , locusx + xsizer , locusy + ysizer, CHSV(ccoolloorr , 255, 255));
      break;

    case 5:
      for (int16_t i = 7; i > 0 ; i--)
        leds.drawFilledRectangle(locusx - i, locusy - i , locusx + 2 * i , locusy + 2 * i, CHSV(ccoolloorr + i * steper, 255, 255));
      break;

    case 6:
      for (int16_t i = raad; i > 0; i--)
        leds.drawFilledCircle(locusx, locusy , i, CHSV(ccoolloorr + i * steper , 255, 255));
      break;

    case 7:
      triangle(locusx, locusy, raad, h, ccoolloorr);
      break;

    case 8:

      sticks();
      sticks();
      break;

    case 9:
      leds.drawPixel(locusx, locusy, CHSV(ccoolloorr, 255, 255));
      leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(random8(), 255, random(180, 255)));
      leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(random8(), 255, random(180, 255)));
      leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(random8(), 255, random(180, 255)));
      leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, 255, random(180, 255)));
      leds.drawPixel(random8() / mstep, random8() / mstep, CHSV(h, random(180, 255), random(180, 255)));
      break;

  }

}

void bounce()
{

  if (counter == 0)
  {
    locusx = MIDLX;
    locusy = MIDLY;
    raad =  random(leds.matrixWidth / 5, 2 + leds.matrixWidth / 3);

    yangle = random(12, 52);
    xangle = (sin8(yangle) - 128.0) / 128.0;
    yangle = (cos8(yangle) - 128.0) / 128.0;
    xsizer = random(leds.matrixWidth / 5, leds.matrixWidth / 2 - 4);
    ysizer = xsizer;
    ccoolloorr = random8();
  }

  locusx =  locusx + xangle;
  locusy =  locusy + yangle;
  driftx = locusx;
  drifty = locusy;

  if (flip3)
    ccoolloorr = ccoolloorr - 6;
  else
    ccoolloorr = ccoolloorr + 6;

  if (blender <= 127) {

    if (locusx > leds.matrixWidth - raad  || locusx < raad ) {
      if (!flip3) ccoolloorr = random8();
      xangle = 0 - xangle;
    }
    if (locusy > leds.matrixHeight - raad  || locusy < raad  ) {
      yangle = 0 - yangle;
      if (!flip3) ccoolloorr = random8();
    }
    leds.drawCircle(locusx, locusy , raad, CHSV(ccoolloorr, 255, 255));
    if (flip2)
      leds.drawCircle(locusx, locusy , raad / 2, CHSV(ccoolloorr, 255, 235));
    if (flip)
      leds.drawCircle(locusx, locusy , raad / 4, CHSV(ccoolloorr, 255, 215));
  }
  if (blender > 127 ) {

    if (locusx > leds.matrixWidth - xsizer || locusx < 1)
      xangle = 0 - xangle;
    if (locusy > leds.matrixHeight - ysizer || locusy < 1 )
      yangle = 0 - yangle;
    leds.drawRectangle(locusx, locusy , locusx + xsizer , locusy + ysizer, CHSV(ccoolloorr , 255, 255));
    if (!flip2)
      leds.drawCircle(locusx + xsizer / 2, locusy + xsizer / 2 , xsizer / 2 - 1, CHSV(ccoolloorr + 85 , 255, 255));
  }
}

//void triangle(int16_t xloc, uint16_t yloc, uint16_t bigg, uint16_t angle, uint16_t kolor)
void starbounce()
{

  if (counter == 0)
  {
    locusx = driftx;
    locusy = drifty;
    raad =  random(leds.matrixWidth * 2 / 3,  leds.matrixWidth); //size
    howmany = random (3, 8);
    yangle = random(16, 48);
    xangle = (sin8(yangle) - 128.0) / 128.0;
    yangle = (cos8(yangle) - 128.0) / 128.0;
    ccoolloorr = random8();
  }

  locusx =  locusx +  xangle;
  locusy =  locusy +  yangle;


  if (locusx > leds.matrixWidth - raad  || locusx < raad ) {

    xangle = 0 - xangle;
  }
  if (locusy > leds.matrixHeight - raad  || locusy < raad  ) {
    yangle = 0 - yangle;

  }

  // triangle(locusx, locusy, raad, h * 2, ccoolloorr - 2 * h);
  drawstar(locusx, locusy, raad, raad / 2, howmany, h * 3, ccoolloorr + 2 * h); // random multipoint star
  if (flip2) {
    drawstar(locusx, locusy, raad / 2, raad / 4, howmany, -h * 3, ccoolloorr + 2 * h + 87);
    drawstar(locusx, locusy, raad / 4, raad / 8 - 1, howmany, h * 3, ccoolloorr + 2 * h - 87);
  }
  else
  { drawstar(locusx, locusy, raad / 2, raad / 4, howmany, -h * 3, ccoolloorr + 2 * h + 128);
    drawstar(locusx, locusy, raad / 4, raad / 8 - 1, howmany, h * 3, ccoolloorr + 2 * h);
  }// random multipoint star
  //void triangle(int16_t xloc, uint16_t yloc, uint16_t bigg, uint16_t angle, uint16_t kolor)
}

void triforce() {
  if (flip &&  flip3)
    solid2();
  else if (flip && !flip2)
    solid();

  for (uint16_t i = 0; i < MIDLX; i += 7) {
    triangle(MIDLX, MIDLY, MIDLX + 2 - i, h,  h - 85);
    if (i % 2 == 0)
      triangle(MIDLX, MIDLY, MIDLX + 2 - i, h,  h + 85);
  }
}
void seasick3()
// horizontal waves
{
  if (counter == 0) sinewidth = random(8, 12);
  for (int jj = 0; jj < leds.matrixWidth; jj++)
  {
    leds.drawLine(jj, 0, jj, leds.matrixHeight  - 1, CHSV(h + jj  + 85, 255, 255));
    leds.drawLine(jj, (MIDLY >> 1) * (sin8(4 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.50 + 1, jj, leds.matrixHeight  - 1, CHSV(h + jj + 170, 255, 255));
    leds.drawLine(jj, (MIDLY >> 1) * (sin8(-4 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.5 + 2 , jj, leds.matrixHeight  - 1, CHSV(h + jj , 255, 255));
  }
}

void seasick2()
// horizontal waves
{

  if (counter == 0)
    sinewidth = random(5, 10);
  solid();
  for (int jj = 0; jj < leds.matrixWidth; jj++)
  {

    leds.drawLine(jj, (MIDLY >> 1) * (sin8(2 * h + jj * 2 * sinewidth) - 128) / 128.0 + MIDLY * 0.50 + 1, jj, 0, CHSV(h + jj + 170, 255, 255));
    leds.drawLine(jj, (MIDLY >> 1) * (sin8(4 * -h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.5 + 2 , jj, leds.matrixHeight  - 1, CHSV(h + jj + 85 , 255, 255));
  }
}
void seasick()
// horizontal waves
{

  if (counter == 0)
    sinewidth = random(4, 12);
  solid2();
  for (int jj = 0; jj < leds.matrixWidth; jj++)
  {

    leds.drawLine(jj, (MIDLY >> 1) * (sin8(-4 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.50 + 1, jj, 0, CHSV(h + jj + 170, 255, 255));
    leds.drawLine(jj, (MIDLY >> 1) * (sin8(2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.5 + 2 , jj, leds.matrixHeight  - 1, CHSV(h + jj , 255, 255));
  }
}

void seasick4()
// horizontal waves
{
  solid();
  if (counter == 0) sinewidth = random(5, 10);
  for (int jj = 0; jj < leds.matrixWidth; jj++)
  {
    for ( int u = 0; u < 5; u++) {
        leds.m_LED[leds.mXY(jj, (MIDLY >> 1) * (sin8(-3 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.70 + u - 2)] += CHSV(h + jj + 128, 255, 255);
        leds.m_LED[leds.mXY(jj, (MIDLY >> 1) * (sin8(2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.3 + u - 2)] += CHSV(h + jj + 64 , 255, 255);
        leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(h + jj * sinewidth) - 128) / 128.0 + MIDLY  + u - 2)] += CHSV(h + jj  - 64, 255, 255);
    }
  }

}


void seasick5()
// horizontal waves
{

  if (counter == 0)
    sinewidth = random(4, 12);
  solid2();
  for (int jj = 0; jj < leds.matrixWidth; jj++)
  {

    leds.drawLine(jj, (MIDLY >> 1) * (sin8(-h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.50 + 1, jj, 0, CHSV(h + jj + 128, 255, 255));
    leds.drawLine(jj, (MIDLY >> 1) * (sin8( h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.5 + 2 , jj, leds.matrixHeight  - 1, CHSV(h + jj , 255, 255));
  }

  if (counter == 0) sinewidth = random(5, 10);
  for (int jj = 0; jj < leds.matrixWidth; jj++)
  {
    leds.m_LED[leds.mXY(jj, (MIDLY >> 1) * (sin8(-2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.80 + 2)] += CHSV(h + jj - 64, 255, 255);
    leds.m_LED[leds.mXY(jj, (MIDLY >> 1) * (sin8(2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.2 + 2)] += CHSV(h + jj + 64, 255, 255);
    leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(-2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.80 + 1)] += CHSV(h + jj - 64, 255, 255);
    leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.2 + 1)] += CHSV(h + jj + 64 , 255, 255);
    leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(-2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.80)] += CHSV(h + jj - 64, 255, 255);
    leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.2)] += CHSV(h + jj + 64, 255, 255);
    leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(-2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 0.80 - 1)] += CHSV(h + jj - 64, 255, 255);
    leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(2 * h + jj * sinewidth) - 128) / 128.0 + MIDLY * 1.2 - 1)] += CHSV(h + jj + 64, 255, 255);
    /*
        leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(h + jj * sinewidth) - 128) / 128.0 + MIDLY  + 2)] += CHSV(h + jj  - 64, 255, 255);
        leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(h + jj * sinewidth) - 128) / 128.0 + MIDLY  + 1)] += CHSV(h  - 64, 255, 255);
        leds.m_LED[leds.mXY(jj , (MIDLY >> 1) * (sin8(h + jj * sinewidth) - 128) / 128.0 + MIDLY )] += CHSV(h + jj - 64, 255, 255);
    */
  }
}

void smile()
{
  leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(h , 255, 255));
  leds.drawFilledCircle(MIDLX, MIDLY, MIDLX , CHSV(h + 100, 255, 255));//face
  leds.drawFilledCircle(MIDLX, MIDLY + 1, MIDLX - 5 , CRGB::Black);
  leds.drawFilledCircle(MIDLX, MIDLY - 1, MIDLX - 5 , CHSV(h + 100 , 255, 255));
  leds.drawFilledCircle((MIDLX / 2) + 1, (MIDLY / 2) + 1 , 3, CRGB::Black); //eye


  leds.HorizontalMirror();

}

void smile2()

{

  //  leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(180 , 255, 255));
  Ringo();
  int16_t sc = min(MIDLX, MIDLY);       //scale down to fit better    

  leds.drawFilledCircle(MIDLX, MIDLY, sc ,  CHSV(58 , 255, 255));//face = yellow;
  leds.drawFilledCircle(MIDLX, MIDLY + 1, sc - 5 , CRGB::Black);
  leds.drawFilledCircle(MIDLX, MIDLY - 1, sc - 5 , CHSV(58 , 255, 255));
  leds.drawFilledCircle((MIDLX / 2) + 8, (MIDLY / 2) + 1 , 3, CRGB::Black); //eye
  leds.HorizontalMirror();

}

void smile4()//one eye cylon
{

  leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(h * 2 , 255, 150));
  leds.drawFilledCircle(MIDLX, MIDLY, MIDLX , CHSV(58 , 255, 255)); //face = yellow
  leds.drawFilledCircle(MIDLX, MIDLY + 1, MIDLX - 5 , CRGB::Black);//mouth
  leds.drawFilledCircle(MIDLX, MIDLY - 1, MIDLX - 5 , CHSV(58 , 255, 255));//antimouth also yellow
  if ((counter / 128) % 2 == 0 && counter > 512)
    leds.drawFilledCircle(MIDLX + MIDLY / 2 * (cos8(counter % 512) - 128.0) / 128.0 , (MIDLY / 2) + 2 , 3, CRGB::Black); //eye
  else
    leds.drawFilledCircle(MIDLX , (MIDLY / 2) + 2 , 3, CRGB::Black); //eye
  //leds.HorizontalMirror();

}
void smile3()
{
  leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(h * 2 + 170 , 255, 255));
  leds.drawFilledCircle(MIDLX, MIDLY, MIDLX , CHSV(h * 2 + 85 , 255, 255)); //face
  leds.drawFilledCircle(MIDLX, MIDLY + 1, MIDLX - 5 , CHSV(h * 2 , 255, 255));
  leds.drawFilledCircle(MIDLX, MIDLY - 1, MIDLX - 5 , CHSV(h * 2 + 85 , 255, 255));
  leds.drawFilledCircle((MIDLX / 2) + 1, (MIDLY / 2) + 1 , 3, CHSV(h * 2, 255, 255)); //eye
  leds.HorizontalMirror();

}




void hypnoduck() {
  // fast growing spiral of hyponic light
  if (counter == 0)
    dot = random(2, 4);

  if (flip2) quash = -20; else quash = 20;//spin speed and direction


  leds.drawCircle(driftx, drifty, 8 + (MIDLX >> 1), CHSV(h , 255, 255));


  for (uint32_t jj = 00; jj < counter % 320; jj += 5)
  {
    xangle =  (sin8(jj + quash * h) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h) - 128.0) / 128.0;
    leds.drawFilledCircle(driftx + xangle * (jj /  17) , drifty + yangle * (jj / 17), dot, CHSV(h, 255, 255));

    xangle =  (sin8(jj + quash * h + 128) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h + 128) - 128.0) / 128.0;
    leds.drawFilledCircle(driftx + xangle * (jj /  17) , drifty + yangle * (jj / 17), dot, CHSV(h - 85, 255, 255));
  }

}

void hypnoduck2()
// growing spirals
{
  if (counter == 0)
    dot = random(2, 4);
  if (!flip2)
    quash = 12 + dot;
  else
    quash = -12 - dot;

  if (flip3 && flip2)
    leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(h, 255, 150));
  else {
    if (flip)
      leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CRGB::Black);
    else
      whitewarp();
  }


  for (uint32_t jj = 50; jj < 100 + (counter % 400); jj += 5)
  {
    xangle =  (sin8(jj + quash * h) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h) - 128.0) / 128.0;

    leds.drawFilledCircle( driftx + xangle * (jj /  (19 - dot)) , drifty + yangle * (jj / (19 - dot)), dot, CHSV(h - 85, 255, 255));


    xangle =  (sin8(jj + quash * h + 128) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h + 128) - 128.0) / 128.0;
    leds.drawFilledCircle( driftx + xangle * (jj /  (19 - dot)) ,  drifty + yangle * (jj / (19 - dot)), dot, CHSV(h , 255, 255));


  }

}


void hypnoduck3()
// spirals with speckles of hyponic light random direction based on flip2
{

  if (counter == 0) {
    dot = random(2, 4);
    ccoolloorr = random8();
  }
  if (!flip2)
    quash = 3 + dot;
  else
    quash = -3 - dot;

  if (flip3)
    leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(ccoolloorr, 255, 255));
  else
  { if (flip)
      leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CRGB::Black);
    else
      leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CRGB::White);
  }
  for (int jj = 0; jj < 890; jj += 5)
  {
    xangle =  (sin8(jj + quash * h) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h) - 128.0) / 128.0;
    leds.drawFilledCircle( driftx + xangle * (jj /  (19 - dot)) , drifty + yangle * (jj / (19 - dot)), dot, CHSV(ccoolloorr - 85, 255, 255));

    xangle =  (sin8(jj + quash * h + 128) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h + 128) - 128.0) / 128.0;
    leds.drawFilledCircle( driftx + xangle * (jj /  (19 - dot)) ,  drifty + yangle * (jj / (19 - dot)), dot, CHSV(ccoolloorr + 85, 255, 255));
  }
  leds.drawFilledCircle(driftx, drifty, dot, CHSV(ccoolloorr - 85 , 255, 255));
}


void hypnoduck4()
// spiral inward with the hyponic light
{
  // MMFLAGS
  uint16_t hd4size = 890;
  uint16_t hd4center = 166; // 166 for 64x64, decrease to 120 for 96x96
  uint8_t  hd4step = 5; // 5 is ok for 64x64, but need to decrease to 3 or 2 for 96x96

  if      (max(leds.matrixHeight, leds.matrixWidth) > 128) { hd4size = 2600; hd4center = 160; hd4step = 1; }
  else if (max(leds.matrixHeight, leds.matrixWidth) > 64)  { hd4size = 1200; hd4center = 166; hd4step = 3; };

  if (flip2) quash = 8; else quash = -8;

  if (flip3)
    leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CRGB::Black);
  else
  { //if (flip)
      leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CHSV(h + 35, 255, 155));
    //else
    //  leds.drawFilledRectangle(0, 0, leds.matrixWidth  - 1, leds.matrixHeight  - 1, CRGB::White);
  }
  for (uint32_t jj = hd4size; jj > hd4center - (counter % 160); jj -= hd4step)
  {
    xangle =  (sin8(jj + quash * h) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h) - 128.0) / 128.0;
    leds.drawFilledCircle(driftx + xangle * (jj /  17) , drifty + yangle * (jj / 17), 2, CHSV(h, 255, 255));

    xangle =  (sin8(jj + quash * h + 128) - 128.0) / 128.0;
    yangle =  (cos8(jj + quash * h + 128) - 128.0) / 128.0;
    leds.drawFilledCircle(driftx + xangle * (jj /  17) , drifty + yangle * (jj / 17), 2, CHSV(h - 115, 255, 255));
  }
}

//void drawstar(int16_t xlocl, int16_t ylocl, int16_t biggy, int16_t little, int16_t points, int16_t dangle, int16_t koler)// random multipoint star

void starer() {
  if (counter == 0)
    pointy = 7;
  if (counter >= ringdelay)
  {
    if (counter - ringdelay <= (unsigned int) leds.matrixWidth + 5)
      drawstar(driftx  , drifty, 2 * (counter - ringdelay), (counter - ringdelay), pointy, blender + h, h * 2 + 85);
    else {
      ringdelay = counter + random(50, 99);
      pointy = random(3, 9);
    }
  }
}

void bkstarer() {
  if (counter == 0)
    pointyfix = 5;
  if (counter >= bringdelay)
  {
    if (counter - bringdelay <= (unsigned int) leds.matrixWidth + 20)
    {
      //void drawstar(int16_t xlocl, int16_t ylocl, int16_t biggy, int16_t little, int16_t points, int16_t dangle, int16_t koler)// random multipoint star
      drawstar(driftx, drifty, 2 * (leds.matrixWidth - (counter - bringdelay) + 15), 15 + leds.matrixWidth - (counter - bringdelay), pointyfix, blender - h, h  - 60);
      drawstar(driftx, drifty, 2 * (leds.matrixWidth - (counter - bringdelay) + 10), 10 + leds.matrixWidth - (counter - bringdelay), pointyfix, blender - h, h  - 50);
      drawstar(driftx, drifty, 2 * (leds.matrixWidth - (counter - bringdelay) + 5), 5 + leds.matrixWidth - (counter - bringdelay), pointyfix, blender - h, h  - 40);
      drawstar(driftx, drifty, 2 * (leds.matrixWidth - (counter - bringdelay)), leds.matrixWidth - (counter - bringdelay), pointyfix, blender - h, h - 30  );
    }
    else
    {
      bringdelay = counter + random(69, 126);
      pointy = random(4, 9);
    }
  }
}

void triangler() {
  if (counter >= ringdelay)
  {
    if (counter - ringdelay <= (unsigned int) leds.matrixWidth + 10)
    {
      triangle(driftx  , drifty ,  (counter - ringdelay), blender + 2 * h, h * dot - 85);
      triangle(driftx  , drifty ,  (counter - ringdelay), blender - 128 + 2 * h, h * dot - 85);
    }
    else
      ringdelay = counter + random(50, 129);
  }
}

void zoro() {
  leds.drawFilledRectangle(0, 0, leds.matrixWidth, leds.matrixHeight, CRGB::Black);
  for ( int16_t i = MIDLX; i > 6; i -= 6) {
    leds.drawFilledCircle(driftx, drifty, i, CHSV(i * 21 + 16, 205, 105));

  }
  for ( int16_t i = 0; i < MIDLX; i += 6) {
    leds.drawCircle(MIDLX, MIDLY, i, CHSV(i * 37, 255, 255));

  }
}

void boxer() {
  if (counter >= ringdelay)
  {
    if (counter - ringdelay <= leds.matrixWidth)
    {
      leds.drawRectangle(driftx - (counter - ringdelay) , drifty - (counter - ringdelay) , driftx + (counter - ringdelay), drifty + (counter - ringdelay), CHSV(h * 2 + 128, 255, 255));
    }
    else
      ringdelay = counter + random(40, 70);
  }
}

void bkboxer() {

  if (counter >= bringdelay)
  {
    if (counter - bringdelay <= (unsigned int) leds.matrixWidth + 13)
    {
      leds.drawRectangle(driftx - 12 - (counter - bringdelay) , drifty - 12 - (counter - bringdelay) , driftx + 12 + (counter - bringdelay), drifty + 12 + (counter - bringdelay), CHSV(h, 255, 255));
      leds.drawRectangle(driftx - 8 - (counter - bringdelay) , drifty - 8 - (counter - bringdelay) , driftx + 8 + (counter - bringdelay), drifty + 8 + (counter - bringdelay), CHSV(h +   steper * 3, 255, 255));
      leds.drawRectangle(driftx - 4 - (counter - bringdelay) , drifty - 4 - (counter - bringdelay) , driftx + 4 + (counter - bringdelay), drifty + 4 + (counter - bringdelay), CHSV(h +  steper * 6, 255, 255));
      leds.drawRectangle(driftx - (counter - bringdelay) , drifty - (counter - bringdelay) , driftx + (counter - bringdelay), drifty + (counter - bringdelay), CHSV(h + steper * 9, 255, 255));
    }
    else {
      bringdelay = counter + random(60, 980);

    }
  }
}


void homer() {
  if (counter == 0 )
  {
    howmany = random (leds.matrixWidth * 3 / 2, leds.matrixWidth * 3 - 2 );
    for (int i = 0; i < howmany; i++) {
      fcount[i] = random8(); //angle
      fcolor[i] = random8();//color
      fpeed[i] = random(1, 7);// bigger = slower

      ccoolloorr = random8();
      xpoffset[i] = random8();
    }
  }

  for (int i = 0; i < howmany; i++)
  {
    if (flip)
      leds.drawPixel(driftx +  (leds.matrixWidth - ((counter + xpoffset[i]) % leds.matrixWidth) / fpeed[i]) * (sin8(fcount[i] + h ) - 128.0) / 128.0 , drifty +  (leds.matrixHeight - ((counter + xpoffset[i]) % (leds.matrixHeight)) / fpeed[i]) * (cos8(fcount[i] + h ) - 128.0) / 128.0,  CHSV(fcolor[i], 255, 255));

    else
      leds.drawPixel(driftx +  (leds.matrixWidth - ((counter + xpoffset[i]) % leds.matrixWidth) / fpeed[i]) * (sin8(fcount[i] + h ) - 128.0) / 128.0, drifty +  (leds.matrixHeight - ((counter + xpoffset[i]) % (leds.matrixHeight)) / fpeed[i]) * (cos8(fcount[i] + h ) - 128.0) / 128.0, CHSV(ccoolloorr, 255, 255));

    /*if (random8()<16 ) {
      fcount[i] = random8(); //angle
      // fcolor[i] = random8();;
      fpeed[i] = random8(1, 7);

      }*/
  }
}

void homer2() {// growing egg
  if (counter == 0 )
  {
    howmany = random (MIDLX + 8, 2 * leds.matrixWidth - 12);
    dot = random(1, 5);
    if (max(leds.matrixHeight, leds.matrixWidth) >  64) dot += 4;
    if (max(leds.matrixHeight, leds.matrixWidth) > 128) dot += 4;
    for (int i = 0; i < howmany; i++) {
      fcount[i] = random8(); //angle
      fcolor[i] = random8();//color
      fpeed[i] = random(1, 4);// bigger = slower

      xpoffset[i] = random8();
      ccoolloorr = random8();
    }
  }
  int16_t tempo = dot * 2 + 50;
  if (counter % tempo == 0) {
    dot++;
    counter = counter + tempo;

  }

  if (dot >= MIDLX) {
    dot = random(3, 7);
    if (max(leds.matrixHeight, leds.matrixWidth) >  64) dot += 4;
    if (max(leds.matrixHeight, leds.matrixWidth) > 128) dot += 4;
    ccoolloorr =  random8();
  }
  leds.drawCircle( MIDLX  , MIDLY, dot + 1, CRGB::White);
  for (int i = 0; i < howmany; i++)
  {
    if (flip)
      leds.drawPixel(MIDLX +  (dot + MIDLX - ((counter + xpoffset[i]) % MIDLX) / fpeed[i]) * (sin8(fcount[i] + 2 * h ) - 128.0) / 128.0 , MIDLY +  (dot + MIDLY - ((counter + xpoffset[i]) % MIDLY) / fpeed[i]) * (cos8(fcount[i] + 2 * h ) - 128.0) / 128.0,  CHSV(fcolor[i], 255, 255));
    else if (!flip2)
      leds.drawPixel(MIDLX +  (dot + MIDLX - ((counter + xpoffset[i]) % MIDLX) / fpeed[i]) * (sin8(fcount[i] + 2 * h ) - 128.0) / 128.0 , MIDLY +  (dot + MIDLY - ((counter + xpoffset[i]) % MIDLY) / fpeed[i]) * (cos8(fcount[i] + 2 * h ) - 128.0) / 128.0,  CRGB::Orange);
    else
      leds.drawPixel(MIDLX +  (dot + MIDLX - ((counter + xpoffset[i]) % MIDLX) / fpeed[i]) * (sin8(fcount[i] + 2 * h ) - 128.0) / 128.0, MIDLY +  (dot + MIDLY - ((counter + xpoffset[i]) % MIDLY) / fpeed[i]) * (cos8(fcount[i] + 2 * h ) - 128.0) / 128.0, CHSV(h, 255, 255));
  }

  leds.drawFilledCircle( MIDLX  , MIDLY, dot  , CHSV(h, 255, 55 + 100 * dot / MIDLX));
}


void homer3() {
  if (counter == 0 )
  {
    howmany = random (MIDLX / 2, MIDLX);
    dot = 2;
    for (int i = 0; i < howmany; i++) {
      fcount[i] = random8(); //angle
      fcolor[i] = random8();//color
      fpeed[i] = random(2, 7);// bigger = slower
      xfire[i] = random(2, 6); //dot size
      xpoffset[i] = random8();
    }
  }

  for (int i = 0; i < howmany; i++)
  {

    leds.drawCircle(driftx +  (dot + leds.matrixWidth - ((counter + xpoffset[i]) % (leds.matrixWidth)) / fpeed[i]) * (sin8(fcount[i] + h ) - 128.0) / 128.0 , drifty +  (dot + leds.matrixHeight - ((counter + xpoffset[i]) % (leds.matrixHeight)) / fpeed[i]) * (cos8(fcount[i] + h ) - 128.0) / 128.0 , xfire[i],  CHSV(fcolor[i], 255, 255));
    if (i % 5 == 0)
      leds.drawPixel(driftx +  (dot + leds.matrixWidth - ((counter + xpoffset[i]) % (leds.matrixWidth)) / fpeed[i]) * (sin8(fcount[i] + h ) - 128.0) / 128.0 , drifty +  (dot + leds.matrixHeight - ((counter + xpoffset[i]) % (leds.matrixHeight)) / fpeed[i]) * (cos8(fcount[i] + h ) - 128.0) / 128.0,  CHSV(fcolor[i] - 128, 255, 255));

  }
  //leds.drawCircle( MIDLX  , MIDLY, dot, CHSV(h, 255, 255));
  //leds.drawFilledCircle( MIDLX  , MIDLY, dot-1, CHSV(h+128, 255, 255));
}

void nringer(int16_t i) {

  if (flip && flip2 && !flip3)
  {
    leds.drawFilledCircle(driftx, drifty , i, CRGB::White);
    leds.drawFilledCircle(driftx, drifty , i - 1, CRGB::Black);
  }
  else
  {
    leds.drawFilledCircle(driftx, drifty , i, CHSV(h * dot, 255, 255));
    leds.drawFilledCircle(driftx, drifty , i - 1, CRGB::Black);
  }
}


void drawtriangle()
{
  for (uint16_t i = 0; i < MIDLX; i ++)
  {
    leds.drawLine(i, i, leds.matrixWidth - i, i, CHSV(h + i * 4, 255, 255));
    leds.drawLine(i, i, MIDLX, leds.matrixHeight - i, CHSV(h + i * 4, 255, 255));
    leds.drawLine(leds.matrixWidth - i, i, MIDLX, leds.matrixHeight - i, CHSV(h + i * 4, 255, 255));

  }
}



void triangle(int16_t xloc, uint16_t yloc, uint16_t bigg, uint16_t angle, uint16_t kolor)
{
  uint16_t ax = xloc + bigg * (sin8(angle) - 128.0) / 128;
  uint16_t ay = yloc + bigg * (cos8(angle) - 128.0) / 128;
  uint16_t bx = xloc + bigg * (sin8(angle + 85) - 128.0) / 128;
  uint16_t by = yloc + bigg * (cos8(angle + 85) - 128.0) / 128;
  uint16_t cx = xloc + bigg * (sin8(angle - 85) - 128.0) / 128;
  uint16_t cy = yloc + bigg * (cos8(angle - 85) - 128.0) / 128;

  leds.drawLine(ax, ay, bx, by, CHSV(kolor, 255, 255));
  leds.drawLine(cx, cy, bx, by, CHSV(kolor, 255, 255));
  leds.drawLine(ax, ay, cx, cy, CHSV(kolor, 255, 255));
}

void drawstar(int16_t xlocl, int16_t ylocl, int16_t biggy, int16_t little, int16_t points, int16_t dangle, int16_t koler)// random multipoint star
{
  if (counter == 0) {
    shifty = 3;//move quick
  }
  radius2 = 255 / points;
  for (int i = 0; i < points; i++)
  {
    leds.drawLine(xlocl + ((little * (sin8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), CHSV(koler , 255, 255));
    leds.drawLine(xlocl + ((little * (sin8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), CHSV(koler , 255, 255));
  }
}

void starz()// 3 stars spin in a circle
{
  if (counter == 0) {
    howmany = random (3, 9);
    inner = random(MIDLY / 5, MIDLY / 2);
    radius2 = 255 / howmany;
  }

  xcen = MIDLX * (sin8(-2 * h - 85) - 128.0) / 128;
  ycen = MIDLY  * (cos8(-2 * h - 85 ) - 128.0) / 128;

  if (h % 16 == 0) {
    inner = inner + flipme;
    if (inner > MIDLY / 2 || inner < MIDLY / 5)
      flipme = -flipme;
  }

  for (int i = 0; i < howmany; i++)
  {
    leds.drawLine(driftx + xcen + ((inner * (sin8(i * radius2 + radius2 / 2 +  h) - 128.0)) / 128), drifty + ycen + ((inner * (cos8(i * radius2 + radius2 / 2 +  h) - 128.0)) / 128), driftx + xcen + ((MIDLX * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ycen + ((MIDLY * (cos8(i * radius2 +  h) - 128.0)) / 128), CHSV(h , 255, 255));
    leds.drawLine(driftx + xcen + ((inner * (sin8(i * radius2 - radius2 / 2 +  h) - 128.0)) / 128), drifty + ycen + ((inner * (cos8(i * radius2 - radius2 / 2 +  h) - 128.0)) / 128), driftx + xcen + ((MIDLX * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ycen + ((MIDLY * (cos8(i * radius2 +  h) - 128.0)) / 128), CHSV(h , 255, 255));
  }

  xcen = MIDLX  * (sin8(-2 * h + 85) - 128.0) / 128;
  ycen = MIDLY  * (cos8(-2 * h + 85) - 128.0) / 128;
  for (int i = 0; i < howmany; i++)
  {
    leds.drawLine(driftx + xcen + ((inner * (sin8(i * radius2 + radius2 / 2 +  h) - 128.0)) / 128), drifty + ycen + ((inner * (cos8(i * radius2 + radius2 / 2 +  h) - 128.0)) / 128), driftx + xcen + ((MIDLX * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ycen + ((MIDLY * (cos8(i * radius2 +  h) - 128.0)) / 128), CHSV(h + 85, 255, 255));
    leds.drawLine(driftx + xcen + ((inner * (sin8(i * radius2 - radius2 / 2 +  h) - 128.0)) / 128), drifty + ycen + ((inner * (cos8(i * radius2 - radius2 / 2 +  h) - 128.0)) / 128), driftx + xcen + ((MIDLX * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ycen + ((MIDLY * (cos8(i * radius2 +  h) - 128.0)) / 128), CHSV(h + 85, 255, 255));
  }

  xcen = MIDLX  * (sin8(-2 * h) - 128.0) / 128;
  ycen = MIDLY  * (cos8(-2 * h) - 128.0) / 128;
  for (int i = 0; i < howmany; i++)
  {
    leds.drawLine(driftx + xcen + ((inner * (sin8(i * radius2 + radius2 / 2 +  h) - 128.0)) / 128), drifty + ycen + ((inner * (cos8(i * radius2 + radius2 / 2 +  h) - 128.0)) / 128), driftx + xcen + ((MIDLX * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ycen + ((MIDLY * (cos8(i * radius2 +  h) - 128.0)) / 128), CHSV(h  - 85, 255, 255));
    leds.drawLine(driftx + xcen + ((inner * (sin8(i * radius2 - radius2 / 2 +  h) - 128.0)) / 128), drifty + ycen + ((inner * (cos8(i * radius2 - radius2 / 2 +  h) - 128.0)) / 128), driftx + xcen + ((MIDLX * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ycen + ((MIDLY * (cos8(i * radius2 +  h) - 128.0)) / 128), CHSV(h  - 85, 255, 255));
  }

}


void starz3()// random multipoint star
{
  if (counter == 0) {
    howmany = random (3, 11);
    inner = random(MIDLY / 4, MIDLY / 2);
    radius2 = 255 / howmany;
    shifty = 2;//move quick
  }
  for (int i = 0; i < howmany; i++)
  {
    leds.drawLine(driftx + ((inner * (sin8(i * radius2 + radius2 / 2 - 2 * h) - 128.0)) / 128), drifty + ((inner * (cos8(i * radius2 + radius2 / 2 - 2 * h) - 128.0)) / 128), driftx + ((MIDLX * (sin8(i * radius2 - 2 * h) - 128.0)) / 128), drifty + ((MIDLY * (cos8(i * radius2 - 2 * h) - 128.0)) / 128), CHSV(h * 2 , 255, 255));
    leds.drawLine(driftx + ((inner * (sin8(i * radius2 - radius2 / 2 - 2 * h) - 128.0)) / 128), drifty + ((inner * (cos8(i * radius2 - radius2 / 2 - 2 * h) - 128.0)) / 128), driftx + ((MIDLX * (sin8(i * radius2 - 2 * h) - 128.0)) / 128), drifty + ((MIDLY * (cos8(i * radius2 - 2 * h) - 128.0)) / 128), CHSV(h * 2 , 255, 255));
  }
}



void starz2()// pulsing 8 point star
{
  if (counter == 0) {
    inner = random(MIDLY / 5, MIDLY / 2);
    shifty = 1;
  }
  if (h % 16 == 0) {
    inner = inner + flipme;
    if (inner > MIDLY / 2 || inner < MIDLY / 5)
      flipme = -flipme;
  }
  for (int i = 0; i < 8; i++)
  {
    leds.drawLine(driftx + ((inner * (sin8(i * 32 + 16 - 2 * h) - 128.0)) / 128), drifty + ((inner * (cos8(i * 32 + 16 - 2 * h) - 128.0)) / 128), driftx + ((MIDLX * (sin8(i * 32 - 2 * h) - 128.0)) / 128), drifty + ((MIDLY * (cos8(i * 32 - 2 * h) - 128.0)) / 128), CHSV(h * 2 , 255, 255));
    leds.drawLine(driftx + ((inner * (sin8(i * 32 - 16 - 2 * h) - 128.0)) / 128), drifty + ((inner * (cos8(i * 32 - 16 - 2 * h) - 128.0)) / 128), driftx + ((MIDLX * (sin8(i * 32 - 2 * h) - 128.0)) / 128), drifty + ((MIDLY * (cos8(i * 32 - 2 * h) - 128.0)) / 128), CHSV(h * 2 , 255, 255));
  }
  if (flip3)// smallr star spinning the other way
    for (int i = 0; i < 8; i++)
    {
      leds.drawLine(driftx + ((inner / 2 * (sin8(i * 32 + 16 + 2 * h) - 128.0)) / 128), drifty + ((inner / 2 * (cos8(i * 32 + 16 + 2 * h) - 128.0)) / 128), driftx + ((inner / 4 * (sin8(i * 32 + 2 * h) - 128.0)) / 128), drifty + ((inner / 4 * (cos8(i * 32 + 2 * h) - 128.0)) / 128), CHSV(h * 2 + 87 , 255, 255));
      leds.drawLine(driftx + ((inner / 2 * (sin8(i * 32 - 16 + 2 * h) - 128.0)) / 128), drifty + ((inner / 2 * (cos8(i * 32 - 16 + 2 * h) - 128.0)) / 128), driftx + ((inner / 4 * (sin8(i * 32 + 2 * h) - 128.0)) / 128), drifty + ((inner / 4 * (cos8(i * 32 + 2 * h) - 128.0)) / 128), CHSV(h * 2 + 87 , 255, 255));
    }
}


void spoker()//2 spokes
{
  if (counter == 0) {
    howmany = random (4, 10);
    radius2 = 64 / howmany;

    poffset = random(50, 88);
  }
  for (int i = 0; i < howmany * 4; i++)
  {
    leds.drawLine(-MIDLX / 2,  -MIDLY / 2, ((2 * leds.matrixWidth * (sin8(i * radius2 -  h % 64) - 128.0)) / 128),  ((2 * leds.matrixHeight * (cos8(i * radius2 -  h % 64) - 128.0)) / 128), CHSV(h  , 255, 255));
    leds.drawLine(leds.matrixWidth + MIDLX / 2 , leds.matrixHeight + MIDLY / 2,  ((2 * leds.matrixWidth * (sin8(i * radius2  +  h % 64 + 128) - 128.0)) / 128),  ((2 * leds.matrixHeight * (cos8(i * radius2  + h % 64 + 128) - 128.0)) / 128), CHSV(h  + poffset , 255, 255));
  }
}

void spoker3()// 4 spokes
{
  if (counter == 0) {
    howmany = random (3, 6);
    radius2 = 64 / howmany;
    poffset = random(50, 88);
  }
  for (int i = 0; i < howmany * 4; i++)
  {
    leds.drawLine(-MIDLX / 2,  -MIDLY / 2, (leds.matrixWidth * (sin8(i * radius2 -  h % 64) - 128.0)) / 128,  (leds.matrixHeight * (cos8(i * radius2 -  h % 64) - 128.0)) / 128, CHSV(h  , 255, 255));
    leds.drawLine(leds.matrixWidth + MIDLX / 2 , leds.matrixHeight + MIDLY / 2,  leds.matrixWidth - (leds.matrixWidth * (sin8(i * radius2  +  h % 64 + 128) - 128.0)) / 128,  leds.matrixHeight - (leds.matrixHeight * (cos8(i * radius2  + h % 64 + 128) - 128.0)) / 128, CHSV(h  + poffset / 2 , 255, 255));

    leds.drawLine(-MIDLX / 2,  leds.matrixHeight + MIDLY / 2, (leds.matrixWidth * (sin8(i * radius2 -  h % 64 + 64) - 128.0)) / 128,  leds.matrixHeight - (leds.matrixHeight * (cos8(i * radius2 -  h % 64 + 64) - 128.0)) / 128, CHSV(h + poffset  , 255, 255));
    leds.drawLine(leds.matrixWidth + MIDLX / 2 , -MIDLY / 2,  leds.matrixWidth - (leds.matrixWidth * (sin8(i * radius2  +  h % 64 - 64) - 128.0)) / 128,  (leds.matrixHeight * (cos8(i * radius2  + h % 64 - 64) - 128.0)) / 128, CHSV(h  - poffset / 2 , 255, 255));
  }
}

void circlearc()// arc of circles
{
  if (counter % 888 == 0 || counter == 0)
  {

    bfade = random (2, 6);
    howmany = random (3, 6);
    radius2 = 64 / howmany;
    radius3 = random (leds.matrixWidth - (leds.matrixWidth >> 2), leds.matrixWidth + (leds.matrixWidth >> 2));
    poffset = random(1, 6);
    inner = random(5,  MIDLX / 2);
    // MM FLAGS: I don't like pattern 3 (filled circle within bigger circle)
    if (poffset == 3) poffset = 2;
    Serial.print("poffset ");
    Serial.println(poffset);


  }
  if ( counter % 20 == 0) {
    radius3 = radius3 + directn;

    if (radius3 <  leds.matrixWidth - (leds.matrixWidth >> 2) ||  radius3 > leds.matrixWidth + (leds.matrixWidth >> 2))
      directn = 0 - directn;
  }
  switch (poffset)
  {


    case 1:  // four -all headed in different direcitons
      for (int i = 0; i < howmany * 4; i++)
      {
        leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CHSV(h, 255, 255));
        //  leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner, CHSV(h  + 128, 255, 255));

        if ( flip2) {
          leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
          //   leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));
        }
      }
      for (int i = 0; i < howmany * 4; i++)
      {
        //   leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CHSV(h, 255, 255));
        leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner, CHSV(h  + 128, 255, 255));

        if ( flip2) {
          //     leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
          leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));
        }
      }
      break;

    case 2:// white 4 headed in dirrerent directions
      for (int i = 0; i < howmany * 4; i++)
      {
        if (flip2) {
          leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CHSV(h, 255, 255));
          //     leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner, CHSV(h  + 128, 255, 255));
          leds.drawCircle( radius3 * (sin8(i * radius2 - h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CRGB::White);
          //      leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner, CRGB::White);
        }
        leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
        //    leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));

        leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CRGB::White);
        //    leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CRGB::White);

      }
      for (int i = 0; i < howmany * 4; i++)
      {
        if (flip2) {
          //      leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CHSV(h, 255, 255));
          leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner, CHSV(h  + 128, 255, 255));
          //       leds.drawCircle( radius3 * (sin8(i * radius2 - h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CRGB::White);
          leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner, CRGB::White);
        }
        //     leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
        leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));

        //    leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CRGB::White);
        leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CRGB::White);

      }


      break;
    case 3:// some filled circles
      for (int i = 0; i < howmany * 4; i++)
      {
        leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner / 2, CHSV(h, 255, 255));
        //  leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner / 2, CHSV(h  + 128, 255, 255));
        if (flip2) {
          leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner , CHSV(h - 32, 255, 255));
          //   leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner , CHSV(h  + 128, 255, 255));
        }
        if (flip3) {
          leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner / 2, CHSV(h + 87, 255, 255));
          //     leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner / 2, CHSV(h  - 64, 255, 255));
          if (flip2)
          {
            leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 87 - 32, 255, 255));
            //     leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));
          }
        }
      }
      for (int i = 0; i < howmany * 4; i++)
      {
        //   leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner / 2, CHSV(h, 255, 255));
        leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner / 2, CHSV(h  - 87, 255, 255));
        if (flip2) {
          //     leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner , CHSV(h, 255, 255));
          leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner , CHSV(h  - 87 - 32, 255, 255));
        }
        if (flip3) {
          //     leds.drawFilledCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner / 2, CHSV(h -87+32, 255, 255));
          leds.drawFilledCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner / 2, CHSV(h  - 87 + 32, 255, 255));
          if (flip2)
          {
            //     leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h -87 + 32, 255, 255));
            leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  + -87 - 32, 255, 255));
          }
        }
      }

      break;
    case 4:// 8  opposite directions  littles
      for (int i = 0; i < howmany * 4; i++)
      {


        leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner / 2, CHSV(h, 255, 255));
        //  leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner / 2, CHSV(h  + 128, 255, 255));

        if (flip) {
          leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner / 2, CHSV(h + 64, 255, 255));
          //  leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner / 2, CHSV(h  - 64, 255, 255));
        }
        else
        {
          leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
          //      leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));
        }
      }
      for (int i = 0; i < howmany * 4; i++)
      {


        //   leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner / 2, CHSV(h, 255, 255));
        leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  - h % 64 + 128) - 128.0) / 128, inner / 2, CHSV(h  + 128, 255, 255));

        if (flip) {
          //     leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner / 2, CHSV(h + 64, 255, 255));
          leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner / 2, CHSV(h  - 64, 255, 255));
        }
        else
        {
          //       leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
          leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  -  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  - h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));
        }
      }
      break;
    default:// four headed together
      for (int i = 0; i < howmany * 4; i++)
      {
        // leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CHSV(h, 255, 255));
        leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  +  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  + h % 64 + 128) - 128.0) / 128, inner, CHSV(h  + 128, 255, 255));
        if (flip2)
        {
          //      leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
          leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  +  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  + h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));
        }
      }
      for (int i = 0; i < howmany * 4; i++)
      {
        leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64) - 128.0) / 128,   radius3 * (cos8(i * radius2 -  h % 64) - 128.0) / 128, inner, CHSV(h, 255, 255));
        //   leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  +  h % 64 + 128) - 128.0) / 128,  leds.matrixHeight - radius3 * (cos8(i * radius2  + h % 64 + 128) - 128.0) / 128, inner, CHSV(h  + 128, 255, 255));
        if (flip2)
        {
          leds.drawCircle( radius3 * (sin8(i * radius2 -  h % 64 - 64) - 128.0) / 128,   leds.matrixHeight - radius3 * (cos8(i * radius2 -  h % 64 - 64) - 128.0) / 128, inner, CHSV(h + 64, 255, 255));
          //    leds.drawCircle( leds.matrixWidth - radius3 * (sin8(i * radius2  +  h % 64 + 64) - 128.0) / 128,  radius3 * (cos8(i * radius2  + h % 64 + 64) - 128.0) / 128, inner, CHSV(h  - 64, 255, 255));
        }
      }

      break;
  }
}

void wheelz()// filled circles in a circle
{
  if (counter == 0) {
    howmany = random (4, 11);
    inner = random(MIDLY / 2, MIDLY);
    radius2 = 255 / howmany;
  }
  for (int i = 0; i < howmany; i++)
  {
    leds.drawFilledCircle(driftx + ((inner * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ((inner * (cos8(i * radius2 +   h) - 128.0)) / 128), inner * .70, CHSV(h  + 12 * i , 255, 255));
  }
  if (flip2 && flip3) {
    for (int i = 0; i < howmany; i++)
    {
      leds.drawFilledCircle(driftx + ((inner * (sin8(i * radius2 +  h) - 128.0)) / 128), drifty + ((inner * (cos8(i * radius2 +   h) - 128.0)) / 128), inner * .4, CHSV(h  + 12 * i , 220, 225));
    }
  }

}

void wheelz2()// circles in a circle
{
  if (counter == 0) {
    howmany = random (4, 11);
    inner = random(MIDLY / 2 - 2, MIDLY - 2);
    radius2 = 255 / howmany;
  }
  for (int i = 0; i < howmany; i++)
  {
    leds.drawCircle(driftx + ((inner * (sin8(i * radius2 -  h) - 128.0)) / 128), drifty + ((inner * (cos8(i * radius2 -   h) - 128.0)) / 128), inner, CHSV(h  + 8 * i , 255, 255));
  }

}

void swirl() {//not round orbits
  if (counter == 0)
  {
    howmany = random(leds.matrixHeight / 2, leds.matrixWidth * 2 / 3);
    blender = random(192);
    for ( uint16_t i = 0; i < howmany; i++)
    {
      xfire[i] = random(4, MIDLX + 9); //xradius
      yfire[i] =  random(xfire[i] - 2, xfire[i] + 2); //yradius

      //if (i % 5 == 0)// make some round
      //yfire[i] = xfire[i];
      fpeed[i] = random(1, 8); //speed
      fcolor[i] = blender + random(64); //color
      fcount[i] = random8(); //poffset
    }
  }
  for ( uint16_t i = 0; i < howmany; i++) {
    leds.drawPixel(driftx + xfire[i] * (sin8(h * fpeed[i] + fcount[i]) - 128.0) / 128 , drifty + yfire[i] * (cos8(h * fpeed[i] + fcount[i]) - 128.0) / 128, CHSV(fcolor[i], 255, 255));
    if (i % 9 == 0)
      leds.drawPixel(driftx + xfire[i] * (sin8(h * fpeed[i] + fcount[i]) - 128.0) / 128 , drifty + yfire[i] * (cos8(h * fpeed[i] + fcount[i]) - 128.0) / 128, CRGB::White);
    if (i == howmany - 1)
      leds.drawCircle(driftx + xfire[i] * (sin8(h * fpeed[i] + fcount[i]) - 128.0) / 128 , drifty + yfire[i] * (cos8(h * fpeed[i] + fcount[i]) - 128.0) / 128, dot, CHSV(fcolor[i], 255, 255));
    if (i == howmany - 2)
      leds.drawCircle(driftx + xfire[i] * (sin8(h * fpeed[i] + fcount[i]) - 128.0) / 128 , drifty + yfire[i] * (cos8(h * fpeed[i] + fcount[i]) - 128.0) / 128, dot - 2, CHSV(fcolor[i], 255, 255));
  }
}

void swirl2() {//round orbit backwards
  if (counter == 0)
  {
    xhowmany = random(leds.matrixHeight * 2 / 3, leds.matrixWidth * 3 / 2);
    xblender = random(192);
    for ( uint16_t i = 0; i < xhowmany; i++)
    {
      xslope[i] = random(4, MIDLX + 12); //radius
      fpeed[i] = random(1, 7); //speed
      xpoffset[i] = xblender + random(64); //color
      fcountr[i] = random8(); //poffset
    }
  }
  for ( uint16_t i = 0; i < xhowmany; i++) {
    leds.drawPixel(driftx + xslope[i] * (sin8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + xslope[i] * (cos8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128, CHSV(xpoffset[i], 255, 255));
    if (i % 7 == 0)
      leds.drawPixel(driftx + xslope[i] * (sin8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + xslope[i] * (cos8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128, CRGB::White);
  }
}

void swirl3() {// inner
  if (counter == 0)
  {
    xhowmany = random(leds.matrixHeight * 2 / 3, leds.matrixWidth * 3 / 2);
    xblender = random(192);
    for ( uint16_t i = 0; i < xhowmany; i++)
    {
      xslope[i] = random(4, MIDLX + 9); //radius
      fpeed[i] = random(1, 6); //speed
      xpoffset[i] = xblender + random(4); //color
      fcountr[i] = random8(); //poffset
    }
  }

  for ( uint16_t i = 0; i < xhowmany; i++) {
    if (counter % int(6 * (fpeed[i] + xslope[i])) == 0)
      xslope[i]--;
    if (xslope[i] == 1)
      xslope[i] = MIDLX + 11;
    leds.drawPixel(driftx + xslope[i] * (sin8(h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + xslope[i] * (cos8(h * fpeed[i] + fcountr[i]) - 128.0) / 128, CHSV(xpoffset[i], 255, 255));
    if (i % 8 == 0)
      leds.drawPixel(driftx + xslope[i] * (sin8(h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + xslope[i] * (cos8(h * fpeed[i] + fcountr[i]) - 128.0) / 128, CRGB::White);
  }
}

void swirl4() {// outer
  if (counter == 0)
  {
    xhowmany = random(leds.matrixHeight * 2 / 3, leds.matrixWidth * 3 / 2);
    xblender = random(192);
    for ( uint16_t i = 0; i < xhowmany; i++)
    {
      xslope[i] = random(4, MIDLX + 9); //radius
      fpeed[i] = random(2, 9); //speed
      xpoffset[i] = xblender + random(64); //color
      fcountr[i] = random8(); //poffset
    }
  }

  for ( uint16_t i = 0; i < xhowmany; i++) {
    // SIGFPE, modulo 0?
    if (counter % int(4 * fpeed[i] + 35 - xslope[i]) == 0)
      xslope[i]++;
    if (xslope[i] > leds.matrixWidth - 6)
      xslope[i] = 1;
    leds.drawPixel(driftx + xslope[i] * (sin8(h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + xslope[i] * (cos8(h * fpeed[i] + fcountr[i]) - 128.0) / 128, CHSV(xpoffset[i], 255, 255));
    if (i % 7 == 0)
      leds.drawPixel(driftx + xslope[i] * (sin8(h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + xslope[i] * (cos8(h * fpeed[i] + fcountr[i]) - 128.0) / 128, CRGB::White);
  }
}

void swirl5() {// both directions not round
  if (counter == 0)
  {
    xhowmany = random(leds.matrixHeight * 2 / 3, leds.matrixWidth * 3 / 2);
    xblender = random8();
    for ( uint16_t i = 0; i < xhowmany; i++)
    {
      xslope[i] = random(4, MIDLX + 9); //radius
      //yslope[i] = random(4, MIDLX + 9); //radius
      yslope[i] =  random(xslope[i] - 2, xslope[i] + 2); //yradius
      fpeed[i] = random(1, 9); //speed
      fcountr[i] = random8(); //poffset
    }
  }
  for ( uint16_t i = 0; i < xhowmany; i++) {
    leds.drawPixel(driftx + xslope[i] * (sin8(h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + yslope[i] * (cos8(h * fpeed[i] + fcountr[i]) - 128.0) / 128, CHSV(blender +  i, 255, 255));
    if (i % 5 == 0)
      leds.drawPixel(driftx + xslope[i] * (sin8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + yslope[i] * (cos8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128, CHSV(blender + 2 * i, 255, 255));
    if (i % 11 == 0)
      leds.drawPixel(driftx + xslope[i] * (sin8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + yslope[i] * (cos8(-h * fpeed[i] + fcountr[i]) - 128.0) / 128, CRGB::White);
    if (i % 13 == 0)
      leds.drawPixel(driftx + xslope[i] * (sin8(h * fpeed[i] + fcountr[i]) - 128.0) / 128 , drifty + yslope[i] * (cos8(h * fpeed[i] + fcountr[i]) - 128.0) / 128, CRGB::White);
  }
}

void audioprocess()
{
  longhammer = millis();//zzz top
  //if (adio)
    fakenoise();
#if 0
  else {
    while (ETin.receiveData() == false)
    {
      delay(5);
      if (longhammer - millis() > 50)
        delay(10);
        longhammer = millis();
        //  Serial.print("h ");
        //  Serial.println(counter);
        break;
      }
    }
    for (uint16_t qq = 0; qq < 31; qq ++)
    {
      faudio[qq * 2] = music.audio[qq];
      faudio[qq * 2 + 1] = (music.audio[qq] + music.audio[qq + 1]) >> 1;
    }

    for (uint16_t qq = 0; qq < 8; qq ++)
    {
      faudio[qq] = music.audio[qq];
    }

    for (uint16_t qq = 4; qq < 24; qq ++)
    {
      faudio[qq * 2 + 1] = music.audio[qq + 8];
      faudio[qq * 2 ] = (music.audio[qq + 8] + music.audio[qq + 7]) >> 1;
    }
  }
#endif
}

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

// vim:sts=2:sw=2
