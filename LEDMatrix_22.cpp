
/* --------------LEDMatrix_22 -------------------------
* LEDMatrix_22 updates and expands previous version, by Paul Dymerski 2021
* modified:  LEDMatrix V5 class by Aaron Liddiment (c) 2016
* modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
* date:      2016/04/27
*
* Inspiration for some of the Matrix functions from Stefan Petrick
*
* FastLED v3.1 library by Daniel Garcia and Mark Kriegsmann.
* Written & tested on a Teensy 3.1 using Arduino V1.6.3 & teensyduino V1.22
*/

#include "LEDMatrix_22.h"

#define pt(msg)     Serial.println(msg);    //Serial.println MACRO
#define ptt(msg)     Serial.print(msg);    //Serial.printl MACRO


#ifdef __AVR__
    #include <avr/pgmspace.h>
    #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#elif defined(ESP8266)
#include <pgmspace.h>
    #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
    #ifdef pgm_read_pointer
        #undef pgm_read_pointer
    #endif
   #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))        //LEDMatrix_22 changed right side word to dword bug fix
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

cLEDMatrixBase::cLEDMatrixBase()
{
  _width = MATRIX_WIDTH;      //_width, _height changes with rotation, etc.
  _height = MATRIX_HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap      = true;
  _cp437    = false;

    #if ENABLED_FONTS        //See ENABLE_FONTS in configuration header
      gfxFont   = NULL;
    #endif
}


struct CRGB* cLEDMatrixBase::operator[](int n)
{
  return(&cLED[n]);
}

struct CRGB& cLEDMatrixBase::operator()(int16_t x, int16_t y)
{
  if ( (x >= 0) && (x < m_Width) && (y >= 0) && (y < m_Height))
    return(cLED[mXY(x, y)]);
  else
    return(m_OutOfBounds);
}

struct CRGB& cLEDMatrixBase::operator()(int16_t i)
{
  if ((i >=0) && (i < (m_Width * m_Height)))
    return(cLED[i]);
  else
    return(m_OutOfBounds);
}

void cLEDMatrixBase::HorizontalMirror(bool FullHeight)
{
  int ty, y, x, xx;

  if (FullHeight)
    ty = m_Height - 1;
  else
    ty = (m_Height / 2);
  for (y=ty; y>=0; --y)
  {
    for (x=(m_Width/2)-1,xx=((m_Width+1)/2); x>=0; --x,++xx)
      cLED[mXY(xx, y)] = cLED[mXY(x, y)];
  }
}


void cLEDMatrixBase::VerticalMirror()
{
  int y, yy, x;

  for (y=(m_Height/2)-1,yy=((m_Height+1)/2); y>=0; --y,++yy)
  {
    for (x=m_Width-1; x>=0; --x)
      cLED[mXY(x, yy)] = cLED[mXY(x, y)];
  }
}


void cLEDMatrixBase::QuadrantMirror()
{
  HorizontalMirror(false);
  VerticalMirror();
}


void cLEDMatrixBase::QuadrantRotateMirror()
{
  int MaxXY, MidXY, x, y, src;

  if (m_Width > m_Height)
    MaxXY = m_Height;
  else
    MaxXY = m_Width;
  MidXY = (MaxXY / 2);
  MaxXY--;
  for (x=MidXY-(MaxXY%2); x>=0; --x)
  {
    for (y=MidXY-(MaxXY%2); y>=0; --y)
    {
      src = mXY(x, y);
      cLED[mXY(MidXY + y, MidXY - (MaxXY % 2) - x)] = cLED[src];
      cLED[mXY(MaxXY - x, MaxXY - y)] = cLED[src];
      cLED[mXY(MidXY - (MaxXY % 2) - y, MidXY + x)] = cLED[src];
    }
  }
}


void cLEDMatrixBase::TriangleTopMirror(bool FullHeight)
{
  int MaxXY, x, y;

  if (m_Width > m_Height)
    MaxXY = m_Height - 1;
  else
    MaxXY = m_Width - 1;
  if (! FullHeight)
    MaxXY /= 2;
  for (y=1; y<=MaxXY; ++y)
  {
    for (x=0; x<y; ++x)
      cLED[mXY(y,x)] = cLED[mXY(x,y)];
  }
}


void cLEDMatrixBase::TriangleBottomMirror(bool FullHeight)
{
  int MaxXY, x, y, xx, yy;

  if (m_Width > m_Height)
    MaxXY = m_Height - 1;
  else
    MaxXY = m_Width - 1;
  if (! FullHeight)
    MaxXY /= 2;
  for (y=0,xx=MaxXY; y<MaxXY; y++,xx--)
  {
    for (x=MaxXY-y-1,yy=y+1; x>=0; --x,++yy)
      cLED[mXY(xx, yy)] = cLED[mXY(x, y)];
  }
}


void cLEDMatrixBase::QuadrantTopTriangleMirror()
{
  TriangleTopMirror(false);
  QuadrantMirror();
}


void cLEDMatrixBase::QuadrantBottomTriangleMirror()
{
  TriangleBottomMirror(false);
  QuadrantMirror();
}

void cLEDMatrixBase::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col)
{
  int16_t dx = x1 - x0;
  int16_t dy = y1 - y0;
  if (abs(dx) >= abs(dy))
  {
    int32_t y = ((int32_t)y0 << 16) + 32768;
    // Support a single dot line without diving by 0 and crashing below
    if (!dx) {
      (*this)(x0, (y >> 16)) = Col;
    } else {
      int32_t f = ((int32_t)dy << 16) / (int32_t)abs(dx);
      if (dx >= 0)
      {
        for (; x0<=x1; ++x0,y+=f)
          (*this)(x0, (y >> 16)) = Col;
      }
      else
      {
        for (; x0>=x1; --x0,y+=f)
          (*this)(x0, (y >> 16)) = Col;
      }
    }
  }
  else
  {
    int32_t f = ((int32_t)dx << 16) / (int32_t)abs(dy);
    int32_t x = ((int32_t)x0 << 16) + 32768;
    if (dy >= 0)
    {
      for (; y0<=y1; ++y0,x+=f)
        (*this)((x >> 16), y0) = Col;
    }
    else
    {
      for (; y0>=y1; --y0,x+=f)
        (*this)((x >> 16), y0) = Col;
    }
  }
}


