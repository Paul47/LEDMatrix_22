/* --------------LEDMatrix_22 -------------------------
  LEDMatrix_22 updated previous by DrOldies 2021
  modified:  LEDMatrix V5 class by Aaron Liddiment (c) 2016
  modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
  date:      2016/04/27
*/

#pragma once

enum MatrixType_t { HORIZONTAL_MATRIX,
                    VERTICAL_MATRIX,
                    HORIZONTAL_ZIGZAG_MATRIX,
                    VERTICAL_ZIGZAG_MATRIX };

enum BlockType_t	{	HORIZONTAL_BLOCKS,
						VERTICAL_BLOCKS,
						HORIZONTAL_ZIGZAG_BLOCKS,
						VERTICAL_ZIGZAG_BLOCKS };

#define FASTLED_INTERNAL        // Suppress build banner
#include <FastLED.h>
#include "configuration_22.h"

/*If ENABLE_FONTS is defined in Configuration_22.h, use this default font.
Both files must be in the libary older. If not defined, these 2 files are not needed.
*/

#if ENABLE_FONTS
    #include "gfxfont.h"
    #include "glcdfont.c"
#endif

/* If XYTable_LookUp is defined in Configuration_22.h, use an external table named XYTable_LookUp.h,
in the library folder to map the LEDs in XYTable[y][x]
instead of calculating with mXY(x,y). The table is stored in PROGMEM. For irregular led
arrays, look at the LEDMatrix manual for details.
*/
//#define XYTable_LookUp
#ifdef XYTable_LookUp
  #include "XYTable_LookUp.h"
#endif


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
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))        //LEDMatrix_22 changed right side word to dword
#endif


#define adagfxswap(a, b) { a = a ^ b; b = a ^ b; a = a ^ b; }

#if !defined(ESP8266)
#define swap(a, b) adagfxswap(a, b)
#endif

/* TRIGGER_LED for a leading LED at the Teensy's end of the DATA/CLOCK leads.
Some people suggest that inserting this trigger LED near the MCU SPI pins
allows for longer wires freom the controller to the 1st LED of the strip or array.
It reduces pixelation and flashing with longer LED strips.
This makes wiring your much project much easier. 
*/

//for future use //ppd
#define TRIGGER_LED 0
#define M_XY(x,y)    (mXY(x, y) + TRIGGER_LED)

class cLEDMatrixBase
{
  friend class cSprite;

public:
 struct CRGB *m_LED;         //LEDMatrix_22 moved to public from protected
 struct CRGB* e_LED;                      //move to public //ppd

 typedef struct SPRITES {
    const long* bitmap;
    int16_t w;
    int16_t h;
    boolean progMem;
    boolean zigzag;
} SPRITES;
SPRITES sprite[48];   //48 max


 typedef struct BLOCKS {
    int16_t xpos;
    int16_t ypos;
    CRGB* b_ptr;         ///ptr to memory block
    uint8_t w;
    uint8_t h;
    boolean progMem;        //future use?
} BLOCKS;
BLOCKS block[48];   //48 max


  protected:
    int16_t m_Width, m_Height, m_WH;
    //struct CRGB *m_LED;         //LEDMatrix moved to publicc from protected
    struct CRGB m_OutOfBounds;

    int16_t
      WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
    int16_t
      _width, _height, // Display w/h as modified by current rotation
      cursor_x, cursor_y;
    CRGB
      textcolor, textbgcolor;
    uint8_t textsize = 1;
     uint8_t rotation = 0;
    boolean
      wrap,   // If set, 'wrap' text at right edge of display
      _cp437; // If set, use correct CP437 charset (default is off)



