#include <drivers/ata.hpp>
#include <io/io.hpp>
#include <fs/fs.hpp>
#include <mem/heap.hpp>
#include <lib/string.hpp>
#include <debug.hpp>

#define REG_DATA                0x0
#define REG_SECTOR_COUNT        0x2
#define REG_LBA_LOW             0x3
#define REG_LBA_MID             0x4
#define REG_LBA_HIGH            0x5
#define REG_SELECT              0x6
#define REG_STATUS              0x7
#define REG_COMMAND             0x7

#define STATUS_ERR              (1 << 0)
#define STATUS_DRQ              (1 << 3)
#define STATUS_BSY              (1 << 7)

#define COMMAND_READ_SECTORS    0x20
#define COMMAND_IDENTIFY        0xEC

void AtaDevice::print(char *str)
{
    debugPrint("ATA | %s->%s | %s\n", primary ? "Primary" : "Secondary", master ? "Master" : "Slave", str);
}

uint8_t AtaDevice::waitBusy()
{
    uint8_t status = inb(ioBase + REG_STATUS);

    while ((status & STATUS_BSY)) {
        if (status & STATUS_ERR) {
            return 0;
        }

        asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");

        status = inb(ioBase + REG_STATUS);
    }

    return 1;
}

uint8_t AtaDevice::waitData()
{
    uint8_t status = inb(ioBase + REG_STATUS);

    while (!(status & STATUS_DRQ)) {
        if (status & STATUS_ERR) {
            return 0;
        }
        
        status = inb(ioBase + REG_STATUS);
    }

    return 1;
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

    // Select drive
    outb(ioBase + REG_SELECT, master ? 0xA0 : 0xB0);

    // Reset
    outb(ioBase + REG_SECTOR_COUNT, 0);
    outb(ioBase + REG_LBA_LOW, 0);
    outb(ioBase + REG_LBA_MID, 0);
    outb(ioBase + REG_LBA_HIGH, 0);

    // Check if device is connected
    if (!inb(ioBase + REG_STATUS)) {
        print("Not connected");
        return;
    }

    /* Extract model number throught identify command */
    outb(ioBase + REG_COMMAND, COMMAND_IDENTIFY);

    if (!waitBusy()) return;
    if (!waitData()) return;

    uint8_t info[512];
    for (int i=0; i < 256; i++) {
		uint16_t data = inw(ioBase + REG_DATA);

		info[i * 2] = (data >> 8);
		info[i * 2 + 1] = data & 0xFF;
	}

    // Find tail of the string
	uint32_t tail = 40;
	while (tail > 0 && ((char) info[0x36 + tail - 1]) == ' ') {
		tail--;
	}

	memcpy(model, info + 0x36, tail);

    print("Init");
    print(model);
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