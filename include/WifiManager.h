#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "Configuration.h"

#define WIFI_ACCESS_POINT_SSID "ESP8266_Setup"
#define WIFI_ACCESS_POINT_PASSWORD "password123"
#define WIFI_CONNECTION_TIMEOUT 3000
#define WIFI_CONNECTION_RETRY_INTERVAL 1000

enum WifiMode : uint8_t
{
    OFF = 0,
    STATION = 1 << 0,
    ACCESS_POINT = 1 << 1,
    STATION_AND_ACCESS_POINT = STATION | ACCESS_POINT,
};

enum struct ConnectionStatus : uint8_t
{
    CONNECTED,
    WRONG_PASSWORD,
    TIMEOUT,
};

class WifiManager
{
public:
    WifiManager();

    ConnectionStatus connect(const WiFiCredentials& credentials);

    void startAccessPoint();

    bool isConnected() const;

    WifiMode getMode() const;
};