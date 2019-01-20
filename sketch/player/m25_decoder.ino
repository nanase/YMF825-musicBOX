#include "m25_decoder.h"
#include "readSdBuffer.h"

bool M25Decoder::progress() {
  byte selx;

  if (!sdReadBuffer(1))
    return false;

  selx = sdBufferByte() & M25_SELX_MASK;

  // Wait
  if (selx == M25_SELX_WAIT) {
    waitAdd((sdBufferByte() & M25_LENX_MASK) + 1);
    return true;
  }

  ymf825ChangeTargetChip(M25SelxToTargetChip(selx));

  // WOPx
  switch (sdBufferByte() & M25_WOPX_MASK) {
  case M25_WOPX_WRITE_DA_DD:
    return m25WriteDADD((sdBufferByte() & M25_LENX_MASK) + 1);

  case M25_WOPX_BURSTWRITE_TONE:
    return m25BurstwriteTone(((int16_t)sdBufferByte() & M25_LENX_MASK) + 1);

  case M25_WOPX_BURSTWRITE_EQ:
    return m25BurstwriteEq();

  default:
    // error
    return false;
  }
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
        sdBufferByteTo(i * 2 + 0), // address
        sdBufferByteTo(i * 2 + 1)  // data
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
      sdBufferByteTo(0),
      &sdBufferByteTo(1),
      length * 30 + 5);
  ymf825ChipUnselect();

  return true;
}

static bool m25BurstwriteEq() {
  if (!sdReadBuffer(16))
    return false;

  ymf825ChipSelect();
  ymf825BurstWrite(
      sdBufferByteTo(0),
      &sdBufferByteTo(1),
      15);
  ymf825ChipUnselect();

  return true;
}
