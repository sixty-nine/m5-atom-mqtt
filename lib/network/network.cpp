/*
    From https://github.com/jnogues/RP2040nanoConnect/blob/main/code/template_RP2040_wifi_mqtt.ino

    May 30 2021
    jnogues@gmail.com
    @rprimTech
*/

#include <WiFi.h>
#include "network.h"

void printMacAddress(unsigned char mac[])
{
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void printWifiData()
{
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("[WIFI] IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("[WIFI] MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet()
{
  // print the SSID of the network you're attached to:
  Serial.print("[WIFI] SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("[WIFI] signal strength (RSSI):");
  Serial.println(rssi);
}

String WifiStatusToString(int status)
{
    switch (status) {
        case WL_CONNECTED: return "WL_CONNECTED";
        case WL_NO_SHIELD: return "WL_NO_SHIELD";
        case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
        case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
        case WL_DISCONNECTED: return "WL_DISCONNECTED";
    }

    return "UNKNOWN";
}
