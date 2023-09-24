#include <drivers/pic.hpp>
#include <io/io.hpp>

#define COMMAND_EOI 0x20

void picInit(uint8_t commandPort, uint8_t dataPort, uint8_t icw1, uint8_t icw2, uint8_t icw3, uint8_t icw4)
{
    outb(commandPort, icw1);
    outb(dataPort, icw2);
    outb(dataPort, icw3);
    outb(dataPort, icw4);
}

void picDemask(uint8_t dataPort)
{
    outb(dataPort, 0x00);
}

void picSendEoi(uint8_t irq)
{
    if (irq > 8) {
        outb(PIC_SLAVE_COMMAND, COMMAND_EOI);
    }

    outb(PIC_MASTER_COMMAND, COMMAND_EOI);
}