#include <M5StickCPlus.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Wi-Fi and Gemini API
const char* ssid = "Josh-12-M-2";
const char* password = "HelloWave1!";
const char* gemini_api_key = "AIzaSyAf0lnGC1ntkWYC4dyQzUoEYiwtGFxJ2wo";

// Prototypes
void playSound(int frequency, bool success = false, bool fail = false);
void showStatusScreen(String message, uint16_t bgColor);

void setup() {
  M5.begin();
  M5.Beep.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);

  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting setup...");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) {
      Serial.println("\nWiFi connection failed!");
      showStatusScreen("WiFi Fail", BLUE);
      while (true); // Stop here
    }
  }
  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  showStatusScreen("Ready!", BLACK);
}

void loop() {
  Serial.println("Starting fact check...");

  String userFact = "The sun is a star";
  Serial.println("Fact to check: " + userFact);

  // Yellow for "processing"
  showStatusScreen("Checking...", YELLOW);

  String geminiResponse = askGemini(userFact);

  if (geminiResponse.length() == 0) {
    Serial.println("Empty response from Gemini.");
    showStatusScreen("No Response", BLUE);
    playSound(1000);  // error sound
  } else {
    String answer = extractGeminiText(geminiResponse);

    Serial.println("Gemini Answer:");
    Serial.println(answer);

    answer.toLowerCase();

    if (answer.indexOf("true") != -1) {
      showStatusScreen("TRUE ✅", GREEN);
      playSound(0, true, false); // success ding-ding
    } else if (answer.indexOf("false") != -1) {
      showStatusScreen("FALSE ❌", RED);
      playSound(0, false, true); // sad low beep
    } else {
      showStatusScreen("UNKNOWN", BLUE);
      playSound(1000);           // unknown => normal beep
    }
  }

  Serial.println("Waiting 10s...");
  delay(10000);
}

// ---- HELPER FUNCTIONS ----

void playSound(int frequency, bool success, bool fail) {
  if (success) {
    M5.Beep.tone(2000, 150);
    delay(200);
    M5.Beep.tone(2500, 150);
    delay(200);
    M5.Beep.mute();
  } 
  else if (fail) {
    M5.Beep.tone(500, 600);
    delay(600);
    M5.Beep.mute();
  } 
  else {
    M5.Beep.tone(frequency, 500);
    delay(500);
    M5.Beep.mute();
  }
}

void showStatusScreen(String message, uint16_t bgColor) {
  M5.Lcd.fillScreen(bgColor);
  M5.Lcd.setTextColor(WHITE, bgColor);
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.setTextSize(3);   // BIGGER TEXT
  M5.Lcd.println(message);
}

String askGemini(String question) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();

  String url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + String(gemini_api_key);
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"contents\":[{\"parts\":[{\"text\":\"Is the following statement true? " + question + "\"}]}]}";

  int httpResponseCode = http.POST(payload);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    return http.getString();
  } else {
    return "";
  }
}

String extractGeminiText(String response) {
  if (response.length() == 0) return "No response";

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, response);
  if (error) return "Invalid JSON";

  const char* text = doc["candidates"][0]["content"]["parts"][0]["text"];
  if (!text) return "No text";

  return String(text);
}
