#include <M5StickCPlus.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ------------- SETTINGS ------------------

// Wi-Fi credentials
const char* ssid = "Josh-12-M-2";           
const char* password = "HelloWave1!";       

// Gemini API Key
const char* gemini_api_key = "YOUR+API+KEY+HERE";  

// ------------- SETUP ------------------

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
      M5.Lcd.fillScreen(BLUE);
      M5.Lcd.setCursor(10, 30);
      M5.Lcd.println("WiFi FAIL!");
      while (true); // Stop program
    }
  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  // Ready screen
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.println("Ready!");
}

// ------------- MAIN LOOP ------------------

void loop() {
  M5.update(); // Make sure to update M5 buttons

  if (M5.BtnA.wasPressed()) {  // Only when button A is clicked
    Serial.println("Button A pressed, starting fake recording...");

    // Simulate recording audio
    delay(1000); // simulate "listening"
    String userFact = sendAudioToApi(NULL); // fake

    Serial.println("Fact to check: " + userFact);

    // Show "Checking..." screen
    M5.Lcd.fillScreen(YELLOW);
    M5.Lcd.setCursor(10, 30);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("Checking...");

    String geminiResponse = askGemini(userFact);

    if (geminiResponse.length() == 0) {
      Serial.println("Empty response from Gemini.");
      showAnswerScreen("No Response", BLUE);
    } else {
      String answer = extractGeminiText(geminiResponse);
      Serial.println("Gemini Answer:");
      Serial.println(answer);

      String lowerAnswer = answer;
      lowerAnswer.toLowerCase();

      if (lowerAnswer.indexOf("true") != -1) {
        showAnswerScreen(answer, GREEN);
        playCheeringSound();
      } else if (lowerAnswer.indexOf("false") != -1) {
        showAnswerScreen(answer, RED);
        playSadSound();
      } else {
        showAnswerScreen(answer, BLUE);
        M5.Beep.tone(1000, 500);
        delay(500);
        M5.Beep.mute();
      }
    }

    Serial.println("Waiting 10s...");
    delay(10000);
  }
}

// ------------- FAKE AUDIO HANDLER ------------------

String sendAudioToApi(uint16_t* buffer) {
  // ❗ Instead of recording, just pretend we got this
  Serial.println("Simulating fake audio recording...");
  return "The sun is a star"; // pretend user said this
}

// ------------- HELPER FUNCTIONS ------------------

void playCheeringSound() {
  int freqs[] = {1000, 1500, 2000, 2500};
  for (int i = 0; i < 4; i++) {
    M5.Beep.tone(freqs[i], 100);
    delay(150);
  }
  M5.Beep.mute();
}

void playSadSound() {
  int freqs[] = {2000, 1500, 1000, 500};
  for (int i = 0; i < 4; i++) {
    M5.Beep.tone(freqs[i], 200);
    delay(250);
  }
  M5.Beep.mute();
}

void showAnswerScreen(String answer, uint16_t bgColor) {
  M5.Lcd.fillScreen(bgColor);
  M5.Lcd.setCursor(5, 10);
  M5.Lcd.setTextColor(WHITE, bgColor);
  M5.Lcd.setTextSize(1);

  int maxCharsPerLine = 40;
  for (int i = 0; i < answer.length(); i += maxCharsPerLine) {
    String line = answer.substring(i, min(i + maxCharsPerLine, (int)answer.length()));
    M5.Lcd.println(line);
  }
}

// ------------- GEMINI FUNCTIONS ------------------

String askGemini(String question) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();

  String url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + String(gemini_api_key);
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"contents\":[{\"parts\":[{\"text\":\"Is the following statement true? (Answer with true or false and then the short explanation. Avoid saying \"not true\" if false and \"not false\" if true)" + question + "\"}]}]}";

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
