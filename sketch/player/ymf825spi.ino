#include <SPI.h>

volatile bool ymf825_playing = false;
volatile bool ymf825_next_file = false;

void ymf825_pause() {
  ymf825_playing = !ymf825_playing;
  Serial.println(ymf825_playing ? "PLAY" : "PAUSE");
  wait_begin();
}

void ymf825_next() {
  Serial.println("NEXT");
  ymf825_next_file = true;
}

void ymf825_write(byte address, byte data) {
  ymf825_select();
  SPI.transfer(address);
  SPI.transfer(data);
  ymf825_unselect();

  // ymf825_wait(address, data);
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

/*
static void ymf825_wait(byte address, byte data) {
  switch(address) {
    case 0x02:
      if (data != 0x00)
        delayMicroseconds(1000);
      break;
    
    case 0x08:
      if (data == 0xf6)
        delayMicroseconds(1000);
      else if (data != 0x00)
        delayMicroseconds(21000);
    
    case 0x1a:
      if (data == 0x0a)
        delayMicroseconds(1000);
      else if (data == 0x00)
        delayMicroseconds(30000);
      break;

    default:
      return;
  }
}
*/
