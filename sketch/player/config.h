#pragma once

// -------------------------------------------------------------------------- //
// Configurations

// Hardware definitions

const byte PIN_SD = 19;

#define PORT_SS PORTC
#define PORT_SS_DDR_R DDRC
const byte PORT_SS_DDR_V = B00111111;
const byte BIT_SS_LCH    = B00001000;
const byte BIT_SS_RCH    = B00010000;
const byte BIT_SS_LR     = BIT_SS_LCH | BIT_SS_RCH;

const byte PIN_BT_PLAY = 2;
const byte PIN_BT_NEXT = 3;

const byte LCD_PIN_R = 9;
const byte LCD_PIN_G = 6;
const byte LCD_PIN_B = 5;

const byte LCD_PIN_RS = 8;
const byte LCD_PIN_E  = 7;
const byte LCD_PIN_D4 = 4;
const byte LCD_PIN_D5 = A2;
const byte LCD_PIN_D6 = A1;
const byte LCD_PIN_D7 = A0;

// min: 486
const int32_t SD_BUFFER_SIZE = 1024;

const char VERSION_STRING[] = "ver.2.0b";

// #define DEBUG_MODE
#define HALT_ON_ERROR
