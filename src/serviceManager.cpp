#include "serviceManager.hpp"
#include "WiFi.h"
#include <Firebase_ESP_Client.h>

void SetDiodeState(bool turnOn)
{
    digitalWrite(13, turnOn ? HIGH : LOW);
}

bool checkConnectionAndTryReconnect()
{
    static unsigned long lastRunTime = 0; // Static variable to keep track of last execution time
    const unsigned long interval = 60000; // 60 seconds interval
    unsigned long currentTime = millis();

    // If the interval hasn't passed, exit early
    if (currentTime - lastRunTime < interval) {
        return true; // Nothing to do yet
    }

    // Update the last execution time
    lastRunTime = currentTime;

    Serial.println("Checking connection...");


    // Check current connection status
    if (WiFi.status() == WL_CONNECTED && Firebase.ready())
    {
        SetDiodeState(false);
        return true;
    }
    
    // Indicate disconnection
    SetDiodeState(true);

    // Attempt to reconnect Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    unsigned long startAttemptTime = millis();
    
    // Wait for connection with timeout
    const unsigned long connectionTimeout = 10000; // 10 seconds
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < connectionTimeout) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("Connected with IP: ");
        Serial.println(WiFi.localIP());
        
        // Setup Firebase
        setupFirebase(API_KEY, DATABASE_URL);
        delay(1000);

        if (Firebase.ready())
        {
            SetDiodeState(false);
            return true;
        }
        else
        {
            Serial.println("Firebase not ready after setup.");
        }
    }
    else
    {
        Serial.println("Failed to connect to Wi-Fi.");
    }

    // If we reach this point, connection failed
    SetDiodeState(true);
    return false;
}

