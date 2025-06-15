#pragma once

#include <Arduino.h>

#include "Configuration.h"
#include "WifiManager.h"
#include "WebServer.h"
#include "MqttManager.h"
#include "sensor/LightSensor.h"


Configuration *configuration;
WifiManager *wifiManager;
WebServer *webServer;
MqttManager* mqttManager;
LightSensor *lightSensor;
char dataTopic[256 * 2];

void setup();

void loop();
