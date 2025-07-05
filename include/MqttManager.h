#pragma once

#include <functional>
#include <vector>

#include <Arduino.h>
#include <AsyncMqttClient.h>
#include "Configuration.h"

class MqttManager
{
public:
    using MqttConnectCallback = std::function<void()>;

    using MqttDisconnectCallback = std::function<void()>;

private:
    AsyncMqttClient mqttClient;

    std::vector<MqttConnectCallback> onConnectCallbacks;

    std::vector<MqttDisconnectCallback> onDisconnectCallbacks;

public:
    MqttManager(Configuration &config);

    void connect(const MqttEndpoint &endpoint, const MqttCredentials *credentials = nullptr);

    void publish(const char *topic, const char *payload);

    void subscribe(const char *topic);

    void onConnect(MqttConnectCallback callback);

    void onDisconnect(MqttDisconnectCallback callback);

    void disconnect();
};
