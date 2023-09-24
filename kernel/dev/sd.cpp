#include <dev/sd.hpp>
#include <drivers/ata.hpp>

StorageDevice::StorageDevice(void *privateData, StorageDeviceBusType busType, char *identifier)
{
    this->privateData = privateData;
    this->busType = busType;
    this->identifier = identifier;
}

char *StorageDevice::getIdentifier()
{
    return identifier;
}

void StorageDevice::read(uint32_t lba, uint8_t sectorCount, uint8_t *data)
{
    switch(busType) {
        case STORAGE_DEVICE_BUS_TYPE_ATA:
            AtaDevice *ataDevice = (AtaDevice *) privateData;
            ataDevice->read(lba, sectorCount, data);

            break;
    }
}