void cLEDMatrixBase::drawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col)
{
  drawLine(x0, y0, x0, y1, Col);
  drawLine(x0, y1, x1, y1, Col);
  drawLine(x1, y1, x1, y0, Col);
  drawLine(x1, y0, x0, y0, Col);
}


void cLEDMatrixBase::drawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col)
{
  int16_t x = -r;
  int16_t y = 0;
  int16_t e = 2 - (2 * r);
  do
  {
    (*this)(xc + x, yc - y) = Col;
    (*this)(xc - x, yc + y) = Col;
    (*this)(xc + y, yc + x) = Col;
    (*this)(xc - y, yc - x) = Col;
    int16_t _e = e;
    if (_e <= y)
      e += (++y * 2) + 1;
    if ((_e > x) || (e > y))
      e += (++x * 2) + 1;
  }
  while (x < 0);
}


void cLEDMatrixBase::drawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col)
{
  int16_t y = min(y0, y1);
  for (int16_t c=abs(y1-y0); c>=0; --c,++y)
    drawLine(x0, y, x1, y, Col);
}


void cLEDMatrixBase::drawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col)
{
  int16_t x = r;
  int16_t y = 0;
  int16_t e = 1 - x;
  while (x >= y)
  {
    drawLine(xc + x, yc + y, xc - x, yc + y, Col);
    drawLine(xc + y, yc + x, xc - y, yc + x, Col);
    drawLine(xc - x, yc - y, xc + x, yc - y, Col);
    drawLine(xc - y, yc - x, xc + y, yc - x, Col);
    ++y;
    if (e >= 0)
    {
      --x;
      e += 2 * ((y - x) + 1);
    }
    else
      e += (2 * y) + 1;
  }
}


//==================LEDMatrix_22  additions ============================

void cLEDMatrixBase::clear(){
    fillScreen(CRGB::Black);
    show();                  //trap to extender show

}

//set both Extender and FastLED brightnesses
void cLEDMatrixBase::setBrightness(uint8_t bght){
    e_brightness = bght;
    FastLED.setBrightness(bght);
}

//this fills the color then lights the leds
void cLEDMatrixBase::showColor(CRGB color){
    fillScreen(color);
    show();                  //trap to extender show
}

void cLEDMatrixBase::fadeAll(uint16_t value) {
  CRGBSet tleds(cLED, NUM_LEDS);           //use FastLED RGBset()
  tleds(0, NUM_LEDS).nscale8(value);
} 

void cLEDMatrixBase::fillScreen(CRGB col) {
   CRGBSet tleds(cLED, NUM_LEDS);            //use FastLED RGBset()
   tleds(0, NUM_LEDS) = col;
}

/*
* Original code in this library did not do rotation
*  new draw routine below does rotation
* void cLEDMatrixBase::drawPixel(int16_t x, int16_t y, CRGB Col) {
*   drawLine(x, y, x, y, Col);
*/

//from FastLED_GFX - can rotate
void cLEDMatrixBase::drawPixel(int16_t x, int16_t y, CRGB color) {

  int16_t t;
  switch(rotation) {
   case 1:
    t = x;
    x = m_Width  - 1 - y;
    y = t;
    break;
   case 2:
    x = m_Width - 1 - x;
    y = m_Height - 1 - y;
    break;
   case 3:
    t = x;
    x = y;
    y = m_Height - 1 - t;
    break;
  }

  if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;   //rotated sizes
  cLED[mXY(x, y)] = color;                //original code
  
  //cLED[M_XY(x, y)] = color;  //future trigger test only	
}

//from FastLED_GFX - if rotated still get correct color
CRGB cLEDMatrixBase::getPixel(int16_t x, int16_t y) {

  int16_t t;
  switch(rotation) {
   case 1:
    t = x;
    x = m_Width  - 1 - y;
    y = t;
    break;
   case 2:
    x = m_Width - 1 - x;
    y = m_Height - 1 - y;
    break;
   case 3:
    t = x;
    x = y;
    y = m_Height - 1 - t;
    break;
  }
  if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return 0;  //rotated out of bounds?
  return cLED[mXY(x, y)];
}

/*
* New function getPixelIndex  - return led index into led strip.
* Takes into account any rotation to match drawPixel() with rotation
* set by setRotation(); //0-3 in 90 deg steps
*/
int16_t  cLEDMatrixBase::getPixelIndex(int16_t x, int16_t y) {

    int16_t t;
    switch (rotation) {
    case 1:
        t = x;
        x = m_Width - 1 - y;
        y = t;
        break;
    case 2:
        x = m_Width - 1 - x;
        y = m_Height - 1 - y;
        break;
    case 3:
        t = x;
        x = y;
        y = m_Height - 1 - t;
        break;
    }
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return -1;       //rotated out of bounds?
    return mXY(x, y);
}

// draw a triangle
void cLEDMatrixBase::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color) 
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void cLEDMatrixBase::drawFastVLine(int16_t x, int16_t y, int16_t h, CRGB color) {
    // Update in subclasses if desired!
    drawLine(x, y, x, y + h - 1, color);
}

void cLEDMatrixBase::drawFastHLine(int16_t x, int16_t y, int16_t w, CRGB color) {
    // Update in subclasses if desired!
    drawLine(x, y, x + w - 1, y, color);
}


// Fill a triangle
void cLEDMatrixBase::drawFilledTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color) {
    int16_t a, b, y, last;
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        adagfxswap(y0, y1); adagfxswap(x0, x1);
    }
    if (y1 > y2) {
        adagfxswap(y2, y1); adagfxswap(x2, x1);
    }
    if (y0 > y1) {
        adagfxswap(y0, y1); adagfxswap(x0, x1);
    }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)      a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a)      a = x2;
        else if (x2 > b) b = x2;
        drawFastHLine(a, y0, b - a + 1, color);
        return;
    }

    int16_t
        dx01 = x1 - x0,
        dy01 = y1 - y0,
        dx02 = x2 - x0,
        dy02 = y2 - y0,
        dx12 = x2 - x1,
        dy12 = y2 - y1;
    int32_t
        sa = 0,
        sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second step will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second step...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1 - 1; // Skip it

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) adagfxswap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This step is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) adagfxswap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }
}

