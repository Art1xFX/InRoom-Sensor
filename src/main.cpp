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
    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
    if (!wifiManager->isConnected() && !(wifiManager->getMode() & WifiMode::STATION))
    {
        auto credentials = configuration->getWifiCredentials();
        if (credentials != nullptr)
        {
            Serial.println("[Main] Connecting to Wi-Fi...");
            for (int i = 0; i < 3; i++)
            {
                switch (wifiManager->connect(*credentials))
                {
                case ConnectionStatus::TIMEOUT:
                    delay(1000);
                    continue;
                case ConnectionStatus::WRONG_PASSWORD:
                    wifiManager->startAccessPoint();
                    webServer = new WebServer(*configuration);
                    return;
                case ConnectionStatus::CONNECTED:
                    return;
                }
            }
        }
        Serial.println("[Main] No Wi-Fi credentials found, starting AP mode");
        wifiManager->startAccessPoint();
        webServer = new WebServer(*configuration);
    }
    delay(1000);
}
