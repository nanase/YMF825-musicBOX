static const unsigned char EQ_DATA[] PROGMEM = {
    0x00, 0x5d, 0xa3, 0x00, 0xbb, 0x46, 0x00, 0x5d, 0xa3, 0x1a, 0x1b, 0x23, 0xf4, 0x6e, 0x4f};
static const unsigned char TONE_DATA[] PROGMEM = {
    0x81, 0x00, 0x00, 0xf0, 0xf0, 0x4f, 0xa8, 0x66, 0x25, 0x00, 0xf1, 0xf3, 0x7f, 0x00, 0x30, 0x11, 0x00, 0xf0, 0xf4,
    0x2f, 0x54, 0x00, 0x00, 0x00, 0xf0, 0xf4, 0xbf, 0x94, 0x00, 0x10, 0x00, 0x80, 0x03, 0x81, 0x80};

extern Decoder *decoder;

static void progmemReadBuffer(byte *buffer, const void *progmem, uint16_t size) {
  for (uint16_t i = 0; i < size; i++)
    buffer[i] = pgm_read_byte((unsigned char *)progmem + i);
}

void opening() {
  lcdSetFadeSpeed(200);
  lcdSetFadeColor(255, 255, 255);

  // phase 0: 500ms / remains 1000ms
  for (uint16_t k = 0; k < 50; k++) {
    lcdFade();
    delay(10);
  }

  // setup EQ
  progmemReadBuffer(sdBuffer, EQ_DATA, 15);
  ymf825BurstWrite(0x20, sdBuffer, 15);

  // phase 1-1: voice 0 set
  ymf825Write(0x0b, 0x00);
  ymf825Write(0x08, 0x16);
  ymf825Write(0x08, 0x00);
  ymf825Write(0x11, 0x01);
  progmemReadBuffer(sdBuffer, TONE_DATA, 35);
  ymf825BurstWrite(0x07, sdBuffer, 35);
  ymf825Write(0x0c, 0x7c);
  ymf825Write(0x12, 0x08);
  ymf825Write(0x13, 0x00);
  ymf825Write(0x10, 0x61);
  ymf825Write(0x0d, 0x15);
  ymf825Write(0x0e, 0x65);
  ymf825Write(0x0f, 0x40);
  // phase 1-2: 410ms / remains 590ms
  for (uint16_t k = 0; k < 41; k++) {
    lcdFade();
    delay(10);
  }

  // phase 2-1: voice 1 set
  ymf825Write(0x0b, 0x01);
  ymf825Write(0x08, 0x16);
  ymf825Write(0x08, 0x00);
  ymf825Write(0x11, 0x01);
  ymf825ChangeTargetChip(YMF825_LCH_ENABLE);
  ymf825Write(0x0c, 0x60);
  ymf825ChangeTargetChip(YMF825_RCH_ENABLE);
  ymf825Write(0x0c, 0x7c);
  ymf825ChangeTargetChip(YMF825_BOTH_ENABLE);
  ymf825Write(0x12, 0x08);
  ymf825Write(0x13, 0x00);
  ymf825Write(0x10, 0x3d);
  ymf825Write(0x0d, 0x15);
  ymf825Write(0x0e, 0x65);
  ymf825Write(0x0f, 0x40);
  // phase 2-2: 410ms / remains 180ms
  for (uint16_t k = 0; k < 41; k++) {
    lcdFade();
    delay(10);
  }

  // phase 3-1: voice 2 set
  ymf825Write(0x0b, 0x02);
  ymf825Write(0x08, 0x16);
  ymf825Write(0x08, 0x00);
  ymf825Write(0x11, 0x01);
  ymf825ChangeTargetChip(YMF825_LCH_ENABLE);
  ymf825Write(0x0c, 0x7c);
  ymf825ChangeTargetChip(YMF825_RCH_ENABLE);
  ymf825Write(0x0c, 0x60);
  ymf825ChangeTargetChip(YMF825_BOTH_ENABLE);
  ymf825Write(0x12, 0x08);
  ymf825Write(0x13, 0x00);
  ymf825Write(0x10, 0x25);
  ymf825Write(0x0d, 0x15);
  ymf825Write(0x0e, 0x65);
  ymf825Write(0x0f, 0x40);
  // phase 3-2: 180ms
  for (uint16_t k = 0; k < 18; k++) {
    lcdFade();
    delay(10);
  }
  // phase 3-3: 230ms
  lcd.setCursor(0, 0);
  lcdWrite('*');
  lcd.setCursor(15, 0);
  lcdWrite('*');
  lcd.setCursor(2, 0);
  delay(50);
  lcdWrite('J');
  delay(50);
  lcdWrite('a');
  delay(50);
  lcdWrite('p');
  delay(50);
  lcdWrite('a');
  delay(30);

  // phase 4-1: voice 3 set
  ymf825Write(0x0b, 0x03);
  ymf825Write(0x08, 0x16);
  ymf825Write(0x08, 0x00);
  ymf825Write(0x11, 0x01);
  ymf825Write(0x0c, 0x7c);
  ymf825Write(0x12, 0x08);
  ymf825Write(0x13, 0x00);
  ymf825Write(0x10, 0x15);
  ymf825Write(0x0d, 0x15);
  ymf825Write(0x0e, 0x65);
  ymf825Write(0x0f, 0x40);
  delay(20);
  lcdWrite('r');
  delay(50);
  lcdWrite('i');
  delay(50);
  lcdWrite(' ');
  delay(50);
  lcdWrite('o');
  delay(50);
  lcdWrite('r');
  delay(50);
  lcdWrite('g');
  delay(50);
  lcdWrite('e');
  delay(50);
  lcdWrite('l');
  delay(40);

  // phase 5:
  delay(110);
  lcd.setCursor(8 - sizeof(VERSION_STRING) / 2, 1);

  for (uint8_t i = 0; i < sizeof(VERSION_STRING) - 1; i++) {
    lcdWrite(VERSION_STRING[i]);
    delay(50);
  }

  ymf825Write(0x0b, 0x00);
  ymf825Write(0x0f, 0x00);
  ymf825Write(0x0b, 0x01);
  ymf825Write(0x0f, 0x00);
  ymf825Write(0x0b, 0x02);
  ymf825Write(0x0f, 0x00);
  ymf825Write(0x0b, 0x03);
  ymf825Write(0x0f, 0x00);
  delay(1000);
}
