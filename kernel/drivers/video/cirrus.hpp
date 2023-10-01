#include <io/pci.hpp>
#include <drivers/video/vga.hpp>

class CirrusDevice {
    private:
        PciDevice *dev;
        uint32_t mmio;

        vgaVideoMode videoMode;
        void *fb;

        void acWrite(uint8_t index, uint8_t value);
        void srWrite(uint8_t index, uint8_t value);
        void grWrite(uint8_t index, uint8_t value);
        void crtcWrite(uint8_t index, uint8_t value);
        void dacWrite(uint8_t value);

    public:
        CirrusDevice(PciDevice *dev);

        void setMode(vgaVideoMode videoMode);
        void *getFramebuffer();

        /* Accelerated rendering functions */
        void drawAccelaratedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
};