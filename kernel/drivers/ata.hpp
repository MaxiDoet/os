#include <inttypes.h>

class AtaDevice {
    private:
        uint16_t ioBase;

        bool primary;
        bool master;
        bool isReady;

        void print(char *str);
        void waitBusy();
        void waitData();
    public:
        AtaDevice(bool primary, bool master);

        void read(uint32_t lba, uint8_t sectorCount, uint8_t *buf);
};