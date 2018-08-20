#include "definitions.h"
#include <SD.h>

static File root;
static File currentFile;
byte sd_buffer[SD_BUFFER_SIZE];
static int32_t sd_position = 0;
static int32_t sd_buffer_position = 0;

bool sd_initialize() {
  if (!SD.begin(PIN_SD)) {
    Serial.println("initialization failed!");
    return false;
  }

  root = SD.open("/");
  return true;
}

void sd_seek_next() {
  if (currentFile.available()) {
     currentFile.close();
  }

  while (true) {
    currentFile = root.openNextFile();

    if (!currentFile) {
      root.rewindDirectory();
      continue;
    }

    if (!sd_is_m25_file())
      continue;
    
    break;
  }
  
  Serial.println(currentFile.name());
  
  sd_position = 0;
  sd_buffer_position = 0;
  sd_read();
}

static bool sd_is_m25_file() {
  char* extension = strrchr(currentFile.name(), '.');
  return extension && strcmp(extension, ".M25") == 0;
}

static bool sd_read() {
  currentFile.seek(sd_position);
  return currentFile.read(sd_buffer, SD_BUFFER_SIZE) != 0;
}

int16_t sd_read_buffer(int16_t size) {
  if (SD_BUFFER_SIZE - sd_buffer_position < size) {
    sd_position += sd_buffer_position;

    sd_buffer_position = size;

    return sd_read() ? 0 : EOF;
  }
  
  sd_buffer_position += size;
  return sd_buffer_position - size;
}
