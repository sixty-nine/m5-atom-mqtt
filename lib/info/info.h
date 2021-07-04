#ifndef info_h
#define info_h

#include <Arduino.h>

namespace sixtynine
{
    struct espInfo {
        char *idfTarget;
        int cores;
        bool hasBt;
        bool hasBle;
        int chipRevision;
        uint32_t flashSize;
        bool embeddedFlash;
        uint32_t heapFree;
    };

    void gatherEspInfo(espInfo *info);
    void printEspInfo(espInfo *info);
    String getEspInfoJson(espInfo *info);
}

#endif
