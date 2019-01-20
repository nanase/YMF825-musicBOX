extern int16_t sdPosition;
extern byte sdBuffer[SD_BUFFER_SIZE];

#define sdBufferByte() (sdBuffer[sdPosition])
#define sdBufferByteTo(x) (sdBuffer[sdPosition + x])
