byte selx = SELX_LR_ENABLE;
static bool paused = false;

extern int16_t sdPosition;
extern byte sdBuffer[SD_BUFFER_SIZE];
extern volatile bool ymf825Playing;
extern volatile bool ymf825NextFile;

bool progress() {
  if (!ymf825Playing) {
    delay(1);

    if (!paused)
      ymf825AllRelease();

    paused = true;
    return true;
  }

  paused = false;

  if (ymf825NextFile) {
    ymf825NextFile = false;
    return false;
  }

  sdReadBuffer(1);

  if (sdPosition == EOF)
    return false;

  selx = sdBuffer[sdPosition] & SELX_MASK;

  // Wait
  if (selx == SELX_WAIT) {
    waitAdd((sdBuffer[sdPosition] & LENX_MASK) + 1);
    return true;
  }

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

static bool writeDADD(byte length) {
  sdReadBuffer(length * 2);

  if (sdPosition == EOF)
    return false;

  for (byte i = 0; i < length; i++) {
    ymf825ChipSelect();
    ymf825Write(
      sdBuffer[sdPosition + i * 2 + 0],   // address
      sdBuffer[sdPosition + i * 2 + 1]    // data
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
    length * 30 + 5
  );
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
    15
  );
  ymf825ChipUnselect();

  return true;
}
