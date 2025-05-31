#include "json/WiFiCredentialsJson.h"

using namespace json;

WiFiCredentialsJson::WiFiCredentialsJson(const WiFiCredentials &credentials): WiFiCredentials(credentials.ssid, credentials.password)
{
}

void WiFiCredentialsJson::toJsonObject(JsonObject &json) const
{
    json["ssid"] = this->ssid;
    json["password"] = this->password;
}

void WiFiCredentialsJson::fromJsonObject(const JsonObject &json)
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
