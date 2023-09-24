#include <drivers/video/cirrus.hpp>
#include <io/io.hpp>
#include <debug.hpp>

#define REG_CRTC_INDEX  0x3D4
#define REG_CRTC_DATA   0x3D5

void CirrusDevice::crtcWrite(uint8_t index, uint8_t value)
{
    outb(dev->getBar(1).ioBase + REG_CRTC_INDEX, index & 0x3F);
    outb(dev->getBar(1).ioBase + REG_CRTC_DATA, value);

    //uint32_t addr = 0xfebf0000;
    //mmioWriteByte(addr + REG_CRTC_INDEX, index & 0x3F);
    //mmioWriteByte(addr + REG_CRTC_DATA, value);
}

CirrusDevice::CirrusDevice(PciDevice *dev)
{
    this->dev = dev;

    debugPrint("CIRRUS | Init\n");

    crtcWrite(0x19, 10);
}