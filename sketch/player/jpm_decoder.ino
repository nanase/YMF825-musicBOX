#include "jpm_decoder.h"
#include "readSdBuffer.h"

extern LiquidCrystal lcd;

bool JpmDecoder::progress() {
  if (!sdReadBuffer(1))
    return false;

  switch (sdBufferByte() & JPM_WOPX_MASK) {
  case JPM_WOPX_WRITE_WAIT:
    if (!JpmWriteOrWait())
      return false;
    break;

  case JPM_WOPX_LCD_OPERATE:
    if (!JpmLcdOperate())
      return false;
    break;

  case JPM_WOPX_BURSTWRITE_TONE:
    if (!JpmBurstwriteTone())
      return false;
    break;

  case JPM_WOPX_BURSTWRITE_EQ:
    if (!JpmBurstwriteEq())
      return false;
    break;
  }

  return false;
}

bool JpmDecoder::IsJpmFile(const char *filename) {
  char *extension = strrchr(filename, '.');

  if (!extension || strcmp(extension, ".JPM") != 0)
    return false;

  if (!sdReadBuffer(4))
    return false;

  return (sdBufferByteTo(0) == 'J' &&
          sdBufferByteTo(1) == 'P' &&
          sdBufferByteTo(2) == 'M' &&
          sdBufferByteTo(3) == 0); // Version
}

static bool JpmWriteOrWait() {
  byte selx   = sdBufferByte() & JPM_SELX_MASK;
  byte length = (sdBufferByte() & JPM_LENX_MASK) + 1;

  if (selx == JPM_SELX_WAIT) {
    waitAdd(length);
    return true;
  }

  ymf825ChangeTargetChip(JpmSelxToTargetChip(selx));

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

static bool JpmLcdOperate() {
  switch (sdBufferByte() & JPM_SELX_MASK) {
  case JPM_SELX_LCD_WRITE:
    return JpmLcdWrite();

  case JPM_SELX_LCD_CONTROL:
    return JpmLcdControl();

  case JPM_SELX_LCD_POSITION0:
    return JpmLcdSetPosition(0);

  case JPM_SELX_LCD_POSITION1:
    return JpmLcdSetPosition(1);

  default:
    // error
    return false;
  }
}

static bool JpmBurstwriteTone() {
  int16_t length = (sdBufferByte() & JPM_LENX_MASK) + 1;

  ymf825ChangeTargetChip(JpmSelxToTargetChip(sdBufferByte() & JPM_SELX_MASK));

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

static bool JpmBurstwriteEq() {
  ymf825ChangeTargetChip(JpmSelxToTargetChip(sdBufferByte() & JPM_SELX_MASK));

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

static bool JpmLcdWrite() {
  static char charBuffer[17];
  byte length = (sdBufferByte() & JPM_LENX_MASK) + 1;

  if (!sdReadBuffer(length))
    return false;

  memcpy(charBuffer, &sdBufferByte(), length);
  lcdPrint((const char *)&sdBufferByte());

  return true;
}

static bool JpmLcdControl() {
  switch (sdBufferByte() & JPM_LENX_MASK) {
  case JPM_LENX_LCD_CLEAR:
    lcd.clear();
    return true;

  case JPM_LENX_LCD_SET_BACKLIGHT:
    return JpmLcdSetBackLight();

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
    return JpmLcdCreateChar(sdBufferByte() & JPM_LENX_LCD_CREATE_CHAR_MASK);

  default:
    // error
    return false;
  }
}

static bool JpmLcdSetPosition(byte line) {
  lcd.setCursor(line, sdBufferByte() & JPM_LENX_MASK);
  return true;
}

static bool JpmLcdSetBackLight() {
  if (!sdReadBuffer(3))
    return false;

  lcdSetFadeColor(sdBufferByteTo(0),  // R
                  sdBufferByteTo(1),  // G
                  sdBufferByteTo(2)); // B

  return true;
}

static bool JpmLcdCreateChar(byte num) {
  static char cgBuffer[8];

  if (!sdReadBuffer(7))
    return false;

  memcpy(cgBuffer, &sdBufferByte(), 7);
  lcdCreateChar(num, sdBuffer);
  return true;
}
