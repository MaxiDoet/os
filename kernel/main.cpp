#include <inttypes.h>

#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <cpu/irq.hpp>
#include <drivers/serial.hpp>
#include <drivers/screen.hpp>
#include <drivers/rtc.hpp>
#include <drivers/pit.hpp>
#include <drivers/ata.hpp>
#include <drivers/sound/ac97.hpp>
#include <drivers/input/mouse.hpp>
#include <drivers/video/vga.hpp>
#include <drivers/video/cirrus.hpp>
#include <dev/dev.hpp>
#include <dev/sd.hpp>
#include <dev/object.hpp>
#include <fs/fs.hpp>
#include <mem/heap.hpp>
#include <io/pci.hpp>
#include <lib/string.hpp>
#include <debug.hpp>

#include <lib/bmp.hpp>

PciDevice *videoDev;

void desktopLoop()
{
    CirrusDevice cirrusDev;
    cirrusInit(&cirrusDev, videoDev);

    vgaVideoMode videoMode = {
        .width = 800,
        .height = 600,
        .colorDepth = VGA_COLOR_DEPTH_24,
        .text = false
    };

    nodeCall(nodeQuery("/video/0/setMode"), &videoMode, nullptr);

    CirrusRectangleDrawData rectangleData = {
        .x = 0,
        .y = 0,
        .width = 800,
        .height = 600,
        .color = 0x575757
    };

    nodeCall(nodeQuery("/video/0/drawRectangle"), &rectangleData, nullptr);
}

void printDevTree()
{
    Device *list = (Device *) heapAlloc(256 * sizeof(Device));

    uint8_t deviceCount = deviceList(list);

    for (int i=0; i < deviceCount; i++) {
        Device *device = &list[i];

        debugPrint("%s | %s | %s\n", device->name, deviceTypeNames[device->type], deviceBusTypeNames[device->busType]);
    }
}

void debugIrqHandler(void *privateData)
{
    SerialPort *serialPort = (SerialPort *) privateData;

    char c = serialPort->readChar();

    switch(c) {
        case 'd':
            printDevTree();
            break;
    }
}

void initPciDevices()
{
    PciDevice *list = (PciDevice *) heapAlloc(sizeof(PciDevice) * 256);
    uint8_t listLength = pciGetList(list);

    // Print out all pci devices
    for (uint8_t i=0; i < listLength; i++) {
        pciInfo(&list[i]);
    }

    for (uint8_t i=0; i < listLength; i++) {
        PciDevice *device = &list[i];

        // Cirrus
        if (device->getVendorId() == 0x1013 && device->getDeviceId() == 0x00B8) {
            videoDev = &list[i];

            //uint8_t *backgroundBuffer = (uint8_t *) heapAlloc(480056);
            //fsRead("/background.bmp", backgroundBuffer);
            //BmpHeader *bmpHeader = (BmpHeader *) backgroundBuffer;
            //cirrusDev.copyRectangle(0, 0, 400, 400, backgroundBuffer + bmpHeader->dataOffset);
        }

        // AC97
        if (device->getVendorId() == 0x8086 && device->getDeviceId() == 0x2415) {
            AC97Device ac97Dev(&list[i]);

            //uint8_t *audioBuffer = (uint8_t *) heapAlloc(25548514);
            //fsRead("/audio.wav", audioBuffer);

            //ac97Dev.play(audioBuffer, 25548514);
        }
    }

    heapFree(list);
}

extern "C" void kmain()
{
    // Init serial port
    SerialPort com0(0x3F8);

    screenClear();

    // Init GDT
    gdtSetup();

    // Init IDT
    idtInstall();

    // Init heap
    uint32_t heapStart = 0x1FC00 + 10*1024*1024;
	uint32_t heapSize = 100000000;
    heapInit(heapStart, heapSize);

    /* Init Platform devices */

    // Init debugging over serial port
    debugInit(com0);
    irqInstallHandler(4, debugIrqHandler, &com0);

    nodeRegister("/video", nullptr, nullptr);

    Rtc::init();
    Time time = Rtc::read();
    debugPrint("Booting up on %d/%d/%d %d:%d:%d\n", time.date_month, time.date_day, time.date_year, time.hours, time.minutes, time.seconds);

    Pit::init();

    // Init ATA drives
    AtaDevice ata0(true, true);
    AtaDevice ata1(true, false);
    AtaDevice ata2(false, true);
    AtaDevice ata3(false, false);

    StorageDevice sd0((void *) &ata0, STORAGE_DEVICE_BUS_TYPE_ATA, "ata0");
    fsProbe(&sd0);

    pciScan();
    initPciDevices();

    Pit::sleep(1000);
    desktopLoop();

    while (true) {

    }
}