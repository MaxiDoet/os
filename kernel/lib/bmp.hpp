#ifndef _bmp_hpp
#define _bmp_hpp

#include <inttypes.h>

typedef struct BmpHeader {
    char signature[2];
    uint32_t size;
    uint8_t reserved[4];
    uint32_t dataOffset;
} __attribute__((packed)) BmpHeader;

#endif