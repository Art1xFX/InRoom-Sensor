#pragma once

#include <optional>
#include <string>

#include <Arduino.h>
#include <EEPROM.h>

#define WIFI_CREDENTIALS_MAGIC_OFFSET 0
#define WIFI_CREDENTIALS_MAGIC_TYPE uint16_t
#define WIFI_CREDENTIALS_MAGIC_VALUE 0xABCD
#define WIFI_CREDENTIALS_MAGIC_SIZE sizeof(WIFI_CREDENTIALS_MAGIC_TYPE)
#define WIFI_CREDENTIALS_OFFSET (WIFI_CREDENTIALS_MAGIC_OFFSET + WIFI_CREDENTIALS_MAGIC_SIZE)

#define MQTT_ENDPOINT_MAGIC_OFFSET WIFI_CREDENTIALS_OFFSET + WIFI_CREDENTIALS_MAGIC_SIZE
#define MQTT_ENDPOINT_MAGIC_TYPE uint16_t
#define MQTT_ENDPOINT_MAGIC_VALUE 0xABCE
#define MQTT_ENDPOINT_MAGIC_SIZE sizeof(MQTT_ENDPOINT_MAGIC_TYPE)
#define MQTT_ENDPOINT_OFFSET (MQTT_ENDPOINT_MAGIC_OFFSET + MQTT_ENDPOINT_MAGIC_SIZE)


struct WiFiCredentials
{
    char ssid[32];
    char password[64];

    WiFiCredentials(const char *ssid = "", const char *password = "");
};

struct MqttEndpoint
{
    char host[255];
    uint16_t port;

    MqttEndpoint(const char *host = "", uint16_t port = 1883);
};

class Configuration
{
private:
    WIFI_CREDENTIALS_MAGIC_TYPE wifiCredentialsMagicValue;
    WiFiCredentials wifiCredentials;
    MQTT_ENDPOINT_MAGIC_TYPE mqttEndpointMagicValue;
    MqttEndpoint mqttEndpoint;

public:
    Configuration();
    ~Configuration();

    const WiFiCredentials* getWifiCredentials() const;
    void setWifiCredentials(const char *ssid, const char *password);
    void clearWifiCredentials();


    const MqttEndpoint* getMqttEndpoint() const;
    void setMqttEndpoint(const char *host, uint16_t port);
    void clearMqttEndpoint();

    bool save();
};