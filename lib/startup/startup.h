#ifndef startup_h
#define startup_h

#include "state.h"
#include "mqtt.h"
#include "M5Atom.h"
#include "icons.h"

namespace sixtynine
{
    void startup(
        State *state,
        Mqtt *mqtt,
        Display *display,
        M5Atom *m5,
        bool hasMatrix = false,
        bool showLogo = true
    );
}

#endif
