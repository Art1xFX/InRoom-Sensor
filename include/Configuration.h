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

#define MQTT_ENDPOINT_MAGIC_OFFSET WIFI_CREDENTIALS_OFFSET + sizeof(WiFiCredentials)
#define MQTT_ENDPOINT_MAGIC_TYPE uint16_t
#define MQTT_ENDPOINT_MAGIC_VALUE 0xABCE
#define MQTT_ENDPOINT_MAGIC_SIZE sizeof(MQTT_ENDPOINT_MAGIC_TYPE)
#define MQTT_ENDPOINT_OFFSET (MQTT_ENDPOINT_MAGIC_OFFSET + MQTT_ENDPOINT_MAGIC_SIZE)

#define MQTT_DATA_TOPIC_MAGIC_OFFSET (MQTT_ENDPOINT_OFFSET + sizeof(MqttEndpoint))
#define MQTT_DATA_TOPIC_MAGIC_TYPE uint8_t
#define MQTT_DATA_TOPIC_MAGIC_VALUE 0x01
#define MQTT_DATA_TOPIC_MAGIC_SIZE sizeof(MQTT_DATA_TOPIC_MAGIC_TYPE)
#define MQTT_DATA_TOPIC_OFFSET (MQTT_DATA_TOPIC_MAGIC_OFFSET + MQTT_DATA_TOPIC_MAGIC_SIZE)

struct WiFiCredentials
{
    char ssid[32];
    char password[64];

    WiFiCredentials(const char *ssid = "", const char *password = "");

    bool operator==(const WiFiCredentials& other) const {
        return strcmp(this->ssid, other.ssid) == 0 && strcmp(this->password, other.password) == 0;
    }
};

struct MqttEndpoint
{
    char host[255];
    uint16_t port;

    MqttEndpoint(const char *host = "", uint16_t port = 1883);

    bool operator==(const MqttEndpoint& other) const {
        return strcmp(this->host, other.host) == 0 && this->port == other.port;
    }
};

class Configuration
{
public:
    using OnMqttDataTopicChangeCallback = std::function<void(const char*)>;
private:
    WIFI_CREDENTIALS_MAGIC_TYPE wifiCredentialsMagicValue;
    WiFiCredentials wifiCredentials;
    MQTT_ENDPOINT_MAGIC_TYPE mqttEndpointMagicValue;
    MqttEndpoint mqttEndpoint;
    MQTT_DATA_TOPIC_MAGIC_TYPE mqttDataTopicMagicValue;
    char mqttDataTopic[255];

    std::vector<OnMqttDataTopicChangeCallback> onMqttDataTopicChangeCallbacks;

public:
    Configuration();
    ~Configuration();

    const WiFiCredentials* getWifiCredentials() const;
    void setWifiCredentials(const char *ssid, const char *password);
    void clearWifiCredentials();


    const MqttEndpoint* getMqttEndpoint() const;
    void setMqttEndpoint(const char *host, uint16_t port);
    void clearMqttEndpoint();

    const char* getMqttDataTopic() const;
    void setMqttDataTopic(const char* topic);
    void clearMqttDataTopic();

    void onMqttDataTopicChange(const OnMqttDataTopicChangeCallback& callback);

    bool save();
};