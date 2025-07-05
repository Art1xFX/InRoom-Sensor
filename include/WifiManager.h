#pragma once

#include <vector>

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "Tickable.h"
#include "Configuration.h"

#define WIFI_ACCESS_POINT_SSID "ESP8266_Setup"
#define WIFI_ACCESS_POINT_PASSWORD "password123"
#define WIFI_CONNECTION_TIMEOUT 30000

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

typedef std::function<void()> ConnectCallback;

typedef std::function<void(const ConnectionStatus)> ErrorCallback;

typedef ConnectCallback DisconnectCallback;

class WifiManager : public TickableBase
{
protected:
    wl_status_t previousStatus;

    std::vector<ConnectCallback> onConnectCallbacks;

    std::vector<ErrorCallback> onErrorCallbacks;

    std::vector<DisconnectCallback> onDisconnectCallbacks;

    std::optional<uint32_t> connectionStartTime;

public:
    WifiManager();

    void connect(const WiFiCredentials& credentials);

    void disconnect();

    void startAccessPoint();

    void stopAccessPoint();

    bool isConnected() const;

    WifiMode getMode() const;

    void onTick(uint32_t now) override;

    void onConnect(ConnectCallback callback);

    void onError(ErrorCallback callback);

    void onDisconnect(DisconnectCallback callback);
};