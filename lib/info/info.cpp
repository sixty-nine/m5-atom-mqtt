#include "info.h"

#include <ArduinoJson.h>
#include "secrets.h"

namespace sixtynine
{

    char nibble_to_hex(uint8_t nibble) {  // convert a 4-bit nibble to a hexadecimal character (31684 B)
        nibble &= 0xF;
        return nibble > 9 ? nibble - 10 + 'A' : nibble + '0';
    }

    String getMacAddress(uint8_t mac[])
    {
        String res = "";

        for (int i = 5; i >= 0; i--) {
            if (mac[i] < 16) {
                res += "0";
            }
            res += nibble_to_hex(mac[i]);
            Serial.print(mac[i], HEX);
            if (i > 0) {
                res += ":";
                Serial.print(":");
            }
        }

        return res;
    }

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
    }

    void gatherMemInfo(memInfo *info)
    {
        info->heapSize = ESP.getHeapSize();
        info->freeHeap = esp_get_free_heap_size();
        info->minFreeHeap = esp_get_minimum_free_heap_size();
        info->psramSize = ESP.getPsramSize();
        info->freePsramSize = ESP.getFreePsram();
    }

    void gatherNetworkInfo(networkInfo *info)
    {
        info->localIp = WiFi.localIP();
        info->subnetMask = WiFi.subnetMask();
        info->gatewayIp = WiFi.gatewayIP();
        WiFi.macAddress((*info).localMacAddr);
        info->rssi = WiFi.RSSI();
    }

    void printEspInfo(espInfo *info)
    {
        Serial.printf("[INFO] Chip %s, %d CPU core(s), WiFi%s%s\r\n",
                info->idfTarget,
                info->cores,
                info->hasBt ? "/BT" : "",
                info->hasBle ? "/BLE" : "");

        Serial.printf("[INFO] Chip revision: %d\r\n", info->chipRevision);

        Serial.printf("[INFO] Memory: %dMB %s flash\r\n", info->flashSize / (1024 * 1024),
                info->embeddedFlash ? "embedded" : "external");
    }

    void printMemInfo(memInfo *info)
    {
        Serial.printf("[INFO] Heap size: %d Kb\r\n", info->heapSize / 1024);
        Serial.printf("[INFO] Free heap size: %d Kb\r\n", info->freeHeap / 1024);
        Serial.printf("[INFO] Minimum free heap size: %d Kb\r\n", info->minFreeHeap / 1024);
        Serial.printf("[INFO] PSRAM size: %d Kb\r\n", info->psramSize / 1024);
        Serial.printf("[INFO] Free PSRAM size: %d Kb\r\n", info->freePsramSize / 1024);
    }

    void printNetworkInfo(networkInfo *info)
    {
        Serial.print("[INFO] Local IP: ");
        Serial.println(info->localIp);
        Serial.print("[INFO] Subnet mask: ");
        Serial.println(info->subnetMask);
        Serial.print("[INFO] Gateway IP: ");
        Serial.println(info->gatewayIp);
        Serial.print("[INFO] Local MAC: ");
        Serial.println(getMacAddress(info->localMacAddr));
        Serial.println("[INFO] SSID: " + String(SSID));
        Serial.print("[INFO] RSSI: ");
        Serial.println(info->rssi);
    }

    void printStatus()
    {
        Serial.printf("[INFO] Up-time: %d sec\r\n", (millis() / 1000));
    }

    void sendDeviceInfo(Mqtt *mqtt, espInfo *info, networkInfo *netInfo)
    {
        StaticJsonDocument<2048> doc;
        JsonObject object = doc.to<JsonObject>();
        JsonObject hardware  = object.createNestedObject("hardware");
        JsonObject network  = object.createNestedObject("network");

        hardware["chip"] = info->idfTarget;
        hardware["revision"] = info->chipRevision;
        hardware["cores"] = info->cores;
        hardware["bt"] = info->hasBt;
        hardware["ble"] = info->hasBle;
        hardware["flashSize"] = info->flashSize;
        hardware["flashType"] = info->embeddedFlash ? "embedded" : "external";

        network["ip"] = netInfo->localIp;
        network["gateway"] = netInfo->gatewayIp;
        network["netmask"] = netInfo->subnetMask;
        network["mac"] = getMacAddress(netInfo->localMacAddr);
        network["ssid"] = String(SSID);
        network["rssi"] = netInfo->rssi;

        mqtt->sendJson("info", &object);
    }

    void sendStatus(Mqtt *mqtt, memInfo *info)
    {
        StaticJsonDocument<1024> doc;
        JsonObject object = doc.to<JsonObject>();
        JsonObject mem = object.createNestedObject("memory");

        mem["heapSize"] = info->heapSize;
        mem["freeHeapSize"] = info->freeHeap;
        mem["minHeapSize"] = info->minFreeHeap;
        mem["psramSize"] = info->psramSize;
        mem["freePsramSize"] = info->freePsramSize;

        mqtt->sendJson("status", &object);
    }
}
