#include <WiFi.h>
#include <DHT.h>

// --- Configuration ---
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// ThingSpeak Settings
const char* TS_SERVER = "api.thingspeak.com";
String TS_API_KEY = "YOUR_THINGSPEAK_WRITE_API_KEY"; // Paste your API key here

// Pin Assignments
#define DHTPIN 4          // DHT11 Data pin connected to GPIO 4
#define DHTTYPE DHT11
#define MOISTURE_PIN 34   // Soil moisture analog pin connected to GPIO 34
#define RELAY_PIN 25      // Relay module connected to GPIO 25

// Thresholds
const int DRY_SOIL_THRESHOLD = 2000; // Adjust based on your sensor calibration (0-4095)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Keep pump off initially
  
  dht.begin();
  connectWiFi();
}

void loop() {
  // Reconnect WiFi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  // Read Sensors
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int soilMoisture = analogRead(MOISTURE_PIN);

  // Check if readings failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print to Serial Monitor
  Serial.printf("Temp: %.1f°C | Hum: %.1f%% | Soil Moisture: %d\n", temperature, humidity, soilMoisture);

  // Automated Irrigation Logic
  if (soilMoisture > DRY_SOIL_THRESHOLD) { 
    Serial.println("Soil is dry! Turning on the water pump...");
    digitalWrite(RELAY_PIN, HIGH); // Turn pump ON
  } else {
    Serial.println("Soil is hydrated. Turning off the water pump.");
    digitalWrite(RELAY_PIN, LOW);  // Turn pump OFF
  }

  // Send Data to ThingSpeak Cloud
  sendToCloud(temperature, humidity, soilMoisture);

  // Wait 15 seconds before the next reading (ThingSpeak's minimum interval)
  delay(15000);
}

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void sendToCloud(float temp, float hum, int moisture) {
  if (client.connect(TS_SERVER, 80)) {
    String postStr = TS_API_KEY;
    postStr += "&field1=" + String(temp);
    postStr += "&field2=" + String(hum);
    postStr += "&field3=" + String(moisture);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + TS_API_KEY + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: " + String(postStr.length()) + "\n\n");
    client.print(postStr);
    
    Serial.println("Data successfully sent to ThingSpeak Cloud.");
  }
  client.stop();
}


