#include "WifiManager.h"

WifiManager::WifiManager() { }

ConnectionStatus WifiManager::connect(const WiFiCredentials& wifiCredentials)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiCredentials.ssid, wifiCredentials.password);
#ifdef DEBUG
    Serial.printf("[WifiManager] Connecting to Wi-Fi %s", wifiCredentials.ssid);
#endif
    for (int remaining = WIFI_CONNECTION_TIMEOUT; remaining > 0; remaining -= WIFI_CONNECTION_RETRY_INTERVAL)
    {
#ifdef DEBUG
        Serial.print(".");
#endif
        switch (WiFi.status())
        {
        case WL_CONNECTED:
#ifdef DEBUG
            Serial.println("Done!");
            Serial.print("[WifiManager] IP address: ");
            Serial.println(WiFi.localIP());
#endif
            return ConnectionStatus::CONNECTED;

        case WL_WRONG_PASSWORD:
#ifdef DEBUG
            Serial.println("Failed! (Reason: Wrong password)");
#endif
            return ConnectionStatus::WRONG_PASSWORD;

        default:
            continue;
        }
        delay(WIFI_CONNECTION_RETRY_INTERVAL);
    }
#ifdef DEBUG
    Serial.println("Failed! (Reason: Connection timeout)");
#endif
    return ConnectionStatus::TIMEOUT;
}

void WifiManager::startAccessPoint()
{
#ifdef DEBUG
    Serial.print("[WifiManager] Starting access point...");
#endif
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(WIFI_ACCESS_POINT_SSID, WIFI_ACCESS_POINT_PASSWORD, 1, 0, 1);
#ifdef DEBUG
    Serial.println("Done!");

    Serial.print("[WifiManager] IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("[WifiManager] SSID: ");
    Serial.println(WiFi.softAPSSID());
    Serial.print("[WifiManager] Password: ");
    Serial.println(WIFI_ACCESS_POINT_PASSWORD);
#endif
}

bool WifiManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

WifiMode WifiManager::getMode() const
{
    return static_cast<WifiMode>(WiFi.getMode());
}