void cLEDMatrixBase::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, CRGB color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

// Used to do circles and roundrects
void cLEDMatrixBase::drawFillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, CRGB color) {

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1) {
            drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

// draw a rounded rectangle
void cLEDMatrixBase::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, CRGB color) {
    // smarter version
    drawFastHLine(x + r, y, w - 2 * r, color); // Top
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    drawFastVLine(x, y + r, h - 2 * r, color); // Left
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle
void cLEDMatrixBase::drawFillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, CRGB color) {
    // smarter version
    drawRectangle(x + r, y, w - 2 * r, h, color);

    // draw four corners
    drawFillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    drawFillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

//==========================characters ==========================================

void cLEDMatrixBase::setCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
}

int16_t cLEDMatrixBase::getCursorX(void) const {
    return cursor_x;
}

int16_t cLEDMatrixBase::getCursorY(void) const {
    return cursor_y;
}

void cLEDMatrixBase::setTextSize(uint8_t s) {
    textsize = (s > 0) ? s : 1;
}

void cLEDMatrixBase::setTextColor(CRGB c) {
    // For 'transparent' background, we'll set the bg
    // to the same as fg instead of using a flag
    textcolor = textbgcolor = c;
}

void cLEDMatrixBase::setTextColor(CRGB c, CRGB bg) {
    textcolor = c;
    textbgcolor = bg;
}

void cLEDMatrixBase::setTextWrap(boolean w) {
    wrap = w;
}

uint8_t cLEDMatrixBase::getRotation(void) const {
    return rotation;
}

void cLEDMatrixBase::setRotation(uint8_t x) {
    rotation = (x & 3);
    switch (rotation) {
    case 0:
    case 2:
        _width = m_Width;   //from #define WIDTH
        _height = m_Height; //from #define HEIGHT
        break;
    case 1:
    case 3:
        _width = m_Height;    //from #define HEIGHT
        _height = m_Width;    //from #define WIDTH
        break;
    }
}

/*
* If true,  "gfxfont.h" and "glcdfont.c" will be included - must be in the library folder
*     and all character functions will be enabled
*/
#if ENABLE_FONTS             //See ENABLE_FONTS in configuration header

// Enable (or disable) Code Page 437-compatible charset.
// There was an error in glcdfont.c for the longest time -- one character
// (#176, the 'light shade' block) was missing -- this threw off the index
// of every character that followed it.  But a TON of code has been written
// with the erroneous character indices.  By default, the library uses the
// original 'wrong' behavior and old sketches will still work.  Pass 'true'
// to this function to use correct CP437 character values in your code.
void cLEDMatrixBase::cp437(boolean x) {
    _cp437 = x;
}

void cLEDMatrixBase::setFont(const GFXfont* f) {
    if (f) {          // Font struct pointer passed in?
        if (!gfxFont) { // And no current font struct?
          // Switching from classic to new font behavior.
          // Move cursor pos down 6 pixels so it's on baseline.
            cursor_y += 6;
        }
    }
    else if (gfxFont) { // NULL passed.  Current font struct defined?
   // Switching from new to classic font behavior.
   // Move cursor pos up 6 pixels so it's at top-left of char.
        cursor_y -= 6;
    }
    gfxFont = (GFXfont*)f;
}

