#include "WebServer.h"

WebServer::WebServer(Configuration &configuration, WifiManager &wifiManager) : server(80)
{
    this->postConfigHandler = new AsyncCallbackJsonWebHandler(
        "/config",
        [&configuration, &wifiManager](AsyncWebServerRequest *request, JsonVariant &json)
        {
            if (json.is<JsonObject>())
            {
                JsonObject jsonObject = json.as<JsonObject>();
                json::Configuration configurationJson;
                configurationJson.fromJsonVariant(jsonObject);
                Serial.println("[WebServer] ===> POST /config");
                Serial.println("  === Wi-Fi ===");
                Serial.print("    SSID: ");
                Serial.println(configurationJson.wifi_ssid);
                Serial.print("    Password: ");
                Serial.println(configurationJson.wifi_password);
                Serial.println("  === MQTT ===");
                Serial.print("    Host: ");
                Serial.println(configurationJson.mqtt_host);
                Serial.print("    Port: ");
                Serial.println(configurationJson.mqtt_port);
                Serial.print("    Data topic: ");
                Serial.println(configurationJson.mqtt_data_topic);
                configuration.setWifiCredentials(configurationJson.wifi_ssid, configurationJson.wifi_password);
                configuration.setMqttEndpoint(configurationJson.mqtt_host, configurationJson.mqtt_port);
                // TODO: Check if MQTT credentials are provided.
                configuration.setMqttCredentials(configurationJson.mqtt_user, configurationJson.mqtt_password);
                configuration.setMqttDataTopic(configurationJson.mqtt_data_topic);
                wifiManager.connect(*configuration.getWifiCredentials());

                // TODO: Save the configuration to persistent storage only if the Wi-Fi connection is successful.
                if (configuration.save())
                {
                    AsyncJsonResponse *response = new AsyncJsonResponse();
                    response->setCode(201);
                    response->setLength();
                    response->addHeader("Location", "/config");
                    request->send(response);
                }
                else
                {
                    request->send(500, "application/json");
                }
            }
            else
            {
                request->send(400, "application/json");
            }
        });
    this->postConfigHandler->setMethod(HTTP_POST);

    server.on(
        "/config",
        HTTP_GET,
        [&configuration](AsyncWebServerRequest *request)
        {
            auto credentials = configuration.getWifiCredentials();
            auto mqttEndpoint = configuration.getMqttEndpoint();
            auto mqttCredentials = configuration.getMqttCredentials();
            auto mqttDataTopic = configuration.getMqttDataTopic();
            if (credentials != nullptr && mqttEndpoint != nullptr)
            {
                json::Configuration configurationJson;
                strlcpy(configurationJson.wifi_ssid, credentials->ssid, sizeof(configurationJson.wifi_ssid) - 1);
                strlcpy(configurationJson.wifi_password, credentials->password, sizeof(configurationJson.wifi_password) - 1);
                strlcpy(configurationJson.mqtt_host, mqttEndpoint->host, sizeof(configurationJson.mqtt_host) - 1);
                configurationJson.mqtt_port = mqttEndpoint->port;
                if (mqttCredentials != nullptr)
                {
                    strlcpy(configurationJson.mqtt_user, mqttCredentials->username, sizeof(configurationJson.mqtt_user) - 1);
                    strlcpy(configurationJson.mqtt_password, mqttCredentials->password, sizeof(configurationJson.mqtt_password) - 1);
                }
                strlcpy(configurationJson.mqtt_data_topic, mqttDataTopic, sizeof(configurationJson.mqtt_data_topic) - 1);
                char jsonString[1024];
                size_t jsonStringLength = configurationJson.toJsonString(jsonString);
                request->send(200, "application/json", (uint8_t *)jsonString, jsonStringLength);
            }
            else
            {
                request->send(404, "application/json");
            }
        });

    this->server.on(
        "/config",
        HTTP_DELETE,
        [&configuration](AsyncWebServerRequest *request)
        {
            configuration.clearWifiCredentials();
            if (configuration.save())
            {
                request->send(200, "application/json");
            }
            else
            {
                request->send(500, "application/json");
            }
        });

    server.addHandler(this->postConfigHandler);
    server.begin();
}

WebServer::~WebServer()
{
    this->server.end();
    this->postConfigHandler = nullptr;
}
