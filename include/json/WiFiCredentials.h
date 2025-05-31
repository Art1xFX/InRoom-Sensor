#pragma once

#include "Json.h"
#include "Configuration.h"

namespace json
{
    class WiFiCredentials : public ::WiFiCredentials, public Json<256>
    {
    public:
        WiFiCredentials() = default;
        WiFiCredentials(const ::WiFiCredentials &credentials);

        void toJsonObject(JsonObject &json) const;

        void fromJsonObject(const JsonObject &json);
    };
}