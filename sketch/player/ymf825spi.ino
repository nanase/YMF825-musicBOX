#include <SPI.h>

volatile bool ymf825Playing = false;
volatile bool ymf825NextFile = false;

void ymf825Pause() {
  ymf825Playing = !ymf825Playing;
  Serial.print("[INFO ] Push button: ");
  Serial.println(ymf825Playing ? "PLAY" : "PAUSE");
  waitBegin();
}

void ymf825Next() {
  Serial.println("[INFO ] Push button: NEXT");
  ymf825NextFile = true;
}

void ymf825Write(byte address, byte data) {
  ymf825ChipSelect();
  SPI.transfer(address);
  SPI.transfer(data);
  ymf825ChipUnselect();
}

void ymf825BurstWrite(byte address, byte* data, uint16_t size) {
  ymf825ChipSelect();
  SPI.transfer(address);
  SPI.transfer(data, size);
  ymf825ChipUnselect();
}

void ymf825ChipSelect() {
  if (selx == SELX_LR_ENABLE)
    enableLRch();
  else if (selx == SELX_LCH_ENABLE)
    enableLch();
  else
    enableRch();
}

void ymf825ChipUnselect() {
  disableSS();
}

void ymf825ResetHardware() {
  disableIC();
  enableIC();
  delayMicroseconds(100);
  disableIC();
}
