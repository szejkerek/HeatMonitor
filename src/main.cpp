#include "peripherals.hpp"
#include "config.hpp"
#include <Arduino.h>
#include <WiFi.h>

#include "firebaseManager.hpp"
#include "whatsappMessaging.hpp"
#include "serviceManager.hpp"

void setup() {
  Serial.begin(9600);
  // Setup button pin
  pinMode(34, INPUT_PULLUP);
}

void loop() {
  checkConnectionAndTryReconnect();
  
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
