# M5 Atom Matrix Example

## What it does

 * (Re)connect WiFi 
 * (Re)connect Mqtt
 * Periodical gateway ping
 * Send & receive Mqtt messages
 
## How to use

Copy `lib/secrets/secrets.h.dist` to `lib/secrets/secrets.h`.

## MQTT

### Server

```bash
# Listen to events
mosquitto_sub -t "/growlab/#"

# Request info
mosquitto_pub -t "/growlab/inTopic" -m info
```

### Messages

Format:

```json
{
  "id": "<device-id>",
  "type": "<message-type>",
  "timestamp": 123,
  "payload": {
  }
}
```

#### IN

##### PING

Request a PONG reply from the device.

##### RESET

Request device reset.

###### Serial output

```
10:44:19.726 > [RST] ************** Remote Reset **************
```

##### STATUS

Get device status, i.e. dynamic values.

###### Serial output

```
10:40:59.610 > ---------- STATUS ----------
10:40:59.614 > [INFO] Up-time: 2478 sec
10:40:59.614 > [INFO] Heap size: 347 Kb
10:40:59.618 > [INFO] Free heap size: 140 Kb
10:40:59.622 > [INFO] Minimum free heap size: 126 Kb
10:40:59.622 > [INFO] PSRAM size: 0 Kb
10:40:59.626 > [INFO] Free PSRAM size: 0 Kb
10:40:59.630 > ----------------------------
```

##### INFO

Get device information, i.e. static info like hardware infos.

###### Serial output

```
10:43:07.978 > ----------  INFO  ----------
10:43:07.982 > [INFO] Chip esp32, 2 CPU core(s), WiFi/BT/BLE
10:43:07.986 > [INFO] Chip revision: 1
10:43:07.990 > [INFO] Memory: 4MB embedded flash
10:43:07.990 > [INFO] Local IP: 192.168.0.69
10:43:07.994 > [INFO] Subnet mask: 255.255.255.0
10:43:07.998 > [INFO] Gateway IP: 192.168.0.1
10:43:08.001 > [INFO] Local MAC: 10:D2:89:91:2:500:2:9:1:02:0
10:43:08.006 > [INFO] SSID: dreamcraft-s
10:43:08.006 > [INFO] RSSI: -63
10:43:08.009 > ----------------------------
```
##### DISPLAY text, color

Display the given _text_ in the given _color_ on the LED matrix.

##### DRAW buffer

Draw the given _buffer_ to the LED matrix.

#### OUT

##### CONNECT

Sent on MQTT connection.

```json
{
  "id":"atom-matrix-1",
  "type":"connect",
  "timestamp":131
}
```

##### RESET

Sent before resetting.

```json
{
  "id":"atom-matrix-1",
  "type":"reset",
  "timestamp":131,
  "payload": {
    "reason": "software | remote"
  }
}
```

##### BUTTON_PRESS

Sent when the button was pressed.

```json
{
  "id":"atom-matrix-1",
  "type":"button-pressed",
  "timestamp":10812
}
```

##### HEARTBEAT

Periodically sent.

```json
{
  "id":"atom-matrix-1",
  "type":"heartbeat",
  "timestamp":10812
}
```

##### PONG

Sent in reply to a PING.

```json
{
  "id":"atom-matrix-1",
  "type":"pong",
  "timestamp": 10812
}
```

##### STATUS

Replies to a status message.


```json
{
   "id":"atom-matrix-1",
   "type":"status",
   "timestamp":10958,
   "payload":{
      "memory":{
         "heapSize":355856,
         "freeHeapSize":134712,
         "minHeapSize":129496,
         "psramSize":0,
         "freePsramSize":0
      }
   }
}
```

##### INFO

Replies to an info message.

```json
{
  "id": "atom-matrix-1",
  "type": "status",
  "timestamp": 18897,
  "payload": {
    "hardware": {
      "chip": "esp32",
      "revision": 1,
      "cores": 2,
      "bt": true,
      "ble": true,
      "flashSize": 4194304,
      "flashType": "embedded"
    },
    "network": {
      "ip": "192.168.0.69",
      "gateway": "192.168.0.1",
      "netmask": "255.255.255.0",
      "mac": "0:2:9:1:02:0",
      "ssid": "dreamcraft-s",
      "rssi": -60
    }
  }
}
```
