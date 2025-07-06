#include "MqttManager.h"

MqttManager::MqttManager(Configuration &config)
{
    this->mqttClient.onConnect([this](bool sessionPresent)
    {
#ifdef DEBUG
        Serial.println("[MqttManager] Connected to MQTT broker.");
#endif
        for (const auto &callback : this->onConnectCallbacks)
        {
            callback();
        }
    });

    this->mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
    {
#ifdef DEBUG
        Serial.println("[MqttManager] Disconnected from MQTT broker.");
#endif
        for (const auto &callback : this->onDisconnectCallbacks)
        {
            callback();
        }
    });
}

void MqttManager::connect(const MqttEndpoint &endpoint, const MqttCredentials *credentials)
{
#ifdef DEBUG
    Serial.printf("[MqttManager] Connecting to MQTT broker at %s:%d...\n", endpoint.host, endpoint.port);
#endif
    this->mqttClient.setServer(endpoint.host, endpoint.port);
    if (credentials != nullptr)
    {
        this->mqttClient.setCredentials(credentials->username, credentials->password);
    }
    this->mqttClient.connect();
}

void MqttManager::publish(const char *topic, const char *payload)
{
    mqttClient.publish(topic, 0, false, payload);
}

void MqttManager::subscribe(const char *topic)
{
    mqttClient.subscribe(topic, 0);
}

void MqttManager::onConnect(MqttConnectCallback callback)
{
    this->onConnectCallbacks.push_back(callback);
}

void MqttManager::onDisconnect(MqttDisconnectCallback callback)
{
    this->onDisconnectCallbacks.push_back(callback);
}

void MqttManager::disconnect()
{
    Serial.println("[MqttManager] Disconnecting from MQTT broker...");
    this->mqttClient.disconnect();
}
