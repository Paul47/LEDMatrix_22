#pragma once


//>>>>>>>>>>>>>>>  BESTPATTERNS selects fewer pattern <<<<<<<<<<<
// This allows a selection of only my favourite patterns.
// Comment this out to get all the patterns -- merlin
#define BESTPATTERNS	//enable a limited number of the best patterns
#ifdef BESTPATTERNS
/**
	uint8_t bestpatterns[] = {
		// 52 and 61 are almost the same
		10, 11, 25, 52, 60, 61, 62, 67, 70, 72, 73, 77, 80, 86, 105,
		4, 59, 60, 72, 89, 104, 110 
		};	  // ok
	#define numbest         sizeof(bestpatterns)
	#define lastpatindex	numbest
*/
	//enable a larger set that are mostly interesting and unique
	uint8_t bestpatterns[] = {
		107, 109, 111, 74, 86, 6, 11, 12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 24, 25, 27, 31, 34, 36, 45, 46, 48,
		49, 50, 51, 52, 53, 54, 56, 58, 59, 61, 62, 63, 64, 65,
		66, 67, 69, 70, 73, 79, 80, 87, 88, 90, 91, 92, 93, 94,
		96, 97, 98, 99, 101, 102, 103, 106, 110, 112
		};
	#define numbest         sizeof(bestpatterns)
	#define lastpatindex	numbest
#else
	#define lastpatindex mpatterns
	// mixit = true;
#endif

#define MIDLX               (MATRIX_WIDTH/2)
#define MIDLY               (MATRIX_HEIGHT/2)
#define mpatterns           113// max number of patterns


// yes, I use a lot of global variables, likey some of these are redundant or not even used...

float mscale = 2.2, fps;
float radius, xslope[MATRIX_WIDTH * 3], yslope[MATRIX_WIDTH * 3], xfire[MATRIX_WIDTH * 3], yfire[MATRIX_WIDTH * 3], cangle, sangle;
int16_t  xxx, yyy, dot = 3, radius2, rr, gg, bb, adjunct = 3, fcount[MATRIX_WIDTH * 3], fcolor[MATRIX_WIDTH * 3], fcountr[MATRIX_WIDTH * 3];
int16_t pointy, laps, hue, steper, xblender, hhowmany, blender = 128, radius3, xpoffset[MATRIX_WIDTH * 3];
unsigned int  counter, ringdelay, bringdelay, sumthum;
int16_t  pointyfix, fpeed[MATRIX_WIDTH * 3], targetfps = 40;
boolean adio, ringme = false, blackringme = false;
int waiter[mpatterns];
boolean flip = true, flip2 = false, flip3 = true, mixit = false, rimmer[MATRIX_WIDTH * 2], xbouncer[MATRIX_WIDTH * 2], ybouncer[MATRIX_WIDTH * 2];
uint16_t ccoolloorr, why1, why2, why3, eeks1, eeks2, eeks3, h = 0, oldpattern, howmany, xhowmany, kk;
unsigned long lasttest, lastmillis, dwell = 5000, longhammer;
float locusx, locusy, driftx, drifty, xcen, ycen, yangle, xangle;
int16_t raad, lender = 128, xsizer, ysizer, xx, yy, flipme = 1;
int16_t shifty = 6, pattern = 0, poffset;
int16_t sinewidth, mstep, faudio[MATRIX_WIDTH], inner, bfade = 6;
int directn = 1, quash = 5;

//all functions
void adjuster();
void audio();
void audio2();
void audio3();
void audioprocess();
void Bargraph();
void bkboxer();
void bkringer();
void bkstarer();
void bluefado(int16_t bbc);
void bounce();
void boxer();
void bubbles();
void bubbles2();
void circlearc();
void confetti();
void confetti2();
void confetti3();
void confetti4();
void corner();
void diagonally();
void Diamondhole();
void drawstar(int16_t xlocl, int16_t ylocl, int16_t biggy, int16_t little, int16_t points, int16_t dangle, int16_t koler);
void drawtriangle();
void drifter();
void drip();
void fakenoise();
void fireball();
void fuzzy();
void greenfado(int16_t bbc);
void homer();
void homer2();
void homer3();
void hypnoduck();
void hypnoduck2();
void hypnoduck3();
void hypnoduck4();
void Inca();
void lfado(int16_t bbc);
void loop();
void magictime();
void mirror();
void newpattern();
void nringer(int16_t i);
void pyrimid();
void Raudio();
void Raudio2();
void Raudio2a();
void Raudio3();
void Raudio4();
void Raudio5();
void Raudio6();
void redfado(int16_t bbc);
void ringer();
void Ringo();
void rmagictime();
void roger();
void Roundhole();
void runpattern();
void seasick();
void seasick2();
void seasick3();
void seasick4();
void seasick5();
void setup();
void sinx();
void siny();
void smile();
void smile2();
void smile3();
void smile4();
void solid();
void solid2();
void solidpyrimid();
void spin();
void spin2();
void spire();
void spire2();
void spire3();
void splat();
void spoker();
void spoker3();
void starbounce();
void starer();
void starz();
void starz2();
void starz3();
void sticks();
void streaker();
void swirl();
void swirl2();
void swirl3();
void swirl4();
void swirl5();
void triangle(int16_t xloc, uint16_t yloc, uint16_t bigg, uint16_t angle, uint16_t kolor);
void triangler();
void triforce();
void triple();
void volcano();
void warp();
void whatami();
void wheelz();
void wheelz2();
void whitewarp();
void xspin();
void zoro();


