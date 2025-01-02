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

String FormatTime(const char* rawTime) {
    // Example input: "Wed Jan  1 18:22:34 2025"
    // Desired output: "2025-01-01_18:22:34"
    String formattedTime;
    String months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    String rawString = String(rawTime);
    String month = rawString.substring(4, 7); // Extract month
    String day = rawString.substring(8, 10); // Extract day
    String time = rawString.substring(11, 19); // Extract HH:MM:SS
    String year = rawString.substring(20, 24); // Extract year

    day.trim();
    if (day.length() == 1) {
        day = "0" + day;
    }

    int monthIndex = months.indexOf(month) / 3 + 1;
    String monthNumber = monthIndex < 10 ? "0" + String(monthIndex) : String(monthIndex);
    formattedTime = year + "-" + monthNumber + "-" + day + "_" + time;
    return formattedTime;
}


bool synchronizeTime()
{
    configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");
    Serial.println("Syncing time with NTP server...");
    delay(3500);
    for (int i = 0; i < 30; i++) {
        time_t now = time(nullptr);
        if (now > 100000) {
        Serial.println("Time synchronized: " + FormatTime(ctime(&now)));
        return true;
        }
        delay(500);
    }

    if (time(nullptr) <= 100000) {
        Serial.println("\nFailed to synchronize time. Check NTP server or Wi-Fi.");
    }
    return false;
}

void setupFirebase(const String& apiKey, const String& databaseUrl) {
  Serial.println("Initializing Firebase...");
  config.api_key = apiKey.c_str();
  config.database_url = databaseUrl.c_str();

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Sign-up successful");
    signupOK = true;
  } else {
    Serial.printf("Error: %s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);

  synchronizeTime();
}


bool setFloatValue(const String& path, float value) {
  if (!Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)) {
    Serial.println("FAILED to write float value to: " + path);
    Serial.println("REASON: " + fbdo.errorReason());
    return false;
  }
  return true;
}

bool setIntValue(const String& path, int value) {
  if (!Firebase.RTDB.setInt(&fbdo, path.c_str(), value)) {
    Serial.println("FAILED to write int value to: " + path);
    Serial.println("REASON: " + fbdo.errorReason());
    return false;
  }
  return true;
}

bool setStringValue(const String& path, const String& value) {
  if (!Firebase.RTDB.setString(&fbdo, path.c_str(), value)) {
    Serial.println("FAILED to write string value to: " + path);
    Serial.println("REASON: " + fbdo.errorReason());
    return false;
  }
  return true;
}

bool setBoolValue(const String& path, bool value) {
  if (!Firebase.RTDB.setBool(&fbdo, path.c_str(), value)) {
    Serial.println("FAILED to write bool value to: " + path);
    Serial.println("REASON: " + fbdo.errorReason());
    return false;
  }
  return true;
}

bool saveDataToFirebase(const CustomData& data, unsigned long timestamp) {
  if (!Firebase.ready() || !signupOK) {
    Serial.println("Firebase not ready or signup failed.");
    return false;
  }
  time_t now = time(nullptr);
  String formattedDate = FormatTime(ctime(&now));
  String basePath = "sensor_data/" + formattedDate + "/";

  if (!setFloatValue(basePath + "temperature1", data.temperature1)) {
    return false;
  }
  if (!setFloatValue(basePath + "temperature2", data.temperature2)) {
    return false;
  }
  if (!setFloatValue(basePath + "temperature3", data.temperature3)) {
    return false;
  }

  Serial.println("Data saved to firebase.");
  return true;
}
