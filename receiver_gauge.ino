#include <TFT_eSPI.h>
#include <SPI.h>

#define TDELAY 500

TFT_eSPI tft = TFT_eSPI(); 

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.fillScreen(0xF81F);
}

void loop() {
  
  uint32_t data = 0;
  data = tft.readcommand32(0x04); // read display ID info
  Serial.print("Display ID info (0x04): ");Serial.println(data,HEX);

  data = tft.readcommand16(0x52); // read display brightness
  Serial.print("Display brightness (0x52): ");Serial.println(data,HEX);

  data = tft.readcommand16(0x54); // read CTRL display value
  Serial.print("Display CTRL Display value (0x54): ");Serial.println(data,HEX);

  data = tft.readcommand16(0x56); // read CABC value
  Serial.print("Display CABC value (0x56): ");Serial.println(data,HEX);

  data = tft.readcommand16(0x5F); // read CABC minimum brightness value
  Serial.print("Display CABC minimum brightness value (0x5F): ");Serial.println(data,HEX);

  delay(5000);

  tft.writecommand(0x53);
  tft.writedata(0x2c);

  uint8_t loop = 0;

  for (uint8_t i=0; i<255; i++) {
    tft.writecommand(0x51);
    tft.writedata(i);
    delay(20);
      data = tft.readcommand16(0x52); // read display brightness
      Serial.print("Display brightness (0x52): ");Serial.println(data,HEX);

  }
}