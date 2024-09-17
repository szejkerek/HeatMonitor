#ifndef WEBSERVERMANAGER_HPP
#define WEBSERVERMANAGER_HPP

#include "config.hpp"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "peripherals.hpp"

class WebServerManager
{
private:
  unsigned long previousMillis = 0;
  const long interval_seconds = 30;

  AsyncWebServer server;
  Peripherals *peripherals;

  String readDHTTemperature(Peripherals *peripherals);

public:
  WebServerManager(Peripherals *_peripherals);

  bool connectToWiFi();
  void checkInternetConnection();
  void begin();
};

#endif