// Pass string and a cursor position, returns UL corner and W,H.
void cLEDMatrixBase::getTextBounds(char* str, int16_t x, int16_t y,
    int16_t* x1, int16_t* y1, uint16_t* w, uint16_t* h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w = *h = 0;

    if (gfxFont) {

        GFXglyph* glyph;
        uint8_t   first = pgm_read_byte(&gfxFont->first),
            last = pgm_read_byte(&gfxFont->last),
            gw, gh, xa;
        int8_t    xo = 0, yo = 0;
        int16_t   minx = _width, miny = _height, maxx = -1, maxy = -1,
            gx1, gy1, gx2, gy2, ts = (int16_t)textsize,
            ya = ts * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);

        while ((c = *str++)) {
            if (c != '\n') { // Not a newline
                if (c != '\r') { // Not a carriage return, is normal char
                    if ((c >= first) && (c <= last)) { // Char present in current font
                        c -= first;
                        glyph = &(((GFXglyph*)pgm_read_pointer(&gfxFont->glyph))[c]);
                        gw = pgm_read_byte(&glyph->width);
                        gh = pgm_read_byte(&glyph->height);
                        xa = pgm_read_byte(&glyph->xAdvance);
                        xo = pgm_read_byte(&glyph->xOffset);
                        yo = pgm_read_byte(&glyph->yOffset);
                        if (wrap && ((x + (((int16_t)xo + gw) * ts)) >= _width)) {
                            // Line wrap
                            x = 0;  // Reset x to 0
                            y += ya; // Advance y by 1 line
                        }
                        gx1 = x + xo * ts;
                        gy1 = y + yo * ts;
                        gx2 = gx1 + gw * ts - 1;
                        gy2 = gy1 + gh * ts - 1;
                        if (gx1 < minx) minx = gx1;
                        if (gy1 < miny) miny = gy1;
                        if (gx2 > maxx) maxx = gx2;
                        if (gy2 > maxy) maxy = gy2;
                        x += xa * ts;
                    }
                } // Carriage return = do nothing
            }
            else { // Newline
                x = 0;  // Reset x
                y += ya; // Advance y by 1 line
            }
        }
        // End of string
        *x1 = minx;
        *y1 = miny;
        if (maxx >= minx) *w = maxx - minx + 1;
        if (maxy >= miny) *h = maxy - miny + 1;

    }
    else { // Default font

        uint16_t lineWidth = 0, maxWidth = 0; // Width of current, all lines

        while ((c = *str++)) {
            if (c != '\n') { // Not a newline
                if (c != '\r') { // Not a carriage return, is normal char
                    if (wrap && ((x + textsize * 6) >= _width)) {
                        x = 0;            // Reset x to 0
                        y += textsize * 8; // Advance y by 1 line
                        if (lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
                        lineWidth = textsize * 6; // First char on new line
                    }
                    else { // No line wrap, just keep incrementing X
                        lineWidth += textsize * 6; // Includes interchar x gap
                    }
                } // Carriage return = do nothing
            }
            else { // Newline
                x = 0;            // Reset x to 0
                y += textsize * 8; // Advance y by 1 line
                if (lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
                lineWidth = 0;     // Reset lineWidth for new line
            }
        }
        // End of string
        if (lineWidth) y += textsize * 8; // Add height of last (or only) line
        *w = maxWidth - 1;               // Don't include last interchar x gap
        *h = y - *y1;

    } // End classic vs custom font
}

// Same as above, but for PROGMEM strings
void cLEDMatrixBase::getTextBounds(const __FlashStringHelper *str,
 int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
  uint8_t *s = (uint8_t *)str, c;

  *x1 = x;
  *y1 = y;
  *w  = *h = 0;

  if(gfxFont) {

    GFXglyph *glyph;
    uint8_t   first = pgm_read_byte(&gfxFont->first),
              last  = pgm_read_byte(&gfxFont->last),
              gw, gh, xa;
    int8_t    xo, yo;
    int16_t   minx = _width, miny = _height, maxx = -1, maxy = -1,
              gx1, gy1, gx2, gy2, ts = (int16_t)textsize,
              ya = ts * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);

    while((c = pgm_read_byte(s++))) {
      if(c != '\n') { // Not a newline
        if(c != '\r') { // Not a carriage return, is normal char
          if((c >= first) && (c <= last)) { // Char present in current font
            c    -= first;
            glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
            gw    = pgm_read_byte(&glyph->width);
            gh    = pgm_read_byte(&glyph->height);
            xa    = pgm_read_byte(&glyph->xAdvance);
            xo    = pgm_read_byte(&glyph->xOffset);
            yo    = pgm_read_byte(&glyph->yOffset);
            if(wrap && ((x + (((int16_t)xo + gw) * ts)) >= _width)) {
              // Line wrap
              x  = 0;  // Reset x to 0
              y += ya; // Advance y by 1 line
            }
            gx1 = x   + xo * ts;
            gy1 = y   + yo * ts;
            gx2 = gx1 + gw * ts - 1;
            gy2 = gy1 + gh * ts - 1;
            if(gx1 < minx) minx = gx1;
            if(gy1 < miny) miny = gy1;
            if(gx2 > maxx) maxx = gx2;
            if(gy2 > maxy) maxy = gy2;
            x += xa * ts;
          }
        } // Carriage return = do nothing
      } else { // Newline
        x  = 0;  // Reset x
        y += ya; // Advance y by 1 line
      }
    }
    // End of string
    *x1 = minx;
    *y1 = miny;
    if(maxx >= minx) *w  = maxx - minx + 1;
    if(maxy >= miny) *h  = maxy - miny + 1;

  } else { // Default font

    uint16_t lineWidth = 0, maxWidth = 0; // Width of current, all lines

    while((c = pgm_read_byte(s++))) {
      if(c != '\n') { // Not a newline
        if(c != '\r') { // Not a carriage return, is normal char
          if(wrap && ((x + textsize * 6) >= _width)) {
            x  = 0;            // Reset x to 0
            y += textsize * 8; // Advance y by 1 line
            if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
            lineWidth  = textsize * 6; // First char on new line
          } else { // No line wrap, just keep incrementing X
            lineWidth += textsize * 6; // Includes interchar x gap
          }
        } // Carriage return = do nothing
      } else { // Newline
        x  = 0;            // Reset x to 0
        y += textsize * 8; // Advance y by 1 line
        if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
        lineWidth = 0;     // Reset lineWidth for new line
      }
    }
    // End of string
    if(lineWidth) y += textsize * 8; // Add height of last (or only) line
    *w = maxWidth - 1;               // Don't include last interchar x gap
    *h = y - *y1;

  } // End classic vs custom font
}

