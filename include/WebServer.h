#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

#include "Configuration.h"
#include "WifiManager.h"
#include "json/Configuration.h"


class WebServer {
private:
    AsyncWebServer server;
    AsyncCallbackJsonWebHandler* postConfigHandler;

public:
    WebServer(Configuration& config, WifiManager& wifiManager);
    ~WebServer();
};