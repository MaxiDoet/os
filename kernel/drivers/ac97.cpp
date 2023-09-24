#include <drivers/ac97.hpp>
#include <cpu/irq.hpp>
#include <io/io.hpp>
#include <mem/heap.hpp>
#include <lib/string.hpp>
#include <debug.hpp>

/* NAM Registers */
#define NAM_RESET 0x00
#define NAM_MASTER_VOL 0x02
#define NAM_MIC_VOL 0x0E
#define NAM_PCM_VOL 0x18
#define NAM_REC_SLC 0x1A
#define NAM_REC_GAIN 0x1C
#define NAM_MIC_GAIN 0x1E
#define NAM_POWER_STS 0x26
#define NAM_EXT_ID 0x28
#define NAM_EXT_CTRL 0x2A
#define NAM_EXT_FRONT_RATE 0x2C

/* NAM_POWER_STS */
#define NAM_POWER_STS_ADC (1 << 0)
#define NAM_POWER_STS_DAC (1 << 1)
#define NAM_POWER_STS_ANL (1 << 2)
#define NAM_POWER_STS_REF (1 << 3)

/* NABM Registers */
#define PI 0x00
#define PO 0x10
#define MIC 0x20
#define GLOB_CNT 0x2C
#define GLOB_STA 0x30

/* Channel Control Registers */
#define BDBAR 0x00
#define CIV 0x04
#define LVI 0x05
#define SR 0x06
#define PICB 0x08
#define PIV 0x0A
#define CR 0x0B

/* CR */
#define RPBM (1 << 0)
#define RR (1 << 1)
#define LVBIE (1 << 2)
#define IOCE (1 << 3)
#define FEIE (1 << 4)

/* SR */
#define DMAS (1 << 0)
#define EOT (1 << 1)
#define LBI (1 << 2)
#define IOCI (1 << 3)
#define FEI (1 << 4)

/* GLOB_CNT */
#define GIE (1 << 0)
#define COLD_RESET (1 << 1)
#define WARM_RESET (1 << 2)

#define MAX_ENTRY_SIZE 0xFFFE
#define LAST_VALID_INDEX 32

void AC97Device::reset()
{
    // Cold reset
    outl(dev->getBar(1).ioBase + GLOB_CNT, COLD_RESET);

    // Enable interrupts
	outl(dev->getBar(1).ioBase + GLOB_CNT, GIE);

    // Reset mixer
	outw(dev->getBar(0).ioBase + NAM_RESET, 1);

    // Reset output channel registers
	outb(dev->getBar(1).ioBase + PO + CR, RR);
	while(inb(dev->getBar(1).ioBase + PO + CR) & RR) {
    }
}

void AC97Device::writeSingleBuffer(uint8_t *data, uint16_t size)
{
    if (size > MAX_ENTRY_SIZE) return;

    /* Check if chip is ready */
	uint16_t powerSts = inw(dev->getBar(0).ioBase + NAM_POWER_STS);

    if (!(powerSts & (NAM_POWER_STS_ADC | NAM_POWER_STS_DAC | NAM_POWER_STS_ANL | NAM_POWER_STS_REF))) {
		debugPrint("AC97 | Not ready\n");
		return;
	}

    debugPrint("AC97 | write | size: %d | wp: %d\n", size, bufDescriptorsWp);

    AC97BufDescriptor *desc = &bufDescriptors[bufDescriptorsWp];

    desc->addr = (uint32_t) data;
	desc->length = size;
	desc->ioc = 1;

    if (bufDescriptorsWp == LAST_VALID_INDEX) desc->bup = 1;

    bufDescriptorsWp = (bufDescriptorsWp + 1) % LAST_VALID_INDEX;
}

