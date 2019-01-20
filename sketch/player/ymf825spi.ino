extern Decoder *decoder;
volatile bool ymf825Playing  = false;
volatile bool ymf825NextFile = false;
static byte ymf825TargetChip = YMF825_BOTH_ENABLE;

void ymf825Initialize() {
  ymf825Write(0x1d, 0x01);
  ymf825Write(0x02, 0x0f);
  delay(10);
  ymf825Write(0x00, 0x01);
  ymf825Write(0x01, 0x00);
  ymf825Write(0x1a, 0xa3);
  delay(10);
  ymf825Write(0x1a, 0x00);
  delay(30);
  ymf825Write(0x02, 0x00);
  ymf825Write(0x19, 0xc0);
  ymf825Write(0x1b, 0x3f);
  ymf825Write(0x14, 0x00);
  ymf825Write(0x03, 0x03);
  ymf825Write(0x08, 0xf6);
  delay(30);
  ymf825Write(0x08, 0x00);
  ymf825Write(0x09, 0xfc);
  ymf825Write(0x0a, 0x00);
  ymf825Write(0x17, 0x40);
  ymf825Write(0x18, 0x00);
  ymf825Write(0x03, 0x03);
  ymf825Write(0x19, 0xa0);
  ymf825Write(0x08, 0xf6);
  delay(10);
  ymf825Write(0x08, 0x00);
}

void ymf825Pause() {
  ymf825Playing = !ymf825Playing;
  PSerial.print("[INFO ] Push button: ");
  PSerial.println(ymf825Playing ? "PLAY" : "PAUSE");
}

void ymf825Next() {
  PSerial.println("[INFO ] Push button: NEXT");
  ymf825NextFile = true;
}

void ymf825AllRelease() {
  PSerial.print("[INFO ] All Release");

  for (byte i = 0; i < 16; i++) {
    enableLRch();
    SPI.transfer(0x0b);
    SPI.transfer(i);
    disableSS();

    enableLRch();
    SPI.transfer(0x0f);
    SPI.transfer(i);
    disableSS();
  }
}

void ymf825Write(byte address, byte data) {
  ymf825ChipSelect();
  SPI.transfer(address);
  SPI.transfer(data);
  ymf825ChipUnselect();
}

void ymf825BurstWrite(byte address, byte *data, uint16_t size) {
  ymf825ChipSelect();
  SPI.transfer(address);
  SPI.transfer(data, size);
  ymf825ChipUnselect();
}

void ymf825ChangeTargetChip(byte targetChip) {
  ymf825TargetChip = targetChip;
}

void ymf825ChipSelect() {
  if (ymf825TargetChip == YMF825_BOTH_ENABLE)
    enableLRch();
  else if (ymf825TargetChip == YMF825_LCH_ENABLE)
    enableLch();
  else
    enableRch();
}

void ymf825ChipUnselect() {
  disableSS();
}
