#ifndef G729COMMON_H
#define G729COMMON_H

#include <stdint.h>
#include <QSharedPointer>

#define INPUTBUFFERLEN 80
#define OUTPUTBUFFERLEN 15
#define BITSTREAMLEN 10

typedef struct G729ENCODERDATA {
    int16_t     data[INPUTBUFFERLEN];
    unsigned    dataLen;
} G729EncoderData;

typedef struct G729DECODERDATA {
    uint8_t     data[BITSTREAMLEN];
    unsigned    dataLen;
} G729DecoderData;

typedef QSharedPointer<G729EncoderData> pG729EncoderData;
typedef QSharedPointer<G729DecoderData> pG729DecoderData;

extern void parametersBitStream2Array(uint8_t bitStream[], uint16_t parameters[]);
extern void parametersArray2BitStream(uint16_t parameters[], uint8_t bitStream[]);

#endif // G729COMMON_H
