#include <drivers/ata.hpp>
#include <io/io.hpp>
#include <fs/fs.hpp>
#include <mem/heap.hpp>
#include <debug.hpp>

#define REG_DATA                0x0
#define REG_SECTOR_COUNT        0x2
#define REG_LBA_LOW             0x3
#define REG_LBA_MID             0x4
#define REG_LBA_HIGH            0x5
#define REG_SELECT              0x6
#define REG_STATUS              0x7
#define REG_COMMAND             0x7

#define STATUS_DRQ              (1 << 3)
#define STATUS_BSY              (1 << 7)

#define COMMAND_READ_SECTORS    0x20

void AtaDevice::print(char *str)
{
    debugPrint("ATA | %s->%s | %s\n", primary ? "Primary" : "Secondary", master ? "Master" : "Slave", str);
}

void AtaDevice::waitBusy()
{
    uint8_t status = inb(ioBase + REG_STATUS);

    while ((status & STATUS_BSY)) {
        asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");

        status = inb(ioBase + REG_STATUS);
    }
}

void AtaDevice::waitData()
{
    uint8_t status = inb(ioBase + REG_STATUS);

    while (!(status & STATUS_DRQ)) {
        status = inb(ioBase + REG_STATUS);
    }
}

AtaDevice::AtaDevice(bool primary, bool master)
{
    this->primary = primary;
    this->master = master;

    this->ioBase = primary ? 0x1F0 : 0x1E8;

    // Check for floating bus
    uint8_t status = inb(ioBase + REG_STATUS);
    if (status == 0xFF) {
        print("Not connected (floating bus)");
        return;
    }

    print("Init");

    // Select drive
    outb(ioBase + REG_SELECT, master ? 0xA0 : 0xB0);

    // Reset
    outb(ioBase + REG_SECTOR_COUNT, 0);
    outb(ioBase + REG_LBA_LOW, 0);
    outb(ioBase + REG_LBA_MID, 0);
    outb(ioBase + REG_LBA_HIGH, 0);

    // Todo: Identify command
}

void AtaDevice::read(uint32_t lba, uint8_t sectorCount, uint8_t *buf)
{
    waitBusy();

    outb(ioBase + REG_SELECT, (master ? 0xE0 : 0xF0) | ((lba & 0x0F000000) >> 24));
    outb(ioBase + REG_SECTOR_COUNT, sectorCount);
    outb(ioBase + REG_LBA_LOW, lba & 0x000000FF);
    outb(ioBase + REG_LBA_MID, (lba & 0x0000FF00) >> 8);
    outb(ioBase + REG_LBA_HIGH, (lba & 0x00FF0000) >> 16);
    outb(ioBase + REG_COMMAND, COMMAND_READ_SECTORS);

    for (uint8_t i=0; i < sectorCount; i++) {
        waitData();

        for (uint16_t j=0; j < 512; j += 2) {
            uint16_t data = inw(ioBase + REG_DATA);

            buf[i * 512 + j] = data & 0xFF;
            buf[i * 512 + j + 1] = (data & 0xFF00) >> 8;
        }
    }
}