byte sdBuffer[SD_BUFFER_SIZE];
int16_t sdPosition = 0;

static FATFS fs;
static FILINFO fileInfo;
static DIR root;
static bool fileOpened          = false;
static int32_t sdSeekPosition   = 0;
static int32_t sdBufferPosition = 0;

bool sdInitialize() {
  debugInfo("SD init");

  while (pf_mount(&fs) != FR_OK) {
    debugWarn("SD init failed");
    delay(1000);
  }

  if (pf_opendir(&root, "/") != FR_OK) {
    debugWarn("open / failed");
    return false;
  }

  return true;
}

void sdSeekNext() {
  fileOpened = false;

  while (!fileOpened) {
    if (pf_readdir(&root, &fileInfo) != FR_OK || fileInfo.fname[0] == 0) {
      debugInfo("Rewind");
      delay(250);

      if (pf_opendir(&root, "/") != FR_OK)
        debugError("open / failed");

      continue;
    }

    if (M25Decoder::IsM25File(fileInfo.fname)) {
      fileOpened = true;
      decoder    = new M25Decoder();
      break;
    }

    if (pf_open(fileInfo.fname) == FR_OK) {
      sdSeekPosition   = 0;
      sdBufferPosition = 0;

      if (JpmDecoder::IsJpmFile(fileInfo.fname)) {
        fileOpened = true;
        decoder    = new JpmDecoder();
        break;
      }
    }
  }

  debugInfo(fileInfo.fname);

  if (pf_open(fileInfo.fname) != FR_OK) {
    debugWarn(fileInfo.fname);
    return;
  }

  sdSeekPosition   = 0;
  sdBufferPosition = 0;
  sdRead();
}

static bool sdRead() {
  uint16_t i;

  if (pf_lseek(sdSeekPosition) != FR_OK)
    debugError("Seek failed");

  for (i = 0; i < SD_BUFFER_SIZE; i++)
    sdBuffer[i] = 0x00;

  return pf_read(sdBuffer, (uint16_t)SD_BUFFER_SIZE, &i) == FR_OK && i > 0;
}

bool sdReadBuffer(int16_t size) {
  if (SD_BUFFER_SIZE - sdBufferPosition < size) {
    sdSeekPosition += sdBufferPosition;

    sdBufferPosition = size;
    sdPosition       = sdRead() ? 0 : EOF;
  } else {
    sdPosition = sdBufferPosition;
    sdBufferPosition += size;
  }

  return sdPosition != EOF;
}
