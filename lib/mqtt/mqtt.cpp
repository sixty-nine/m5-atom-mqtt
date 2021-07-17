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
    _pubSubClient.setBufferSize(1024);

    auto cb = [onReceiveCallback](char* topic, byte* payload, unsigned int length)
    {
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
    bool result = _pubSubClient.publish(_outTopic, data.c_str(), data.length());

    if (!result) {
        Serial.println("[ERR] MQTT publish failed");
    }
}

void Mqtt::sendJson(String msgType, JsonObject *payload, bool pretty)
{
    String buff;
    StaticJsonDocument<1024> doc;
    JsonObject obj;

    doc["id"] = _clientId;
    doc["type"] = msgType;
    doc["timestamp"] = millis();

    if (NULL != payload) {
        obj = doc.createNestedObject("payload");
        obj.set(*payload);
    }

    if (pretty) {
        serializeJsonPretty(doc, buff);
    } else {
        serializeJson(doc, buff);
    }

    send(buff);
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
            sendJson("connect");
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
