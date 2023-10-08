#include <dev/dev.hpp>
#include <lib/string.hpp>

char *deviceTypeNames[] = {
    "Serial Port",
    "Hard Disk Drive",
    "Graphics Card",
    "Sound Card",
    "RTC",
    "Timer"
};

char *deviceBusTypeNames[] = {
    "IO",
    "PCI",
    "ATA"
};

Device devices[256];
uint8_t devicesIndex;

void deviceAdd(Device device)
{
    devices[devicesIndex++] = device;
}

uint8_t deviceList(Device *list)
{
    memcpy(list, devices, devicesIndex * sizeof(Device));

    return devicesIndex;
}