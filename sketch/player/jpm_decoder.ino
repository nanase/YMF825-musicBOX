#include "jpm_decoder.h"

extern int16_t sdPosition;
extern byte sdBuffer[SD_BUFFER_SIZE];
extern LiquidCrystal lcd;

bool JpmDecoder::progress() {
  sdReadBuffer(1);

  if (sdPosition == EOF)
    return false;

  switch (sdBuffer[sdPosition] & JPM_WOPX_MASK) {
  case JPM_WOPX_WRITE_WAIT:
    if (!writeOrWait())
      return false;
    break;

  case JPM_WOPX_LCD_OPERATE:
    if (!lcdControl())
      return false;
    break;

  case JPM_WOPX_BURSTWRITE_TONE:
    if (!burstwriteTone())
      return false;
    break;

  case JPM_WOPX_BURSTWRITE_EQ:
    if (!burstwriteEq())
      return false;
    break;
  }

  return false;
}

bool JpmDecoder::IsJpmFile(const char *filename) {
  char *extension = strrchr(filename, '.');

  if (extension && strcmp(extension, ".JPM") != 0)
    return false;

  sdReadBuffer(4);

  if (sdPosition == EOF)
    return false;

  return (sdBuffer[0] == 'j' &&
          sdBuffer[1] == 'p' &&
          sdBuffer[2] == 'm' &&
          sdBuffer[3] == 0); // Version
}

static bool writeOrWait() {
  byte selx   = sdBuffer[sdPosition] & JPM_SELX_MASK;
  byte length = (sdBuffer[sdPosition] & JPM_LENX_MASK) + 1;

  if (selx == JPM_SELX_WAIT) {
    waitAdd(length);
    return true;
  }

  ymf825ChangeTargetChip(JpmSelxToTargetChip(selx));
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

static bool lcdOperate() {
  switch (sdBuffer[sdPosition] & JPM_SELX_MASK) {
  case JPM_SELX_LCD_WRITE:
    return lcdWrite();

  case JPM_SELX_LCD_CONTROL:
    return lcdOperate();

  case JPM_SELX_LCD_POSITION0:
    return lcdSetPosition(0);

  case JPM_SELX_LCD_POSITION1:
    return lcdSetPosition(1);
  }
}

static bool burstwriteTone() {
  int16_t length = sdBuffer[sdPosition] & JPM_LENX_MASK;

  ymf825ChangeTargetChip(JpmSelxToTargetChip(sdBuffer[sdPosition] & JPM_SELX_MASK));
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
  ymf825ChangeTargetChip(JpmSelxToTargetChip(sdBuffer[sdPosition] & JPM_SELX_MASK));
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

static bool lcdWrite() {
  static char charBuffer[17];
  byte length = (sdBuffer[sdPosition] & JPM_LENX_MASK) + 1;
  sdReadBuffer(length);

  if (sdPosition == EOF)
    return false;

  memcpy(charBuffer, &sdBuffer[sdPosition], length);
  lcd.print((const char *)&sdBuffer[sdPosition]);

  return true;
}

static bool lcdControl() {
  switch (sdBuffer[sdPosition] & JPM_LENX_MASK) {
  case JPM_LENX_LCD_CLEAR:
    lcd.clear();
    return true;

  case JPM_LENX_LCD_SET_BACKLIGHT:
    return lcdSetBackLight();

  case JPM_LENX_LCD_SCROLL_LEFT:
    lcd.scrollDisplayLeft();
    return true;

  case JPM_LENX_LCD_SCROLL_RIGHT:
    lcd.scrollDisplayRight();
    return true;

  case JPM_LENX_LCD_HIDE_CURSOR:
    lcd.noCursor();
    return true;

  case JPM_LENX_LCD_SHOW_CURSOR:
    lcd.cursor();
    return true;

  case JPM_LENX_LCD_NO_BLINK:
    lcd.noBlink();
    return true;

  case JPM_LENX_LCD_BLINK:
    lcd.blink();
    return true;

  case JPM_LENX_LCD_CREATE_CHAR0:
  case JPM_LENX_LCD_CREATE_CHAR1:
  case JPM_LENX_LCD_CREATE_CHAR2:
  case JPM_LENX_LCD_CREATE_CHAR3:
  case JPM_LENX_LCD_CREATE_CHAR4:
  case JPM_LENX_LCD_CREATE_CHAR5:
  case JPM_LENX_LCD_CREATE_CHAR6:
  case JPM_LENX_LCD_CREATE_CHAR7:
    return lcdCreateChar(sdBuffer[sdPosition] & JPM_LENX_LCD_CREATE_CHAR_MASK);

  default:
    // error
    return false;
  }
}

static bool lcdSetPosition(byte line) {
  lcd.setCursor(line, sdBuffer[sdPosition] & JPM_LENX_MASK);
  return true;
}

static bool lcdSetBackLight() {
  sdReadBuffer(3);

  if (sdPosition == EOF)
    return false;

  lcdSetFadeColor(sdBuffer[sdPosition + 0],  // R
                  sdBuffer[sdPosition + 1],  // G
                  sdBuffer[sdPosition + 2]); // B

  return true;
}

static bool lcdCreateChar(byte num) {
  static char cgBuffer[8];
  sdReadBuffer(7);

  if (sdPosition == EOF)
    return false;

  memcpy(cgBuffer, &sdBuffer[sdPosition], 7);
  lcd.createChar(num, sdBuffer);
  return true;
}
