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
#include <ESP32Ping.h>

namespace sixtynine
{
    using namespace std::chrono;
    using namespace std::this_thread;

    void displayBuffer(void * buff)
    {
        M5.dis.displaybuff((uint8_t *)buff, 0, 0);
    }

    void SoftwareResetTask::run(void* data)
    {
        auto td = (taskData *)data;
        State *state = td->state;

        Serial.println("[TASK] Starting Task SoftwareReset");

        std::this_thread::sleep_for(TIME_BETWEEN_RESETS_MS);

        Serial.println("[RST] *******Recurrent Reset*******");

        if (state->isWifiConnected())
        {
            WiFi.disconnect();
            std::this_thread::sleep_for(milliseconds { 1000 });
        }

        ESP.restart();
    }

    /**
     * Try to connect to WiFi if not connected.
     * Triggers task_animateConnection().
     */
    void ReconnectWifi::run(void* data)
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


                auto t3 = AnimateConnection(10240, 5);
                t3.start(data);

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

    void ReconnectMqtt::run(void *data)
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
    void AnimateConnection::run(void* data)
    {
        static bool started = false;

        auto td = (taskData *)data;
        State *state = td->state;

        if (!started)
        {
            started = true;

            Serial.println("[TASK] Starting Task animateConnection");

            int d = 500;
            unsigned char network1[77];
            unsigned char network2[77];
            unsigned char network3[77];
            unsigned char ok[77];

            copyIcon(icons::network1, network1, { 0x45, 0x8b, 0xd1 });
            copyIcon(icons::network2, network2, { 0x32, 0x65, 0x98 });
            copyIcon(icons::network3, network3, { 0x23, 0x46, 0x6a });
            copyIcon(icons::ok, ok, { 0x55, 0xaa, 0x00 });

            bool connected = false;
            while (!connected)
            {
                displayBuffer(network1);
                sleep_for(milliseconds { d });
                displayBuffer(network2);
                sleep_for(milliseconds { d });
                displayBuffer(network3);
                sleep_for(milliseconds { 2 * d });
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
    void GatewayPingTask::run(void* data)
    {
        auto td = (taskData *)data;
        State *state = td->state;

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
                }
            }

            sleep_for(milliseconds { 60000 });
        }
    }

}