// draw a character
void cLEDMatrixBase::drawChar(int16_t x, int16_t y, unsigned char c,
    CRGB color, CRGB bg, uint8_t size) {

    if (!gfxFont) { // 'Classic' built-in font

        if ((x >= _width) || // Clip right
            (y >= _height) || // Clip bottom
            ((x + 6 * size - 1) < 0) || // Clip left
            ((y + 8 * size - 1) < 0))   // Clip top
            return;

        if (!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

        for (int8_t i = 0; i < 6; i++) {
            uint8_t line;
            if (i < 5) line = pgm_read_byte(font + (c * 5) + i);
            else      line = 0x0;
            for (int8_t j = 0; j < 8; j++, line >>= 1) {
                if (line & 0x1) {
                    if (size == 1) drawPixel(x + i, y + j, color);
                    else          drawRectangle(x + (i * size), y + (j * size), size, size, color);
                }
                else if (bg != color) {
                    if (size == 1) drawPixel(x + i, y + j, bg);
                    else          drawFilledRectangle(x + i * size, y + j * size, size, size, bg);
                }
            }
        }

    }
    else { // Custom font

   // Character is assumed previously filtered by write() to eliminate
   // newlines, returns, non-printable characters, etc.  Calling drawChar()
   // directly with 'bad' characters of font may cause mayhem!

        c -= pgm_read_byte(&gfxFont->first);
        GFXglyph* glyph = &(((GFXglyph*)pgm_read_pointer(&gfxFont->glyph))[c]);
        uint8_t* bitmap = (uint8_t*)pgm_read_pointer(&gfxFont->bitmap);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w = pgm_read_byte(&glyph->width),
            h = pgm_read_byte(&glyph->height);
           // xa = pgm_read_byte(&glyph->xAdvance); //not used
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
            yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;

        if (size > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here
        /*
        * NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        * THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        /* has typically been used with the 'classic' font to overwrite old
        * screen contents with new data.  This ONLY works because the
        * characters are a uniform size; it's not a sensible thing to do with
        * proportionally-spaced fonts with glyphs of varying sizes (and that
        * may overlap).  To replace previously-drawn text when using a custom
        * font, use the getTextBounds() function to determine the smallest
        * rectangle encompassing a string, erase the area with fillRect(),
        * then draw new text.  This WILL infortunately 'blink' the text, but
        * is unavoidable.  drawing 'background' pixels will NOT fix this,
        * only creates a new set of problems.  Have an idea to work around
        * this (a canvas object type for MCUs that can afford the RAM and
        * displays supporting setAddrWindow() and pushColors()), but haven't
        * implemented this yet.
        */
        for (yy = 0; yy < h; yy++) {
            for (xx = 0; xx < w; xx++) {
                if (!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if (bits & 0x80) {
                    if (size == 1) {
                        drawPixel(x + xo + xx, y + yo + yy, color);
                    }
                    else {
                        drawFilledRectangle(x + (xo16 + xx) * size, y + (yo16 + yy) * size, size, size, color);
                    }
                }
                bits <<= 1;
            }
        }

    } // End classic vs custom font
}

// print a text string ex: char text[] = "HELLO WORLD!";
void cLEDMatrixBase::print(char text[]) {
    for (uint8_t i = 0; i < strlen(text); i++) {
        write(text[i]);      // write the letter
    }
       show();     //replace FastLED.show() for Extender
    }


void cLEDMatrixBase::write(char c) {

    if (!gfxFont) { // 'Classic' built-in font

        if (c == '\n') {
            cursor_y += textsize * 8;
            cursor_x = 0;
        }
        else if (c == '\r') {
            // skip em
        }
        else {
            if (wrap && ((cursor_x + textsize * 6) >= _width)) { // Heading off edge?
                cursor_x = 0;            // Reset x to zero
                cursor_y += textsize * 8; // Advance y one line
            }
            drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
            cursor_x += textsize * 6;
        }

    }
    else { // Custom font

        if (c == '\n') {
            cursor_x = 0;
            cursor_y += (int16_t)textsize *
                (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        }
        else if (c != '\r') {
            uint8_t first = pgm_read_byte(&gfxFont->first);
            if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
                uint8_t   c2 = c - pgm_read_byte(&gfxFont->first);
                GFXglyph* glyph = &(((GFXglyph*)pgm_read_pointer(&gfxFont->glyph))[c2]);
                uint8_t   w = pgm_read_byte(&glyph->width),
                    h = pgm_read_byte(&glyph->height);
                if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if (wrap && ((cursor_x + textsize * (xo + w)) >= _width)) {
                        // drawing character would go off right edge; wrap to new line
                        cursor_x = 0;
                        cursor_y += (int16_t)textsize *
                            (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                    }
                    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
                }
                cursor_x += pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize;
            }
        }

    }
}

#endif      //ENABLED_FONTS            //See ENABLE_FONTS in configuration header

void cLEDMatrixBase::invertDisplay() 
{
   for (uint16_t i = 0; i < m_Height * m_Width; i++) {
        cLED[i] = -cLED[i];               //FastLED math
    }
}

void cLEDMatrixBase::invertSquare(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    for (int16_t y = y0; y < y1; y++) {
        for (int16_t x = x0; x < x1; x++) {
            cLED[mXY(x, y)] = -cLED[mXY(x, y)];   //FastLED math
        }
    }

}
/*
* ---------------------------- bitmaps -----------------------------
*
* draw a 1 - bit image(bitmap) at the specified(x, y) position from the
* provided bitmap buffer using the specified
* foreground color (unset bits are transparent).
*/
void cLEDMatrixBase::drawBitmap(int16_t x, int16_t y, const uint8_t * bitmap, int16_t w, int16_t h, CRGB color) {
    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte = 0;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (i & 7) byte <<= 1;
            else      byte = pgm_read_byte(bitmap + j * byteWidth + i / 8);
            if (byte & 0x80) drawPixel(x + i, y + j, color);
        }
    }
}

// drawBitmap() variant for RAM-resident (not PROGMEM) bitmaps.
void cLEDMatrixBase::drawBitmap(int16_t x, int16_t y,
 uint8_t *bitmap, int16_t w, int16_t h, CRGB color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte = 0;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte <<= 1;
      else      byte   = bitmap[j * byteWidth + i / 8];
      if(byte & 0x80) drawPixel(x+i, y+j, color);
    }
  }
}

// drawBitmap() variant w/background for RAM-resident (not PROGMEM) bitmaps.
void cLEDMatrixBase::drawBitmap(int16_t x, int16_t y,
 uint8_t *bitmap, int16_t w, int16_t h, CRGB color, CRGB bg) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte = 0;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte <<= 1;
      else      byte   = bitmap[j * byteWidth + i / 8];
      if(byte & 0x80) drawPixel(x+i, y+j, color);
      else            drawPixel(x+i, y+j, bg);
    }
  }
}

//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void cLEDMatrixBase::drawXBitmap(int16_t x, int16_t y,
 const uint8_t *bitmap, int16_t w, int16_t h, CRGB color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte = 0;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte >>= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x01) drawPixel(x+i, y+j, color);
    }
  }
}

/*
* ---------------------------24 bit full color bitmaps ------------------------------
*         ============ background of Black = transparent ===============
* 24 bit color bitnmap image - Draw a bitmap image at the specified(x, y) position from the
* bitmap must be PROGMEM memory) using the specified foreground color.
* A bg color of 0x000000 (black) = transparent, otherwise color of bg
* each row must be from left to right
*/
void cLEDMatrixBase::drawBitmap24(int16_t x, int16_t y, const long* bitmap, int16_t w, int16_t h, CRGB bg) {
    uint8_t i, j;
    uint8_t  k = 0;
    CRGB col;

        for (j = 0; j < h; j++) {
            for (i = 0; i < w; i++) {
                CRGB color = pgm_read_dword(&(bitmap[k]));
                if (color) {
                    drawPixel(x + i, y + j, color);
                }
                else if (bg) { 
                    drawPixel(x + i, y + j, bg);
                }
                k++;
            }
        }
}

