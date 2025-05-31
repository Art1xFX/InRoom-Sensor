#pragma once

#include "Json.h"

namespace json
{
    struct Configuration : public Json<1024>
    {
        char wifi_ssid[32];
        char wifi_password[64];
        char mqtt_host[255];
        uint16_t mqtt_port;

        Configuration() = default;

        void toJsonObject(JsonObject &json) const override;

        void fromJsonObject(const JsonObject &json) override;
    };
}