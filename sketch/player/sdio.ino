#include "definitions.h"
#include "SdFat.h"
SdFat SD;

static File root;
static File currentFile;
byte sd_buffer[SD_BUFFER_SIZE];
static int32_t sd_position = 0;
static int32_t sd_buffer_position = 0;
char filename[SD_FILE_NAME_LENGTH];

bool sd_initialize() {
  Serial.println("[DEBUG] SD initialization");

  if (!SD.begin(PIN_SD)) {
    Serial.println("[ERROR] SD initialization failed");
    return false;
  }

  root = SD.open("/");
  return true;
}

void sd_seek_next() {
  if (currentFile.available()) {
    Serial.print("[DEBUG] Close: ");
    Serial.println(sd_filename());
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

    if (!sd_is_m25_file()) {
      Serial.print("[DEBUG] Not m25 format: ");
      Serial.println(sd_filename());
      currentFile.close();
      continue;
    }
    
    break;
  }
  
  Serial.print("[DEBUG] Open: ");
  Serial.println(sd_filename());
  
  sd_position = 0;
  sd_buffer_position = 0;
  sd_read();
}

static bool sd_is_m25_file() {
  char* extension = strrchr(sd_filename(), '.');
  return extension && strcmp(extension, ".m25") == 0;
}

static char* sd_filename() {
  if (!currentFile.getName(filename, SD_FILE_NAME_LENGTH)) {
    Serial.print("[ERROR] Can't read file name: ");
    filename[SD_FILE_NAME_LENGTH - 1] = 0x00;
    Serial.println(filename);

    while (true)
      delay(100);
  }

  return filename;
}

static bool sd_read() {
  currentFile.seek(sd_position);

  for (int16_t i = 0; i < SD_BUFFER_SIZE; i++)
    sd_buffer[i] = 0x00;

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
