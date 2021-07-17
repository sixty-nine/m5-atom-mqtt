#ifndef MyMqtt_h
#define MyMqtt_h

#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "state.h"

#define MQTT_ON_RECEIVE_SIGNATURE void (*onReceive)(char*, char*)

class Mqtt
{
    public:
        Mqtt(
            const char *clientId,
            const char *mqttServer,
            const char *inTopic,
            const char *outTopic,
            void (*onReceiveCallback)(char*, char*)
        );
        void checkMqtt(State *state);
        JsonObject *createJsonObject();
        void send(String data);
        void sendJson(String msgType, JsonObject *payload = 0, bool pretty = true);
        void disconnect();

    private:
        WiFiClient _mqttClient;
        PubSubClient _pubSubClient;
        const char *_inTopic;
        const char *_outTopic;
        const char *_clientId;
    };

#endif
