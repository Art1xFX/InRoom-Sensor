#pragma once

#include <Arduino.h>

#include "Configuration.h"
#include "WifiManager.h"
#include "WebServer.h"
#include "MqttManager.h"
#include "sensor/LightSensor.h"
#include "sensor/MotionSensor.h"

#define MQTT_DATA_TOPIC_LIGHT_SENSOR_SUFFIX "/light"
#define MQTT_DATA_TOPIC_MOTION_SENSOR_SUFFIX "/motion"

#define WIFI_RETRIES 5
#define MQTT_RETRIES 5

Configuration *configuration;
WifiManager *wifiManager;
WebServer *webServer;
MqttManager* mqttManager;
LightSensor *lightSensor;
MotionSensor *motionSensor;
uint8_t wifiRetriesLeft = 0;
uint8_t mqttRetriesLeft = 0;
char lightSensorDataTopic[255 + sizeof(MQTT_DATA_TOPIC_LIGHT_SENSOR_SUFFIX)];
char motionSensorDataTopic[255 + sizeof(MQTT_DATA_TOPIC_MOTION_SENSOR_SUFFIX)];

void setup();

void loop();
