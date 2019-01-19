#include "decoder.h"
#include "definitions.h"
#include <SPI.h>

Decoder *decoder;
extern volatile bool ymf825Playing;
extern volatile bool ymf825NextFile;

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
  static bool paused = false;
  byte canProgress;

  sdSeekNext();
  ymf825ChipUnselect();
  waitBegin();

  PSerial.println("[INFO ] Playing Start");

  do {
    if (!ymf825Playing) {
      delay(1);

      if (!paused)
        ymf825AllRelease();

      paused      = true;
      canProgress = true;
    } else {
      paused = false;

      if (ymf825NextFile) {
        ymf825NextFile = false;
        canProgress    = false;
      } else
        canProgress = decoder->progress();
    }
    waitInvoke();
  } while (canProgress);

  PSerial.println("[INFO ] EOF");
  delete decoder;
}
