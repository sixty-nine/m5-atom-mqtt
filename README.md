# M5 Atom Matrix Example

## What it does

 * (Re)connect WiFi 
 * (Re)connect Mqtt
 * Periodical gateway ping
 * Send & receive Mqtt messages
 
## How to use

Copy `lib/secrets/secrets.h.dist` to `lib/secrets/secrets.h`.

### Mqtt server

```bash
# Listen to events
mosquitto_sub -t "/growlab/#"

# Request info
mosquitto_pub -t "/growlab/inTopic" -m info
```
