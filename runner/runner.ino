#include <WiFi.h>
#include <WiFiClientSecure.h>
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
const char* ssid = "Josh-12-M-2";
const char* password = "HelloWave1!";
const char* gemini_api_key = "AIzaSyAf0lnGC1ntkWYC4dyQzUoEYiwtGFxJ2wo";

void setup() {
  Serial.begin(115200);  // Changed to match your Serial Monitor speed
  delay(1000);            // Give some time for Serial Monitor to open

  Serial.println("Starting setup...");

  // Connect to Wi-Fi
  Serial.println("Starting WiFi connection...");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int wifi_attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_attempts++;
    if (wifi_attempts > 20) { // Timeout after 10 seconds
      Serial.println("\nWiFi connection failed!");
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.println(WiFi.localIP());
  }

  // Initialize OLED
  Serial.println("Initializing OLED...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for most OLEDs
    Serial.println(F("SSD1306 allocation failed!"));
    while (true); // Don't continue if OLED fails
  }
  Serial.println("OLED initialized.");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Ready to check facts!");
  display.display();
  Serial.println("Setup complete.");
}

void loop() {
  Serial.println("Starting new fact check loop...");

  // Simulate recording voice and converting to text
  String userFact = "The sun is a star"; 
  Serial.println("Fact to check: " + userFact);

  // Display "Checking..." and user fact
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Checking...");
  display.println(userFact);
  display.display();

  // Ask Gemini
  Serial.println("Sending request to Gemini...");
  String geminiResponse = askGemini(userFact);

  if (geminiResponse.length() == 0) {
    Serial.println("Empty response from Gemini.");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Error getting");
    display.println("response!");
    display.display();
  } else {
    Serial.println("Received response:");
    Serial.println(geminiResponse);

    // Extract just the answer text
    String answer = extractGeminiText(geminiResponse);

    // Display the Gemini answer on the OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);

    int maxCharsPerLine = 21; // Fit nicely on 128x64
    for (int i = 0; i < answer.length(); i += maxCharsPerLine) {
      String line = answer.substring(i, min(i + maxCharsPerLine, answer.length()));
      display.println(line);
    }
    display.display();
  }

  Serial.println("Waiting 10 seconds before next check...");
  delay(10000); // Wait 10 seconds before next check
}


String askGemini(String question) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure(); // Accept all certificates

  Serial.println("Building request for Gemini...");
  String url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + String(gemini_api_key);
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"contents\":[{\"parts\":[{\"text\":\"Is the following statement true? " + question + "\"}]}]}";
  Serial.println("Payload:");
  Serial.println(payload);

  int httpResponseCode = http.POST(payload);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Received raw response from Gemini:");
    Serial.println(response);
    return response;
  } else {
    Serial.println("Error during HTTP POST");
    return "";
  }
}

String parseGeminiResponse(String response) {
  if (response.length() == 0) {
    Serial.println("Empty response received for parsing.");
    return "Unknown";
  }

  // Parse JSON
  Serial.println("Deserializing JSON...");
  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, response);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return "Unknown";
  }

  // Check if "candidates" exists and has at least one element
  if (!doc.containsKey("candidates") || doc["candidates"].size() == 0) {
    Serial.println("No candidates found in response.");
    return "Unknown";
  }

  // Extract the reply text
  const char* text = doc["candidates"][0]["content"]["parts"][0]["text"];
  if (!text) {
    Serial.println("No text found in candidates.");
    return "Unknown";
  }

  Serial.println("Parsed text:");
  Serial.println(text);

  String result = String(text);

  // Analyze the response
  result.toLowerCase();
  if (result.indexOf("true") != -1 || result.indexOf("correct") != -1) {
    return "True";
  } else if (result.indexOf("false") != -1 || result.indexOf("incorrect") != -1) {
    return "False";
  } else {
    return "Unknown";
  }
}

String extractGeminiText(String response) {
  if (response.length() == 0) {
    Serial.println("Empty response received for extraction.");
    return "No response";
  }

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, response);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return "Invalid JSON";
  }

  if (!doc.containsKey("candidates") || doc["candidates"].size() == 0) {
    Serial.println("No candidates found in response.");
    return "No candidates";
  }

  const char* text = doc["candidates"][0]["content"]["parts"][0]["text"];
  if (!text) {
    Serial.println("No text found in candidates.");
    return "No text";
  }

  return String(text);
}

