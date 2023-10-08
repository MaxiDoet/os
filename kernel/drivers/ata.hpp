#include <inttypes.h>

class AtaDevice {
    private:
        uint16_t ioBase;

        bool primary;
        bool master;
        bool isReady;

        char *model;

        void print(char *str);
        uint8_t waitBusy();
        uint8_t waitData();
        void identify(uint8_t data[512]);
        
    public:
        AtaDevice(bool primary, bool master);

        void read(uint32_t lba, uint8_t sectorCount, uint8_t *buf);
};