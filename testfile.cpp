#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED reset pin (can be -1 if sharing reset)
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi and Gemini API
const char* ssid = "your_wifi";
const char* password = "your_password";
const char* gemini_api_key = "AIzaSyAf0lnGC1ntkWYC4dyQzUoEYiwtGFxJ2wo";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for most OLEDs
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Don't continue if OLED fails
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Ready to check facts!");
  display.display();

  // (Optional) Initialize microphone here if you have it
  // For now, we simulate input
}

void loop() {
  // Simulate recording voice and converting to text
  String userFact = "The sun is a star"; 

  // Display user input
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Checking...");
  display.println(userFact);
  display.display();

  String geminiResponse = askGemini(userFact);
  Serial.println(geminiResponse);

  String verdict = parseGeminiResponse(geminiResponse);

  // Display verdict
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Result:");
  display.println(verdict);
  display.display();

  // "Speak" verdict (here simulated by Serial, later with speaker)
  if (verdict == "True") {
    Serial.println("Speaker says: TRUE ✅");
    // Here you'd trigger playing a "True" audio file or TTS
  } else if (verdict == "False") {
    Serial.println("Speaker says: FALSE ❌");
    // Play "False" audio file
  } else {
    Serial.println("Speaker says: Could not determine ❓");
  }

  delay(10000); // Wait 10 seconds before next check
}

String askGemini(String question) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure(); // Accept all certificates

  http.begin(client, "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=" + String(gemini_api_key));
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"contents\":[{\"parts\":[{\"text\":\"Is the following statement true? " + question + "\"}]}]}";

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Received response:");
    Serial.println(response);
    return response;
  } else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpResponseCode);
    return "";
  }
}

String parseGeminiResponse(String response) {
  if (response.length() == 0) return "Unknown";

  // Parse JSON
  DynamicJsonDocument doc(4096); // Adjust size if needed
  DeserializationError error = deserializeJson(doc, response);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return "Unknown";
  }

  // Navigate to the text content
  const char* text = doc["candidates"][0]["content"]["parts"][0]["text"];

  Serial.println("Parsed text:");
  Serial.println(text);

  String result = String(text);

  // Decide based on response
  result.toLowerCase();
  if (result.indexOf("true") != -1 || result.indexOf("correct") != -1) {
    return "True";
  } else if (result.indexOf("false") != -1 || result.indexOf("incorrect") != -1) {
    return "False";
  } else {
    return "Unknown";
  }
}
