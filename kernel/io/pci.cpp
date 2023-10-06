#include <io/pci.hpp>
#include <io/io.hpp>
#include <mem/heap.hpp>
#include <lib/string.hpp>
#include <debug.hpp>

static PciDevice *pciDevices;
uint8_t pciDevicesIndex = 0;

char *classNames[] = {
    "",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge",
    "Communication Controller",
    "System Component",
    "Input Device Controller",
    "Docking Station",
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller"
};

void pciScan()
{
    pciDevices = (PciDevice *) heapAlloc(sizeof(PciDevice) * 256);
    pciDevicesIndex = 0;

    for (int bus = 0; bus < 256; bus++) {
        for (int slot = 0; slot < 32; slot++) {
            for (int function = 0; function < 8; function++) {
                PciDevice pciDevice(bus, slot, function);

                if (pciDevice.detect()) {
                    pciDevices[pciDevicesIndex++] = pciDevice;
                }
            }
        }
    }

    debugPrint("PCI | Scan resulted in %d devices\n", pciDevicesIndex);
}

uint8_t pciGetList(PciDevice *list)
{
    memcpy(list, pciDevices, pciDevicesIndex * sizeof(PciDevice));
    
    return pciDevicesIndex;
}

uint8_t pciCount(uint16_t vendorId, uint16_t deviceId)
{
    uint8_t count = 0;

    for (uint8_t i=0; i < pciDevicesIndex; i++) {        
        if (pciDevices[i].getVendorId() == vendorId && pciDevices[i].getDeviceId() == deviceId) {    
            count++;
        }
    }

    return count;
}

uint8_t pciFind(PciDevice *list, uint16_t vendorId, uint16_t deviceId)
{
    uint8_t count = 0;

    for (uint8_t i=0; i < pciDevicesIndex; i++) {
        if (pciDevices[i].getVendorId() == vendorId && pciDevices[i].getDeviceId() == deviceId) {        
            memcpy(&list[count++], &pciDevices[i], sizeof(PciDevice));
        }

        
    }

    return count;
}

void pciInfo(PciDevice *dev)
{
    debugPrint("PCI | %d/%d/%d | %x %x | %s\n", dev->getBus(), dev->getSlot(), dev->getFunction(), dev->getVendorId(), dev->getDeviceId(), classNames[dev->getClassCode()]);
}