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
}

void loop()
{
    if (!wifiManager->isConnected() && !(wifiManager->getMode() & WifiMode::STATION))
    {
        auto credentials = configuration->getWifiCredentials();
        if (credentials != nullptr) 
        {
            Serial.println("[Main] Connecting to Wi-Fi...");
            wifiManager->connect(*credentials);
        }
        else 
        {
            Serial.println("[Main] No Wi-Fi credentials found, starting AP mode");
            wifiManager->startAccessPoint();
            webServer = new WebServer(*configuration);
        }
    }
    delay(1000);
}
