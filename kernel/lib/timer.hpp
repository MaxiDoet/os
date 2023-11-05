#ifndef _lib_timer_hpp
#define _lib_timer_hpp

#include <inttypes.h>

class Timer {
    private:
        uint32_t startTicks;
        uint32_t endTicks;

    public:
        void start();
        void stop();
        uint32_t getTicks();
};

#endif