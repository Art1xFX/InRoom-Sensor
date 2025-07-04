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
        char mqtt_user[255];
        char mqtt_password[255];
        char mqtt_data_topic[255];

        Configuration() = default;

        void toJsonObject(JsonObject &json) const override;

        void fromJsonObject(const JsonObject &json) override;
    };
}