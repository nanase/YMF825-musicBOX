const byte SELX_MASK            = B11000000;
const byte SELX_WAIT            = B00000000;
const byte SELX_LCH_ENABLE      = B01000000;
const byte SELX_RCH_ENABLE      = B10000000;
const byte SELX_LR_ENABLE       = B11000000;
const byte WOPX_MASK            = B00110000;
const byte WOPX_WRITE_DA_DD     = B00000000;
const byte WOPX_RESERVED        = B00010000;
const byte WOPX_BURSTWRITE_TONE = B00100000;
const byte WOPX_BURSTWRITE_EQ   = B00110000;
const byte LENX_MASK            = B00001111;

#define M25SelxToTargetChip(x) (x >> 6)

extern int16_t sdPosition;
extern byte sdBuffer[SD_BUFFER_SIZE];

bool M25Decoder::progress() {
  byte selx;

  sdReadBuffer(1);

  if (sdPosition == EOF)
    return false;

  selx = sdBuffer[sdPosition] & SELX_MASK;

  // Wait
  if (selx == SELX_WAIT) {
    waitAdd((sdBuffer[sdPosition] & LENX_MASK) + 1);
    return true;
  }

  ymf825ChangeTargetChip(M25SelxToTargetChip(selx));

  // WOPx
  switch (sdBuffer[sdPosition] & WOPX_MASK) {
  case WOPX_WRITE_DA_DD:
    if (!writeDADD((sdBuffer[sdPosition] & LENX_MASK) + 1))
      return false;
    break;

  case WOPX_BURSTWRITE_TONE:
    if (!burstwriteTone(((int16_t)sdBuffer[sdPosition] & LENX_MASK) + 1))
      return false;
    break;

  case WOPX_BURSTWRITE_EQ:
    if (!burstwriteEq())
      return false;
    break;
  }

  return true;
}

bool M25Decoder::IsM25File(const char *filename) {
  char *extension = strrchr(filename, '.');
  return extension && strcmp(extension, ".M25") == 0;
}

static bool writeDADD(byte length) {
  sdReadBuffer(length * 2);

  if (sdPosition == EOF)
    return false;

  for (byte i = 0; i < length; i++) {
    ymf825ChipSelect();
    ymf825Write(
        sdBuffer[sdPosition + i * 2 + 0], // address
        sdBuffer[sdPosition + i * 2 + 1]  // data
    );
    ymf825ChipUnselect();
  }

  return true;
}

static bool burstwriteTone(int16_t length) {
  sdReadBuffer(length * 30 + 6);

  if (sdPosition == EOF)
    return false;

  ymf825ChipSelect();
  ymf825BurstWrite(
      sdBuffer[sdPosition + 0],
      &sdBuffer[sdPosition + 1],
      length * 30 + 5);
  ymf825ChipUnselect();

  return true;
}

static bool burstwriteEq() {
  sdReadBuffer(16);

  if (sdPosition == EOF)
    return false;

  ymf825ChipSelect();
  ymf825BurstWrite(
      sdBuffer[sdPosition + 0],
      &sdBuffer[sdPosition + 1],
      15);
  ymf825ChipUnselect();

  return true;
}
