#include <Arduino.h>
#include <WiFi.h>

#include "config.hpp"
#include "peripherals.hpp"
#include "serviceManager.hpp"
#include "firebaseManager.hpp"
#include "whatsappMessaging.hpp"

void setup() {
  Serial.begin(9600);
  setupPeripherals();
}

void loop() {
  checkConnectionAndTryReconnect();
  
  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime > 15000) {
    lastSendTime = millis();

    CustomData sensorData;
    sensorData.temperature1 = temperature1.GetTemperature();
    sensorData.temperature2 = temperature2.GetTemperature();
    sensorData.temperature3 = temperature3.GetTemperature();

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
