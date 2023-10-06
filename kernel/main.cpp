#include <inttypes.h>

#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <drivers/serial.hpp>
#include <drivers/screen.hpp>
#include <drivers/ata.hpp>
#include <drivers/sound/ac97.hpp>
#include <drivers/input/mouse.hpp>
#include <drivers/video/vga.hpp>
#include <drivers/video/cirrus.hpp>
#include <dev/sd.hpp>
#include <fs/fs.hpp>
#include <mem/heap.hpp>
#include <io/pci.hpp>
#include <lib/string.hpp>
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

    // Init ATA drives
    AtaDevice ata0(true, true);
    AtaDevice ata1(true, false);
    AtaDevice ata2(false, true);
    AtaDevice ata3(false, false);

    StorageDevice sd0((void *) &ata0, STORAGE_DEVICE_BUS_TYPE_ATA, "ata0");
    fsProbe(&sd0);

    pciScan();

    PciDevice *list = (PciDevice *) heapAlloc(sizeof(PciDevice) * 256);
    uint8_t listLength = pciGetList(list);

    for (uint8_t i=0; i < listLength; i++) {
        pciInfo(&list[i]);
    }

    heapFree(list);

    mouseInit();

    const vgaVideoMode videoMode = {
        .width = 800,
        .height = 600,
        .colorDepth = VGA_COLOR_DEPTH_24,
        .text = false
    };

    // Init Cirrus
    list = (PciDevice *) heapAlloc(sizeof(PciDevice) * pciCount(0x1013, 0x00B8));
    listLength = pciFind(list, 0x1013, 0x00B8);

    if (listLength) {
        CirrusDevice cirrusDev(&list[0]);
        cirrusDev.setMode(videoMode);

        /*
        uint16_t cursorX = 400, cursorY = 300;
        while(true) {
            mouseInputState state = mouseRead();

            cursorX += state.x;
            cursorY -= state.y;

            if (cursorX < 0) cursorX = 0;
            if (cursorX > 799) cursorX = 799;
            if (cursorY > 599) cursorY = 599;
            if (cursorY < 0) cursorY = 0;

            cirrusDev.drawAccelaratedRectangle(20, 40, 100, 80, 0xFF0000);

            cirrusDev.drawAccelaratedRectangle(cursorX, cursorY, 10, 10, 0xFFFFFF);

            cirrusDev.drawAccelaratedRectangle(0, 0, 800, 600, 0x000000);
        }
        */
    }

    // Init AC97
    list = (PciDevice *) heapAlloc(sizeof(PciDevice) * pciCount(0x8086, 0x2415));
    listLength = pciFind(list, 0x8086, 0x2415);
    
    if (listLength) {
        AC97Device ac97Dev(&list[0]);

        // Read audio file
        uint8_t *buf = (uint8_t *) heapAlloc(25548514);
        fsRead("/audio.wav", buf);

        ac97Dev.play(buf, 25548514);
    }
}