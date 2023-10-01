#include <drivers/video/vga.hpp>

/* This converts 24bit color to the selected depth */
uint32_t convertToDepth(uint32_t color, vgaColorDepth depth)
{
    uint32_t result = 0;

    switch(depth) {
        case 24:
            result = color;
            break;

        case 16:
            break;

        case 8:
            break;
    }

    return result;
}