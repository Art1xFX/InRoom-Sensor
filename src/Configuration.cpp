#include "Configuration.h"

WiFiCredentials::WiFiCredentials(const char *ssid, const char *password)
{
    strncpy(this->ssid, ssid, sizeof(this->ssid) - 1);
    this->ssid[sizeof(this->ssid) - 1] = '\0';

    strncpy(this->password, password, sizeof(this->password) - 1);
    this->password[sizeof(this->password) - 1] = '\0';
}

Configuration::Configuration(): wifiCredentials()
{
    int requiredSize = sizeof(WiFiCredentials) + sizeof(WIFI_CREDENTIALS_MAGIC_TYPE);
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

const WiFiCredentials* Configuration::getWifiCredentials() const
{
    EEPROM.get(WIFI_CREDENTIALS_MAGIC_OFFSET, this->wifiCredentialsMagicValue);

    if (this->wifiCredentialsMagicValue != WIFI_CREDENTIALS_MAGIC_VALUE)
    {
#ifdef DEBUG
        Serial.println("[Configuration] No valid WiFi credentials signature found in EEPROM.");
#endif
        return nullptr;
    }

    EEPROM.get(WIFI_CREDENTIALS_OFFSET, this->wifiCredentials);
#ifdef DEBUG
    Serial.println("[Configuration] WiFi credentials from EEPROM:");
    Serial.print("[Configuration] SSID: ");
    Serial.println(this->wifiCredentials.ssid);
    Serial.print("[Configuration] Password: ");
    Serial.println(this->wifiCredentials.password);
#endif

    return &this->wifiCredentials;
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

void Configuration::clearWifiCredentials()
{
    if (this->wifiCredentialsMagicValue == WIFI_CREDENTIALS_MAGIC_VALUE)
    {
        this->wifiCredentialsMagicValue = 0;
        EEPROM.put(WIFI_CREDENTIALS_MAGIC_OFFSET, this->wifiCredentialsMagicValue);
    }
}

bool Configuration::save()
{
    return EEPROM.commit();
}