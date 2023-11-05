#include <lib/timer.hpp>
#include <drivers/pit.hpp>
#include <debug.hpp>

void Timer::start()
{
    startTicks = Pit::getTicks();
}

void Timer::stop()
{
    endTicks = Pit::getTicks();
}

uint32_t Timer::getTicks()
{
    return endTicks - startTicks;
}