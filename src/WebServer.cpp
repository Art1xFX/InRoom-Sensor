#include "WebServer.h"

WebServer::WebServer(Configuration &configuration) : server(80)
{
    this->postConfigHandler = new AsyncCallbackJsonWebHandler(
        "/config",
        [&configuration](AsyncWebServerRequest *request, JsonVariant &json)
        {
            if (json.is<JsonObject>())
            {
                JsonObject jsonObject = json.as<JsonObject>();
                json::WiFiCredentials credentialsJson;
                credentialsJson.fromJsonObject(jsonObject);
                Serial.print("[WebServer] SSID: ");
                Serial.println(credentialsJson.ssid);
                Serial.print("[WebServer] Password: ");
                Serial.println(credentialsJson.password);
                configuration.setWifiCredentials(credentialsJson.ssid, credentialsJson.password);
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
            if (credentials != nullptr)
            {
                json::WiFiCredentials credentialsJson(*credentials);
                char jsonString[256];
                credentialsJson.toJsonString(jsonString);
                request->send(200, "application/json", jsonString);
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
    delete this->postConfigHandler;
}
