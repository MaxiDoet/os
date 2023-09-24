#include <drivers/video/cirrus.hpp>
#include <io/io.hpp>
#include <debug.hpp>

#define REG_CRTC_INDEX  0x3D4
#define REG_CRTC_DATA   0x3D5

void CirrusDevice::crtcWrite(uint8_t index, uint8_t value)
{
    outb(mmio + REG_CRTC_INDEX, index & 0x3F);
    outb(mmio + REG_CRTC_DATA, value);
}

CirrusDevice::CirrusDevice(PciDevice *dev)
{
    this->dev = dev;
    this->mmio = 0xB4000000; // Don't ask!

    this->dev->enableBusMastering();

    fb = dev->getBar(1).memBase;

    uint8_t *framebuffer = (uint8_t *) fb;

    *framebuffer = 0x2;

    debugPrint("CIRRUS | Framebuffer: %x\n", fb);

    crtcWrite(0x07, 0x15);
    crtcWrite(0x0b, 0x23);
    crtcWrite(0x0c, 0x23);
    crtcWrite(0x0d, 0x23);
    crtcWrite(0x0e, 0x23);
    crtcWrite(0x13, 0x00);
    crtcWrite(0x1b, 0x14);
    crtcWrite(0x1c, 0x14);
    crtcWrite(0x1d, 0x14);
    crtcWrite(0x1e, 0x14);
    crtcWrite(0x1a, 0x00);
    crtcWrite(0x1b, 0x32);
    crtcWrite(0x1d, 0x00);
    crtcWrite(0x1b, 0x32);

    debugPrint("CIRRUS | Init done\n");
}