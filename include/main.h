#pragma once

#include <Arduino.h>

#include "Configuration.h"
#include "WifiManager.h"
#include "WebServer.h"


Configuration *configuration;
WifiManager *wifiManager;
WebServer *webServer;

void setup();

void loop();
