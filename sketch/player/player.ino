#include "definitions.h"
#include <SPI.h>

void setup() {
  Serial.begin(9600);
  Serial.println("[DEBUG] Setup");

  setupPort();
  Serial.println("[DEBUG] Port setup");
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();
  Serial.println("[DEBUG] SPI Begin");

  ymf825ChipUnselect();
  ymf825ResetHardware();
  sdInitialize();

  ymf825Pause();
  attachInterrupt(0, ymf825Pause, FALLING);
  attachInterrupt(1, ymf825Next, FALLING);
}

void loop() {
  sdSeekNext();
  ymf825ChipUnselect();
  ymf825ResetHardware();
  waitBegin();

  Serial.println("[INFO ] Playing Start");

  while (progress()) {
    waitInvoke();
  }

  Serial.println("[INFO ] EOF");
}
