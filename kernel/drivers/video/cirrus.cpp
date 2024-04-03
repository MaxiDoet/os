#include <drivers/video/cirrus.hpp>
#include <drivers/video/vga.hpp>
#include <io/io.hpp>
#include <dev/dev.hpp>
#include <lib/string.hpp>
#include <lib/video/features.hpp>
#include <debug.hpp>

#define REG_AC_INDEX            0x3C0
#define REG_AC_DATA             0x3C1
#define REG_MISC                0x3C2
#define REG_SR_INDEX            0x3C4
#define REG_SR_DATA             0x3C5
#define REG_HIDDEN_DAC          0x3C6
#define REG_GR_INDEX            0x3CE
#define REG_GR_DATA             0x3CF
#define REG_CRTC_INDEX          0x3D4
#define REG_CRTC_DATA           0x3D5

/* SR registers */
#define REG_SR_SEQUENCER_MEMORY_MODE                        0x04
#define REG_SR_EXTENDED_SEQUENCER_MODE                      0x07

#define SR_EXTENDED_SEQUENCER_MODE_ENABLE_HIGH_RES          (1 << 0)
#define SR_EXTENDED_SEQUENCER_MODE_DEPTH_8BPP               0x0
#define SR_EXTENDED_SEQUENCER_MODE_DEPTH_24BPP              0x4
#define SR_EXTENDED_SEQUENCER_MODE_DEPTH_16BPP              0x6
#define SR_EXTENDED_SEQUENCER_MODE_DEPTH_32BPP              0x8
#define SR_EXTENDED_SEQUENCER_MODE_ENABLE_LFB               (1 << 4)

/* Hidden DAC registers */
#define HIDDEN_DAC_EXTENDED_MODE_565                        0x01
#define HIDDEN_DAC_EXTENDED_MODE_888                        0x05
#define HIDDEN_DAC_EXTENDED_MODE_332                        0x09
#define HIDDEN_DAC_ENABLE_ALL_EXTENDED_MODES                (1 << 6)
#define HIDDEN_DAC_ENABLE_555_MODE                          (1 << 7)

/* GR / BITBLT registers */
#define REG_BITBLT_BACKGROUND_COLOR_BYTE_0                  0x00
#define REG_BITBLT_FOREGROUND_COLOR_BYTE_0                  0x01
#define REG_GR_GRAPHICS_MODE                                0x05
#define REG_GR_MISC                                         0x06
#define REG_BITBLT_BACKGROUND_COLOR_BYTE_1                  0x10
#define REG_BITBLT_FOREGROUND_COLOR_BYTE_1                  0x11
#define REG_BITBLT_BACKGROUND_COLOR_BYTE_2                  0x12
#define REG_BITBLT_FOREGROUND_COLOR_BYTE_2                  0x13
#define REG_BITBLT_BACKGROUND_COLOR_BYTE_3                  0x14
#define REG_BITBLT_FOREGROUND_COLOR_BYTE_3                  0x15
#define REG_BITBLT_WIDTH_BYTE_0                             0x20
#define REG_BITBLT_WIDTH_BYTE_1                             0x21
#define REG_BITBLT_HEIGHT_BYTE_0                            0x22
#define REG_BITBLT_HEIGHT_BYTE_1                            0x23
#define REG_BITBLT_DESTINATION_PITCH_BYTE_0                 0x24
#define REG_BITBLT_DESTINATION_PITCH_BYTE_1                 0x25
#define REG_BITBLT_SOURCE_PITCH_BYTE_0                      0x26
#define REG_BITBLT_SOURCE_PITCH_BYTE_1                      0x27
#define REG_BITBLT_DESTINATION_START_BYTE_0                 0x28
#define REG_BITBLT_DESTINATION_START_BYTE_1                 0x29
#define REG_BITBLT_DESTINATION_START_BYTE_2                 0x2A
#define REG_BITBLT_SOURCE_START_BYTE_0                      0x2C
#define REG_BITBLT_SOURCE_START_BYTE_1                      0x2D
#define REG_BITBLT_SOURCE_START_BYTE_2                      0x2E
#define REG_BITBLT_BLT_MODE                                 0x30
#define REG_BITBLT_BLT_START_STATUS                         0x31
#define REG_BITBLT_ROP                                      0x32
#define REG_BITBLT_BLT_MODE_EXTENSIONS                      0x33

