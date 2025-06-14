#pragma once

#include <Arduino.h>

#include "Configuration.h"
#include "WifiManager.h"
#include "WebServer.h"
#include "MqttManager.h"


Configuration *configuration;
WifiManager *wifiManager;
WebServer *webServer;
MqttManager* mqttManager;

void setup();

void loop();
