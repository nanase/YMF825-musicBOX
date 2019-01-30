const byte M25_SELX_MASK            = B11000000;
const byte M25_SELX_WAIT            = B00000000;
const byte M25_SELX_LCH_ENABLE      = B01000000;
const byte M25_SELX_RCH_ENABLE      = B10000000;
const byte M25_SELX_LR_ENABLE       = B11000000;
const byte M25_WOPX_MASK            = B00110000;
const byte M25_WOPX_WRITE_DA_DD     = B00000000;
const byte M25_WOPX_RESERVED        = B00010000;
const byte M25_WOPX_BURSTWRITE_TONE = B00100000;
const byte M25_WOPX_BURSTWRITE_EQ   = B00110000;
const byte M25_LENX_MASK            = B00001111;

#define M25SelxToTargetChip(x) ((x) >> 6)
