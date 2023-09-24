#include <inttypes.h>
#include <lib/time.h>

class RtcDevice {
    private:
        uint8_t readReg(uint8_t reg);

    public:
        Time read();
};