#include "Configuration.h"

WiFiCredentials::WiFiCredentials(const char *ssid, const char *password)
{
    strncpy(this->ssid, ssid, sizeof(this->ssid) - 1);
    this->ssid[sizeof(this->ssid) - 1] = '\0';

    strncpy(this->password, password, sizeof(this->password) - 1);
    this->password[sizeof(this->password) - 1] = '\0';
}

MqttEndpoint::MqttEndpoint(const char *host, uint16_t port)
{
    strncpy(this->host, host, sizeof(this->host) - 1);
    this->host[sizeof(this->host) - 1] = '\0';
    this->port = port;
}

Configuration::Configuration(): wifiCredentials(), mqttEndpoint()
{
    int requiredSize = sizeof(WiFiCredentials) + sizeof(WIFI_CREDENTIALS_MAGIC_TYPE) +
                        sizeof(MqttEndpoint) + sizeof(MQTT_ENDPOINT_MAGIC_TYPE);
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
    Serial.print("  SSID: ");
    Serial.println(this->wifiCredentials.ssid);
    Serial.print("  Password: ");
    Serial.println(this->wifiCredentials.password);
#endif

    return &this->wifiCredentials;
}

void Configuration::setWifiCredentials(const char *ssid, const char *password)
{
    strncpy(this->wifiCredentials.ssid, ssid, sizeof(this->wifiCredentials.ssid) - 1);
    strncpy(this->wifiCredentials.password, password, sizeof(this->wifiCredentials.password) - 1);
#ifdef DEBUG
    Serial.print("[Configuration] Checking WiFi credentials magic value ... ");
#endif
    if (this->wifiCredentialsMagicValue != WIFI_CREDENTIALS_MAGIC_VALUE)
    {
#ifdef DEBUG
        Serial.println("Missing.");
        Serial.print("[Configuration] Writing WiFi credentials magic value: ");
        Serial.println(WIFI_CREDENTIALS_MAGIC_VALUE);
#endif
        this->wifiCredentialsMagicValue = WIFI_CREDENTIALS_MAGIC_VALUE;
        EEPROM.put(WIFI_CREDENTIALS_MAGIC_OFFSET, this->wifiCredentialsMagicValue);
    }
#ifdef DEBUG
    else
    {
        Serial.println("Found.");
    }
    Serial.println("[Configuration] Writing WiFi credentials to EEPROM:");
    Serial.print("  SSID: ");
    Serial.println(this->wifiCredentials.ssid);
    Serial.print("  Password: ");
    Serial.println(this->wifiCredentials.password);
#endif
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

const MqttEndpoint *Configuration::getMqttEndpoint() const
{
    EEPROM.get(MQTT_ENDPOINT_MAGIC_OFFSET, this->mqttEndpointMagicValue);

    if (this->mqttEndpointMagicValue != MQTT_ENDPOINT_MAGIC_VALUE)
    {
#ifdef DEBUG
        Serial.println("[Configuration] No valid MQTT endpoint signature found in EEPROM.");
#endif
        return nullptr;
    }

    EEPROM.get(MQTT_ENDPOINT_OFFSET, this->mqttEndpoint);
#ifdef DEBUG
    Serial.println("[Configuration] MQTT endpoint from EEPROM:");
    Serial.print("  Host: ");
    Serial.println(this->mqttEndpoint.host);
    Serial.print("  Port: ");
    Serial.println(this->mqttEndpoint.port);
#endif
    return &this->mqttEndpoint;
}

void Configuration::setMqttEndpoint(const char *host, uint16_t port)
{
    strncpy(this->mqttEndpoint.host, host, sizeof(this->mqttEndpoint.host) - 1);
    this->mqttEndpoint.port = port;
#ifdef DEBUG
    Serial.print("[Configuration] Checking MQTT endpoint magic value ... ");
#endif
    if (this->mqttEndpointMagicValue != MQTT_ENDPOINT_MAGIC_VALUE)
    {
#ifdef DEBUG
        Serial.println("Missing.");
        Serial.print("[Configuration] Writing MQTT endpoint magic value: ");
        Serial.println(MQTT_ENDPOINT_MAGIC_VALUE);
#endif
        this->mqttEndpointMagicValue = MQTT_ENDPOINT_MAGIC_VALUE;
        EEPROM.put(MQTT_ENDPOINT_MAGIC_OFFSET, this->mqttEndpointMagicValue);
    }
#ifdef DEBUG
    else
    {
        Serial.println("Found.");
    }
    Serial.println("[Configuration] Writing MQTT endpoint to EEPROM:");
    Serial.print("  Host: ");
    Serial.println(this->mqttEndpoint.host);
    Serial.print("  Port: ");
    Serial.println(this->mqttEndpoint.port);
#endif
    EEPROM.put(MQTT_ENDPOINT_OFFSET, this->mqttEndpoint);
}

void Configuration::clearMqttEndpoint()
{
    if (this->mqttEndpointMagicValue == MQTT_ENDPOINT_MAGIC_VALUE)
    {
        this->mqttEndpointMagicValue = 0;
        EEPROM.put(MQTT_ENDPOINT_MAGIC_OFFSET, this->mqttEndpointMagicValue);
    }
}

bool Configuration::save()
{
#ifdef DEBUG
    Serial.println("[Configuration] Saving configuration to EEPROM.");
#endif
    return EEPROM.commit();
}