    /*===== List of all definitions describing the matrix panel ======
    USING VARIABLES INSTEAD OF DEFINITIONS DURING CODING REMOVES undefined ERRORS
    and confusing #if....#endif brackets. "c."" is the user defined class such as "leds."

        variables        #defines            Settings
    =================================================================================
    **Matrix Panel**
        c.matrixWidth       MATRIX_WIDTH	        //former LEDMatrix use negative value for rev
        c.matriHeight       MATRIX_HEIGHT	        //former LEDMatrix use negative value for rev
        c.matrixType        MATRIX_TYPE	        //HORIZONTAL_MATRIX, VERTICAL_MATRIX,
                                            //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_M
                    **what direction does the FIRST row of LEDs flow?
    c.ledHrorizDir      HORIZ_DIR	            //LEFT_2_RIGHT, RIGHT_2_LEFT
    c.ledVertDir        VERT_DIR	            //BOTTOc.UP, TOP_DOWN
    **Tiles/Blocks**
    c.tileWidth        MATRIX_TILE_WIDTH	    // width of EACH MATRIX "cell" (not total dis
    c.tileHeight       MATRIX_TILE_HEIGHT	    // height of each matrix "cell"
    c.tilesPerRow      MATRIX_TILE_H	        // number of matrices arranged horizontally (
    c.tilesPerCol      MATRIX_TILE_V 	        // number of matrices arranged vertically (po
    c.tileLedsFlow     LEDS_IN_TILE	        //HORIZONTAL_MATRIX, VERTICAL_MATRIX,
                                            //HORIZONTAL_ZIGZAG_MATRIX, VERTICAL_ZIGZAG_MATRIX
    c.tileFlow         TILES_IN_MATRIX	        //HORIZONTAL_BLOCKS, VERTICAL_BLOCKS,
                                            //HORIZONTAL_ZIGZAG_BLOCKS, VERTICAL_ZIGZAG_BLOCKS
    c.tileLedHorizDir  LEDS_HORIZ_DIR	        //LEFT_2_RIGHT, RIGHT_2_LEFT
    c.tileLedVertDir   LEDS_VERT_DIR     	    //BOTTOM_UP, TOP_DOWN
    **Extender**
    c.numBanks         NUM_BANKS 	            // 1 to 4 extender "banks"
    c.stripsPerBank    STRIPS_PER_BANK	        //1 or more but 4 strips per Bank is the most
    c.ledsPerBank      LEDS_PER_BANK	        //equally split the total number of leds across
    c.ledsPerStrip     LEDS_PER_STRIP 
    */

public:
    //global declarations for definition replacements with constant variables
    const uint8_t matrixWidth = MATRIX_WIDTH;
    const uint8_t matrixHeight = MATRIX_HEIGHT;
    const uint8_t ledHrorizDir = HORIZ_DIR;	         //0 = LEFT_2_RIGHT, 1 = RIGHT_2_LEFT
    const uint8_t ledVertDir = VERT_DIR;	            //0 = BOTTOM_UP, 1 = TOP_DOWN

    //capture block & extender info as integers rather than #defines so we don't get undefined errors.
    //default is 0 if define = 0 or not defined
#if HAS_BLOCKS
    const uint8_t hasBlocks = HAS_BLOCKS;                  //otherwise leave these a defalut of zero
    const uint8_t numTiles = MATRIX_TILE_H * MATRIX_TILE_V;
    const uint8_t tilesPerRow = MATRIX_TILE_H;
    const uint8_t tilesPerCol = MATRIX_TILE_V;
    const uint8_t tileWidth = MATRIX_TILE_WIDTH;      //leds in a tile's row
    const uint8_t tileHeight = MATRIX_TILE_HEIGHT;    //leds in a tile's column
    const uint8_t tileLedsFlow = LEDS_IN_TILE;        //0 = HORIZONTAL_MATRIX, 1 = VERTICAL_MATRIX, 2 = HORIZONTAL_ZIGZAG_MATRIX, 3 = VERTICAL_ZIGZAG_MATRIX
    const uint8_t tileFlow = TILES_IN_MATRIX;        //0 = HORIZONTAL_BLOCKS, 1 = VERTICAL_BLOCKS, 2 = HORIZONTAL_ZIGZAG_BLOCKS, 3 = VERTICAL_ZIGZAG_BLOCKS
    const uint8_t tileLedHorizDir = LEDS_HORIZ_DIR;	//0 = LEFT_2_RIGHT, 1 = RIGHT_2_LEFT
    const uint8_t tileLedVertDir = LEDS_VERT_DIR;    //0 = BOTTOM_UP, 1 = TOP_DOWN
#else
    const uint8_t hasBlocks = 0;
    const uint8_t numTiles = 0;
    const uint8_t tilesPerRow = 0;
    const uint8_t tilesPerCol = 0;
    const uint8_t tileWidth = 0;
    const uint8_t tileHeight = 0;
    const uint8_t tileLedsFlow = 0;
    const uint8_t tileFlow = 0;
    const uint8_t tileLedVertDir = 0;
#endif  //if true


