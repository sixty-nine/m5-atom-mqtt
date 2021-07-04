#include "info.h"

#include <ArduinoJson.h>

namespace sixtynine
{
    void gatherEspInfo(espInfo *info)
    {
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);

        info->idfTarget = (char*)CONFIG_IDF_TARGET;
        info->cores = chip_info.cores;
        info->hasBt = chip_info.features & CHIP_FEATURE_BT;
        info->hasBle = chip_info.features & CHIP_FEATURE_BLE;
        info->chipRevision = chip_info.revision;
        info->flashSize = spi_flash_get_chip_size();
        info->embeddedFlash = chip_info.features & CHIP_FEATURE_EMB_FLASH;
        info->heapFree = esp_get_minimum_free_heap_size();
    }

    void printEspInfo(espInfo *info)
    {
        Serial.printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
                info->idfTarget,
                info->cores,
                info->hasBt ? "/BT" : "",
                info->hasBle ? "/BLE" : "");

        Serial.printf("silicon revision %d, ", info->chipRevision);

        Serial.printf("%dMB %s flash\n", info->flashSize / (1024 * 1024),
                info->embeddedFlash ? "embedded" : "external");

        Serial.printf("Minimum free heap size: %d Kb\n", info->heapFree / 1024);
    }

    String getEspInfoJson(espInfo *info)
    {
        String buff;
        StaticJsonDocument<512> doc;

        unsigned long upTime = millis() / 1000;

        doc["chip"] = info->idfTarget;
        doc["revision"] = info->chipRevision;
        doc["cores"] = info->cores;
        doc["bt"] = info->hasBt;
        doc["ble"] = info->hasBle;
        doc["flashSize"] = info->flashSize;
        doc["flashType"] = info->embeddedFlash ? "embedded" : "external";
        doc["freeHeap"] = info->heapFree;
        doc["uptime"] = upTime;

        serializeJson(doc, buff);
        return buff;
    }
}