void AC97Device::play(uint8_t *data, uint32_t size)
{
    // Alloc space and copy the buffer into the driver internal buffer
	audioBuf = (uint8_t *) heapAlloc(size);
	memcpy(audioBuf, data, size);

	audioOffset = 0;
	audioAvailable = size;

	if (audioAvailable) {
		uint8_t last = 0;

		uint16_t status = inw(dev->getBar(1).ioBase + PO + SR);
		uint8_t control = inb(dev->getBar(1).ioBase + PO + CR);

		for (int i=0; i < LAST_VALID_INDEX; i++) {
			if (audioAvailable >= 0x20000) {
				writeSingleBuffer(&audioBuf[audioOffset], 0xFFFE);
				audioAvailable -= 0x20000;
				audioOffset += 0x20000;
				last = 32;
			} else {
				writeSingleBuffer(&audioBuf[audioOffset], audioAvailable >> 1);
				last = i;
				audioAvailable = 0;

				break;
			}
		}

		// Reset output channel registers
		outb(dev->getBar(1).ioBase + PO + CR, RR);
		while(inb(dev->getBar(1).ioBase + PO + CR) & RR) {
		}

		outl(dev->getBar(1).ioBase + PO + BDBAR, (uint32_t) bufDescriptors);
		outb(dev->getBar(1).ioBase + PO + LVI, last - 1);

		// Start playback
		outb(dev->getBar(1).ioBase + PO + CR, RPBM | IOCE | LVBIE | FEIE); // Start DMA; Enable IOC interrupt; Enable Last Buffer Entry interrupt
	}
}

void AC97IrqHandler(void *privateData)
{
    AC97Device *dev = (AC97Device *) privateData;

    uint16_t status = inw(dev->dev->getBar(1).ioBase + PO + SR);

	debugPrint("AC97 | IRQ\n");

    if (status & LBI) {
		#ifdef AC97_DEBUG
		kdebug("[ac97] lvb interrupt\r\n");
		#endif

		dev->bufDescriptorsRp = 0;
		dev->bufDescriptorsWp = 0;
	}

    if (status & IOCI) {
		dev->bufDescriptorsRp = (dev->bufDescriptorsRp + 1) % 32;

		debugPrint("AC97 | IOC interrupt | rp: %d\r\n", dev->bufDescriptorsRp);

		if (status & DMAS && dev->audioAvailable) {
			uint8_t last = 0;

			uint16_t status = inw(dev->dev->getBar(1).ioBase + PO + SR);
			uint8_t control = inb(dev->dev->getBar(1).ioBase + PO + CR);

			for (int i=0; i < LAST_VALID_INDEX; i++) {
				if (dev->audioAvailable >= 0x20000) {
					dev->writeSingleBuffer(&dev->audioBuf[dev->audioOffset], 0xFFFE);
					dev->audioAvailable -= 0x20000;
					dev->audioOffset += 0x20000;
					last = 32;
				} else {
					dev->writeSingleBuffer(&dev->audioBuf[dev->audioOffset], dev->audioAvailable >> 1);
					last = i;
					dev->audioAvailable = 0;

					heapFree(dev->audioBuf);

					break;
				}
			}

			// Reset output channel registers
			outb(dev->dev->getBar(1).ioBase + PO + CR, RR);
			while(inb(dev->dev->getBar(1).ioBase + PO + CR) & RR) {
			}

			outl(dev->dev->getBar(1).ioBase + PO + BDBAR, (uint32_t) dev->bufDescriptors);
			outb(dev->dev->getBar(1).ioBase + PO + LVI, last - 1);

			// Start playback
			outb(dev->dev->getBar(1).ioBase + PO + CR, RPBM | IOCE | LVBIE | FEIE); // Start DMA; Enable IOC interrupt; Enable Last Buffer Entry interrupt
		}
	}

	if (status & FEI) {
		debugPrint("AC97 | FIFO error interrupt\n");
	}

	outw(dev->dev->getBar(1).ioBase + PO + SR, 0x1C);
}

AC97Device::AC97Device(PciDevice *dev)
{
	this->dev = dev;

    dev->enableBusMastering();

	irqInstallHandler(dev->getIrq(), AC97IrqHandler, this);

    reset();

	// Set master volume
	outw(dev->getBar(0).ioBase + NAM_MASTER_VOL, 0x0000);
	// Set PCM output volume
	outw(dev->getBar(0).ioBase + NAM_PCM_VOL, 0x0000);

	debugPrint("AC97 | Init done\n");
}