#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

#include "Configuration.h"
#include "json/WiFiCredentialsJson.h"


class WebServer {
private:
    AsyncWebServer server;
    Configuration& configuration;

    void handleGetConfig(AsyncWebServerRequest *request) const;

public:
    WebServer(Configuration& config);
};