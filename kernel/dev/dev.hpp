#include <inttypes.h>

typedef enum deviceType {
    DEVICE_TYPE_SERIAL_PORT =   0x00,
    DEVICE_TYPE_HDD =           0x01,
    DEVICE_TYPE_GRAPHICS_CARD = 0x02,
    DEVICE_TYPE_SOUND_CARD =    0x03,
    DEVICE_TYPE_RTC =           0x04,
    DEVICE_TYPE_TIMER =         0x05
} deviceType;

extern char *deviceTypeNames[];

typedef enum deviceBusType {
    BUS_TYPE_IO = 0x00,
    BUS_TYPE_PCI = 0x01,
    BUS_TYPE_ATA = 0x02
} deviceBusType;

extern char *deviceBusTypeNames[];

typedef struct Device {
    char *name;
    deviceType type;

    deviceBusType busType;
    void *busData;

    void *driverData;
} Device;

void deviceAdd(Device device);
uint8_t deviceList(Device *list);