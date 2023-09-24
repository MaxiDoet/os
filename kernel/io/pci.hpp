#include <inttypes.h>

#ifndef _pci_hpp
#define _pci_hpp

typedef struct PciDeviceBar {
    uint32_t ioBase;
    uint32_t memBase; 
} PciDeviceBar;

class PciDevice {
    private:
        uint8_t bus;
        uint8_t slot;
        uint8_t function;

        uint16_t vendorId;
        uint16_t deviceId;

        uint8_t classCode;
        uint8_t subclassCode;

        uint8_t irq;

        PciDeviceBar bars[6];

        uint16_t readConfigWord(uint8_t offset);
        uint32_t readConfigDword(uint8_t offset);
        void writeConfigWord(uint8_t offset, uint16_t value);
        void writeConfigDword(uint8_t offset, uint32_t value);

    public:
        PciDevice(uint8_t bus, uint8_t slot, uint8_t function);

        bool detect();

        uint8_t getBus();
        uint8_t getSlot();
        uint8_t getFunction();
        uint16_t getVendorId();
        uint16_t getDeviceId();
        uint8_t getClassCode();
        uint8_t getSubclassCode();
        uint8_t getIrq();

        PciDeviceBar getBar(uint8_t bar);

        /* Commands */
        void enableBusMastering();
        void disableBusMastering();
};

void pciScan();
uint8_t pciGetList(PciDevice *list);
uint8_t pciCount(uint16_t vendorId, uint16_t deviceId);
uint8_t pciFind(PciDevice *list, uint16_t vendorId, uint16_t deviceId);
void pciInfo(PciDevice *dev);

#endif
