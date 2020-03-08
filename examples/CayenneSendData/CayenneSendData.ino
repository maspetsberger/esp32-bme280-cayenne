/** 
 * ESP32 Cayenne Example.
 * 
 * This example connects to the Cayenne IoT platform using native Arduino Wifi functionality.
 * @author maspetsberger 
 */

#include <WiFi.h>

#define CAYENNE_PRINT Serial
#include <CayenneArduinoMQTTClient.h>

// WiFi network info.
const char *ssid = "your-wifi-ssid";
const char *pass = "your-wifi-password";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
const char *username = "mqtt-user-name-id";
const char *password = "mqtt-password-id";
const char *clientID = "mqtt-client-id";

WiFiClient wifi;
CayenneArduinoMQTTClient Cayenne;

void setup() {
  Serial.begin(115200);
  while(!Serial);    // time to get serial running
  
  // Connect to the WiFi network
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize Cayenne MQTT
	Cayenne.begin(wifi, username, password, clientID);
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	Cayenne.virtualWrite(0, millis());
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

