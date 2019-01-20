#include "m25_decoder.h"

extern int16_t sdPosition;
extern byte sdBuffer[SD_BUFFER_SIZE];

bool M25Decoder::progress() {
  byte selx;

  if (!sdReadBuffer(1))
    return false;

  selx = sdBuffer[sdPosition] & M25_SELX_MASK;

  // Wait
  if (selx == M25_SELX_WAIT) {
    waitAdd((sdBuffer[sdPosition] & M25_LENX_MASK) + 1);
    return true;
  }

  ymf825ChangeTargetChip(M25SelxToTargetChip(selx));

  // WOPx
  switch (sdBuffer[sdPosition] & M25_WOPX_MASK) {
  case M25_WOPX_WRITE_DA_DD:
    if (!m25WriteDADD((sdBuffer[sdPosition] & M25_LENX_MASK) + 1))
      return false;
    break;

  case M25_WOPX_BURSTWRITE_TONE:
    if (!m25BurstwriteTone(((int16_t)sdBuffer[sdPosition] & M25_LENX_MASK) + 1))
      return false;
    break;

  case M25_WOPX_BURSTWRITE_EQ:
    if (!m25BurstwriteEq())
      return false;
    break;
  }

  return true;
}

bool M25Decoder::IsM25File(const char *filename) {
  char *extension = strrchr(filename, '.');
  return extension && strcmp(extension, ".M25") == 0;
}

static bool m25WriteDADD(byte length) {
  if (!sdReadBuffer(length * 2))
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

static bool m25BurstwriteTone(int16_t length) {
  if (!sdReadBuffer(length * 30 + 6))
    return false;

  ymf825ChipSelect();
  ymf825BurstWrite(
      sdBuffer[sdPosition + 0],
      &sdBuffer[sdPosition + 1],
      length * 30 + 5);
  ymf825ChipUnselect();

  return true;
}

static bool m25BurstwriteEq() {
  if (!sdReadBuffer(16))
    return false;

  ymf825ChipSelect();
  ymf825BurstWrite(
      sdBuffer[sdPosition + 0],
      &sdBuffer[sdPosition + 1],
      15);
  ymf825ChipUnselect();

  return true;
}
