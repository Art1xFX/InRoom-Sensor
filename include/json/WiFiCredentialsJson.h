#pragma once

#include "Json.h"
#include "Configuration.h"

namespace json
{
    class WiFiCredentialsJson : public WiFiCredentials, public Json<256>
    {
    public:
        WiFiCredentialsJson() = default;
        WiFiCredentialsJson(const WiFiCredentials &credentials);

        void toJsonObject(JsonObject &json) const;

        void fromJsonObject(const JsonObject &json);
    };
}