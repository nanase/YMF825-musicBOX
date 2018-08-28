#include "definitions.h"
#include "SdFat.h"

byte sdBuffer[SD_BUFFER_SIZE];
int16_t sdPosition = 0;

static SdFat SD;
static File root;
static File currentFile;
static int32_t sdSeekPosition = 0;
static int32_t sdBufferPosition = 0;

bool sdInitialize() {
  Serial.println("[DEBUG] SD initialization");

  if (!SD.begin(PIN_SD)) {
    Serial.println("[ERROR] SD initialization failed");
    return false;
  }

  root = SD.open("/");
  return true;
}

void sdSeekNext() {
  if (currentFile.available()) {
    sdReadFileName();
    Serial.print("[DEBUG] Close: ");
    Serial.println((char*)sdBuffer);
    currentFile.close();
  }

  while (true) {
    currentFile = root.openNextFile();

    if (!currentFile) {
      root.rewindDirectory();
      Serial.println("[DEBUG] Rewind");
      delay(250);
      continue;
    }

    if (!sdIsM25File()) {
      sdReadFileName();
      Serial.print("[DEBUG] Not m25 format: ");
      Serial.println((char*)sdBuffer);
      currentFile.close();
      continue;
    }
    
    break;
  }
  
  sdReadFileName();
  Serial.print("[DEBUG] Open: ");
  Serial.println((char*)sdBuffer);
  
  sdSeekPosition = 0;
  sdBufferPosition = 0;
  sdRead();
}

static bool sdIsM25File() {
  sdReadFileName();
  char* extension = strrchr((char*)sdBuffer, '.');
  return extension && strcmp(extension, ".m25") == 0;
}

static void sdReadFileName() {
  if (!currentFile.getName((char*)sdBuffer, SD_BUFFER_SIZE)) {
    Serial.print("[ERROR] Can't read file name: ");
    sdBuffer[SD_BUFFER_SIZE - 1] = 0x00;
    Serial.println((char*)sdBuffer);

    while (true)
      delay(100);
  }
}

static bool sdRead() {
  currentFile.seek(sdSeekPosition);

  for (int16_t i = 0; i < SD_BUFFER_SIZE; i++)
    sdBuffer[i] = 0x00;

  return currentFile.read(sdBuffer, SD_BUFFER_SIZE) != 0;
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