#define ROP_SCRCOPY                                         0x0D

#define GR_GRAPHICS_MODE_256_COLOR                          (1 << 6)

#define GR_MISC_GRAPHICS_MODE                               (1 << 0)

#define BITBLT_BLT_MODE_SYSTEM_MEMORY                       (1 << 2)
#define BITBLT_BLT_MODE_COLOR_EXPAND_WIDTH_8BBP             0x00
#define BITBLT_BLT_MODE_COLOR_EXPAND_WIDTH_16BPP            0x10
#define BITBLT_BLT_MODE_COLOR_EXPAND_WIDTH_24BPP            0x20
#define BITBLT_BLT_MODE_COLOR_EXPAND_WIDTH_32BPP            0x30
#define BITBLT_BLT_MODE_ENABLE_8X8_PATTERN                  (1 << 6)
#define BITBLT_BLT_MODE_ENABLE_COLOR_EXPAND                 (1 << 7)

#define BITBLT_BLT_START_STATUS_STATUS                      (1 << 0)
#define BITBLT_BLT_START_STATUS_START                       (1 << 1)
#define BITBLT_BLT_START_STATUS_RESET                       (1 << 2)

#define BITBLT_BLT_MODE_EXTENSIONS_ENABLE_SOLID_COLOR_FILL  (1 << 2)

/* CRTC registers */
#define REG_CRTC_HORIZONTAL_TOTAL                           0x00
#define REG_CRTC_HORIZONTAL_END                             0x01
#define REG_CRTC_HORIZONTAL_BLANKING_START                  0x02
#define REG_CRTC_HORIZONTAL_BLANKING_END                    0x03
#define REG_CRTC_HORIZONTAL_SYNC_START                      0x04
#define REG_CRTC_HORIZONTAL_SYNC_END                        0x05
#define REG_CRTC_VERTICAL_TOTAL                             0x06
#define REG_CRTC_OVERFLOW                                   0x07
#define REG_CRTC_CHARACTER_CELL_HEIGHT                      0x09
#define REG_CRTC_VERTICAL_SYNC_START                        0x10
#define REG_CRTC_VERTICAL_SYNC_END                          0x11
#define REG_CRTC_VERTICAL_END                               0x12
#define REG_CRTC_PITCH                                      0x13
#define REG_CRTC_VERTICAL_BLANKING_START                    0x15
#define REG_CRTC_VERTICAL_BLANKING_END                      0x16
#define REG_CRTC_MODE_CONTROL                               0x17
#define REG_CRTC_LINE_COMPARE                               0x18
#define REG_CRTC_EXTENDED_DISPLAY_CONTROLS                  0x1B

#define CRTC_MODE_CONTROL_BYTE_MODE                         (1 << 6)
#define CRTC_MODE_CONTROL_TIMING_ENABLE                     (1 << 7)

void CirrusDevice::acWrite(uint8_t index, uint8_t value)
{
    outb(this->mmio + REG_AC_INDEX, index);
    outb(this->mmio + REG_AC_DATA, value);
}

void CirrusDevice::srWrite(uint8_t index, uint8_t value)
{
    outb(this->mmio + REG_SR_INDEX, index);
    outb(this->mmio + REG_SR_DATA, value);
}

void CirrusDevice::grWrite(uint8_t index, uint8_t value)
{
    outb(this->mmio + REG_GR_INDEX, index);
    outb(this->mmio + REG_GR_DATA, value);
}

