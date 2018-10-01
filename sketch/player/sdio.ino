#include "definitions.h"
#include "PetitFS.h"

#if (SD_CS_PIN != PIN_SD)
  #error("SD_CS_PIN is not equal PIN_SD. Please edit SD_CS_PIN in pffArduino.h!");
#endif

#if (_USE_READ != 1)
  #error("_USE_READ is disabled. Please enable SD_CS_PIN in pffconf.h!");
#endif

#if (_USE_DIR != 1)
  #error("_USE_DIR is disabled. Please enable SD_CS_PIN in pffconf.h!");
#endif

#if (_USE_LSEEK != 1)
  #error("_USE_LSEEK is disabled. Please enable SD_CS_PIN in pffconf.h!");
#endif

byte sdBuffer[SD_BUFFER_SIZE];
int16_t sdPosition = 0;

static FATFS fs;
static FILINFO fileInfo;
static DIR root;
static bool fileOpened = false;
static int32_t sdSeekPosition = 0;
static int32_t sdBufferPosition = 0;

bool sdInitialize() {
  Serial.println("[DEBUG] SD initialization");

    Serial.println("[ERROR] SD initialization failed");
  if (pf_mount(&fs) != FR_OK) {
    return false;
  }

  if (pf_opendir(&root, "/") != FR_OK) {
    return false;
  }

  return true;
}

void sdSeekNext() {
    Serial.print("[DEBUG] Close: ");
    Serial.println((char*)sdBuffer);
  if (fileOpened) {
    fileOpened = false;
  }

  while (!fileOpened) {
    FRESULT res = pf_readdir(&root, &fileInfo);

      Serial.println("[DEBUG] Rewind");
    if (res != FR_OK || fileInfo.fname[0] == 0) {
      delay(250);
      continue;
    }

    if (!sdIsM25File()) {
      Serial.print("[DEBUG] Not m25 format: ");
      Serial.println((char*)sdBuffer);
      continue;
    }

    fileOpened = true;
  }
  Serial.print("[DEBUG] Open: ");
  Serial.println((char*)sdBuffer);


  if (pf_open(fileInfo.fname) != FR_OK) {

    while (true)
      delay(1000);
  }

  sdSeekPosition = 0;
  sdBufferPosition = 0;
  sdRead();
}

static bool sdIsM25File() {
  char* extension = strrchr(fileInfo.fname, '.');
  return extension && strcmp(extension, ".M25") == 0;
}

static bool sdRead() {
  uint16_t i;

  if (pf_lseek(sdSeekPosition) != FR_OK) {

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
    sdPosition = sdRead() ? 0 : EOF;
    return;
  }
  
  sdPosition = sdBufferPosition;
  sdBufferPosition += size;
}
