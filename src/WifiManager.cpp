#include "WifiManager.h"

WifiManager::WifiManager()
{
    this->previousStatus = WiFi.status();
}

void WifiManager::connect(const WiFiCredentials &wifiCredentials)
{
    if (!(this->getMode() & WifiMode::ACCESS_POINT))
    {
        Serial.println("[WifiManager] Access point mode is not enabled. Entering station mode.");
        WiFi.mode(WIFI_STA);
    }
    this->connectionStartTime = millis();

    WiFi.begin(wifiCredentials.ssid, wifiCredentials.password);
#ifdef DEBUG
    Serial.printf("[WifiManager] Connecting to Wi-Fi \"%s\"...", wifiCredentials.ssid);
#endif
    // TODO: Handle status changes and connection errors.
}

void WifiManager::disconnect()
{
#ifdef DEBUG
    Serial.println("[WifiManager] Disconnecting from Wi-Fi...");
#endif
    WiFi.disconnect();
    this->connectionStartTime.reset();
}

void WifiManager::startAccessPoint()
{
#ifdef DEBUG
    Serial.print("[WifiManager] Starting access point...");
#endif
    // Destroy the current access point if it exists.
    WiFi.softAPdisconnect(true);

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

void WifiManager::stopAccessPoint()
{
#ifdef DEBUG
    Serial.println("[WifiManager] Stopping access point...");
#endif
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
}

bool WifiManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

WifiMode WifiManager::getMode() const
{
    return static_cast<WifiMode>(WiFi.getMode());
}

void WifiManager::onTick(uint32_t now)
{
    auto currentStatus = WiFi.status();
    if (currentStatus != this->previousStatus)
    {
#ifdef DEBUG
        Serial.printf("[WifiManager] Wi-Fi status changed from %d to %d.\n", previousStatus, currentStatus);
#endif
        switch (currentStatus)
        {
        case WL_CONNECTED:
            for (const auto &callback : this->onConnectCallbacks)
            {
                callback();
            }
            this->connectionStartTime.reset();
            break;

        case WL_WRONG_PASSWORD:
            for (const auto &callback : this->onErrorCallbacks)
            {
                callback(ConnectionStatus::WRONG_PASSWORD);
            }
            this->connectionStartTime.reset();
            WiFi.disconnect();

        case WL_DISCONNECTED:
            for (const auto &callback : this->onDisconnectCallbacks)
            {
                callback();
            }
        }
        this->previousStatus = currentStatus;
    }
    if (currentStatus != WL_CONNECTED && this->connectionStartTime.has_value())
    {
        if (now - this->connectionStartTime.value() > WIFI_CONNECTION_TIMEOUT)
        {
#ifdef DEBUG
            Serial.println("[WifiManager] Connection timeout reached.");
#endif
            for (const auto &callback : this->onErrorCallbacks)
            {
                callback(ConnectionStatus::TIMEOUT);
            }
            this->connectionStartTime.reset();
        }
    }
}

void WifiManager::onConnect(ConnectCallback callback)
{
    this->onConnectCallbacks.push_back(callback);
}

void WifiManager::onError(ErrorCallback callback)
{
    this->onErrorCallbacks.push_back(callback);
}

void WifiManager::onDisconnect(DisconnectCallback callback)
{
    this->onDisconnectCallbacks.push_back(callback);
}
