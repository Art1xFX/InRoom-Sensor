#pragma once

#include <list>
#include <functional>

#include <Arduino.h>
#include <AsyncMqttClient.h>
#include "Configuration.h"

typedef std::function<void()> MqttConnectCallback;

typedef std::function<void()> MqttDisconnectCallback;

class MqttManager
{
private:
    AsyncMqttClient mqttClient;

    std::list<MqttConnectCallback> onConnectCallbacks;

    std::list<MqttDisconnectCallback> onDisconnectCallbacks;

public:
    MqttManager(Configuration &config);

    void connect(const MqttEndpoint &endpoint);

    void publish(const char *topic, const char *payload);

    void subscribe(const char *topic);

    void onConnect(MqttConnectCallback callback);

    void onDisconnect(MqttDisconnectCallback callback);

    void disconnect();
};