    boolean hasExtender = HAS_EXTENDER;     //otherwise leave these a defalut of zero
    const uint8_t numBanks = NUM_BANKS;
    const uint8_t stripsPerBank = STRIPS_PER_BANK;
    const int16_t ledsPerBank = LEDS_PER_BANK;
    const int16_t ledsPerStrip = LEDS_PER_STRIP;


  public:
    cLEDMatrixBase();
    virtual uint32_t mXY(uint16_t x, uint16_t y)=0;
    void SetLEDArray(struct CRGB *pLED);	// Only used with externally defined LED arrays

    struct CRGB *operator[](int n);
    struct CRGB &operator()(int16_t x, int16_t y);
    struct CRGB &operator()(int16_t i);

    int Size()  { return(m_WH); }
    int Width() { return(m_Width);  }
    int Height()  { return(m_Height); }

    void HorizontalMirror(bool FullHeight = true);
    void VerticalMirror();
    void QuadrantMirror();
    void QuadrantRotateMirror();
    void TriangleTopMirror(bool FullHeight = true);
    void TriangleBottomMirror(bool FullHeight = true);
    void QuadrantTopTriangleMirror();
    void QuadrantBottomTriangleMirror();

    void drawPixel(int16_t x, int16_t y, CRGB Col);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void drawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void drawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
    void drawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void drawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
    
    //================LEDMatrix_22 additions ============================
    CRGB getPixel(int16_t x, int16_t y);
    void fadeAll(uint16_t value);
    void fillScreen(CRGB color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, CRGB color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, CRGB color);
    void drawFilledTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, CRGB color);
    void drawFillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, CRGB color);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, CRGB color);
    void drawFillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, CRGB color);
    void clear();
    void showColor(CRGB color);
    void setBrightness(uint8_t bght);

   // ----------------text---------------------------
        //If ENABLE_FONTS is true in the configuration header,"gfxfont.h" and "glcdfont.c" will be included - 
    // they must be in the library folder and all character functions will be enabled
    #if ENABLE_FONTS 
        GFXfont
            * gfxFont;
        void setFont(const GFXfont* f = NULL);
    #endif

    void setCursor(int16_t x, int16_t y);
    void setTextColor(CRGB c);
    void setTextColor(CRGB c, CRGB bg);
    void setTextSize(uint8_t s);
    void setTextWrap(boolean w);
    void setRotation(uint8_t r);
    void cp437(boolean x=true);
    int16_t getCursorX(void) const;      // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
    int16_t getCursorY(void) const;
    uint8_t getRotation(void) const;
    void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void drawChar(int16_t x, int16_t y, unsigned char c, CRGB color, CRGB bg, uint8_t size);
    void write(char);
    void print(char text[]);
    void invertDisplay();
    void invertSquare(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

  //-----------------------------bitmaps ----------------------------------
    void drawBitmap(int16_t x, int16_t y, const uint8_t * bitmap, int16_t w, int16_t h, CRGB color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t * bitmap, int16_t w, int16_t h, CRGB color, CRGB bg);
    void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, CRGB color);
    void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, CRGB color, CRGB bg);
    void drawXBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, CRGB color);

    //================LEDMatrix_22  additions - 24 bit full color bitmaps ============================
    void _bitmapZigzag(int16_t x, int16_t y, uint8_t i,  uint8_t j, int16_t w, boolean zigzag, CRGB col); 
    void drawBitmap24(int16_t x, int16_t y, const long* bitmap, int16_t w, int16_t h, boolean progMem, boolean zigzag, CRGB bg); 
    void drawBitmap24(int16_t x, int16_t y, const long* bitmap, int16_t w, int16_t h, CRGB bg);

     //================LEDMatrix_22  additions - 24 bit full color sprites ============================   
    void spriteInit(uint8_t spriteNum, int16_t w, int16_t h, const long* bitmap, boolean progMem, boolean zigzag);
    void drawSprite(uint8_t spriteNum, int16_t x, int16_t y, CRGB bg);
    void eraseSprite(uint8_t spriteNum, int16_t x1, int16_t y1, CRGB bg);

    //================LEDMatrix_22  aditions - 24 bit full color screen block save-restore ============================   
    boolean blockInit(uint8_t blockNum, uint8_t w, uint8_t h);
    boolean blockStore(uint8_t blockNum, int16_t x1, int16_t y1);
    void blockRestore(uint8_t blockNum, int16_t x1, int16_t y1);
    void blockRestore(uint8_t blockNum);
    void freeBlock(uint8_t blockNum);

    //=====================LEDMatrix_22  EXTENDER hardware ===============================
    void LEDShow();
    void LEDShow(uint8_t gBrightness);
    void LEDShow(uint8_t Bank, uint8_t gBrightness);
    void LEDShow(uint8_t Bank1, uint8_t Bank2, uint8_t gBrightness);
    void LEDShow(uint8_t Bank1, uint8_t Bank2, uint8_t Bank3, uint8_t gBrightness);
    void addLeds();
    void defineBanks();

