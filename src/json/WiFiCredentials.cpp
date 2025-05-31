#include "json/WiFiCredentials.h"

using namespace json;

json::WiFiCredentials::WiFiCredentials(const ::WiFiCredentials &credentials): ::WiFiCredentials(credentials.ssid, credentials.password)
{
}

void json::WiFiCredentials::toJsonObject(JsonObject &json) const
{
    json["ssid"] = this->ssid;
    json["password"] = this->password;
}

void json::WiFiCredentials::fromJsonObject(const JsonObject &json)
{
    if (json["ssid"].is<const char*>())
    {
        strlcpy(this->ssid, json["ssid"], sizeof(this->ssid) - 1);
    }
    if (json["password"].is<const char*>())
    {
        strlcpy(this->password, json["password"], sizeof(this->password) - 1);
    }
}
