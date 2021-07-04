#ifndef startup_h
#define startup_h

#include "state.h"
#include "mqtt.h"

namespace sixtynine
{
    void startup(State *state, Mqtt *mqtt, bool hasMatrix = false, bool showLogo = true);
}

#endif
