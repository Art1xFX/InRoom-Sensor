#include "sensor/MotionSensor.h"
#include <Arduino.h>

MotionSensor::MotionSensor()
{
    pinMode(D5, INPUT);
}

void MotionSensor::onTick(uint32_t now)
{
    bool currentMotion = digitalRead(D5);
    if (currentMotion != this->previousValue)
    {
        for (const auto &callback : this->onChangeCallbacks)
        {
            callback(currentMotion);
        }
        this->previousValue = currentMotion;
    }
}

void MotionSensor::onChange(const OnChangeCallback &callback)
{
    this->onChangeCallbacks.push_back(callback);
}
