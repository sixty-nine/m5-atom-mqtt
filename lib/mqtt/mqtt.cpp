#include "mqtt.h"

#include "PubSubClient.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include "icons.h"

Mqtt::Mqtt(
    const char *clientId,
    const char *mqttServer,
    const char *inTopic,
    const char *outTopic,
    void (*onReceiveCallback)(char*, char*)
)
{
    _clientId = clientId;
    _inTopic = inTopic;
    _outTopic = outTopic;
    _pubSubClient.setClient(_mqttClient);
    _pubSubClient.setServer(mqttServer, 1883);

    auto cb = [onReceiveCallback](char* topic, byte* payload, unsigned int length) {
        char buffer[length + 1];
        memset(buffer, 0, length + 1);

        for (int i = 0; i < length; i++)
        {
          buffer[i] = payload[i];
        }

        onReceiveCallback(topic, buffer);
    };

    _pubSubClient.setCallback(cb);
}

void Mqtt::disconnect()
{
    _pubSubClient.disconnect();
}

void Mqtt::send(String data)
{
    _pubSubClient.publish(_outTopic, data.c_str());
}

void Mqtt::checkMqtt(State *state)
{
    auto wifiOk = state->isWifiConnected();
    auto mqttOk = _pubSubClient.connected();

    if (mqttOk && wifiOk)
    {
        _pubSubClient.loop();
        return;
    }

    if (!wifiOk)
    {
        if (mqttOk) {
            _pubSubClient.disconnect();
        }

        mqttOk = false;
        state->setMqttConnected(false);
    }

    if (!mqttOk && wifiOk)
    {
        Serial.print("[MQTT] Attempting MQTT connection...");

        if (_pubSubClient.connect(_clientId))
        {
            Serial.println("connected");

            String message = "Connect from " + String(_clientId);
            _pubSubClient.publish(_outTopic, message.c_str());

            _pubSubClient.subscribe(_inTopic);

            state->setMqttConnected(true);

        } else {

            Serial.print("failed, rc=");
            Serial.print(_pubSubClient.state());
            Serial.println(" try again in  seconds");
            _pubSubClient.disconnect();
            state->setMqttConnected(false);
        }
    }

}
