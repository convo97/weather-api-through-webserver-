#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your Wi-Fi credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// OpenWeather API Key (Replace with your actual API key)
const char* apiKey = "YOUR_API_KEY";  
// Latitude & Longitude for your location
const char* lat = "13.0891";
const char* lon = "80.2448";

// API Endpoint
String apiEndpoint = "http://api.openweathermap.org/data/2.5/weather?lat=" + String(lat) + "&lon=" + String(lon) + "&units=metric&appid=" + String(apiKey);

// Create an AsyncWebServer on port 80
AsyncWebServer server(80);

// Function to fetch weather data
String getWeatherData() {
  HTTPClient http;
  http.begin(apiEndpoint);
  int httpCode = http.GET();

  String weatherData = "";
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println(payload);

    // Parse JSON
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    // Extract weather details
    String temp = doc["main"]["temp"].as<String>();
    String humidity = doc["main"]["humidity"].as<String>();
    String description = doc["weather"][0]["description"].as<String>();

    // Create formatted HTML data
    weatherData = "<h2>Current Weather</h2>";
    weatherData += "<p><b>Temperature:</b> " + temp + "°C</p>";
    weatherData += "<p><b>Humidity:</b> " + humidity + "%</p>";
    weatherData += "<p><b>Condition:</b> " + description + "</p>";
  } else {
    weatherData = "<p>Failed to fetch weather data.</p>";
  }

  http.end();
  return weatherData;
}

// Web page stored in flash memory
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Weather</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; text-align: center; background: #f4f4f4; padding: 20px; }
    h1 { color: #333; }
    .weather-box { background: white; padding: 20px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); display: inline-block; }
    footer { margin-top: 20px; font-size: 12px; color: gray; }
  </style>
</head>
<body>
  <h1>ESP32 Weather Station</h1>
  <div class="weather-box" id="weatherData">Loading weather...</div>
  <footer>&copy; 2024 Weather Station</footer>

  <script>
    function fetchWeather() {
      fetch('/weather')
      .then(response => response.text())
      .then(data => {
        document.getElementById('weatherData').innerHTML = data;
      });
    }
    setInterval(fetchWeather, 10000);
    fetchWeather();
  </script>
</body>
</html>
)rawliteral";

// Setup WiFi & Server
void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Serve HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Serve weather data
  server.on("/weather", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", getWeatherData());
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing needed here, handled by async server
}
