#include <inttypes.h>

void irqHandler(uint8_t irq);

void irqInstallHandler(uint8_t irq, void (*handler) (void *privateData), void *privateData);