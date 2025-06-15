#pragma once

#include <functional>
#include <vector>

#include <Adafruit_TSL2561_U.h>

#include "Tickable.h"

class LightSensor : public TickableBase
{
public:
    using OnChangeCallback = std::function<void(float)>;

private:
    Adafruit_TSL2561_Unified tsl;

    std::vector<OnChangeCallback> onChangeCallbacks;

    float previousValue = 0;

public:
    LightSensor();

    void onChange(const OnChangeCallback &callback);

    void onTick(uint32_t now) override;
};
