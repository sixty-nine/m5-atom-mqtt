#ifndef tasks_h
#define tasks_h

#include "M5Atom.h"
#include "state.h"
#include "mqtt.h"
#include "icons.h"

namespace sixtynine
{
    struct taskData {
        State *state;
        Mqtt *mqtt;
        Display *display;
        M5Atom *m5;
    };

    class GenericTask: public Task
    {
      public:
        explicit GenericTask(
            const char *name,
            void (*run)(void*),
            uint8_t priority = 5,
            uint16_t taskSize = 10240
        ) : Task(name, taskSize, priority)
        {
            _run = run;
        }

        void run(void* data) override;

      private:
        void (*_run)(void*);
    };

    void SoftwareResetTask(void* data);
    void ReconnectWifiTask(void* data);
    void ReconnectMqttTask(void *data);
    void GatewayPingTask(void* data);
    void AnimateConnectionTask(void* data);
    void ButtonWatchTask(void* data);

    void doSoftwareReset(State *state, Mqtt *mqtt, String reason);
}

#endif
