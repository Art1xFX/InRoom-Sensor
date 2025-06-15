#pragma once

#include <functional>
#include <vector>

#include "Tickable.h"

class MotionSensor : public TickableBase
{
public:
    using OnChangeCallback = std::function<void(bool)>;

private:
    std::vector<OnChangeCallback> onChangeCallbacks;

    bool previousValue;

public:
    MotionSensor();

    void onChange(const OnChangeCallback &callback);

    void onTick(uint32_t now) override;
};
