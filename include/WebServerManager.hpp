#ifndef WEBSERVERMANAGER_HPP
#define WEBSERVERMANAGER_HPP

#include "config.hpp"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "peripherals.hpp"

class WebServerManager {
  private:
    AsyncWebServer server;
    Peripherals* peripherals;

    static String processor(const String& var, Peripherals* peripherals) {
      if (var == "TEMPERATURE") {
        float t = peripherals->temperture1.GetTemperature();
        if (isnan(t)) {    
          return "--";
        } else {
          return String(t);
        }
      }
      return String();
    }

    static String readDHTTemperature(Peripherals* peripherals) {
      float t = peripherals->temperture1.GetTemperature();
      if (isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return "--";
      } else {
        Serial.println(t);
        return String(t);
      }
    }

  public:
    WebServerManager(Peripherals* _peripherals) : server(80), peripherals(_peripherals) {}

    void begin() 
    {
      // Initialize SPIFFS
      if(!SPIFFS.begin()){
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
      }
      Serial.println("SPIFFS mounted successfully");

      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
      }
      Serial.println(WiFi.localIP());

      // Serve static HTML, CSS, and JS files from SPIFFS
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false);
      });

      server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/styles.css", "text/css");
      });

      server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "application/javascript");
      });

      server.on("/temperature", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(200, "text/plain", readDHTTemperature(peripherals).c_str());
      });

      server.on("/humidity", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "HUMIDITY VALUE"); // Replace with humidity fetching logic
      });

      server.begin();
    }
};

#endif
