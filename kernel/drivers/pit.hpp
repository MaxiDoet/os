#include <inttypes.h>

#define PIT_FREQ 1000

namespace Pit {
    void init();
    void sleep(uint32_t ticks);
    uint32_t getTicks();
}