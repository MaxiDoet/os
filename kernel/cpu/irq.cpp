#include <cpu/irq.hpp>
#include <drivers/pic.hpp>
#include <debug.hpp>

typedef struct IrqHandler {
    uint8_t irq;
    void (*handler) (void *privateData);
    void *privateData;
} IrqHandler;

IrqHandler irqHandlers[255];
uint8_t irqHandlersIndex = 0;

void irqHandler(uint8_t irq)
{
    for (uint8_t i=0; i < irqHandlersIndex; i++) {
        if (irq == irqHandlers[i].irq) {
            void (*handler) (void *privateData) = irqHandlers[i].handler;
            void *privateData = irqHandlers[i].privateData;

            handler(privateData);
        }
    }

    picSendEoi(irq);
}

void irqInstallHandler(uint8_t irq, void (*handler) (void *privateData), void *privateData)
{
    irqHandlers[irqHandlersIndex].irq = irq;
    irqHandlers[irqHandlersIndex].handler = handler;
    irqHandlers[irqHandlersIndex++].privateData = privateData;
}