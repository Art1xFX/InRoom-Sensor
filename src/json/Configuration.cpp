#include "json/Configuration.h"

void json::Configuration::toJsonVariant(JsonVariant &json) const
{
    json["wifi_ssid"] = this->wifi_ssid;
    json["wifi_password"] = this->wifi_password;
    json["mqtt_host"] = this->mqtt_host;
    json["mqtt_port"] = this->mqtt_port;
    json["mqtt_user"] = this->mqtt_user;
    json["mqtt_password"] = this->mqtt_password;
    json["mqtt_data_topic"] = this->mqtt_data_topic;
}

void json::Configuration::fromJsonVariant(const JsonVariant &json)
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
    if (json["mqtt_user"].is<const char*>())
    {
        strlcpy(this->mqtt_user, json["mqtt_user"], sizeof(this->mqtt_user) - 1);
    }
    if (json["mqtt_password"].is<const char*>())
    {
        strlcpy(this->mqtt_password, json["mqtt_password"], sizeof(this->mqtt_password) - 1);
    }
    if (json["mqtt_data_topic"].is<const char*>())
    {
        strlcpy(this->mqtt_data_topic, json["mqtt_data_topic"], sizeof(this->mqtt_data_topic) - 1);
    }
}
