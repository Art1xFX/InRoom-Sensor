#pragma once

#include <functional>
#include <vector>

#include <Arduino.h>
#include <AsyncMqttClient.h>
#include "Configuration.h"

typedef std::function<void()> MqttConnectCallback;

typedef std::function<void()> MqttDisconnectCallback;

class MqttManager
{
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
