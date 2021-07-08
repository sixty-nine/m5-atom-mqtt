#include "M5Atom.h"
#include "startup.h"
#include "state.h"
#include "info.h"
#include "mqtt.h"
#include "secrets.h"
#include "tasks.h"

using namespace sixtynine;

void onReceive(char *topic, char *payload);

State *state = new State();

Mqtt *mqtt = new Mqtt(
    MQTT_CLIENT_ID,
    MQTT_SERVER,
    IN_TOPIC,
    OUT_TOPIC,
    onReceive
);

void setup()
{
    M5.begin(true, false, true);

    startup(state, mqtt, &M5, true);
}

void loop()
{
}

// ============================================================================

void showInfo()
{
    espInfo info;
    gatherEspInfo(&info);

    String json = getEspInfoJson(&info);
    Serial.println("[MQTT] " + json);
    mqtt->send(json);
}

void showStatus()
{
    Serial.println("STATUS");
}

void resetDevice()
{
    Serial.println("[RST] ************** Remote Reset **************");
    Serial.println("\n");
    doSoftwareReset(state);
}

void onReceive(char *topic, char *payload)
{
    Serial.print("[MQTT] Message arrived: ");
    Serial.println(payload);

    String myPayload = String(payload);
    myPayload.toLowerCase();

    if (myPayload.equals("info")) showInfo();
    else if (myPayload.equals("status")) showStatus();
    else if (myPayload.equals("reset")) resetDevice();
    else if (myPayload.startsWith("display")) resetDevice();
    else if (myPayload.startsWith("draw")) resetDevice();
}

