#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "MUTHU 8054";       // STA Mode SSID
const char* password = "Muthu@123";  // STA Mode Password

const char* ap_ssid = "ESP32-Weather";    // AP Mode SSID
const char* ap_password = "12345678";     // AP Mode Password

const char* apiKey = "6fe8e0c10a2b5260f05bcace79a9a428";  // OpenWeatherMap API Key
const char* latitude = "13.08908593488645";    // Latitude (e.g., "28.6139")
const char* longitude = "80.24477306367015";   // Longitude (e.g., "77.2090")

WiFiServer server(80);
String weatherData = "Fetching data...";

// Function to fetch weather data from OpenWeatherMap API
void fetchWeather() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected! Skipping API request.");
        return;
    }

    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + String(latitude) + "&lon=" + String(longitude) + "&appid=" + apiKey + "&units=metric";

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.println(payload);
        
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        float temp = doc["main"]["temp"];
        String description = doc["weather"][0]["description"].as<String>();
        
        weatherData = "Temperature: " + String(temp) + "°C<br>Description: " + description;
    } else {
        weatherData = "Error fetching data!";
    }

    http.end();
}

// Function to return HTML with embedded CSS
String getHTML() {
    return String(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang='en'>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<title>ESP32 Weather</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; background-color: #f4f4f4; text-align: center; padding: 20px; }"
        ".container { max-width: 400px; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); margin: auto; }"
        "h1 { color: #007bff; font-size: 24px; }"
        ".weather-data { font-size: 18px; margin: 15px 0; padding: 10px; background: #eee; border-radius: 8px; }"
        ".refresh-btn { background: #007bff; color: white; border: none; padding: 10px 20px; font-size: 16px; border-radius: 5px; cursor: pointer; }"
        ".refresh-btn:hover { background: #0056b3; }"
        "</style>"
        "</head>"
        "<body>"
        "<div class='container'>"
        "<h1>ESP32 Weather Station</h1>"
        "<div class='weather-data'>" + weatherData + "</div>"
        "<button class='refresh-btn' onclick='window.location.reload();'>Refresh</button>"
        "</div>"
        "</body>"
        "</html>"
    );
}

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi in STA mode
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    int retries = 30;
    while (WiFi.status() != WL_CONNECTED && retries-- > 0) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi Connection Failed! Running in AP mode only.");
    }

    // Start AP mode
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Access Point Started");
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    // Start Web Server
    server.begin();
    
    fetchWeather();  // Initial API call
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New Client Connected!");
        while (client.available()) {
            client.readStringUntil('\r');
        }

        client.print(getHTML()); // Serve the HTML page with CSS
        client.stop();

        fetchWeather(); // Update weather data on each request
    }
}