/*       ============ zigzag or not ============== =
*        ============ background of Black = transparent ===============
* 24 bit color bitnmap image - Draw a bitmap image at the specified(x, y) position from the
* bitmap must be PROGMEM memory) using the specified // foreground color
* A bg color of 0x000000 (black) = transparent, otherwise color of bg
* if all rows are left to right, zigzag = 0. If Odd rows are reversed, zigzag = 1;
*/
void cLEDMatrixBase::drawBitmap24(int16_t x, int16_t y, const long* bitmap, int16_t w, int16_t h, boolean progMem, boolean zigzag, CRGB bg) {
    uint8_t i, j;
    uint8_t  k = 0;
    CRGB color;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (progMem) {
                color = pgm_read_dword(&(bitmap[k]));
            }
            else {
                color = bitmap[k];
            }
            if (color) {
                _bitmapZigzag(x, y, i, j, w, zigzag, color);
            }
            else if (bg) {
                _bitmapZigzag(x, y, i, j, w, zigzag, bg);
            }
            k++;
        }
    }
}

//zigzag service function
void cLEDMatrixBase::_bitmapZigzag(int16_t x, int16_t y, uint8_t i,  uint8_t j, int16_t w, boolean zigzag, CRGB col) {
     if (zigzag) {           //every odd line reversed
        if (j % 2) {            //odd row
            drawPixel((w - 1) - i + x, y + j, col);
        }
        else {
            drawPixel(x + i, y + j, col);
        }
    }
    else {
        drawPixel(x + i, y + j, col);
    }
}

//-------------------------------------------- sprites -----------------------------------

void cLEDMatrixBase::spriteInit(uint8_t spriteNum, int16_t w, int16_t h, const long* bitmap, boolean progMem, boolean zigzag) {

    sprite[spriteNum].bitmap = bitmap;
    sprite[spriteNum].w = w;
    sprite[spriteNum].h = h;
    sprite[spriteNum].progMem = progMem;
    sprite[spriteNum].zigzag = zigzag;
}

void cLEDMatrixBase::drawSprite(uint8_t spriteNum, int16_t x, int16_t y, CRGB bg) {
    drawBitmap24(x, y, sprite[spriteNum].bitmap, sprite[spriteNum].w, sprite[spriteNum].h, sprite[spriteNum].progMem, sprite[spriteNum].zigzag, bg);
}

void cLEDMatrixBase::eraseSprite(uint8_t spriteNum, int16_t x1, int16_t y1, CRGB bg) {
    for (int y = y1; y < sprite[spriteNum].h; y++) {
        for (int x = x1; x < sprite[spriteNum].w; x++) {
            drawPixel(x, y, bg);
        }
    }

}

//-------------------------------screen blocks -----------------------------------
//constructing the block buffer sprite uses ram memory so be careful of size and number
// for now, do nothing if this allocation fails.  methods that require it should silently fail if _buffer is null.
boolean cLEDMatrixBase::blockInit(uint8_t blockNum, uint8_t w, uint8_t h) {

    block[blockNum].w = w;
    block[blockNum].h = h;
    block[blockNum].b_ptr = (struct CRGB*)calloc(w * h, sizeof(CRGB));     //calloc = clear the buffer
    if (! block[blockNum].b_ptr) {
        return false; }
    else {
        return true;
    }
}

boolean cLEDMatrixBase::blockStore(uint8_t blockNum, int16_t x1, int16_t y1) {
    CRGB color;

    if (!block[blockNum].b_ptr) return false;

    if (y1 < 0) { y1 = 0; }     //if out of bounds move into matrix
    if (x1 < 0) { x1 = 0; }
    block[blockNum].xpos = x1;      //save the sorner
    block[blockNum].ypos = y1;

    int16_t index = 0;
    for (int16_t y = y1; y < y1 + block[blockNum].h; y++) {
        for (int16_t x = x1; x < x1 + block[blockNum].w; x++) {
            color = getPixel(x, y);                //READ SCREEN HERE
            block[blockNum].b_ptr[index] = color;      //adress the memory as an array
            index++;
        }
    }
    return true;
}

//restore and move
void cLEDMatrixBase::blockRestore(uint8_t blockNum, int16_t x1, int16_t y1) {
    CRGB color;
    int16_t index = 0;
    for (int16_t y = y1; y < y1 + block[blockNum].h; y++) {
        for (int16_t x = x1; x < x1 + block[blockNum].w; x++) {
            color = block[blockNum].b_ptr[index];     //adress the memory array starts a zero
            index++;
            drawPixel(x, y, color);
        }
    }
}

//restore to same location
void cLEDMatrixBase::blockRestore(uint8_t blockNum) {
    CRGB color;
    uint8_t x1 = block[blockNum].xpos;
    uint8_t y1 = block[blockNum].ypos;

    int16_t index = 0;
    for (int16_t y = y1; y < y1 + block[blockNum].h; y++) {
        for (int16_t x = x1; x < x1 + block[blockNum].w; x++) {
            color = block[blockNum].b_ptr[index];     //adress the memory array starts a zero
            index++;
            drawPixel(x, y, color);
        }
    }
}

//free up the memory of this block
void cLEDMatrixBase::freeBlock(uint8_t blockNum) {
    free(block[blockNum].b_ptr);
}

//============================= show with or without blocks/Extender =================================

/*
No enable pins - display entire matrix panel WITH DEFAULT _brightness
*/

void cLEDMatrixBase::show() {
    show(e_brightness);  //Extender show leds 
}

/*
no enable pins given - display entire matrix panel
duplicate code here for speed-don't call sinmgle bank routine 4 times!
WITH local _brightness
*/
void cLEDMatrixBase::show(uint8_t gBrightness) {

    for (uint8_t Bank = 0; Bank < e_numBanks; Bank++) {
        #if HAS_TILES
            digitalWrite(e_enableBank[Bank], HIGH);
        #endif
        for (uint8_t i = 0; i < e_stripsPerBank; i++) {
            uint8_t index = i + (Bank * e_stripsPerBank);
            memmove8(&e_LED[0], &cLED[e_stripStart[index]], e_ledsPerStrip * sizeof(CRGB)); //FastLED version
            controllers[i]->showLeds(gBrightness);
        }
        #if HAS_TILES
            digitalWrite(e_enableBank[Bank], LOW);
        #endif
    }
}


