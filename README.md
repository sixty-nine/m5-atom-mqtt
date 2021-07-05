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

#### IN

##### RESET

Request device reset.

##### STATUS

Get device status, i.e. dynamic values.

##### INFO

Get device information, i.e. static info like hardware infos.

##### DISPLAY text, color

Display the given _text_ in the given _color_ on the LED matrix.

##### DRAW buffer

Draw the given _buffer_ to the LED matrix.

#### OUT

##### BUTTON_PRESS

Sent when the button was pressed.

##### KEEP_ALIVE

Periodically sent.

##### STATUS

TBD

##### INFO

TBD
