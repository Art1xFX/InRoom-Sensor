#include "sensor/LightSensor.h"

LightSensor::LightSensor() : tsl(TSL2561_ADDR_FLOAT)
{
    // TODO: Setting up the I2C pins using macroses.
    Wire.begin(D2, D1);
    tsl.begin();
    // TODO: Add possibility to set gain and integration time from outside.
    tsl.setGain(TSL2561_GAIN_16X);
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
}

void LightSensor::onTick(uint32_t now)
{
    sensors_event_t event;
    tsl.getEvent(&event);

    auto currentValue = event.light;
    if (currentValue != this->previousValue)
    {
        for (const auto &callback : this->onChangeCallbacks)
        {
            callback(currentValue);
        }
        this->previousValue = currentValue;
    }
}

void LightSensor::onChange(const OnChangeCallback &callback)
{
    this->onChangeCallbacks.push_back(callback);
}