//show for Banks 

/*
One Bank NUMBER (0-3) not PIN#
enbale Bank pin. Must step through all strips in this bank
*/
void cLEDMatrixBase::show(uint8_t Bank, uint8_t gBrightness) {

    digitalWrite(e_enableBank[Bank], HIGH);
    for (uint8_t i = 0; i < e_stripsPerBank; i++) {
        uint8_t index = i + (Bank * e_stripsPerBank);
        memmove8(&e_LED[0], &cLED[e_stripStart[index]], e_ledsPerStrip * sizeof(CRGB));  //FastLED vsersiom
        controllers[i]->showLeds(gBrightness); 
    }
    digitalWrite(e_enableBank[Bank], LOW);
}

/*
2 banks, 2 enable pins w/ teensy transactions optional
ifdef inserts teensy transaction code if defined
*/
void cLEDMatrixBase::show(uint8_t Bank1, uint8_t Bank2, uint8_t gBrightness) {
    show(Bank1, gBrightness);
    show(Bank2, gBrightness);
}

/*
3 banks, 3 enbale pins IN ANY ORDER  w/ teensy transactions optional
ifdef inserts teensy transaction code if defined
*/
void cLEDMatrixBase::show(uint8_t Bank1, uint8_t Bank2, uint8_t Bank3, uint8_t gBrightness) {
    show(Bank1, gBrightness);
    show(Bank2, gBrightness);
    show(Bank3, gBrightness);
}

/* ==================== FGastLED Conyrollers setup ================================
* memory array: Instead of one large led array, to use strips and Banks we must use these Controllers - one per led strip.
* BUT for my Banks, the "output" array is only part of the full led array. For a 1024 led array with 4 Banks, each is
* only 256 leds inm 4 strips. Further, for 4 strips in each bank we now have 64 leds in each strip and 4 controllers as below.
* Finally, each BANK is using the same 4 pins and the SAME 256 LED array. memmov8 (FastLED's faster version of memcpy) copies the 256 led portion of
* the fill array into this 256 led output array, once for each Bank.
*
* ============== explaining all the layered conditiional compiles ======================
* NOTE: Using #ifs because of the required #defines. Using #ifs cause compile errors if, for
* example, DATA_1 is not defined in config by the user. This will display a compile time #error message!
* So must test for 1-wire, or 2-wire leds. Number of Banks defined, is the MCU a Teensy, and if ATA_RATE_MHZ(SPI_MHZ) are defined.
*/
void cLEDMatrixBase::defineBanks() {

    /*
    Controller index MUST BE IN THIS ORDER from zero up. This is backwards from the extender Banks A B C D order,
    so code must step thru strips in reverse! Controller index MUST BE IN THIS ORDER fron zero up
    */
   
    //initialize up to 4 controllers. Even with no Extender, Controller0 is used.
    for (uint8_t i = 0; i < e_numBanks; i++) {
   
        #if NUM_STRIPS == 1 
            setControllerA(0);  //controller 0
        #elif (NUM_STRIPS == 2)
            setControllerA(1);  //controller 1
            setControllerB(0);  //controller 0
        #elif (NUM_STRIPS == 3)
            setControllerA(2);  //controller 2
            setControllerB(1);  //controller 1
            setControllerC(0);  //controller 0
        #elif (NUM_STRIPS == 4)
            setControllerA(3);  //controller 3
            setControllerB(2);  //controller 2
            setControllerC(1);  //controller 1
            setControllerD(0);  //controller 0
        #endif  //controller selection
    }
}

/*
* LEDMatrix addleds() version. Creates 1 to 4 controller(s) (1 for simple matrix), or blocks/Entender hardware.
* Need to do in reverse order to get the hardware stacking correct. With top extender always #1.
* Enable pin 3,4,5,6 as needed.
* Otherwise pin 3 moves down as more banks are added.
*/
void cLEDMatrixBase::addLeds() {

    #if (NUM_BANKS == 1)
        BankPin0(0);
    #elif (NUM_BANKS == 2)
        BankPin1(0);
        BankPin0(1);
    #elif (NUM_BANKS == 3)
        BankPin2(0);
        BankPin1(1);
        BankPin0(2);
    #elif (NUM_BANKS == 4)
        BankPin3(0);
        BankPin2(1);
        BankPin1(2);
        BankPin0(3);
    #endif

    //Create an output CRGB array for all banks or full matrix if no Extenders
    //just one bank size up to LED strips used by 1 to 4 strips, used for 0 4 Banks
    e_LED = (struct CRGB*)malloc(e_ledsPerStrip * e_stripsPerBank * sizeof(CRGB));

    //offset into leds[] for each strip - used for memcopy LEDS[] to e_LED[]
    //NOTE: this index is reversed - from top down - thats  the way FastLED sets up the memory
    uint8_t index = e_numStrips - 1;
    for (int8_t i = 0; i < e_numStrips; i++) {
        e_stripStart[index] = i * e_ledsPerStrip;   //Use array so we can loop through
        index--;
    }
    FastLED.setBrightness(e_brightness);     // global brightness not set when using Controllers
    setRotation(0); //0-3 in 90 deg steps

    if (e_numBanks == 0) { return; }    //no Banks to define

    for (uint8_t i = 0; i < e_numBanks; i++) {
        pinMode(e_enableBank[i], OUTPUT);
        digitalWrite(e_enableBank[i], LOW);
    }
    defineBanks();         //enable Bank Controllers
}

void cLEDMatrixBase::BankPin0(uint8_t pin) {
    #ifdef BANK_PIN_0               //handle undefined BANKS to avoid compile errors
        e_enableBank[pin] = BANK_PIN_0;       //use array so we can loop through
    #else
        e_enableBank[pin] = 99;             //unavaliable pin
    #endif
}

