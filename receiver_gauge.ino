
#include <SPI.h>				// must include this here (or else IDE can't find it)
                                           
#include "PDQ_ST7735_config.h"			// PDQ: ST7735 pins and other setup for this sketch
#include <PDQ_GFX.h>				// PDQ: Core graphics library
#include <PDQ_ST7735.h>			// PDQ: Hardware-specific driver library
#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>

#include <Adafruit_GFX.h>

struct gaugeConfig{
  uint originX,originY;
  uint height,width;
  uint gfxHeight,gfxWidth;
  bool isVert;
  bool needsSetup;
  char[] label;
  int min,warn,max;
  int value;
}

PDQ_ST7735 tft;			// PDQ: create LCD object (using pins in "PDQ_ST7735_config.h")

// canvas for drawing the gauge
GFXcanvas1 canvas(128,64);

GFXcanvas1 small_horiz_canvas(smallGaugeWidth-(2*padding)-2,meterWidth-2);
GFXcanvas1 small_vert_canvas(meterWidth-2,smallGaugeHeight-(2*padding)-2);
GFXcanvas1 wide_gauge_canvas(wideGaugeWidth - (2*padding),meterwidth-2);

// lookup table for fast sin/cos calculations
static const uint8_t isinTable8[] = { 
  0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 44, 
  49, 53, 57, 62, 66, 70, 75, 79, 83, 87, 
  91, 96, 100, 104, 108, 112, 116, 120, 124, 128, 
  131, 135, 139, 143, 146, 150, 153, 157, 160, 164, 
  167, 171, 174, 177, 180, 183, 186, 190, 192, 195, 
  198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 
  223, 225, 227, 229, 231, 233, 235, 236, 238, 240, 
  241, 243, 244, 245, 246, 247, 248, 249, 250, 251, 
  252, 253, 253, 254, 254, 254, 255, 255, 255, 255, 
};

long startTime;
long endTime;

long lastDraw;
long thisDraw;

const int padding = 5;
const int meterWidth = 8;
const int smallGaugeWidth = 64;
const int smallGaugeHeight = 60;
const int wideGaugeWidth = 128;
const int wideGaugeHeight = 40;

void setup(void) {
  Serial.begin(9600);

}

void loop() {

}

void drawPage(gaugeConfig[] gauges, int length) {
  for (int i=0; i < length; i++) {
    drawGauge(gauges[i]);
  }
  thisDraw = millis();
  int drawTime = thisDraw - lastDraw;
  lastDraw = thisDraw;
  if (printFPS) {
    tft.setCursor(0,0);
    tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
    tft.setTextSize(1);
    tft.setTextWrap(false);
    tft.print(F("FPS: "));
    //lastDraw - thisDraw = draw interval in ms
    //fps is draws per second, or draws per 1000 ms
    tft.setTextColor(ST7735_MAGENTA,ST7735_BLACK);
    tft.print(1000/drawTime);
    tft.print(F("."));
    tft.print((10000/drawTime)%10);
  }
}

void drawGauge(gaugeConfig gauge) {
  if(gauge.needsSetup) {
    //blank the gauge area
    tft.fillRect(gauge.originX,gauge.originY,gauge.width,gauge.height,ST7735_BLACK);
    //draw the outline boxes of the gauge bar
    if (gauge.isVert) {
      gauge.gfxHeight = gauge.height-(2*padding);
      gauge.gfxWidth = meterWidth;
    }
    else {
      gauge.gfxHeight = meterWidth;
      gauge.gfxWidth = gauge.width-(2*padding);
    }
    tft.drawRect(gauge.originX+padding,gauge.originY+padding,gauge.gfxWidth,gauge.gfxHeight,ST7735_WHITE);
    //draw the gauge label centered at the top
    //label width = sizeof(gauge.label)
    tft.setCursor(gauge.originX+(gauge.width/2)-(sizeof(gauge.label)*3),gauge.originY+2);
    tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
    tft.setTextSize(1);
    tft.setTextWrap(false);
    tft.print(gauge.label);
    //TODO: draw the gauge limits at the ends of he gauge
  }
  //long map(value, fromLow, fromHigh, toLow, toHigh)
  //draw the meter itself on a canvas
  /*
  0,0                   ||  gauge.max
                        ||
                        ||
                        ||  value
                        ||
                        ||
                        ||
  canvas.height,0       ||  gauge.min

  
  
  */
  if (gauge.isVert) {
    small_vert_canvas.fillScreen(0);
    small_vert_canvas.fillRect(0,map(gauge.value,gauge.min,gauge.max,small_vert_canvas.height,0),small_vert_canvas.width,small_vert_canvas.height)
  }
  //paste the canvas on the gauge area

  //draw the number on the gauge

}

void drawRPMGaugeCanvas(int rpm, int maxrpm, int redline) {

  int angle = map(rpm, 0, maxrpm, 0, 180);

  int digits;

  if (rpm < 10) {
    digits = 1;
  }
  else if (rpm < 100) {
    digits = 2;
  }
  else if (rpm < 1000) {
    digits = 3;
  }
  else {
    digits = 4;
  }

  //canvas is 128w, 64h

    canvas.fillRect(0, 0, 128, 64, ST7735_BLACK); // blank the gauge area
    canvas.fillCircle(64, 64, 60, ST7735_WHITE); // draw the half circle 
    canvas.fillCircle(64, 64, (60 * 9) / 10, ST7735_BLACK);

    // need a line from (64,64) to (64+cos(angle),64-sin(angle))
    canvas.drawLine(64, 64, 64-(i_cos(angle)*64L/127), 64-(i_sin(angle)*64L/127), ST7735_WHITE);

  canvas.setCursor(64-(digits*9), 64-24);
  canvas.setTextColor(ST7735_WHITE,ST7735_BLACK);
  canvas.setTextSize(3);
  canvas.setTextWrap(false);
  canvas.print(rpm);

  tft.drawBitmap(0,0,canvas.getBuffer(),canvas.width(),canvas.height(),0xFFFF,0x0000);
}

int i_sin(int x) {
  boolean pos = true;  // positive - keeps an eye on the sign.
  uint8_t idx;
   //remove next 6 lines for fastestl!
  if (x > 180) 
  {
    idx = x - 180;
    pos = !pos;
  }
  else idx = x;
  if (idx > 90) idx = 180 - idx;
  if (pos) return isinTable8[idx]/2 ;
  return -(isinTable8[idx]/2);
}

int i_cos(int x){
  return i_sin(x+90);
}



