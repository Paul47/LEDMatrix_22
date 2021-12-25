//Tom Thumb Numbers as 1 color sprites
//These are small 3x5 numbers for displaying in tiles

#include <avr/pgmspace.h>

#define NUM_CHARS 10
#define NUM_W 3 
#define NUM_H 5

const unsigned char num0[5] PROGMEM ={
B01000000,
B10100000,
B10100000,
B10100000,
B01000000
};

const unsigned char num1[5] PROGMEM ={
B01000000,
B11000000,
B01000000,
B01000000,
B11100000
};

const unsigned char num2[5] PROGMEM ={
B11000000,
B00100000,
B01000000,
B11000000,
B11100000
};


const unsigned char num3[5] PROGMEM ={
B11000000,
B00100000,
B11000000,
B00100000,
B11000000
};

const unsigned char num4[5] PROGMEM ={
B10100000,
B10100000,
B01100000,
B00100000,
B00100000
};

const unsigned char num5[5] PROGMEM ={
B11100000,
B10000000,
B11000000,
B00100000,
B11000000
};

const unsigned char num6[5] PROGMEM ={
B01100000,
B10000000,
B11000000,
B10100000,
B01000000
};

const unsigned char num7[5] PROGMEM ={
B11100000,
B00100000,
B01000000,
B10000000,
B10000000
};

const unsigned char num8[5] PROGMEM ={
B01000000,
B10100000,
B01000000,
B10100000,
B01000000
};

const unsigned char num9[5] PROGMEM ={
B01000000,
B10100000,
B01000000,
B00100000,
B11000000
};
