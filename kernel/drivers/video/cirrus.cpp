#include <drivers/video/cirrus.hpp>
#include <io/io.hpp>
#include <debug.hpp>

#define REG_AC_INDEX            0x3C0
#define REG_AC_DATA             0x3C1
#define REG_MISC                0x3C2
#define REG_SEQ_INDEX           0x3C4
#define REG_SEQ_DATA            0x3C5
#define REG_GC_INDEX            0x3CE
#define REG_GC_DATA             0x3CF
#define REG_CRTC_INDEX          0x3D4
#define REG_CRTC_DATA           0x3D5

#define REG_CRTC_HORIZONTAL_TOTAL           0x00
#define REG_CRTC_HORIZONTAL_END             0x01
#define REG_CRTC_HORIZONTAL_BLANKING_START  0x02
#define REG_CRTC_HORIZONTAL_BLANKING_END    0x03
#define REG_CRTC_HORIZONTAL_SYNC_START      0x04
#define REG_CRTC_HORIZONTAL_SYNC_END        0x05
#define REG_CRTC_VERTICAL_TOTAL             0x06
#define REG_CRTC_OVERFLOW                   0x07
#define REG_CRTC_CHARACTER_CELL_HEIGHT      0x09
#define REG_CRTC_VERTICAL_SYNC_START        0x10
#define REG_CRTC_VERTICAL_SYNC_END          0x11
#define REG_CRTC_VERTICAL_END               0x12
#define REG_CRTC_PITCH                      0x13
#define REG_CRTC_VERTICAL_BLANKING_START    0x15
#define REG_CRTC_VERTICAL_BLANKING_END      0x16
#define REG_CRTC_LINE_COMPARE               0x18

void CirrusDevice::acWrite(uint8_t index, uint8_t value)
{
    outb(mmio + REG_AC_INDEX, index);
    outb(mmio + REG_AC_DATA, value);
}

void CirrusDevice::sequencerWrite(uint8_t index, uint8_t value)
{
    outb(mmio + REG_SEQ_INDEX, index);
    outb(mmio + REG_SEQ_DATA, value);
}

void CirrusDevice::grWrite(uint8_t index, uint8_t value)
{
    outb(mmio + REG_GC_INDEX, index);
    outb(mmio + REG_GC_DATA, value);
}

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

    // Allow writing to CR0-CR7
    crtcWrite(REG_CRTC_VERTICAL_SYNC_END, 0x00);

    // Set VGA registers
    crtcWrite(REG_CRTC_HORIZONTAL_TOTAL, 0x5F);
    crtcWrite(REG_CRTC_HORIZONTAL_END, 0x4F);
    crtcWrite(REG_CRTC_HORIZONTAL_BLANKING_START, 0x50);
    crtcWrite(REG_CRTC_HORIZONTAL_BLANKING_END, 0x82);
    crtcWrite(REG_CRTC_HORIZONTAL_SYNC_START, 0x54);
    crtcWrite(REG_CRTC_HORIZONTAL_SYNC_END, 0x80);
    crtcWrite(REG_CRTC_VERTICAL_TOTAL, 0x0B);
    crtcWrite(REG_CRTC_OVERFLOW, 0x3E);
    crtcWrite(REG_CRTC_CHARACTER_CELL_HEIGHT, 0x40);
    crtcWrite(REG_CRTC_VERTICAL_SYNC_START, 0xEA);
    crtcWrite(REG_CRTC_VERTICAL_SYNC_END, 0x8C);
    crtcWrite(REG_CRTC_VERTICAL_END, 0xDF);
    crtcWrite(REG_CRTC_PITCH, 0x28);
    crtcWrite(REG_CRTC_VERTICAL_BLANKING_START, 0xE7);
    crtcWrite(REG_CRTC_VERTICAL_BLANKING_END, 0x04);

    fb = dev->getBar(1).memBase;

    uint32_t *framebuffer = (uint32_t *) fb;

    debugPrint("CIRRUS | Framebuffer: %x\n", fb);

    /*
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
    */

    debugPrint("CIRRUS | Init done\n");
}