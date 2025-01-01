#include "peripherals.hpp"
#include "WebServerManager.hpp"
#include <Arduino.h>
#include <WiFi.h>

#include "firebaseManager.hpp"
#include "whatsappMessaging.hpp"

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  setupFirebase(API_KEY, DATABASE_URL);

  // Setup button pin
  pinMode(34, INPUT_PULLUP);
}

void loop() {
  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime > 15000) {
    lastSendTime = millis();

    // Simulate sensor data
    CustomData sensorData;
    sensorData.temperature = 20.0 + random(0, 100) / 10.0; // Random temperature
    sensorData.humidity = 30.0 + random(0, 100) / 10.0;     // Random humidity

    unsigned long timestamp = millis();

    if (saveDataToFirebase(sensorData, timestamp)) {
      Serial.println("Data saved successfully.");
    } else {
      Serial.println("Failed to save data.");
    }
  }

  // Handle button press
  if (digitalRead(34) == LOW) {
    delay(50); // Debounce
    if (digitalRead(34) == LOW) {
      sendMessage("Button clicked", PHONE_NUMBER, WHATSAPP_KEY);
      Serial.println("Button clicked, message sent.");
      while (digitalRead(34) == LOW);
    }
  }
}
