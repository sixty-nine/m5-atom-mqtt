#ifndef state_h
#define state_h

#include <FreeRTOS.h>

class State
{
    public:
        State();
        bool isWifiConnected();
        bool isMqttConnected();
        void setWifiConnected(bool state);
        void setMqttConnected(bool state);
        void logState();

    private:
        bool _wifiConnected = false;
        bool _mqttConnected = false;
        SemaphoreHandle_t _semaphore = NULL;

};

#endif
