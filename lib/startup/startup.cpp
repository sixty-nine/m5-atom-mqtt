#include "startup.h"
#include "M5Atom.h"
#include "tasks.h"
#include "mqtt.h"
#include "secrets.h"
#include <pthread.h>
#include <thread>

namespace sixtynine
{
    const unsigned char  logoImageData[272]=
    {
    /* width  018 */ 0x12,
    /* height 005 */ 0x05,
    /* Line   000 */ 0xff,0xff,0xff, 0xe1,0xe1,0xe1, 0xce,0xce,0xce, 0xb4,0xb4,0xb4, 0x9a,0x9a,0x9a, 0x84,0x84,0x84, 0x75,0x75,0x75, 0x64,0x64,0x64, 0x58,0x58,0x58, 0x4b,0x4b,0x4b, 0x3d,0x3d,0x3d, 0x2f,0x2f,0x2f, 0x1b,0x1b,0x1b, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, //
    /* Line   001 */ 0xe1,0xe1,0xe1, 0xce,0xce,0xce, 0xb4,0xb4,0xb4, 0x9a,0x9a,0x9a, 0x84,0x84,0x84, 0x75,0x75,0x75, 0x64,0x64,0x64, 0x58,0x58,0x58, 0x4b,0x4b,0x4b, 0x3d,0x3d,0x3d, 0x2f,0x2f,0x2f, 0x1b,0x1b,0x1b, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, //
    /* Line   002 */ 0xce,0xce,0xce, 0xb4,0xb4,0xb4, 0x9a,0x9a,0x9a, 0x84,0x84,0x84, 0x75,0x75,0x75, 0x64,0x64,0x64, 0x58,0x58,0x58, 0x4b,0x4b,0x4b, 0x3d,0x3d,0x3d, 0x2f,0x2f,0x2f, 0x1b,0x1b,0x1b, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, //
    /* Line   003 */ 0xb4,0xb4,0xb4, 0x9a,0x9a,0x9a, 0x84,0x84,0x84, 0x75,0x75,0x75, 0x64,0x64,0x64, 0x58,0x58,0x58, 0x4b,0x4b,0x4b, 0x3d,0x3d,0x3d, 0x2f,0x2f,0x2f, 0x1b,0x1b,0x1b, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, //
    /* Line   004 */ 0x9a,0x9a,0x9a, 0x84,0x84,0x84, 0x75,0x75,0x75, 0x64,0x64,0x64, 0x58,0x58,0x58, 0x4b,0x4b,0x4b, 0x3d,0x3d,0x3d, 0x2f,0x2f,0x2f, 0x1b,0x1b,0x1b, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, //
    };

    void startupAnimate(M5Atom *m5)
    {
        m5->dis.animation(
            (uint8_t *)logoImageData,
            200,
            LED_DisPlay::kMoveLeft,
            0x0d
        );
    }

    void startup(
        State *state,
        Mqtt *mqtt,
        M5Atom *m5,
        bool hasMatrix,
        bool showLogo
    ) {
        if (hasMatrix && showLogo)
        {
            startupAnimate(m5);
            delay(3000);
        }

        taskData data = { state, mqtt, m5 };

        GenericTask t1 = GenericTask("reconnect-wifi-task", SoftwareResetTask);
        GenericTask t2 = GenericTask("reconnect-mqtt-task", ReconnectMqttTask);
        GenericTask t3 = GenericTask("gateway-ping-task", GatewayPingTask);
        GenericTask t4 = GenericTask("software-reset-task", ReconnectWifiTask);
        GenericTask t5 = GenericTask("button-watch-task", ButtonWatchTask);
        t1.start(&data);
        t2.start(&data);
        t3.start(&data);
        t4.start(&data);
        t5.start(&data);

        Serial.println("[SYS] System started up");
    }
}
