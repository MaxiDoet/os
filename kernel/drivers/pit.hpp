#include <inttypes.h>

namespace Pit {
    void init();
    void sleep(uint32_t ticks);
    uint32_t getTicks();
}