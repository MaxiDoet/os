#include <inttypes.h>

#define PIC_MASTER_COMMAND  0x20
#define PIC_MASTER_DATA     0x21
#define PIC_SLAVE_COMMAND   0xA0
#define PIC_SLAVE_DATA      0xA1

void picInit(uint8_t commandPort, uint8_t dataPort, uint8_t icw1, uint8_t icw2, uint8_t icw3, uint8_t icw4);
void picDemask(uint8_t dataPort);

void picSendEoi(uint8_t irq);