#include "WebServer.h"

WebServer::WebServer(Configuration& config) : server(80), configuration(config)
{
    server.on("/config", HTTP_GET, [this](AsyncWebServerRequest *request)
    { 
        handleGetConfig(request); 
    });
    server.begin();
}

void WebServer::handleGetConfig(AsyncWebServerRequest *request) const
{
    auto credentials = configuration.getWifiCredentials();
    if (credentials != nullptr)
    {
        WiFiCredentialsJson credentialsJson(*credentials);
        char jsonString[256];
        credentialsJson.toJsonString(jsonString);
        request->send(200, "application/json", jsonString);
    }
    else
    {
        request->send(404, "application/json");
    }
}
