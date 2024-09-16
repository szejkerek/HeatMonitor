#ifndef WEBSERVERMANAGER_HPP
#define WEBSERVERMANAGER_HPP

#include "config.hpp"
#include <ESPAsyncWebServer.h>
#include "peripherals.hpp"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

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
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
      }
      Serial.println(WiFi.localIP());

      server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, [this](const String& var) {
          return processor(var, peripherals);
        });
      });

      server.on("/temperature", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", readDHTTemperature(peripherals).c_str());
      });
\
      server.begin();
    }
};

#endif