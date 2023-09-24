#include <inttypes.h>

#ifndef sd_hpp
#define sd_hpp

enum StorageDeviceBusType {
    STORAGE_DEVICE_BUS_TYPE_ATA = 0
};

class StorageDevice {
    private:
        void *privateData;
        StorageDeviceBusType busType;
        char *identifier;

    public:
        StorageDevice(void *privateData, StorageDeviceBusType busType, char *identifier);

        char *getIdentifier();

        void read(uint32_t lba, uint8_t sectorCount, uint8_t *data);
};

#endif