uint8_t CirrusDevice::grRead(uint8_t index)
{
    uint8_t result = 0;

    outb(this->mmio + REG_GR_INDEX, index);
    result = inb(this->mmio + REG_GR_DATA);

    return result;
}

void CirrusDevice::crtcWrite(uint8_t index, uint8_t value)
{
    outb(this->mmio + REG_CRTC_INDEX, index);
    outb(this->mmio + REG_CRTC_DATA, value);
}

void CirrusDevice::dacWrite(uint8_t value)
{
    inb(this->mmio + 0x3C6);
    inb(this->mmio + 0x3C6);
    inb(this->mmio + 0x3C6);
    inb(this->mmio + 0x3C6);

    outb(this->mmio + 0x3C6, value);
}

// Checks if bitblt engine is ready
bool CirrusDevice::isBitbltReady()
{
    uint8_t status;

    status = this->grRead(REG_BITBLT_BLT_START_STATUS);

    return !(status & BITBLT_BLT_START_STATUS_STATUS);
}

CirrusDevice::CirrusDevice(PciDevice *dev)
{
    this->dev = dev;
    this->mmio = 0xB4000000; // Don't ask!

    dev->enableBusMastering();
}

void CirrusDevice::setMode(vgaVideoMode videoMode)
{
    this->videoMode = videoMode;

    const uint8_t bytesPerPixel = videoMode.colorDepth / 8;
    const uint32_t pitch = bytesPerPixel * videoMode.width;

    crtcWrite(REG_CRTC_VERTICAL_SYNC_END, 0x00); // Allow writing to CR0-CR7
    crtcWrite(REG_CRTC_HORIZONTAL_TOTAL, 0xFF);
    crtcWrite(REG_CRTC_HORIZONTAL_END, videoMode.width / 8 - 1);
    crtcWrite(REG_CRTC_HORIZONTAL_BLANKING_START, 0xFF);
    crtcWrite(REG_CRTC_HORIZONTAL_BLANKING_END, 0xFF);
    crtcWrite(REG_CRTC_HORIZONTAL_SYNC_START, 0xFF);
    crtcWrite(REG_CRTC_HORIZONTAL_SYNC_END, 0xFF);
    crtcWrite(REG_CRTC_VERTICAL_TOTAL, 0xFF);
    crtcWrite(REG_CRTC_OVERFLOW, 0xF0);
    crtcWrite(REG_CRTC_VERTICAL_SYNC_START, 0x00);
    crtcWrite(REG_CRTC_VERTICAL_SYNC_END, 0x00);
    crtcWrite(REG_CRTC_VERTICAL_END, videoMode.height - 1);
    crtcWrite(REG_CRTC_PITCH, (pitch / 8) & 0xFF);
    crtcWrite(REG_CRTC_VERTICAL_BLANKING_START, 0xFF);
    crtcWrite(REG_CRTC_VERTICAL_BLANKING_END, 0x98);
    crtcWrite(REG_CRTC_LINE_COMPARE, 0x3F);

    if (!videoMode.text) {
        grWrite(REG_GR_GRAPHICS_MODE, GR_GRAPHICS_MODE_256_COLOR);
        grWrite(REG_GR_MISC, GR_MISC_GRAPHICS_MODE);

        srWrite(REG_SR_SEQUENCER_MEMORY_MODE, 0x00);

        crtcWrite(REG_CRTC_EXTENDED_DISPLAY_CONTROLS, (pitch >> 4) & 0x10);

        crtcWrite(REG_CRTC_MODE_CONTROL, CRTC_MODE_CONTROL_BYTE_MODE | CRTC_MODE_CONTROL_TIMING_ENABLE | 0x02 | 0x01);

        // Set color depth
        uint8_t srExtendedSequencerMode = SR_EXTENDED_SEQUENCER_MODE_ENABLE_HIGH_RES | SR_EXTENDED_SEQUENCER_MODE_ENABLE_LFB;
        uint8_t hiddenDacExtendedMode = HIDDEN_DAC_ENABLE_ALL_EXTENDED_MODES | HIDDEN_DAC_ENABLE_555_MODE;

        switch(videoMode.colorDepth) {
            case 8:
                srExtendedSequencerMode |= SR_EXTENDED_SEQUENCER_MODE_DEPTH_8BPP;
                hiddenDacExtendedMode |= HIDDEN_DAC_EXTENDED_MODE_332;
                break;

            case 16:
                srExtendedSequencerMode |= SR_EXTENDED_SEQUENCER_MODE_DEPTH_16BPP;
                hiddenDacExtendedMode |= HIDDEN_DAC_EXTENDED_MODE_565;
                break;

            case 24:
                srExtendedSequencerMode |= SR_EXTENDED_SEQUENCER_MODE_DEPTH_24BPP;
                hiddenDacExtendedMode |= HIDDEN_DAC_EXTENDED_MODE_888;
                break;

            case 32:
                srExtendedSequencerMode |= SR_EXTENDED_SEQUENCER_MODE_DEPTH_32BPP;
                hiddenDacExtendedMode |= HIDDEN_DAC_EXTENDED_MODE_888;
                break;

            default:
                srExtendedSequencerMode |= SR_EXTENDED_SEQUENCER_MODE_DEPTH_8BPP;
                hiddenDacExtendedMode |= HIDDEN_DAC_EXTENDED_MODE_332;
                break;
        }

        srWrite(REG_SR_EXTENDED_SEQUENCER_MODE, srExtendedSequencerMode);
        dacWrite(hiddenDacExtendedMode);

        this->fb = (void *) 0xFC000000;

        for (int i=0; i < videoMode.width * videoMode.height; i++) {
            ((uint8_t *) this->fb)[i] = 0x00;
        }
    }

    debugPrint("CIRRUS | %dx%d %dbit\n", videoMode.width, videoMode.height, videoMode.colorDepth);
}

