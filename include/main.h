#pragma once

#include <Arduino.h>

#include "Configuration.h"
#include "WifiManager.h"
#include "WebServer.h"
#include "MqttManager.h"
#include "sensor/LightSensor.h"

#define MQTT_DATA_TOPIC_LIGHT_SENSOR_SUFFIX "/light"

Configuration *configuration;
WifiManager *wifiManager;
WebServer *webServer;
MqttManager* mqttManager;
LightSensor *lightSensor;
char lightSensorDataTopic[255 + sizeof(MQTT_DATA_TOPIC_LIGHT_SENSOR_SUFFIX)];

void setup();

void loop();
