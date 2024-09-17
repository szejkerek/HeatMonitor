#include "WebServerManager.hpp"

String WebServerManager::readDHTTemperature(Peripherals *peripherals)
{
    float t = peripherals->temperture1.GetTemperature();
    if (isnan(t))
    {
        Serial.println("Failed to read from DHT sensor!");
        return "--";
    }
    else
    {
        return String(t);
    }
}

WebServerManager::WebServerManager(Peripherals *_peripherals) : server(80), peripherals(_peripherals)
{
    if (!SPIFFS.begin())
    {
        Serial.println("SPIFFS mounted successfully");
        return;
    }
    Serial.println("An error has occurred while mounting SPIFFS");
}

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

    if (currentMillis - previousMillis >= interval_seconds * 1000)
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

void WebServerManager::begin()
{
    connectToWiFi();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", String(), false); });

    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/styles.css", "text/css"); });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/script.js", "application/javascript"); });

    server.on("/temperature", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", readDHTTemperature(peripherals).c_str()); });

    server.on("/humidity", HTTP_GET, [this](AsyncWebServerRequest *request)
              {
                  request->send(200, "text/plain", "HUMIDITY VALUE"); // Replace with humidity fetching logic
              });

    server.begin();
}
