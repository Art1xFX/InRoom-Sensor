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
    motionSensor = new MotionSensor();

    configuration->onMqttDataTopicChange([](const char *topic)
    {
        Serial.printf("[Main] MQTT data topic changed: %s\n", topic);

        strcpy(lightSensorDataTopic, topic);
        strcat(lightSensorDataTopic, MQTT_DATA_TOPIC_LIGHT_SENSOR_SUFFIX);

        strcpy(motionSensorDataTopic, topic);
        strcat(motionSensorDataTopic, MQTT_DATA_TOPIC_MOTION_SENSOR_SUFFIX);
    });

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
    wifiManager->onDisconnect([]()
    {
        Serial.println("[Main] Wi-Fi disconnected.");
    });

    mqttManager->onConnect([]()
    {
        mqttRetriesLeft = 3;
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
        if (mqttRetriesLeft > 0)
        {
            Serial.printf("[Main] Retrying (%d retries left)...\n", mqttRetriesLeft);
            mqttRetriesLeft--;
            mqttManager->connect(*configuration->getMqttEndpoint());
            return;
        }
        else
        {
            Serial.println("[Main] No more MQTT retries left.");
            wifiManager->disconnect();
            wifiManager->startAccessPoint();
            if (webServer == nullptr)
            {
                webServer = new WebServer(*configuration, *wifiManager);
            }
        }
    });

    lightSensor->setInterval(500);
    lightSensor->onChange([](float value)
    {
        Serial.printf("[Main] Light sensor value changed: %.2f lux\n", value);
        char payload[16];
        snprintf(payload, sizeof(payload), "%.2f", value);
        Serial.printf("[Main] Publishing to MQTT topic '%s': %s\n", lightSensorDataTopic, payload);
        mqttManager->publish(lightSensorDataTopic, payload);
    });

    motionSensor->setInterval(2000);
    motionSensor->onChange([](bool motionDetected)
    {
        auto payload = motionDetected ? "true" : "false";
        Serial.printf("[Main] Motion sensor value changed: %s\n", payload);
        Serial.printf("[Main] Publishing to MQTT topic '%s': %s\n", motionSensorDataTopic, payload);
        mqttManager->publish(motionSensorDataTopic, payload);
    });

    auto mqttDataTopic = configuration->getMqttDataTopic();
    if (mqttDataTopic != nullptr)
    {
        strcpy(lightSensorDataTopic, mqttDataTopic);
        strcat(lightSensorDataTopic, MQTT_DATA_TOPIC_LIGHT_SENSOR_SUFFIX);
        Serial.printf("[Main] MQTT data topic set: %s\n", lightSensorDataTopic);

        strcpy(motionSensorDataTopic, mqttDataTopic);
        strcat(motionSensorDataTopic, MQTT_DATA_TOPIC_MOTION_SENSOR_SUFFIX);
        Serial.printf("[Main] MQTT data topic set: %s\n", motionSensorDataTopic);
    }

    auto wifiCredentials = configuration->getWifiCredentials();
    if (wifiCredentials != nullptr)
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
    motionSensor->tick();
}
