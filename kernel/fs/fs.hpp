#include <stdint.h>
#include <dev/sd.hpp>

class Fs {
    public:
        char *identifier;   // Something like ata0
        char *name;
        void *privateData;
        StorageDevice *dev;
};

void fsProbe(StorageDevice *dev);
void fsRead(char *path, uint8_t *buf);