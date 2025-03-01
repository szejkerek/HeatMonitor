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
  
  // Skip if interval has not passed
  if (currentTime - lastRunTime < interval) {
    return WiFi.status() == WL_CONNECTED && Firebase.ready();
  }
  
  lastRunTime = currentTime;
  Serial.println("Checking connection...");
  
  // Already connected - return true
  if (WiFi.status() == WL_CONNECTED && Firebase.ready()) {
    SetDiodeState(false);
    return true;
  }
  
  // Indicate disconnection
  SetDiodeState(true);
  
  // Disconnect first and set to station mode
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  
  // Attempt to reconnect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  // Use non-blocking approach for connection attempt
  unsigned long startAttemptTime = millis();
  const unsigned long connectionTimeout = 15000; // 15 seconds
  bool connected = false;
  
  while (millis() - startAttemptTime < connectionTimeout) {
    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      break;
    }
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  
  // If connected to regular WiFi
  if (connected) {
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    
    if (setupFirebase(API_KEY, DATABASE_URL) && Firebase.ready()) {
      SetDiodeState(false);
      return true;
    } else {
      Serial.println("Firebase not ready after setup.");
    }
  } else {
    // Failed to connect - start softAP as fallback
    Serial.println("Failed to connect to Wi-Fi. Starting softAP mode...");
    
    // Configure soft AP
    WiFi.mode(WIFI_AP_STA); // Both AP and station mode
    IPAddress softIP(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    
    if (WiFi.softAPConfig(softIP, softIP, subnet) && WiFi.softAP("DeviceAP", "password123")) {
      Serial.println("SoftAP started successfully");
      Serial.print("SoftAP IP address: ");
      Serial.println(WiFi.softAPIP());
      
      // Keep trying to reconnect in background while softAP is running
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      
      // We're in fallback mode - still return false, but softAP is running
      SetDiodeState(true); // Blink or use different pattern to indicate AP mode
    } else {
      Serial.println("Failed to start softAP");
    }
  }
  
  return false;
}
