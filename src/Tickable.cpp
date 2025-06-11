#include "Tickable.h"

void TickableBase::tick()
{
    auto currentTime = millis();
    if (currentTime - this->lastTickTime >= this->interval)
    {
        this->lastTickTime = currentTime;
        this->onTick(currentTime);
    }
}

uint32_t TickableBase::getInterval() const
{
    return this->interval;
}

void TickableBase::setInterval(uint32_t interval)
{
    this->interval = interval;
}