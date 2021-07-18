#include "tasks.h"
#include <chrono>
#include <pthread.h>
#include <thread>
#include <WiFi.h>
#include "network.h"
#include "icons.h"
#include "secrets.h"
#include "mqtt.h"
#include "state.h"
#include "secrets.h"
#include "icons.h"
#include <ESP32Ping.h>

namespace sixtynine
{
    using namespace std::chrono;
    using namespace std::this_thread;

    void displayBuffer(void * buff)
    {
        M5.dis.displaybuff((uint8_t *)buff, 0, 0);
    }

    void GenericTask::run(void* data)
    {
        _run(data);
    }

    void doSoftwareReset(State *state, Mqtt *mqtt, String reason)
    {
        if (state->isWifiConnected())
        {
            StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
            JsonObject object = doc.to<JsonObject>();
            object["reason"] = reason.c_str();
            mqtt->sendJson("remote-reset", &object);
            std::this_thread::sleep_for(milliseconds { 1000 });

            // TODO: disable ReconnectWifiTask
            WiFi.disconnect();
            std::this_thread::sleep_for(milliseconds { 1000 });
        }


        ESP.restart();
    }


    void SoftwareResetTask(void* data)
    {
        auto td = (taskData *)data;
        State *state = td->state;
        Mqtt *mqtt = td->mqtt;

        Serial.println("[TASK] Starting Task SoftwareReset");

        std::this_thread::sleep_for(TIME_BETWEEN_RESETS_MS);

        Serial.println("[RST] ************** Recurrent Reset **************");
        Serial.println("\n");

        doSoftwareReset(state, mqtt, "software");
    }

    /**
     * Try to connect to WiFi if not connected.
     * Triggers task_animateConnection().
     */
    void ReconnectWifiTask(void* data)
    {
        static int status = WL_IDLE_STATUS;

        auto td = (taskData *)data;
        State *state = td->state;
        Mqtt *mqtt = td->mqtt;

        Serial.println("[TASK] Starting Task reconnectWifi");

        while (true)
        {
            if (WiFiClass::status() == WL_CONNECTED)
            {
                if (!state->isWifiConnected())
                {
                    Serial.println("[WIFI] Connection to WLAN router successful");
                    printCurrentNet();
                    printWifiData();
                    state->setWifiConnected(true);
                }
            }

            if (WiFiClass::status() != WL_CONNECTED)
            {
                state->setWifiConnected(false);

                Serial.println("[WIFI] Trying to connect to WLAN router");


                GenericTask t = GenericTask("animate-connection-task", AnimateConnectionTask);
                t.start(data);

                if (state->isMqttConnected()) {
                    mqtt->disconnect();
                }

                WiFi.disconnect();
                sleep_for(milliseconds { 1000 });

                WiFi.config(IP_ADDR, GATEWAY, SUBNET);

                status = WiFi.begin(SSID, PASSWORD);

                sleep_for(milliseconds { 5000 });

                Serial.printf(
                    "[WIFI] Wifi status = %s [%d]\r\n",
                    WifiStatusToString(status).c_str(),
                    status
                );
            }

            sleep_for(milliseconds { 10 });
        }
    }

    void ReconnectMqttTask(void *data)
    {

        auto td = (taskData *)data;
        State *state = td->state;
        Mqtt *mqtt = td->mqtt;

        Serial.println("[TASK] Starting Task reconnectMqtt");

        while (true)
        {
            mqtt->checkMqtt(state);
            sleep_for(milliseconds { 100 });
        }
    }

    /**
     * Show an animation while trying to connect to WiFi.
     * Show an OK icon and exit when connected.
     */
    void AnimateConnectionTask(void* data)
    {
        static bool started = false;

        auto td = (taskData *)data;
        State *state = td->state;
        Display *display = td->display;

        if (!started)
        {
            started = true;

            Serial.println("[TASK] Starting Task animateConnection");

            auto anim = new animations::NetworkConnectAnimation();

            unsigned char ok[77];
            copyIcon(icons::ok, ok, { 0x55, 0xaa, 0x00 });

            bool connected = false;
            while (!connected)
            {
                display->showAnimation(anim->getFrames());
                connected = state->isWifiConnected();
            }

            displayBuffer(ok);

            sleep_for(milliseconds { 2000 });

            M5.dis.clear();

            started = false;
        }

        Serial.println("[TASK] Stopping Task animateConnection");
        vTaskDelete(NULL);
    }

    /**
     * Ping the gateway and invalidate the wifi connection if it fails.
     */
    void GatewayPingTask(void* data)
    {
        auto td = (taskData *)data;
        Mqtt *mqtt = td->mqtt;
        State *state = td->state;
        Display *display = td->display;

        auto anim = new animations::HeartAnimation();

        Serial.println("[TASK] Starting Task pingGateway");

        while (true)
        {
            if (state->isWifiConnected())
            {
                Serial.println("[PING] Ping sent to gateway");

                bool success = Ping.ping(GATEWAY);

                if (!success)
                {
                    Serial.println("[PING] Gateway ping failed");
                    state->setWifiConnected(false);
                }
                else {
                    Serial.println("[PING] OK");
                    mqtt->sendJson("heartbeat");
                    display->showAnimation(anim->getFrames());
                    display->showAnimation(anim->getFrames());
                }
            }

            sleep_for(milliseconds { 60000 });
        }
    }

    void ButtonWatchTask(void *data)
    {
        auto td = (taskData *)data;
        Mqtt *mqtt = td->mqtt;
        M5Atom *m5 = td->m5;
        Display *display = td->display;

        auto anim = new animations::PressAnimation();

        Serial.println("[TASK] Starting Task ButtonWatch");

        while (true)
        {
            if (m5->Btn.wasPressed())
            {
                display->showAnimation(anim->getFrames());

                Serial.println("[SYS] Button pressed");
                mqtt->sendJson("button-pressed");

                m5->dis.clear();
            }

            m5->Btn.read();
        }
    }
}