private:
    uint16_t e_numLeds;
    uint8_t e_numStrips;
    uint8_t e_numBanks;
    uint8_t  e_stripsPerBank;
    uint16_t e_ledsPerStrip;
    uint8_t  e_brightness;

    //arrays
    //CRGB* e_LED;                      //moved to public
    CLEDController* controllers[4];
    uint8_t e_enableBank[4];
    uint16_t e_stripStart[16];
};

template<int16_t tMWidth, int16_t tMHeight, MatrixType_t tMType, int8_t tBWidth = 1, int8_t tBHeight = 1, BlockType_t tBType = HORIZONTAL_BLOCKS> class cLEDMatrix : public cLEDMatrixBase
{
  private:
    static const int16_t m_absMWidth = (tMWidth * ((tMWidth < 0) * -1 + (tMWidth > 0)));
    static const int16_t m_absMHeight = (tMHeight * ((tMHeight < 0) * -1 + (tMHeight > 0)));
    static const int16_t m_absBWidth = (tBWidth * ((tBWidth < 0) * -1 + (tBWidth > 0)));
    static const int16_t m_absBHeight = (tBHeight * ((tBHeight < 0) * -1 + (tBHeight > 0)));
    struct CRGB *p_LED;

  public:
    cLEDMatrix(bool doMalloc=true)
    {
      m_Width = m_absMWidth * m_absBWidth;
      m_Height = m_absMHeight * m_absBHeight;
      m_WH =  m_Width * m_Height;
      if (doMalloc) {
	  // On ESP32, there is more memory available via malloc than static global arrays
          p_LED = (struct CRGB *) malloc(((m_absMWidth * m_absBWidth * m_absMHeight * m_absBHeight) + TRIGGER_LED) * sizeof(CRGB));   //ppd  added trigger led
          m_LED = p_LED;
	  if (! p_LED) {
	     Serial.begin(115200);
	     Serial.println("Malloc LEDMatrix Failed");
	     while (1);
	  }
      } else {
	  Serial.println("LED array not intialized, must be set by SetLEDArray");
      }
    }
    void SetLEDArray(struct CRGB *pLED)
    {
      p_LED = pLED;
      m_LED = pLED;
    }
    virtual uint32_t mXY(uint16_t x, uint16_t y)
    {
      #ifdef XYTable_LookUp         //keeping it simple, leave the rest of the code
        return XYTable[y][x];      //x,y indexes are always result in y,x arrays //ppd
      #endif

		if ((tBWidth == 1) && (tBHeight == 1))
		{
			// No Blocks, just a Matrix
	    if (tMWidth < 0)
	    x = (m_absMWidth - 1) - x;
		if (tMHeight < 0)
	    y = (m_absMHeight - 1) - y;
	    if (tMType == HORIZONTAL_MATRIX)
	    return((y * m_absMWidth) + x);
	    else if (tMType == VERTICAL_MATRIX)
	    return((x * m_absMHeight) + y);
	    else if (tMType == HORIZONTAL_ZIGZAG_MATRIX)
	    {
	    if (y % 2)
	        return((((y + 1) * m_absMWidth) - 1) - x);
	    else
	        return((y * m_absMWidth) + x);
	    }
	    else /* if (tMType == VERTICAL_ZIGZAG_MATRIX) */
	    {
	    if (x % 2)
	        return((((x + 1) * m_absMHeight) - 1) - y);
	    else
	        return((x * m_absMHeight) + y);
	    }
		}
		else
		{
			// Reverse Block/Matrix X coordinate if needed
			if ((tBWidth < 0) && (tMWidth < 0))
				x = (((m_absBWidth - 1) - (x / m_absMWidth)) * m_absMWidth) + ((m_absMWidth - 1) - (x % m_absMWidth));
			else if (tBWidth < 0)
				x = (((m_absBWidth - 1) - (x / m_absMWidth)) * m_absMWidth) + (x % m_absMWidth);
			else if (tMWidth < 0)
				x = x - ((x % m_absMWidth) * 2) + (m_absMWidth - 1);
			// Reverse Block/Matrix Y coordinate if needed
			if ((tBHeight < 0) && (tMHeight < 0))
				y = (((m_absBHeight - 1) - (y / m_absMHeight)) * m_absMHeight) + ((m_absMHeight - 1) - (y % m_absMHeight));
			else if(tBHeight < 0)
				y = (((m_absBHeight - 1) - (y / m_absMHeight)) * m_absMHeight) + (y % m_absMHeight);
			else if (tMHeight < 0)
				y = y - ((y % m_absMHeight) * 2) + (m_absMHeight - 1);
			// Calculate Block base
	    uint16_t Base;
		if (tBType == HORIZONTAL_BLOCKS)
		    Base = (((y / m_absMHeight) * m_absBWidth) + (x / m_absMWidth)) * (m_absMWidth * m_absMHeight);
		else if (tBType == VERTICAL_BLOCKS)
		    Base = (((x / m_absMWidth) * m_absBHeight) + (y / m_absMHeight)) * (m_absMHeight * m_absMWidth);
		else if (tBType == HORIZONTAL_ZIGZAG_BLOCKS)
		{
	    if ((y / m_absMHeight) % 2)
			    Base = (((y / m_absMHeight) * m_absBWidth) + ((m_absBWidth - 1) - (x / m_absMWidth))) * (m_absMWidth * m_absMHeight);
	    else
			    Base = (((y / m_absMHeight) * m_absBWidth) + (x / m_absMWidth)) * (m_absMWidth * m_absMHeight);
	    }
	    else /* if (tBType == VERTICAL_ZIGZAG_BLOCKS) */
	    {
	    if ((x / m_absMWidth) % 2)
			    Base = (((x / m_absMWidth) * m_absBHeight) + ((m_absBHeight - 1) - (y / m_absMHeight))) * (m_absMHeight * m_absMWidth);
	    else
			    Base = (((x / m_absMWidth) * m_absBHeight) + (y / m_absMHeight)) * (m_absMHeight * m_absMWidth);
	    }
			// Calculate Matrix offset
	    if (tMType == HORIZONTAL_MATRIX)
	    return(Base + ((y % m_absMHeight) * m_absMWidth) + (x % m_absMWidth));
	    else if (tMType == VERTICAL_MATRIX)
	    return(Base + ((x % m_absMWidth) * m_absMHeight) + (y % m_absMHeight));
	    else if (tMType == HORIZONTAL_ZIGZAG_MATRIX)
	    {
	    if ((y % m_absMHeight) % 2)
	        return(Base + ((((y % m_absMHeight) + 1) * m_absMWidth) - 1) - (x % m_absMWidth));
	    else
	        return(Base + ((y % m_absMHeight) * m_absMWidth) + (x % m_absMWidth));
	    }
	    else /* if (tMType == VERTICAL_ZIGZAG_MATRIX) */
	    {
	    if ((x % m_absMWidth) % 2)
	        return(Base + ((((x % m_absMWidth) + 1) * m_absMHeight) - 1) - (y % m_absMHeight));
	    else
	        return(Base + ((x % m_absMWidth) * m_absMHeight) + (y % m_absMHeight));
	    }
	}
 }

    void shiftLeft(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
		    for (int16_t x=1; x<m_Width; ++x)
  			{
				  for (int16_t y=0; y<m_Height; ++y)
      			m_LED[mXY(x - 1, y)] = m_LED[mXY(x, y)];
			  }
			  for (int16_t y=0; y<m_Height; ++y)
     			m_LED[mXY(m_Width - 1, y)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
            if (tMWidth > 0)
              HPWSL();
            else
              HNWSL();
            break;
          case VERTICAL_MATRIX:
            if (tMWidth > 0)
              VPWSL();
            else
              VNWSL();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              HZPWSL();
            else
              HZNWSL();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              VZPWSL();
            else
              VZNWSL();
            break;
        }
      }
    }

    void shiftRight(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
		    for (int16_t x=m_Width-1; x>=1; --x)
  			{
				  for (int16_t y=0; y<m_Height; ++y)
      			m_LED[mXY(x, y)] = m_LED[mXY(x - 1, y)];
			  }
			  for (int16_t y=0; y<m_Height; ++y)
     			m_LED[mXY(0, y)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
            if (tMWidth > 0)
              HNWSL();
            else
              HPWSL();
            break;
          case VERTICAL_MATRIX:
            if (tMWidth > 0)
              VNWSL();
            else
              VPWSL();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              HZNWSL();
            else
              HZPWSL();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              VZNWSL();
            else
              VZPWSL();
            break;
        }
      }
    }

    void shiftDown(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
			  for (int16_t y=1; y<m_Height; ++y)
  			{
			    for (int16_t x=0; x<m_Width; ++x)
      			m_LED[mXY(x, y - 1)] = m_LED[mXY(x, y)];
			  }
		    for (int16_t x=0; x<m_Width; ++x)
     			m_LED[mXY(x, m_Height - 1)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
            if (tMHeight > 0)
              HPHSD();
            else
              HNHSD();
            break;
          case VERTICAL_MATRIX:
            if (tMHeight > 0)
              VPHSD();
            else
              VNHSD();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              HZPHSD();
            else
              HZNHSD();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              VZPHSD();
            else
              VZNHSD();
            break;
        }
      }
    }

    void shiftUp(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
			  for (int16_t y=m_Height-1; y>=1; --y)
  			{
			    for (int16_t x=0; x<m_Width; ++x)
      			m_LED[mXY(x, y)] = m_LED[mXY(x, y - 1)];
			  }
		    for (int16_t x=0; x<m_Width; ++x)
     			m_LED[mXY(x, 0)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
          	if (tMHeight > 0)
              HNHSD();
            else
              HPHSD();
            break;
          case VERTICAL_MATRIX:
            if (tMHeight > 0)
              VNHSD();
            else
              VPHSD();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              HZNHSD();
            else
              HZPHSD();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              VZNHSD();
            else
              VZPHSD();
            break;
        }
      }
    }

  private:
  	// Optimised functions used by shiftLeft & shiftRight in non block mode
    void HPWSL(void)
    {
      uint32_t i = 0;
      for (int16_t y=m_absMHeight; y>0; --y,++i)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
      }
    }
    void HNWSL(void)
    {
      uint32_t i = m_absMWidth - 1;
      for (int16_t y=m_absMHeight; y>0; --y)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += ((m_absMWidth * 2) - 1);
      }
    }
    void VPWSL(void)
    {
      uint32_t i = 0;
      uint32_t j = m_absMHeight;
      for (uint16_t x=m_absMWidth-1; x>0; --x)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i++] = p_LED[j++];
      }
      for (int16_t y=m_absMHeight; y>0; --y)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void VNWSL(void)
    {
      uint32_t i = (m_absMHeight * m_absMWidth) - 1;
      uint32_t j = i - m_absMHeight;
      for (uint16_t x=m_absMWidth-1; x>0; --x)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i--] = p_LED[j--];
      }
      for (int16_t y=m_absMHeight; y>0; --y)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void HZPWSL(void)
    {
      uint32_t i = 0;
      for (int16_t y=m_absMHeight; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i++;
        if (y > 1)
        {
          i += (m_absMWidth - 1);
          for (uint16_t x=m_absMWidth-1; x>0; --x,--i)
            p_LED[i] = p_LED[i - 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMWidth;
        }
      }
    }
    void HZNWSL(void)
    {
      uint32_t i = m_absMWidth - 1;
      for (int16_t y=m_absMHeight; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        if (y > 1)
        {
          i += m_absMWidth;
          for (uint16_t x=m_absMWidth-1; x>0; --x,++i)
            p_LED[i] = p_LED[i + 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMWidth;
        }
      }
    }
    void VZPWSL(void)
    {
      uint32_t i = 0;
      uint32_t j = (m_absMHeight * 2) - 1;
      for (uint16_t x=m_absMWidth-1; x>0; x-=2)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i++] = p_LED[j--];
        if (x > 1)
        {
          j += (m_absMHeight * 2);
          for (int16_t y=m_absMHeight; y>0; --y)
            p_LED[i++] = p_LED[j--];
          j += (m_absMHeight * 2);
        }
      }
      for (int16_t y=m_absMHeight; y>0; y--)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void VZNWSL(void)
    {
      uint32_t i = (m_absMHeight * m_absMWidth) - 1;
      uint32_t j = m_absMHeight * (m_absMWidth - 2);
      for (uint16_t x=m_absMWidth-1; x>0; x-=2)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i--] = p_LED[j++];
        if (x > 1)
        {
          j -= (m_absMHeight * 2);
          for (int16_t y=m_absMHeight; y>0; --y)
            p_LED[i--] = p_LED[j++];
          j -= (m_absMHeight * 2);
        }
      }
      for (int16_t y=m_absMHeight; y>0; y--)
        p_LED[i--] = CRGB(0, 0, 0);
    }

  	// Optimised functions used by shiftDown & shiftUp in non block mode
    void HPHSD(void)
    {
      uint32_t i = 0;
      uint32_t j = m_absMWidth;
      for (uint16_t y=m_absMHeight-1; y>0; --y)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i++] = p_LED[j++];
      }
      for (uint16_t x=m_absMWidth; x>0; --x)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void HNHSD(void)
    {
      uint32_t i = (m_absMWidth * m_absMHeight) - 1;
      uint32_t j = i - m_absMWidth;
      for (uint16_t y=m_absMHeight-1; y>0; --y)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i--] = p_LED[j--];
      }
      for (uint16_t x=m_absMWidth; x>0; --x)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void VPHSD(void)
    {
      uint32_t i = 0;
      for (uint16_t x=m_absMWidth; x>0; --x,++i)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
      }
    }
    void VNHSD(void)
    {
      uint32_t i = m_absMHeight - 1;
      for (uint16_t x=m_absMWidth; x>0; --x)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += ((m_absMHeight * 2) - 1);
      }
    }
    void HZPHSD(void)
    {
      uint32_t i = 0;
      uint32_t j = (m_absMWidth * 2) - 1;
      for (uint16_t y=m_absMHeight-1; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i++] = p_LED[j--];
        if (y > 1)
        {
          j += (m_absMWidth * 2);
          for (uint16_t x=m_absMWidth; x>0; --x)
            p_LED[i++] = p_LED[j--];
          j += (m_absMWidth * 2);
        }
      }
      for (uint16_t x=m_absMWidth; x>0; x--)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void HZNHSD(void)
    {
      uint32_t i = (m_absMWidth * m_absMHeight) - 1;
      uint32_t j = m_absMWidth * (m_absMHeight - 2);
      for (uint16_t y=m_absMHeight-1; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i--] = p_LED[j++];
        if (y > 1)
        {
          j -= (m_absMWidth * 2);
          for (uint16_t x=m_absMWidth; x>0; --x)
            p_LED[i--] = p_LED[j++];
          j -= (m_absMWidth * 2);
        }
      }
      for (uint16_t x=m_absMWidth; x>0; x--)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void VZPHSD(void)
    {
      uint32_t i = 0;
      for (uint16_t x=m_absMWidth; x>0; x-=2)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i++;
        if (x > 1)
        {
          i += (m_absMHeight - 1);
          for (uint16_t y=m_absMHeight-1; y>0; --y,--i)
            p_LED[i] = p_LED[i - 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMHeight;
        }
      }
    }
    void VZNHSD(void)
    {
      uint32_t i = m_absMHeight - 1;
      for (uint16_t x=m_absMWidth; x>0; x-=2)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        if (x > 1)
        {
          i += m_absMHeight;
          for (uint16_t y=m_absMHeight-1; y>0; --y,++i)
            p_LED[i] = p_LED[i + 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMHeight;
        }
      }
    }

};

