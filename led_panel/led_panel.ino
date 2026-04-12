#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <HardwareSerial.h>

#define PANEL_RES_X 64
#define PANEL_RES_Y 64
#define PANEL_CHAIN 1



HUB75_I2S_CFG::i2s_pins pins = {
  .r1 = 2,
  .g1 = 32,
  .b1 = 15,
  .r2 = 4,
  .g2 = 33,
  .b2 = 16,
  .a = 17,
  .b = 26,
  .c = 5,
  .d = 27,   
  .e = 25,
  .lat = 14,
  .oe = 19,
  .clk = 18
};

MatrixPanel_I2S_DMA *display;

void setup() {


  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,
    PANEL_RES_Y,
    PANEL_CHAIN,
    pins
  );
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;
mxconfig.clkphase = false;
  mxconfig.driver = HUB75_I2S_CFG::SHIFTREG;
  mxconfig.mx_height = 64;  
  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(255);
}
void loop() {

  display->fillScreen(display->color565(0, 0, 255));
  delay(1000);
  display->fillScreen(display->color565(0, 0, 0));
  delay(1000);


}