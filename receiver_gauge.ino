
#include <SPI.h>				// must include this here (or else IDE can't find it)
                                           
#include "PDQ_ST7735_config.h"			// PDQ: ST7735 pins and other setup for this sketch
#include <PDQ_GFX.h>				// PDQ: Core graphics library
#include <PDQ_ST7735.h>			// PDQ: Hardware-specific driver library
#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>

#include <Adafruit_GFX.h>

PDQ_ST7735 tft;			// PDQ: create LCD object (using pins in "PDQ_ST7735_config.h")

GFXcanvas1 canvas(128,64);

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

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST7735 TFT Test"));

#if defined(ST7735_RST_PIN)	// reset like Adafruit does
  Serial.println(F("Using hardware reset pin"));
	FastPin<ST7735_RST_PIN>::setOutput();
	FastPin<ST7735_RST_PIN>::hi();
	FastPin<ST7735_RST_PIN>::lo();
	delay(1);
	FastPin<ST7735_RST_PIN>::hi();
#endif

  tft.begin();  

  tft.fillScreen(ST7735_BLACK);

}

void loop() {

  int revs = 0;
  do {
    drawRPMGaugeCanvas(revs,6000,5500);
    revs+=10;
  } while (revs < 6250);
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
/*     if (x < 0) {
       x = -x;
       pos = !pos;  
     }  
    if (x >= 360) x %= 360;   */ 
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



