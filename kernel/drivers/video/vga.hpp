#include <inttypes.h>

#ifndef _vga_hpp
#define _vga_hpp

typedef enum vgaColorDepth {
    VGA_COLOR_DEPTH_8 = 8,
    VGA_COLOR_DEPTH_16 = 16,
    VGA_COLOR_DEPTH_24 = 24,
} vgaColorDepth;

typedef struct vgaVideoMode {
    uint16_t width;
    uint16_t height;
    vgaColorDepth colorDepth;

    bool text;
} vgaVideoMode;

uint32_t convertToDepth(uint32_t color, vgaColorDepth depth);

#endif