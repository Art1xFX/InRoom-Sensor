#include "json/Configuration.h"

void json::Configuration::toJsonObject(JsonObject &json) const
{
    json["wifi_ssid"] = this->wifi_ssid;
    json["wifi_password"] = this->wifi_password;
    json["mqtt_host"] = this->mqtt_host;
    json["mqtt_port"] = this->mqtt_port;
    json["mqtt_data_topic"] = this->mqtt_data_topic;
}

void json::Configuration::fromJsonObject(const JsonObject &json)
{
    if (json["wifi_ssid"].is<const char*>())
    {
        strlcpy(this->wifi_ssid, json["wifi_ssid"], sizeof(this->wifi_ssid) - 1);
    }
    if (json["wifi_password"].is<const char*>())
    {
        strlcpy(this->wifi_password, json["wifi_password"], sizeof(this->wifi_password) - 1);
    }
    if (json["mqtt_host"].is<const char*>())
    {
        strlcpy(this->mqtt_host, json["mqtt_host"], sizeof(this->mqtt_host) - 1);
    }
    if (json["mqtt_port"].is<uint16_t>())
    {
        this->mqtt_port = json["mqtt_port"];
    }
    if (json["mqtt_data_topic"].is<const char*>())
    {
        strlcpy(this->mqtt_data_topic, json["mqtt_data_topic"], sizeof(this->mqtt_data_topic) - 1);
    }
}
