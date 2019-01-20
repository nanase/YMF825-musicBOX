
#include "definitions.h"

Decoder *decoder;
extern volatile bool ymf825Playing;
extern volatile bool ymf825NextFile;

void setup() {
  lcdInitialize();
  debugInitialize();
  setupPort();
  spiInitialize();

  if (!sdInitialize())
    while (true)
      delay(1000);

  ymf825Initialize();

  ymf825Pause();
  attachInterrupt(0, ymf825Pause, FALLING);
  attachInterrupt(1, ymf825Next, FALLING);

  opening();
  lcdSetFadeColor(255, 0, 0);
}

void loop() {
  bool paused = false;
  byte canProgress;

  sdSeekNext();
  ymf825ChipUnselect();
  waitBegin();

  do {
    if (!ymf825Playing) {
      if (!paused)
        ymf825AllRelease();

      paused      = true;
      canProgress = true;
      waitBegin();
      waitAdd(1);
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

  debugInfo("EOF");
  delete decoder;
}
