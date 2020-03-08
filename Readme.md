
# Connecting to Cayenne IoT Platform with Native Wifi

The [Cayenne IoT Platform](https://cayenne.mydevices.com/) offers a platform to connect IoT devices, collect sensor data, and send actuator inputs. It provides out-of-the-box support for many Arduino devices, including the ESP32 chip.  The Arduino "Cayenne MQTT" library from mydevices provides diverse API routines to connect and initialize to the Cayenne platform.

If you do not want to use the Cayenne API to handle the Wifi but do it yourself, then use this repo.

## Pre-Requisites
This repo has been tested with the following tools:
* Arduino IDE, 1.8.5
* Cayenne MQTT library by mydevices, 1.3.0
* Adafruit BME280 Library by Adafruit, 2.0.1


## Standard Cayenne Connectivity
When using the standard API, you would first select the corresponding header file for your connectivity, e.g. `CayenneMQTTEthernet.h`, `CayenneMQTTWifi.h`, or `CayenneMQTTESP32.h`:
```
#include <CayenneMQTTESP32.h>
```
And during the `setup()` phase you would pass your connectivity credentials to the `begin()` call:
```
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
```
While this aproach is as simple as it gets, it also takes away the flexibility of your setup, whehter you want to try multiple Wifis, or if it is simply a question of whom to trust your credentials.

## Cayenne with Native Wifi
Although there is no example for it, you can use the native Arduino Wifi API to connect to Cayenne. In fact, the various options are mostly just wrappers around that native API. The following example highlights the approach for an ESP32 wifi connection.

```
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>

WiFiClient wifi;
CayenneArduinoMQTTClient Cayenne;

// ...
void setup() {
	Serial.begin(9600);
	WiFi.begin(ssid, pass);
	Cayenne.begin(wifi, username, password, clientID);
}

void loop() {
	Cayenne.loop();
}

```
Instead of passing the credentials, you simply pass the wifi client directly to the MQTT client.
