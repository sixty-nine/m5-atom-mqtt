#include "M5Atom.h"
#include "startup.h"
#include "state.h"
#include "info.h"
#include "mqtt.h"
#include "secrets.h"

using namespace sixtynine;

void onReceive(char *topic, char *payload);

State *state = new State();
Mqtt *mqtt = new Mqtt("atom-matrix-1", MQTT_SERVER, IN_TOPIC, OUT_TOPIC, onReceive);

void setup()
{
    M5.begin(true, false, true);

    startup(state, mqtt, true);
}

void loop()
{
    M5.update();
}



void onReceive(char *topic, char *payload)
{
    Serial.print("[MQTT] Message arrived: ");
    Serial.println(payload);

    if (String(payload).equals("info"))
    {
        espInfo info;
        gatherEspInfo(&info);

        String json = getEspInfoJson(&info);
        Serial.println("[MQTT] " + json);
        mqtt->send(json);
    }
}

