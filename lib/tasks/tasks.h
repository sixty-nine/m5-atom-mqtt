#ifndef tasks_h
#define tasks_h

#include "M5Atom.h"
#include "state.h"
#include "mqtt.h"

namespace sixtynine
{
    struct taskData {
        State *state;
        Mqtt *mqtt;
    };

    class ReconnectWifi: public Task
    {
      public:
        explicit ReconnectWifi(uint16_t taskSize = 10240, uint8_t priority = 5)
          : Task("reconnect-wifi", taskSize, priority)
        {}

        void run(void* data) override;
    };

    class ReconnectMqtt: public Task
    {
      public:
        explicit ReconnectMqtt(uint16_t taskSize = 10240, uint8_t priority = 5)
          : Task("reconnect-mqtt", taskSize, priority)
        {}

        void run(void* data) override;
    };

    class AnimateConnection: public Task
    {
      public:
        explicit AnimateConnection(uint16_t taskSize = 10240, uint8_t priority = 5)
          : Task("animate-connection", taskSize, priority)
        {}

        void run(void* data) override;
    };

    class GatewayPingTask: public Task
    {
      public:
        explicit GatewayPingTask(uint16_t taskSize = 10240, uint8_t priority = 5)
          : Task("ping-gateway", taskSize, priority)
        {}

        void run(void* /*data*/) override;
    };

}

#endif
