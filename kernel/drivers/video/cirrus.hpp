#include <io/pci.hpp>
#include <drivers/video/vga.hpp>

typedef struct CirrusDevice {
    PciDevice *dev;
    uint32_t mmio;

    vgaVideoMode videoMode;
    void *fb;    
} CirrusDevice;

typedef struct CirrusRectangleDrawData {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint32_t color;
} CirrusRectangleDrawData;

void cirrusInit(CirrusDevice *dev, PciDevice *pciDev);