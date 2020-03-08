/** 
 * ESP32 I2C BME280 Cayenne Example.
 * 
 * This example sends BME280 sensor data to the Cayenne IoT platform.
 * @author maspetsberger 
 */

#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneArduinoMQTTClient.h>

// WiFi network info.
const char *ssid = "your-wifi-ssid";
const char *pass = "your-wifi-password";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
const char *username = "mqtt-user-name-id";
const char *password = "mqtt-password-id";
const char *clientID = "mqtt-client-id";

#define SEALEVELPRESSURE_HPA (1013.25)

WiFiClient wifi;
Adafruit_BME280 bme;

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
  
  // Initialize BME Sensor using I2C
  // Note:
  // * I've connected:
  //   - SDA to GPIO 21
  //   - SCL to GPIO 22
  // * My bme 280 is at address 0x76, while the default appears to be 0x77.
  unsigned status = bme.begin(0x76);
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }

  // Initialize Cayenne MQTT
	Cayenne.begin(client, username, password, clientID);
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
  // Read BME data
  float h = bme.readHumidity();
  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0f;
  float a = bme.readAltitude(SEALEVELPRESSURE_HPA);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(p)) {
    Serial.println("Failed to read from sensor!");
    delay(500);
    return;
  }

  Serial.printf("Temperature: %.2f °C\t", t);
  Serial.printf("Humidity: %.2f %\t", h);
  Serial.printf("Pressure: %.2f hPa\t", p);
  Serial.printf("Altitude: %.2f m\t", a);
  Serial.println();

  // Write data to Cayenne.
  // Note, for some data types there are also helperfunctions, like `Cayenne.celsiusWrite(...)`.
	Cayenne.virtualWrite(1, t, TYPE_TEMPERATURE, UNIT_CELSIUS);
  Cayenne.virtualWrite(2, h, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
  Cayenne.virtualWrite(3, p, TYPE_BAROMETRIC_PRESSURE, UNIT_HECTOPASCAL);
}
