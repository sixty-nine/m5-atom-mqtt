#ifndef info_h
#define info_h

#include <Arduino.h>
#include <WiFi.h>
#include "mqtt.h"

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
    };

    struct memInfo {
        uint32_t heapSize;
        uint32_t freeHeap;
        uint32_t minFreeHeap;
        uint32_t psramSize;
        uint32_t freePsramSize;
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
    void gatherMemInfo(memInfo *info);

    void sendDeviceInfo(Mqtt *mqtt, espInfo *info, networkInfo *netInfo);
    void sendStatus(Mqtt *mqtt, memInfo *info);

    void printEspInfo(espInfo *info);
    void printMemInfo(memInfo *info);
    void printNetworkInfo(networkInfo *info);
    void printStatus();
}

#endif
