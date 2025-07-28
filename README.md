
# 🌤️ ESP32 Weather Station with OpenWeatherMap API

This project demonstrates how to create a **Weather Station** using the **ESP32 microcontroller**. It fetches real-time weather data from the **OpenWeatherMap API** based on your geographic coordinates and serves it via a local **web server**, accessible through both **Wi-Fi Station (STA)** and **Access Point (AP)** modes.

## 🛠 Features

- Connects to Wi-Fi or creates its own hotspot
- Fetches weather data using OpenWeatherMap REST API
- Displays temperature and weather description on a mobile-friendly web page
- Clean, responsive HTML interface with refresh button
- Supports both local IP access and AP fallback mode

## 🔧 Requirements

- **ESP32 development board**
- Arduino IDE with the following libraries:
  - `WiFi.h`
  - `HTTPClient.h`
  - `ArduinoJson.h`
- OpenWeatherMap API Key (free account required)

## 📷 Screenshot

![ESP32 Weather Web Interface](screenshot.png)  
*A modern, mobile-friendly interface served from the ESP32 itself.*

## 📡 Setup Instructions

1. **Clone this repository**:
   ```bash
   git clone https://github.com/your-username/esp32-weather-station.git
   cd esp32-weather-station
Open the code in Arduino IDE and install these libraries if not already available:

WiFi

HTTPClient

ArduinoJson

Update the following placeholders in the code with your actual values:

cpp
Copy
Edit
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "YOUR_OPENWEATHERMAP_API_KEY";
const char* latitude = "YOUR_LATITUDE";     // e.g., "28.6139"
const char* longitude = "YOUR_LONGITUDE";   // e.g., "77.2090"
Upload the code to your ESP32 using Arduino IDE.

Open Serial Monitor at 115200 baud to view connection and IP status.

Access the weather web page:

If Wi-Fi is connected: Visit the IP shown in Serial Monitor.

If Wi-Fi fails: Connect to ESP32-Weather hotspot and visit 192.168.4.1.

🧠 How It Works
Connects to a specified Wi-Fi network or creates a fallback Access Point.

Makes an HTTP GET request to OpenWeatherMap using your location and API key.

Parses the JSON response using ArduinoJson.

Serves a styled HTML page showing current temperature and conditions.

Refresh button fetches updated weather data.

🌍 OpenWeatherMap API
Sign up and generate a free API key here:
👉 https://openweathermap.org/api
