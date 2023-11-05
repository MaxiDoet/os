#include <drivers/pit.hpp>
#include <dev/dev.hpp>
#include <cpu/irq.hpp>
#include <io/io.hpp>
#include <debug.hpp>

uint32_t ticksTotal;

#define REG_CHANNEL_0           0x40
#define REG_CHANNEL_1           0x41
#define REG_CHANNEL_2           0x42
#define REG_MODE                0x43

#define MODE_MODE_0             0x00
#define MODE_MODE_1             0x02
#define MODE_MODE_2             0x04
#define MODE_ACCESS_LOBYTE      0x10
#define MODE_ACCESS_HIBYTE      0x20
#define MODE_ACCESS_LOHIBYTE    0x30
#define MODE_CHANNEL_0          0x00
#define MODE_CHANNEL_1          0x40
#define MODE_CHANNEL_2          0x80

void setFreq(uint32_t freq)
{
    uint16_t reloadValue = 1193182 / freq;

    outb(REG_CHANNEL_0, reloadValue & 0xFF);
    outb(REG_CHANNEL_0, (reloadValue >> 8) & 0xFF);
}

void irqHandler(void *privateData)
{
    ticksTotal++;

    setFreq(1000);
}

void Pit::init()
{
    outb(REG_MODE, MODE_MODE_2 | MODE_ACCESS_LOHIBYTE | MODE_CHANNEL_0);

    setFreq(1000);

    irqInstallHandler(0, irqHandler, nullptr);

    deviceAdd({
        .name = "Timer",
        .type = DEVICE_TYPE_TIMER,
        .busType = BUS_TYPE_IO
    });
}

void Pit::sleep(uint32_t ticks)
{
    uint32_t startTicks = ticksTotal;

    while (ticksTotal < startTicks + ticks) {
        asm("hlt");
    }
}

uint32_t Pit::getTicks()
{
    return ticksTotal;
}