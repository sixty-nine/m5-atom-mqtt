#ifndef info_h
#define info_h

#include <Arduino.h>
#include <WiFi.h>

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

    struct networkInfo {
        IPAddress localIp;
        IPAddress subnetMask;
        IPAddress gatewayIp;
        uint8_t localMacAddr[6];
        long rssi;
    };

    struct status {
        char *deviceId;
        unsigned long upTime;

    };

    void gatherEspInfo(espInfo *info);
    void gatherNetworkInfo(networkInfo *info);

    String getEspInfoJson(espInfo *info);

    void printEspInfo(espInfo *info);
    void printNetworkInfo(networkInfo *info);
    void printStatus();
}

#endif