/* Accelerated bitblt functions */

void CirrusDevice::drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    while (!this->isBitbltReady()) {}

    debugPrint("drawRectangle\n");

    const uint8_t bytesPerPixel = this->videoMode.colorDepth / 8;
    const uint32_t pitch = bytesPerPixel * this->videoMode.width;

    this->grWrite(REG_BITBLT_ROP, ROP_SCRCOPY);

    uint32_t localColor = convertToDepth(color, this->videoMode.colorDepth);
    this->grWrite(REG_BITBLT_FOREGROUND_COLOR_BYTE_0, localColor & 0xFF);
    if (this->videoMode.colorDepth >= 16) this->grWrite(REG_BITBLT_FOREGROUND_COLOR_BYTE_1, (localColor >> 8) & 0xFF);
    if (this->videoMode.colorDepth == 24) this->grWrite(REG_BITBLT_FOREGROUND_COLOR_BYTE_2, (localColor >> 16) & 0xFF);

    // Width
    this->grWrite(REG_BITBLT_WIDTH_BYTE_0, (width * bytesPerPixel - 1) & 0xFF);
    this->grWrite(REG_BITBLT_WIDTH_BYTE_1, ((width * bytesPerPixel - 1) >> 8) & 0x1F);

    // Height
    this->grWrite(REG_BITBLT_HEIGHT_BYTE_0, (height - 1) & 0xFF);
    this->grWrite(REG_BITBLT_HEIGHT_BYTE_1, ((height - 1) >> 8) & 0x07);

    // Destination pitch
    this->grWrite(REG_BITBLT_DESTINATION_PITCH_BYTE_0, pitch & 0xFF);
    this->grWrite(REG_BITBLT_DESTINATION_PITCH_BYTE_1, (pitch >> 8) & 0x1F);

    // Destination
    uint32_t dest = pitch * y + x * bytesPerPixel;
    this->grWrite(REG_BITBLT_DESTINATION_START_BYTE_0, dest & 0xFF);
    this->grWrite(REG_BITBLT_DESTINATION_START_BYTE_1, (dest >> 8) & 0xFF);
    this->grWrite(REG_BITBLT_DESTINATION_START_BYTE_2, (dest >> 16) & 0x3F);

    uint8_t bltMode = BITBLT_BLT_MODE_ENABLE_8X8_PATTERN | BITBLT_BLT_MODE_ENABLE_COLOR_EXPAND;

    switch(this->videoMode.colorDepth) {
        case 8: bltMode |= BITBLT_BLT_MODE_COLOR_EXPAND_WIDTH_8BBP; break;
        case 16: bltMode |= BITBLT_BLT_MODE_COLOR_EXPAND_WIDTH_16BPP; break;
        case 24: bltMode |= BITBLT_BLT_MODE_COLOR_EXPAND_WIDTH_24BPP; break;
    }

    this->grWrite(REG_BITBLT_BLT_MODE, bltMode);
    this->grWrite(REG_BITBLT_BLT_MODE_EXTENSIONS, BITBLT_BLT_MODE_EXTENSIONS_ENABLE_SOLID_COLOR_FILL);

    this->grWrite(REG_BITBLT_BLT_START_STATUS, BITBLT_BLT_START_STATUS_START);
}

