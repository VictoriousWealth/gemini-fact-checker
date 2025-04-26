#include <M5StickCPlus.h>

void setup() {
  M5.begin();
  pinMode(10, OUTPUT); // On M5StickC Plus, LED is on GPIO 10
}

void loop() {
  digitalWrite(10, HIGH);  // Turn LED on
  delay(500);              // Wait 0.5 second
  digitalWrite(10, LOW);   // Turn LED off
  delay(500);              // Wait 0.5 second
}
