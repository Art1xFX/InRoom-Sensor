#include <cstdint>

#include <Arduino.h>

class TickableBase
{
private:
    uint32_t interval = 1000;
    uint32_t lastTickTime = 0;

public:
    virtual void onTick(uint32_t now) = 0;

    virtual void tick() final;

    uint32_t getInterval() const;

    void setInterval(uint32_t interval);
};