void cirrusInit(CirrusDevice *dev, PciDevice *pciDev)
{
    
}

/*
void CirrusDevice::copyRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data)
{
    while (!isReady()) {}

    const uint8_t bytesPerPixel = videoMode.colorDepth / 8;
    const uint32_t pitch = videoMode.width * bytesPerPixel;

    grWrite(REG_BITBLT_BLT_START_STATUS, 0x00); // Disable autostart

    grWrite(REG_BITBLT_ROP, ROP_SCRCOPY);

    // Width
    grWrite(REG_BITBLT_WIDTH_BYTE_0, (width * bytesPerPixel - 1) & 0xFF);
    grWrite(REG_BITBLT_WIDTH_BYTE_1, ((width * bytesPerPixel - 1) >> 8) & 0x1F);

    // Height
    grWrite(REG_BITBLT_HEIGHT_BYTE_0, (height - 1) & 0xFF);
    grWrite(REG_BITBLT_HEIGHT_BYTE_1, ((height- 1) >> 8) & 0x07);

    // Destination pitch
    grWrite(REG_BITBLT_DESTINATION_PITCH_BYTE_0, pitch & 0xFF);
    grWrite(REG_BITBLT_DESTINATION_PITCH_BYTE_1, (pitch >> 8) & 0x1F);

    // Source pitch
    //grWrite(REG_BITBLT_SOURCE_PITCH_BYTE_0, pitch & 0xFF);
    //grWrite(REG_BITBLT_SOURCE_PITCH_BYTE_1, (pitch >> 8) & 0x1F);

    // Destination
    uint32_t dest = pitch * y + x * bytesPerPixel;
    grWrite(REG_BITBLT_DESTINATION_START_BYTE_0, dest & 0xFF);
    grWrite(REG_BITBLT_DESTINATION_START_BYTE_1, (dest >> 8) & 0xFF);
    grWrite(REG_BITBLT_DESTINATION_START_BYTE_2, (dest >> 16) & 0x3F);

    // Source
    grWrite(REG_BITBLT_SOURCE_START_BYTE_0, 0);
    grWrite(REG_BITBLT_SOURCE_START_BYTE_1, 0);
    grWrite(REG_BITBLT_SOURCE_START_BYTE_2, 0);

    grWrite(REG_BITBLT_BLT_MODE, BITBLT_BLT_MODE_SYSTEM_MEMORY | (1 << 3));
    grWrite(REG_BITBLT_BLT_MODE_EXTENSIONS, 0x00);

    grWrite(REG_BITBLT_BLT_START_STATUS, BITBLT_BLT_START_STATUS_START);

    memcpy((void *) (0x000A0000), data, pitch * height);
}
*/