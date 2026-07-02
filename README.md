# IoT Smart Farming & Automated Irrigation System

An open-source Internet of Things (IoT) project designed to optimize crop growth, monitor environmental conditions, and automate water consumption using an ESP32 microcontroller.

## 🚀 Features
* **Real-time Monitoring:** Tracks air temperature, humidity, and soil moisture levels.
* **Automated Irrigation:** Dynamically triggers a water pump relay when soil moisture drops below a specified threshold.
* **Cloud Dashboard:** Uploads live analytics to ThingSpeak for remote graphical visualization.
a
## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 (or NodeMCU ESP8266)
* **Sensors:** DHT11 (Temperature & Humidity), Capacitive Soil Moisture Sensor v1.2
* **Actuators:** 5V Relay Module + 5V Mini Submersible Water Pump
* **Power Source:** 5V Power Supply / USB Cable
* **Miscellaneous:** Breadboard, Jumper Wires

## 📊 System Architecture
1. Sensors gather soil moisture and climate parameters.
2. ESP32 processes data locally to make irrigation decisions (On/Off).
3. ESP32 transmits data packages over Wi-Fi to the Cloud Dashboard.

## ⚙️ Setup Instructions
1. **Hardware Setup:** Connect your sensors to the designated GPIO pins specified in `firmware.ino`.
2. **Cloud Configuration:** Sign up at [ThingSpeak](https://thingspeak.com/), create a new channel with 3 fields, and grab your **Write API Key**.
3. **Software Configuration:** Open `firmware.ino` in the Arduino IDE, update your Wi-Fi credentials, and insert your ThingSpeak Write API Key.
4. **Deploy:** Compile and upload the sketch to your ESP32 board.


