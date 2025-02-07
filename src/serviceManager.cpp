// serviceManager.cpp
#include "serviceManager.hpp"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

void SetDiodeState(bool turnOn)
{
    digitalWrite(13, turnOn ? HIGH : LOW);
}



bool checkConnectionAndTryReconnect() {
  static unsigned long lastRunTime = 0;
  const unsigned long interval = 10000; // Recheck connection every 10 seconds
  unsigned long currentTime = millis();

  if (currentTime - lastRunTime < interval) {
    return true; // Skip if interval has not passed
  }

  lastRunTime = currentTime;
  Serial.println("Checking connection...");

  if (WiFi.status() == WL_CONNECTED && Firebase.ready()) {
    SetDiodeState(false);
    return true;
  }

  // Indicate disconnection
  SetDiodeState(true);

  // Attempt to reconnect Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long startAttemptTime = millis();
  const unsigned long connectionTimeout = 10000; // 10 seconds

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < connectionTimeout) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    delay(1000);

    if (setupFirebase(API_KEY, DATABASE_URL) && Firebase.ready()) {
      SetDiodeState(false);
      return true;
    } else {
      Serial.println("Firebase not ready after setup.");
    }
  } else {
    Serial.println("Failed to connect to Wi-Fi.");
  }

  SetDiodeState(true);
  return false;
}
