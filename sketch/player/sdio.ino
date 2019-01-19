#include "PetitFS.h"
#include "definitions.h"

#if (SD_CS_PIN != PIN_SD)
#error("SD_CS_PIN is not equal PIN_SD. Please edit SD_CS_PIN in pffArduino.h!");
#endif

#if (_USE_READ != 1)
#error("_USE_READ is disabled. Please enable _USE_READ in pffconf.h!");
#endif

#if (_USE_DIR != 1)
#error("_USE_DIR is disabled. Please enable _USE_DIR in pffconf.h!");
#endif

#if (_USE_LSEEK != 1)
#error("_USE_LSEEK is disabled. Please enable _USE_LSEEK in pffconf.h!");
#endif

byte sdBuffer[SD_BUFFER_SIZE];
int16_t sdPosition = 0;

static FATFS fs;
static FILINFO fileInfo;
static DIR root;
static bool fileOpened          = false;
static int32_t sdSeekPosition   = 0;
static int32_t sdBufferPosition = 0;

bool sdInitialize() {
  PSerial.println("[DEBUG] SD initialization");

  while (pf_mount(&fs) != FR_OK) {
    PSerial.println("[ERROR] SD initialization failed");
    delay(1000);
  }

  if (pf_opendir(&root, "/") != FR_OK) {
    PSerial.println("[ERROR] open root dir failed");
    return false;
  }

  return true;
}

void sdSeekNext() {
  if (fileOpened) {
    PSerial.print("[DEBUG] Close: ");
    PSerial.println(fileInfo.fname);
    fileOpened = false;
  }

  while (!fileOpened) {
    if (pf_readdir(&root, &fileInfo) != FR_OK || fileInfo.fname[0] == 0) {
      PSerial.println("[DEBUG] Rewind");
      delay(250);

      if (pf_opendir(&root, "/") != FR_OK) {
        PSerial.println("[ERROR] open root dir failed");

        while (true)
          delay(1000);
      }

      continue;
    }

    if (sdIsM25File()) {
      fileOpened = true;
      break;
    }

    PSerial.print("[DEBUG] Unknown format: ");
    PSerial.println(fileInfo.fname);
  }

  PSerial.print("[DEBUG] Open: ");
  PSerial.println(fileInfo.fname);

  if (pf_open(fileInfo.fname) != FR_OK) {
    PSerial.print("[ERROR] Open failed!");

    while (true)
      delay(1000);
  }

  sdSeekPosition   = 0;
  sdBufferPosition = 0;
  sdRead();
}

static bool sdIsM25File() {
  char *extension = strrchr(fileInfo.fname, '.');
  return extension && strcmp(extension, ".M25") == 0;
}

static bool sdRead() {
  uint16_t i;

  if (pf_lseek(sdSeekPosition) != FR_OK) {
    PSerial.println("[ERROR] Seek failed");

    while (true)
      delay(1000);
  }

  for (i = 0; i < SD_BUFFER_SIZE; i++)
    sdBuffer[i] = 0x00;

  return pf_read(sdBuffer, (uint16_t)SD_BUFFER_SIZE, &i) == FR_OK && i > 0;
}

void sdReadBuffer(int16_t size) {
  if (SD_BUFFER_SIZE - sdBufferPosition < size) {
    sdSeekPosition += sdBufferPosition;

    sdBufferPosition = size;
    sdPosition       = sdRead() ? 0 : EOF;
    return;
  }

  sdPosition = sdBufferPosition;
  sdBufferPosition += size;
}
