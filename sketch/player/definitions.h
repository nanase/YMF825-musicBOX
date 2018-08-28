#pragma once

// Hardware definitions

const byte PIN_SD                 = 5;

#define PORT_SS                     PORTD
#define PORT_SS_DDR_R               DDRD
const byte PORT_SS_DDR_V          = B11000000;
const byte BIT_SS_LCH             = B10000000;
const byte BIT_SS_RCH             = B01000000;
const byte BIT_SS_LR              = BIT_SS_LCH | BIT_SS_RCH;

#define PORT_IC                     PORTB
#define PORT_IC_DDR_R               DDRB
const byte PORT_IC_DDR_V          = B00000001;
const byte BIT_IC                 = B00000001;

const byte PIN_BT_PLAY            = 2;
const byte PIN_BT_NEXT            = 3;
const byte MODE_BT_PLAY           = INPUT_PULLUP;
const byte MODE_BT_NEXT           = INPUT_PULLUP;

// Software definitions

const byte WAIT_RESOLUTION        = 10;

const byte SELX_MASK              = B11000000;
const byte SELX_WAIT              = B00000000;
const byte SELX_LCH_ENABLE        = B01000000;
const byte SELX_RCH_ENABLE        = B10000000;
const byte SELX_LR_ENABLE         = B11000000;

const byte WOPX_MASK              = B00110000;
const byte WOPX_WRITE_DA_DD       = B00000000;
const byte WOPX_WRITE_SA_DD       = B00010000;
const byte WOPX_BURSTWRITE_TONE   = B00100000;
const byte WOPX_BURSTWRITE_EQ     = B00110000;

const byte LENX_MASK              = B00001111;


// Sketch definitions

#define EOF                         (-1)

// min: 486, recommanded max: 720
const int32_t SD_BUFFER_SIZE      = 486;
const size_t SD_FILE_NAME_LENGTH  = 32;
