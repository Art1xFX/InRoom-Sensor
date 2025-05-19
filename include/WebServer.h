#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

#include "Configuration.h"
#include "json/WiFiCredentialsJson.h"


class WebServer {
private:
    AsyncWebServer server;
    AsyncCallbackJsonWebHandler* postConfigHandler;

public:
    WebServer(Configuration& config);
    ~WebServer();
};