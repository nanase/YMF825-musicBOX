#include "definitions.h"
#include <SPI.h>

void setup() {
  PSerial.begin(9600);
  PSerial.println("[DEBUG] Setup");

  setupPort();
  PSerial.println("[DEBUG] Port setup");
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();
  PSerial.println("[DEBUG] SPI Begin");

  if (!sdInitialize())
    while (true)
      delay(1000);

  ymf825Initialize();
  lcdInitialize();

  ymf825Pause();
  attachInterrupt(0, ymf825Pause, FALLING);
  attachInterrupt(1, ymf825Next, FALLING);

  opening();

}

void loop() {
  sdSeekNext();
  ymf825ChipUnselect();
  waitBegin();

  PSerial.println("[INFO ] Playing Start");

  while (progress())
    waitInvoke();

  PSerial.println("[INFO ] EOF");
}
