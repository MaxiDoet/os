#include <io/pci.hpp>

class CirrusDevice {
    private:
        PciDevice *dev;

        void crtcWrite(uint8_t index, uint8_t value);

    public:
        CirrusDevice(PciDevice *dev);
};