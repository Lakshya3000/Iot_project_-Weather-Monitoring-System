/*************************************************************
   Blynk IoT Weather Monitor Station
 *************************************************************/

// ---- Blynk Template Info ----
#define BLYNK_TEMPLATE_ID "TMPL3k1hCUj05"
#define BLYNK_TEMPLATE_NAME "Weather Monitor Station"
#define BLYNK_AUTH_TOKEN "8ZtzhEgovqlYCrPYISg0WxU8MZ8EEnqJ"

// ---- Libraries ----
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

// ---- Wi-Fi Credentials ----
char ssid[] = "Lucifer";         // Your WiFi / Hotspot name
char pass[] = "1234512345";      // Your WiFi password

// ---- Sensor Pins ----
#define DHTPIN D2                // GPIO4
#define DHTTYPE DHT11
#define LDRPIN D5                // GPIO14 (Digital)
#define RAINPIN A0               // Analog Input

// ---- Objects ----
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// ---- Function to Send Sensor Data ----
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int ldrValue = digitalRead(LDRPIN);
  int rainValue = analogRead(RAINPIN); // 0 (wet) -> 1023 (dry)

  // Handle potential DHT read errors
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print to Serial Monitor
  Serial.print("Temperature: "); Serial.print(t); Serial.print(" °C | ");
  Serial.print("Humidity: "); Serial.print(h); Serial.print(" % | ");
  Serial.print("Rain: "); Serial.print(rainValue); Serial.print(" | ");
  Serial.print("LDR: "); Serial.println(ldrValue);

  // Send data to Blynk virtual pins
  Blynk.virtualWrite(V0, t);          // Temperature
  Blynk.virtualWrite(V1, h);          // Humidity
  Blynk.virtualWrite(V2, rainValue);  // Rain Sensor
  Blynk.virtualWrite(V3, ldrValue);   // LDR
}

// ---- Setup ----
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Connect to Blynk Cloud
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  // Initialize sensors
  dht.begin();
  pinMode(LDRPIN, INPUT);

  // Send sensor data every 5 seconds
  timer.setInterval(5000L, sendSensorData);
}

// ---- Loop ----
void loop() {
  Blynk.run();
  timer.run();
}
