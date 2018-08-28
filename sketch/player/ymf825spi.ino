#include <SPI.h>

volatile bool ymf825_playing = false;
volatile bool ymf825_next_file = false;

void ymf825_pause() {
  ymf825_playing = !ymf825_playing;
  Serial.print("[INFO ] Push button: ");
  Serial.println(ymf825_playing ? "PLAY" : "PAUSE");
  wait_begin();
}

void ymf825_next() {
  Serial.println("[INFO ] Push button: NEXT");
  ymf825_next_file = true;
}

void ymf825_write(byte address, byte data) {
  ymf825_select();
  SPI.transfer(address);
  SPI.transfer(data);
  ymf825_unselect();
}

void ymf825_burstwrite(byte address, byte* data, uint16_t size) {
  ymf825_select();
  SPI.transfer(address);
  SPI.transfer(data, size);
  ymf825_unselect();
}

void ymf825_select() {
  if (selx == SELX_LR_ENABLE)
    enable_lrch();
  else if (selx == SELX_LCH_ENABLE)
    enable_lch();
  else
    enable_rch();
}

void ymf825_unselect() {
  disable_ss();
}

void ymf825_reset_hardware() {
  disable_ic();
  enable_ic();
  delayMicroseconds(100);
  disable_ic();
}
