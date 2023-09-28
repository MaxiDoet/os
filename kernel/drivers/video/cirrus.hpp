#include <io/pci.hpp>

class CirrusDevice {
    private:
        PciDevice *dev;

        uint32_t fb;
        uint32_t mmio;

        void acWrite(uint8_t index, uint8_t value);
        void sequencerWrite(uint8_t index, uint8_t value);
        void grWrite(uint8_t index, uint8_t value);
        void crtcWrite(uint8_t index, uint8_t value);

    public:
        CirrusDevice(PciDevice *dev);
};