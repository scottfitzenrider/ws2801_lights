#include "WS2801.h"

WS2801::WS2801() {
  striplen = 0;
  strdata = NULL;
  numbytes = 0;
}
WS2801::~WS2801() { free(strdata); }
void WS2801::writeByte(byte n) {
  byte b;
  //    Serial.printf("writing %d\n", n);
  for (b = 0x80; b; b >>= 1) {
    if (b & n)
      digitalWrite(dat, HIGH);
    else
      digitalWrite(dat, LOW);
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }
}

void WS2801::resetStrip() {
  writeByte(0);
  writeByte(0);
  writeByte(0);
  writeByte(0);
  writeByte(0);
  writeByte(0);
  writeByte(0);
  writeByte(0);
  writeByte(0);
  delay(1);
}
void WS2801::clearStrip() { memset(strdata, 0x80, numbytes); }
void WS2801::show() {
  int n;
  for (n = 0; n < numbytes; n++) {
    // Serial.printf("writing strdata byte %d %d\n", n, strdata[n]);
    // delay(5);
    writeByte(strdata[n]);
  }
  resetStrip();
}
void WS2801::setPixel(int p, int r, int g, int b) {
  if (p < striplen) {
    strdata[p * 3] = (byte)b | 0x80;
    strdata[p * 3 + 1] = (byte)r | 0x80;
    strdata[p * 3 + 2] = (byte)g | 0x80;
  }
  // Serial.printf("Setting Pixel %d %d %d %d\n", p, r, g, b);
  // delay(5);
}
void WS2801::begin(int numpixels, int clkpin, int datapin, bool isrgb) {
  clk = clkpin;
  dat = datapin;
  rgb = isrgb;
  this->striplen = numpixels;
  if (strdata) {
    free(strdata);
  }
  strdata = (byte *)malloc(striplen * 3);
  numbytes = strdata ? striplen * 3 : 0;
  pinMode(clk, OUTPUT);
  pinMode(dat, OUTPUT);
  digitalWrite(clk, LOW);
  resetStrip();
  clearStrip();
  show();
  // Serial.printf("\nnumbytes %d", numbytes);
}
