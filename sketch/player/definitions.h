#pragma once

// Software definitions

const byte WAIT_RESOLUTION = 10;

const byte YMF825_LCH_ENABLE  = B00000001;
const byte YMF825_RCH_ENABLE  = B00000010;
const byte YMF825_BOTH_ENABLE = B00000011;

// Sketch definitions

#define EOF (-1)
#define _USE_PETIT_SERIAL

// -------------------------------------------------------------------------- //

#ifdef _USE_PETIT_SERIAL
#include "PetitSerial.h"
PetitSerial PSerial;
#else
#define PSerial Serial
#endif

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
