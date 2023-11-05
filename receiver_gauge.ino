#include <TouchScreen.h>

#include "User_Setup.h"
#include <TFT_eSPI.h>     // Hardware-specific library
#include <TFT_eWidget.h>  // Widget library

#include <SPI.h>
#include <FS.h>
#include <SD.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

#define GAUGES 1
#define GRAPH 2

#define DISPLAY_SPLASH

#define SIMULATE_COMMS  // comment this out if you don't want simulated comms

uint8_t data[128];  // data buffer for data from Speeduino

#ifdef DISPLAY_SPLASH
// add a bitmap here stored in PROGMEM
#endif

TFT_eSPI tft = TFT_eSPI(); 


unsigned long start,finish;

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.fillScreen(0xF81F);

  #ifdef DISPLAY_SPLASH
  //display the splash screen
  #endif


}


void loop() {

  getData();

  compositeData();

  drawScreen();

  logData();

}


void compositeData() {
  start = millis();

  finish = millis();
  Serial.print("drawScreen routine took ");
  Serial.print(finish-start);
  Serial.print(" ms.\n");
}

void drawScreen() {
  start = millis();

  if (currentScreen == GAUGES) {

  }
  else if (currentScreen == GRAPH) {

  }


  finish = millis();
  Serial.print("drawScreen routine took ");
  Serial.print(finish-start);
  Serial.print(" ms.\n");
}

void getData() {
  start = millis();
  #ifdef SIMULATE_COMMS
    // simulate requesting and receiving data here
  #else 
    // do real data exchange here
  #endif
  finish = millis();
  Serial.print("getData routine took ");
  Serial.print(finish-start);
  Serial.print(" ms.\n");
}

void logData() {

}