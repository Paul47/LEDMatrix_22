
# LEDMatrix_22
------------
[![GitHub version](https://img.shields.io/github/release/Jorgen-VikingGod/LEDMatrix.svg)](https://github.com/Jorgen-VikingGod/LEDMatrix/releases/latest) 
[![FastLED dependencies](https://img.shields.io/badge/dependencies-FastLED-blue.svg)](https://github.com/FastLED/FastLED) 

> A fork of ([LEDMatrix](https://jorgen-vikinggod.github.io/LEDMatrix) by [Jürgen Skrotzky](https://github.com/Jorgen-VikingGod))

<img src="https://github.com/Paul47/LEDMatrix_22/blob/main/wiki_images/Star_burst.png" width = "400" align = "RIGHT">


<table>
  <tr>
    <td colspan="2">
      The LEDMatrix_22 library medium weight, two-dimensional graphics library for LED matricies/arrays using FastLED. This is an update and enhancement of previous LEDMatrix libraries. In addition to drawing shapes, and text, this library expands image and animation handling to 24 bit color. Larger displays can be formed using tiles of LED strip/panels - to build one big matrix. New and enhanced feature incude:

* 2-wire LEDs: APA102, Adafruit's DotStar, SK9822, HD1701, LPD8806, SM16716, WS2801, and more.
* 1-wire LEDs: NEOPIXEL, WS2811, WS2812, WS2812B, TX1813N1 (newer 2.2mm x 2.2mm), and more.
* 24 bit color image and basic sprite display (no edge detection).
* Block save/restore to restopre backgrounds.
* Transparent sprite drawing.
* Text, image, and sprite rotation in 90 deg increments.
* Option to read an XYTable_LookUp.h to replace slower, complex coordinate calculations. 
    * An Arduino sketch utility to create the lookup table is included.
    * The lookup table option allows for irregularly shaped LED physical layouts. 
* A report generator to comfirm proper LED array mapping configuration.
* Method names now use Adafruit_GFX naming (for similar methods).
* Along with a soon-to-be-release LEDExtender shield, 1-wire and 2-wire LEDs can be multiplexed in multiple Banks to total reduce LED strip length or control 16 seperate strings. The extender: 
    * Eliminates LED "sparkle" and flashing (a frustrating problem with 2-wire LED types) by reducing the number of LEDS needed in series.
    * With proper power supplies, and wiring, the Extender can support up to 256 LEDs per strip. That's 4,096 LEDs! A 32 bit MCU such as the Teensy 4.0/4.1 is required.
    * Multiplexes controller wiring, reducing pin count. Up to 16 LED strips/panels with only 8 wires! (4 for 2 DATA + 2 CLOCK pins, and up to 4 more "enable" pins to switch between Banks of LED strips.
    * Voltage step up from 3.3v to 5v.
    * Isolates of the MCU from the LED wiring. 
* Limitations in this library version:
    * SmartMatrix has transitioned into a software + hardware solution for HUB75 LED panels. 
       LEDMatrix_22 currently does not support SmartMatrix or HUB75 hardware.




###The LEDMatrix_22 manual can be accessed with the wiki button above

### Using multiple LED Strips together but not as a matrix or panel? 
This library is designed to use up to 4 seperate led strips (1-wire or 2-wire). With optional LEDExtender shields control up to 16 LED strips with 8 controller pins. 

## Single Matrix 
In this arrangement, one long led strip is cut into multiple rows to create an x,y array. As with previous LEDMatrix versions, the strips can be arranged into rows a zigzag, or left-2-right/right-2-left patterns. In this configuration you are limited to the length of the strip before sparkles/breakup occurs. SPI pins can be used to meet the demand of refreshing long data strings. This approach uses one FastLED Controller. 

## Tile Matrix 
Tiling (also called blocks) breaks the one long LED strip into smaller tiles. Popular 8x8 led panels are an example of this. The 8x8 panels can be strung together. LEDMatrix_22 can handle any arragement of these in normal or zizag LEDS in tiles, and normal or zizag tiles in the larger matrix panel. This approach uses one FastLED Controller.  

## Multiplexing with Multiple-controllers (_new_) 
LEDMatrix_22 supports a small shield PCB and FastLED's multiple Controllers to drive shorter strips or tiles rather than one long strip. For example, to drive a 32x32 matrix of 1024 LEDs with one long string, data send rates, brightness, and frames-per-second (fps) to avoid color breakdown as to be unusable.

Now, using the LEDExtender we can break up the 32x32 matrix and insert a second DATA/CLOCK line for LEDs 512-1024. No more sparkle even with higher data/brightness/fps rates. Additing additional data lines for 256, 128, or 64 LED segments allows even high data rates. 

## Available Methods
```c
  void SetLEDArray(struct CRGB *pLED);	// Only used with externally defined LED arrays

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
    void setCursor(int16_t x, int16_t y);
    void setTextColor(CRGB c);
    void setTextColor(CRGB c, CRGB bg);
    void setTextSize(uint8_t s);
    void setTextWrap(boolean w);
    void setRotation(uint8_t r);
    void cp437(boolean x=true);
    void setFont(const GFXfont *f = NULL);
    int16_t getCursorX(void) const; 
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
    void addLeds());  (replaces FastLed.addLeds)
    void defineBanks();

```

## Compatibility with SmartMatrix and its various library combinations

LEDMatrix_22 is designed as a medium weight library, with just enough features to support your project. Beyond FastLED (and the LED types FasyLED supports), this library is not intended to be combined or layered with other libraries. 
