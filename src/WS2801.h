#ifndef __WS2801__
#define __WS2801__

#include <Arduino.h>
#
class WS2801 {
public:
  WS2801();
  ~WS2801();
  void writeByte(byte n);
  void resetStrip();
  void clearStrip();
  void show();
  void setPixel(int p, int r, int g, int b);
  void begin(int numpixels, int clkpin, int datapin, bool isrgb);

private:
  int clk;
  int dat;
  bool rgb;
  int striplen;
  int numbytes;
  byte *strdata = NULL;
};
#endif // __WS2801__
