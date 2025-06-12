#include "main.h"

const int LED_PIN = LED_BUILTIN;

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
    Serial.println("\n[Main] Starting...");
#endif
    configuration = new Configuration();
    wifiManager = new WifiManager();
    wifiManager->onConnect([]()
    {
        Serial.println("[Main] Wi-Fi connected.");
    });
    wifiManager->onError([](ConnectionStatus status)
    {
        switch (status)
        {
        case ConnectionStatus::TIMEOUT:
            Serial.println("[Main] Wi-Fi connection error (timeout).");
            break;

        case ConnectionStatus::WRONG_PASSWORD:
            Serial.println("[Main] Wi-Fi connection error (wrong password).");
            break;
        }
        wifiManager->startAccessPoint();
        webServer = new WebServer(*configuration, *wifiManager);
    });

    auto wifiCredentials = configuration->getWifiCredentials();
    if (wifiCredentials)
    {
        wifiManager->connect(*wifiCredentials);
    }
    else
    {
        Serial.println("[Main] No Wi-Fi credentials found. Starting access point.");
        wifiManager->startAccessPoint();
        webServer = new WebServer(*configuration, *wifiManager);
    }
}

void loop()
{
    wifiManager->tick();
}
