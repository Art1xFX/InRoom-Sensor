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
    mqttManager = new MqttManager(*configuration);
    lightSensor = new LightSensor();

    wifiManager->onConnect([]()
    {
        Serial.println("[Main] Wi-Fi connected.");
        mqttManager->connect(*configuration->getMqttEndpoint());
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
        if (webServer == nullptr)
        {
            webServer = new WebServer(*configuration, *wifiManager);
        }
    });

    mqttManager->onConnect([]()
    {
        Serial.println("[Main] MQTT connected.");
        if (webServer != nullptr)
        {
            delete webServer;
            webServer = nullptr;
        }
        wifiManager->stopAccessPoint();
    });
    mqttManager->onDisconnect([]()
    {
        Serial.println("[Main] MQTT disconnected.");
        wifiManager->disconnect();
        wifiManager->startAccessPoint();
        if (webServer == nullptr)
        {
            webServer = new WebServer(*configuration, *wifiManager);
        }
    });

    lightSensor->setInterval(500);
    lightSensor->onChange([](float value)
    {
        Serial.printf("[Main] Light sensor value changed: %.2f lux\n", value);
        char payload[16];
        // TODO: Change topic length to match the actual topic length using macros or constants.
        char topic[256 * 2];
        snprintf(payload, sizeof(payload), "%.2f", value);
        strcpy(topic, configuration->getMqttDataTopic());
        strcat(topic, "/light");
        mqttManager->publish(topic, payload);
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
    lightSensor->tick();
}
