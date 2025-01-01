// firebase_manager.cpp
#include "firebaseManager.hpp"
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

String formatTimestamp(unsigned long timestamp) {
  time_t rawTime = timestamp / 1000; // Convert milliseconds to seconds
  struct tm *timeInfo = gmtime(&rawTime);
  char buffer[11]; // DD-MM-YYYY format
  snprintf(buffer, sizeof(buffer), "%02d-%02d-%04d", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
  return String(buffer);
}

void setupFirebase(const String& apiKey, const String& databaseUrl) {
  Serial.println("Initializing Firebase...");

  /* Assign the API key (required) */
  config.api_key = apiKey.c_str();

  /* Assign the RTDB URL (required) */
  config.database_url = databaseUrl.c_str();

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Sign-up successful");
    signupOK = true;
  } else {
    Serial.printf("Error: %s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

bool saveDataToFirebase(const CustomData& data, unsigned long timestamp) {
  if (!Firebase.ready() || !signupOK) {
    Serial.println("Firebase not ready or signup failed.");
    return false;
  }

  String formattedDate = formatTimestamp(timestamp);
  String basePath = "sensor_data/" + formattedDate + "/";

  // Save temperature
  String tempPath = basePath + "temperature";
  if (!Firebase.RTDB.setFloat(&fbdo, tempPath.c_str(), data.temperature)) {
    Serial.println("FAILED to write temperature data");
    Serial.println("REASON: " + fbdo.errorReason());
    return false;
  }

  // Save humidity
  String humidityPath = basePath + "humidity";
  if (!Firebase.RTDB.setFloat(&fbdo, humidityPath.c_str(), data.humidity)) {
    Serial.println("FAILED to write humidity data");
    Serial.println("REASON: " + fbdo.errorReason());
    return false;
  }

  Serial.println("Custom data written to Firebase:");
  Serial.println("Date: " + formattedDate);
  Serial.println("Temperature: " + String(data.temperature));
  Serial.println("Humidity: " + String(data.humidity));
  return true;
}
