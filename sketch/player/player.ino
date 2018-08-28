#include "definitions.h"
#include <SPI.h>

void setup() {
  Serial.begin(9600);
  Serial.println("[DEBUG] Setup");

  setup_port();
  Serial.println("[DEBUG] Port setup");
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();
  Serial.println("[DEBUG] SPI Begin");

  ymf825_unselect();
  ymf825_reset_hardware();
  sd_initialize();

  ymf825_pause();
  attachInterrupt(0, ymf825_pause, FALLING);
  attachInterrupt(1, ymf825_next, FALLING);
}

void loop() {
  sd_seek_next();
  ymf825_unselect();
  ymf825_reset_hardware();
  wait_begin();

  Serial.println("[INFO ] Playing Start");

  while (progress()) {
    wait_invoke();
  }

  Serial.println("[INFO ] EOF");
}
