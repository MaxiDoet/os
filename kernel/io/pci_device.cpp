#include <io/pci.hpp>
#include <io/io.hpp>
#include <debug.hpp>

#define COMMAND_BUS_MASTER  (1 << 2)

#define CONFIG_ADDRESS      0xCF8
#define CONFIG_DATA         0xCFC

uint16_t PciDevice::readConfigWord(uint8_t offset)
{
    uint32_t address = 0;

    address |= (offset & 0xFC);
    address |= (function & 0x7) << 8;
    address |= (slot & 0x1F) << 11;
    address |= bus << 16;
    address |= (1 << 31); // Enable bit

    outl(CONFIG_ADDRESS, address);

    uint16_t data = inl(CONFIG_DATA) >> ((offset & 3) * 8);

    return data;
}

uint32_t PciDevice::readConfigDword(uint8_t offset)
{
    uint32_t address = 0;

    address |= (offset & 0xFC);
    address |= (function & 0x7) << 8;
    address |= (slot & 0x1F) << 11;
    address |= bus << 16;
    address |= (1 << 31); // Enable bit

    outl(CONFIG_ADDRESS, address);

    uint32_t data = inl(CONFIG_DATA);

    return data;
}

void PciDevice::writeConfigWord(uint8_t offset, uint16_t value)
{
    uint32_t address = 0;

    address |= (offset & 0xFC);
    address |= (function & 0x7) << 8;
    address |= (slot & 0x1F) << 11;
    address |= bus << 16;
    address |= (1 << 31); // Enable bit

    outl(CONFIG_ADDRESS, address);
    outl(CONFIG_DATA, value);
}

void PciDevice::writeConfigDword(uint8_t offset, uint32_t value)
{
    uint32_t address = 0;

    address |= (offset & 0xFC);
    address |= (function & 0x7) << 8;
    address |= (slot & 0x1F) << 11;
    address |= bus << 16;
    address |= (1 << 31); // Enable bit

    outl(CONFIG_ADDRESS, address);
    outl(CONFIG_DATA, value);
}

PciDevice::PciDevice(uint8_t bus, uint8_t slot, uint8_t function)
{
    this->bus = bus;
    this->slot = slot;
    this->function = function;
}

bool PciDevice::detect()
{
    uint16_t vendorId = readConfigDword(0x00) & 0x0000FFFF;

    if (vendorId == 0xFFFF) {
        return false;
    }

    this->vendorId = vendorId;
    deviceId = (readConfigDword(0x00) & 0xFFFF0000) >> 16;

    this->classCode = (readConfigDword(0x08) & 0xFF000000) >> 24;
    this->subclassCode = (readConfigDword(0x08) & 0x00FF0000) >> 16;

    irq = readConfigWord(0x3C);

    // Detect BARs
    for (int i=0; i < 6; i++) {
        uint32_t bar = readConfigDword(0x10 + i * 0x04);

        if (bar & (1 << 0)) {
            // IO mapped bar
            bars[i].ioBase = (bar & 0xFFFFFFFC);
        } else {
            // Memory mapped bar
            bars[i].memBase = (bar & 0xFFFFFFF0);
        }
    }

    return true;
}

uint8_t PciDevice::getBus()
{
    return bus;
}

uint8_t PciDevice::getSlot()
{
    return slot;
}

uint8_t PciDevice::getFunction()
{
    return function;
}

uint16_t PciDevice::getVendorId()
{
    return vendorId;
}

uint16_t PciDevice::getDeviceId()
{
    return deviceId;
}

uint8_t PciDevice::getClassCode()
{
    return classCode;
}

uint8_t PciDevice::getSubclassCode()
{
    return subclassCode;
}

uint8_t PciDevice::getIrq()
{
    return irq;
}

PciDeviceBar PciDevice::getBar(uint8_t bar)
{
    return bars[bar];
}

void PciDevice::enableBusMastering()
{
    uint16_t command = readConfigWord(0x04);

    command |= COMMAND_BUS_MASTER;

    writeConfigWord(0x04, command);
}

void PciDevice::disableBusMastering()
{
    uint16_t command = readConfigWord(0x04);

    command &= ~COMMAND_BUS_MASTER;

    writeConfigWord(0x04, command);
}