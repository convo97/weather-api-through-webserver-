#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "";
const char* password = "";
const char* apiKey = "YOUR API KEY";
const char* latitude = "";
const char* longitude = "";

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  WiFi.begin(ssid, password);
  display.println("Connecting to WiFi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void fetchWeatherData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
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

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temp);
    display.println(" C");
    display.print("Weather: ");
    display.println(description);
    display.display();
  } else {
    Serial.print("Error: ");
    Serial.println(httpResponseCode);
    display.clearDisplay();
    display.println("Failed to fetch data");
    display.display();
  }

  http.end();
}

void loop() {
  fetchWeatherData();
  delay(60000); // Fetch data every 60 seconds
}
