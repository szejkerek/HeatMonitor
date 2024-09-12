#include <OneWire.h>
#include <DallasTemperature.h>
#include <WebServerManager.hpp>
#include "peripherals.hpp"
#include "WiFi.h"

Peripherals peripherals;          // Create peripherals object
WebServerManager webServer(&peripherals);  // Pass peripherals to the WebServerManager

void setup() {
  Serial.begin(9600);  
  peripherals.Setup();
  webServer.begin();
}

void loop() {
  peripherals.GetTemperature(0);
  peripherals.GetTemperature(1);
  peripherals.GetTemperature(2);
  Serial.print("\n");
  delay(3000);
}
