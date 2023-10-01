#include <io/pci.hpp>

typedef struct AC97BufDescriptor {
    uint32_t addr;
	uint16_t length;
 	int reserved : 14;
	unsigned int bup : 1;
	unsigned int ioc : 1;
} __attribute__((packed)) AC97BufDescriptor;

class AC97Device {
    public:
        PciDevice *dev;
    
        AC97BufDescriptor bufDescriptors[32];
        uint8_t bufDescriptorsRp;
        uint8_t bufDescriptorsWp;

        uint8_t *audioBuf;
        uint32_t audioOffset;
        uint32_t audioAvailable;

        void reset();
        void writeSingleBuffer(uint8_t *data, uint16_t size);
        void play(uint8_t *data, uint32_t size);

        AC97Device(PciDevice *dev);
};