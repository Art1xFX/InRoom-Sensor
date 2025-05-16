#include "Configuration.h"

Configuration::Configuration(): wifiCredentials()
{
    int requiredSize = sizeof(WiFiCredentials);
    EEPROM.begin(requiredSize);
#ifdef DEBUG
    Serial.print("[Configuration] Initializing EEPROM with size: ");
    Serial.println(requiredSize);
#endif
}

Configuration::~Configuration()
{
#ifdef DEBUG
    Serial.println("[Configuration] Deinitializing EEPROM.");
#endif
    EEPROM.end();
}

const std::optional<const WiFiCredentials> Configuration::getWifiCredentials() const
{
    EEPROM.get(WIFI_CREDENTIALS_MAGIC_OFFSET, this->wifiCredentialsMagicValue);

    if (this->wifiCredentialsMagicValue != WIFI_CREDENTIALS_MAGIC_VALUE)
    {
#ifdef DEBUG
        Serial.println("[Configuration] No valid WiFi credentials signature found in EEPROM.");
#endif
        return std::nullopt;
    }

#ifdef DEBUG
    Serial.println("[Configuration] WiFi credentials from EEPROM:");
    Serial.print("[Configuration] SSID: ");
    Serial.println(this->wifiCredentials.ssid);
    Serial.print("[Configuration] Password: ");
    Serial.println(this->wifiCredentials.password);
#endif

    return this->wifiCredentials;
}

void Configuration::setWifiCredentials(const char *ssid, const char *password)
{
    strncpy(this->wifiCredentials.ssid, ssid, sizeof(this->wifiCredentials.ssid) - 1);
    strncpy(this->wifiCredentials.password, password, sizeof(this->wifiCredentials.password) - 1);
    
    if (this->wifiCredentialsMagicValue != WIFI_CREDENTIALS_MAGIC_VALUE)
    {
        this->wifiCredentialsMagicValue = WIFI_CREDENTIALS_MAGIC_VALUE;
        EEPROM.put(WIFI_CREDENTIALS_MAGIC_OFFSET, this->wifiCredentialsMagicValue);
    }
    EEPROM.put(WIFI_CREDENTIALS_OFFSET, this->wifiCredentials);
}

bool Configuration::save()
{
    return EEPROM.commit();
}