void cLEDMatrixBase::BankPin1(uint8_t pin) {
    #ifdef BANK_PIN_1
        e_enableBank[pin] = BANK_PIN_1;
    #else
        e_enableBank[pin] = 99;             //unavaliable pin
    #endif
}

void cLEDMatrixBase::BankPin2(uint8_t pin) {
    #ifdef BANK_PIN_2
        e_enableBank[pin] = BANK_PIN_2;
    #else
        e_enableBank[pin] = 99;             //unavaliable pin
    #endif
}

void cLEDMatrixBase::BankPin3(uint8_t pin) {
    #ifdef BANK_PIN_3
        e_enableBank[pin] = BANK_PIN_3;
    #else
        e_enableBank[pin] = 99;             //unavaliable pin
    #endif
}

/*
* Create 1 to 4 FastLED controllers as needed for Extender hardware. Create just 1 if no Extender.
* FastLED Controller functions require #defined constants in the calls so each controller version must use seperate code. 
* For example for 2-wire, DATA1 and DATA2 cannot be assigned to a simple array. For clarity its easier just to use #if..#endif.
* 2-wire format DATA+CLOCK FastLED format:
* FastLED.addLeds<CHIPSET, DATA, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(CORRECTION is the Sketch equivalent format for 2-wire leds
* 1-wire format DATA only FastLED format:
* Replaces FastLED sketch equivalent format for 4 DATA lines:
* FastLED.addLeds<CHIPSET, DATA, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(CORRECTION)
*/
void cLEDMatrixBase::setControllerA(uint8_t index) {
    #if CLOCK_PIN_REQUIRED		//must be atleast 1 clock pin defined for 2-wire
        #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #if defined DATA_1 && defined CLOCK_1
                #if defined SPI_MHZ                //because of FastLED definitions, must have 2 forms hard coded
                     controllers[index] = &FastLED.addLeds<CHIPSET, DATA_1, CLOCK_1, COLOR_ORDER, DATA_RATE_MHZ(SPI_MHZ)>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #else
                    controllers[index] = &FastLED.addLeds<CHIPSET, DATA_1, CLOCK_1, COLOR_ORDER>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #endif
            #else
                #error "DATA_1 or CLOCK_1 pin not defined-cannot enable Bank 1" 
            #endif
        #endif
    #else        //1-wire format DATA only
        #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #ifdef DATA_1
                controllers[index] = &FastLED.addLeds<CHIPSET, DATA_1, COLOR_ORDER >(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
            #else
                #error "DATA_1 pin not defined-cannot enable Bank 1"
            #endif
        #endif
    #endif
}
void cLEDMatrixBase::setControllerB(uint8_t index) {

    #if CLOCK_PIN_REQUIRED		//must be atleast 1 clock pin defined for 2-wire
        #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #if defined DATA_2 && defined CLOCK_1
                #if defined SPI_MHZ                 //because of FastLED definitions, must have 2 forms hard coded
                    controllers[index] = &FastLED.addLeds<CHIPSET, DATA_2, CLOCK_1, COLOR_ORDER, DATA_RATE_MHZ(SPI_MHZ)>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #else
                    controllers[index] = &FastLED.addLeds<CHIPSET, DATA_2, CLOCK_1, COLOR_ORDER>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #endif
            #else
                #error "DATA_2 or CLOCK_1 pin not defined-cannot enable Bank 2"  
            #endif
        #endif
    #else        //1-wire format DATA only
            #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #ifdef DATA_2
                controllers[index] = &FastLED.addLeds<CHIPSET, DATA_2, COLOR_ORDER >(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
            #else
                #error "DATA_2 pin not defined-cannot enable Bank 2"
            #endif
        #endif
    #endif
}
void cLEDMatrixBase::setControllerC(uint8_t index) {

    #if CLOCK_PIN_REQUIRED		//must be atleast 1 clock pin defined for 2-wire
        #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #if defined DATA_1 && defined CLOCK_2
                #if defined SPI_MHZ                //because of FastLED definitions, must have 2 forms hard coded
                    controllers[index] = &FastLED.addLeds<CHIPSET, DATA_1, CLOCK_2, COLOR_ORDER, DATA_RATE_MHZ(SPI_MHZ)>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #else
                    controllers[index] = &FastLED.addLeds<CHIPSET, DATA_1, CLOCK_2, COLOR_ORDER>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #endif
            #else
                #error "DATA_1 or CLOCK_2 pin not defined-cannot enable Bank 3"  
            #endif
        #endif
    #else        //1-wire format DATA only
        #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #ifdef DATA_3
                controllers[index] = &FastLED.addLeds<CHIPSET, DATA_3, COLOR_ORDER >(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
            #else
                #error "DATA_3 pin not defined-cannot enable Bank 3"
            #endif
        #endif
    #endif
}
void cLEDMatrixBase::setControllerD(uint8_t index) {

    #if CLOCK_PIN_REQUIRED		//must be atleast 1 clock pin defined for 2-wire
        #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #if defined DATA_2 && defined CLOCK_2
                #if defined SPI_MHZ                //because of FastLED definitions, must have 2 forms hard coded
                    controllers[index] = &FastLED.addLeds<CHIPSET, DATA_2, CLOCK_2, COLOR_ORDER, DATA_RATE_MHZ(SPI_MHZ)>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #else
                    controllers[index] = &FastLED.addLeds<CHIPSET, DATA_2, CLOCK_2, COLOR_ORDER>(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
                #endif
            #else
                #error "DATA_1 or CLOCK_1 pin not defined-cannot enable Bank 4"  
            #endif
        #endif
    #else        //1-wire format DATA only
        #if (NUM_STRIPS > index)    //must avoid undefined pin(s)
            #ifdef DATA_4
                controllers[index] = &FastLED.addLeds<CHIPSET, DATA_4, COLOR_ORDER >(e_LED, LEDS_PER_STRIP).setCorrection(CORRECTION);
            #else
                #error "DATA_4 pin not defined-cannot enable Bank 4"
            #endif
        #endif
    #endif
}

