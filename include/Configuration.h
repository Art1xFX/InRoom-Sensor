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


struct WiFiCredentials
{
    char ssid[32];
    char password[64];
};

class Configuration
{
private:
    WIFI_CREDENTIALS_MAGIC_TYPE wifiCredentialsMagicValue;
    const WiFiCredentials wifiCredentials;

public:
    Configuration();

    void begin();
    const std::optional<const WiFiCredentials> getWifiCredentials() const;
    void setWifiCredentials(const char *ssid, const char *password);

    bool save();
};