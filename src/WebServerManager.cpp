#include "WebServerManager.hpp"

bool WebServerManager::connectToWiFi()
{
    peripherals->SetDiodeState(true);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print("\nConnecting to WiFi");
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 15) 
        {
            delay(500);
            Serial.print(".");
            attempts++;
        }
        Serial.print("\n");
    }

    if (WiFi.status() == WL_CONNECTED) 
    {
        Serial.println("\nConnected to WiFi!\n" + WiFi.localIP().toString());
        peripherals->SetDiodeState(false);
    } 
    else 
    {
        Serial.println("\nFailed to connect to WiFi.");
    }

    return WiFi.status() == WL_CONNECTED;
}

void WebServerManager::checkInternetConnection()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) 
    {
        previousMillis = currentMillis;

        if (WiFi.status() == WL_CONNECTED) 
        {
            Serial.println("WiFi connected.");
            return;
        }

        Serial.println("WiFi not connected. Trying to reconnect...");
        WiFi.disconnect();
        connectToWiFi();
    }
}
