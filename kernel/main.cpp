#include <inttypes.h>

#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <drivers/serial.hpp>
#include <drivers/screen.hpp>
#include <drivers/ata.hpp>
#include <drivers/ac97.hpp>
#include <drivers/video/cirrus.hpp>
#include <dev/sd.hpp>
#include <fs/fs.hpp>
#include <mem/heap.hpp>
#include <io/pci.hpp>
#include <debug.hpp>

extern "C" void kmain()
{
    // Init serial port
    SerialPort com0(0x3F8);

    screenClear();

    // Init debugging over serial port
    debugInit(com0);

    // Init GDT
    debugPrint("GDT init\n");
    gdtSetup();

    // Init IDT
    debugPrint("IDT init\n");
    idtInstall();

    // Init heap
    uint32_t heapStart = 0x1FC00 + 10*1024*1024;
	uint32_t heapSize = 100000000;
    heapInit(heapStart, heapSize);

    pciScan();

    PciDevice *list = (PciDevice *) heapAlloc(sizeof(PciDevice) * 256);
    uint8_t listLength = pciGetList(list);

    for (uint8_t i=0; i < listLength; i++) {
        pciInfo(&list[i]);
    }

    heapFree(list);

    // Init AC97
    //list = (PciDevice *) heapAlloc(sizeof(PciDevice) * pciCount(0x8086, 0x2415));
    //pciFind(list, 0x8086, 0x2415);
    //AC97Device ac97Dev(&list[0]);

    // Init Cirrus
    list = (PciDevice *) heapAlloc(sizeof(PciDevice) * pciCount(0x1013, 0x00B8));
    pciFind(list, 0x1013, 0x00B8);
    CirrusDevice cirrusDev(&list[0]);

    // Init ATA drives
    AtaDevice ata0(true, true);
    AtaDevice ata1(true, false);
    AtaDevice ata2(false, true);
    AtaDevice ata3(false, false);

    StorageDevice sd0((void *) &ata0, STORAGE_DEVICE_BUS_TYPE_ATA, "ata0");
    fsProbe(&sd0);

    // Read audio file
    uint8_t *buf = (uint8_t *) heapAlloc(25548514);
    fsRead("/audio.wav", buf);

    //ac97Dev.play(buf, 25548514);
}